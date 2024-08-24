#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stddef.h>
#include <math.h>
#include "vec3.h"

// Implementation of Vec3 methods and type

static PyMemberDef Vec3_members[] = {
    {"x", Py_T_DOUBLE, offsetof(Vec3Object, ob_xval), 0, "X value of the vector"},
    {"y", Py_T_DOUBLE, offsetof(Vec3Object, ob_yval), 0, "Y value of the vector"},
    {"z", Py_T_DOUBLE, offsetof(Vec3Object, ob_zval), 0, "Z value of the vector"},
    {NULL}
};

static PyMethodDef Vec3_methods[] = {
    {"length", (PyCFunction) Vec3_length, METH_NOARGS, "Get the magnitude of the vector"},
    {"get_as_ppm_color", (PyCFunction) Vec3_ppm_color, METH_VARARGS, "Get the vector as an RGB color in PPM format"},
    {"normalize", (PyCFunction) Vec3_normalize, METH_NOARGS, "Normalize the vector in-place"},
    {"get_normalized", (PyCFunction) Vec3_get_normalized, METH_NOARGS, "Get a normalized copy of the vector"},
    {NULL}
};

static PyNumberMethods Vec3_number_methods = {
    .nb_add = (binaryfunc) Vec3_add,
    .nb_subtract = (binaryfunc) Vec3_subtract,
    .nb_multiply = (binaryfunc) Vec3_multiply,
    .nb_negative = (unaryfunc) Vec3_negative,
};

extern PyTypeObject Vec3Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "vectors.vec3",
    .tp_doc = PyDoc_STR("A class implementing a standard 3-element Vector"),
    .tp_basicsize = sizeof(Vec3Object),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_members = Vec3_members,
    .tp_methods = Vec3_methods,
    .tp_as_number = &Vec3_number_methods,
    .tp_init = (initproc) Vec3_init,
    .tp_dealloc = (destructor)Vec3_dealloc,
    .tp_str = (reprfunc) Vec3_str,
};


/**
 * Vec3 Initialization and Destruction
 */
static void Vec3_dealloc(Vec3Object* self) {
    Py_TYPE(self)->tp_free((PyObject*) self);
}

static int Vec3_init(Vec3Object* self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, "ddd", &self->ob_xval, &self->ob_yval, &self->ob_zval))
	{
		return -1;
	}

	return 0;
}

static PyObject* Vec3_str(Vec3Object* self) {
    char buffer[128];
    snprintf(buffer, sizeof buffer, "%lf %lf %lf", self->ob_xval, self->ob_yval, self->ob_zval);
    return PyUnicode_FromString(buffer);
}


/**
 * Vec 3 Member Methods
 */
static PyObject*
Vec3_length(PyObject* _self, PyObject* Py_UNUSED(ignored))
{
    Vec3Object* self = (Vec3Object*) _self;

    return PyFloat_FromDouble(sqrt(self->ob_xval * self->ob_xval + self->ob_yval * self->ob_yval + self->ob_zval * self->ob_zval));
}

static PyObject*
Vec3_normalize(PyObject* _self, PyObject* Py_UNUSED(ignored))
{
    Vec3Object* self = (Vec3Object*) _self;
    double magnitude = PyFloat_AsDouble(Vec3_length(_self, NULL));
    if (magnitude == 0) {
        PyErr_SetString(PyExc_ZeroDivisionError, "Cannot normalize a zero vector");
        return NULL;
    }

    self->ob_xval /= magnitude;
    self->ob_yval /= magnitude;
    self->ob_zval /= magnitude;

    return (PyObject*) self;
}

static PyObject*
Vec3_get_normalized(PyObject* _self, PyObject* Py_UNUSED(ignored))
{
    Vec3Object* self = (Vec3Object*) _self;

    double magnitude = PyFloat_AsDouble(Vec3_length(_self, NULL));
    if (magnitude == 0) {
        PyErr_SetString(PyExc_ZeroDivisionError, "Cannot normalize a zero vector");
        return NULL;
    }

    Vec3Object* ret = (Vec3Object*) PyObject_New(Vec3Object, &Vec3Type);
    if (ret == NULL) return NULL;

    ret->ob_xval = self->ob_xval / magnitude;
    ret->ob_yval = self->ob_yval / magnitude;
    ret->ob_zval = self->ob_zval / magnitude;

    return (PyObject*) ret;
}

static PyObject*
Vec3_ppm_color(PyObject* _self, PyObject* args)
{
    Vec3Object* self = (Vec3Object*) _self;

    int max_colors;
    if (!PyArg_ParseTuple(args, "i", &max_colors)) {
        return NULL;
    }

    char buffer[128];
    snprintf(buffer, sizeof buffer, "%d %d %d\n", (int) (self->ob_xval * max_colors), (int) (self->ob_yval * max_colors), (int) (self->ob_zval * max_colors));
    return PyUnicode_FromString(buffer);
}



/**
 * Numeric Methods
 */
static PyObject*
Vec3_add(Vec3Object* lhs, Vec3Object* rhs)
{
    Vec3Object* ret = (Vec3Object*) PyObject_New(Vec3Object, &Vec3Type);
    if (ret == NULL) return NULL;

    ret->ob_xval = lhs->ob_xval + rhs->ob_xval;
    ret->ob_yval = lhs->ob_yval + rhs->ob_yval;
    ret->ob_zval = lhs->ob_zval + rhs->ob_zval;

    return (PyObject*) ret;
}

static PyObject*
Vec3_subtract(Vec3Object* lhs, Vec3Object* rhs)
{
    Vec3Object* ret = (Vec3Object*) PyObject_New(Vec3Object, &Vec3Type);
    if (ret == NULL) return NULL;

    ret->ob_xval = lhs->ob_xval - rhs->ob_xval;
    ret->ob_yval = lhs->ob_yval - rhs->ob_yval;
    ret->ob_zval = lhs->ob_zval - rhs->ob_zval;

    return (PyObject*) ret;
}

static PyObject*
Vec3_multiply(PyObject* lhs, PyObject* rhs)
{
    // both operands are vectors
    if (PyObject_TypeCheck(lhs, &Vec3Type) && PyObject_TypeCheck(rhs, &Vec3Type)) {
        Vec3Object* u = (Vec3Object*) lhs;
        Vec3Object* v = (Vec3Object*) rhs;

        Vec3Object* ret = (Vec3Object*) PyObject_New(Vec3Object, &Vec3Type);
        if (!ret) return NULL;

        ret->ob_xval = u->ob_xval * v->ob_xval;
        ret->ob_yval = u->ob_yval * v->ob_yval;
        ret->ob_zval = u->ob_zval * v->ob_zval;

        return (PyObject*) ret;
    }


    // left operand is vector
    if (PyObject_TypeCheck(lhs, &Vec3Type) && (PyFloat_Check(rhs) || PyLong_Check(rhs))) {
        double scalar = PyFloat_AsDouble(rhs);
        Vec3Object* u = (Vec3Object*) lhs;

        Vec3Object* ret = (Vec3Object*) PyObject_New(Vec3Object, &Vec3Type);
        if (!ret) return NULL;

        ret->ob_xval = u->ob_xval * scalar;
        ret->ob_yval = u->ob_yval * scalar;
        ret->ob_zval = u->ob_zval * scalar;

        return (PyObject*) ret;
    }

    // right operand is vector
    if ((PyFloat_Check(lhs) || PyLong_Check(rhs)) && PyObject_TypeCheck(rhs, &Vec3Type)) {
        Vec3Object* v = (Vec3Object*) rhs;
        double scalar = PyFloat_AsDouble(lhs);

        Vec3Object* ret = (Vec3Object*) PyObject_New(Vec3Object, &Vec3Type);
        if (!ret) return NULL;

        ret->ob_xval = v->ob_xval * scalar;
        ret->ob_yval = v->ob_yval * scalar;
        ret->ob_zval = v->ob_zval * scalar;

        return (PyObject*) ret;
    }

    PyErr_SetString(PyExc_TypeError, "Can only multiply by float or int");
    return NULL;
}

static PyObject*
Vec3_negative(Vec3Object* self)
{
    self->ob_xval = -self->ob_xval;
    self->ob_yval = -self->ob_yval;
    self->ob_zval = -self->ob_zval;

    return (PyObject*) self;
}
