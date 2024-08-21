#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stddef.h>
#include <math.h>

/**
 * Module definition
 */
static PyModuleDef vectorsmodule = {
	.m_base = PyModuleDef_HEAD_INIT,
	.m_name = "vectors",
	.m_doc = "Module implementing different vectors",
	.m_size = -1,
};


/**
 * Vector 3 Definition
 */
typedef struct {
	PyObject_HEAD
	double ob_xval;
	double ob_yval;
	double ob_zval;
} Vec3Object;

static PyTypeObject Vec3Type;

/**
 * Initialization and Destruction
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


/**
 * Members
 */
static PyMemberDef Vec3_members[] = {
	{"x", Py_T_DOUBLE, offsetof(Vec3Object, ob_xval), 0, "X value of the vector"},
	{"y", Py_T_DOUBLE, offsetof(Vec3Object, ob_yval), 0, "Y value of the vector"},
	{"z", Py_T_DOUBLE, offsetof(Vec3Object, ob_zval), 0, "Z value of the vector"},
	{NULL}
};


/**
 * Methods
 */
// TODO: Make this a module method and not an object method
static PyObject*
Vec3_dot(PyObject* self /* = NULL */, PyObject* args)
{
    Vec3Object* lhs = NULL;
    Vec3Object* rhs = NULL; 

    if (!PyArg_ParseTuple(args, "O!O!", &Vec3Type, &lhs, &Vec3Type, &rhs)) {
        return NULL;
    }

    return PyFloat_FromDouble(lhs->ob_xval * rhs->ob_xval + lhs->ob_yval * rhs->ob_yval + lhs->ob_zval * rhs->ob_zval);
}

static PyObject*
Vec3_length(PyObject* _self, PyObject* Py_UNUSED(ignored))
{
    Vec3Object* self = (Vec3Object*) _self;

    return PyFloat_FromDouble(sqrt(self->ob_xval * self->ob_xval + self->ob_yval * self->ob_yval + self->ob_zval * self->ob_zval));
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

static PyMethodDef Vec3_methods[] = {
	// {"dot", (PyCFunction) Vec3_dot, METH_STATIC | METH_VARARGS, "Get the dot product of 2 vectors"},
    {"length", (PyCFunction) Vec3_length, METH_NOARGS, "Get the magnitude of the vector"},
    {"get_as_ppm_color", (PyCFunction) Vec3_ppm_color, METH_VARARGS, "Get the vector as an RGB color in PPM format"},
	{NULL}
};


/**
 * Number Methods
 */
static PyObject*
Vec3_add(Vec3Object* lhs, Vec3Object* rhs)
{
    Vec3Object* ret = (Vec3Object*) PyObject_New(Vec3Object, &Vec3Type);
    ret->ob_xval = lhs->ob_xval + rhs->ob_xval;
    ret->ob_yval = lhs->ob_yval + rhs->ob_yval;
    ret->ob_zval = lhs->ob_zval + rhs->ob_zval;

    return (PyObject*) ret;
}

static PyNumberMethods Vec3_number_methods = {
    .nb_add = (binaryfunc) Vec3_add,
    // .nb_subtract
    // .nb_multiply w/ double
    // .nb_negative
    // .nb_invert
};

// TODO:
// - Vec3_length_squared
// - Vec3_length
// - Vec3_cross


static PyObject* Vec3_str(Vec3Object* self) {
    char buffer[128];

    snprintf(buffer, sizeof buffer, "%lf %lf %lf", self->ob_xval, self->ob_yval, self->ob_zval);
    return PyUnicode_FromString(buffer);
}



static PyTypeObject Vec3Type = {
	.ob_base = PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "vectors.Vec3",
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
 * Module initialization
 */
PyMODINIT_FUNC
PyInit_vectors(void)
{
	PyObject* m;
	if (PyType_Ready(&Vec3Type) < 0) {
		return NULL;
	}

	m = PyModule_Create(&vectorsmodule);
	if (!m) {
		return NULL;
	}

	Py_INCREF(&Vec3Type);
	if (PyModule_AddObject(m, "Vec3", (PyObject *) &Vec3Type) < 0) {
		Py_DECREF(&Vec3Type);
		Py_DECREF(m);
		return NULL;
	}

	return m;
}
