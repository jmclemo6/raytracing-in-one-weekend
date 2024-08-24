#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "vec3.h"
#include "vectorsmodule.h"

// Module-level function implementations

static PyObject* vectors_dot(PyObject* self, PyObject* args) {
    Vec3Object* lhs = NULL;
    Vec3Object* rhs = NULL;

    if (!PyArg_ParseTuple(args, "O!O!", &Vec3Type, &lhs, &Vec3Type, &rhs)) {
        return NULL;
    }

    return PyFloat_FromDouble(lhs->ob_xval * rhs->ob_xval + lhs->ob_yval * rhs->ob_yval + lhs->ob_zval * rhs->ob_zval);
}

static PyObject* vectors_cross(PyObject* self, PyObject* args) {
    Vec3Object* lhs = NULL;
    Vec3Object* rhs = NULL;

    if (!PyArg_ParseTuple(args, "O!O!", &Vec3Type, &lhs, &Vec3Type, &rhs)) {
        return NULL;
    }

    Vec3Object* result = PyObject_New(Vec3Object, &Vec3Type);
    if (!result) {
        return NULL;
    }

    result->ob_xval = lhs->ob_yval * rhs->ob_zval - lhs->ob_zval * rhs->ob_yval;
    result->ob_yval = lhs->ob_zval * rhs->ob_xval - lhs->ob_xval * rhs->ob_zval;
    result->ob_zval = lhs->ob_xval * rhs->ob_yval - lhs->ob_yval * rhs->ob_xval;

    return (PyObject*) result;
}

static PyMethodDef VectorsMethods[] = {
    {"dot", vectors_dot, METH_VARARGS, "Get the dot product of 2 vectors."},
    {"cross", vectors_cross, METH_VARARGS, "Get the cross product of 2 vectors."},
    {NULL, NULL, 0, NULL}
};

static PyModuleDef vectorsmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "vectors",
    .m_doc = "Module implementing different vectors",
    .m_size = -1,
    .m_methods = VectorsMethods,
};

PyMODINIT_FUNC PyInit_vectors(void) {
    PyObject* m;
    if (PyType_Ready(&Vec3Type) < 0) {
        return NULL;
    }

    m = PyModule_Create(&vectorsmodule);
    if (!m) {
        return NULL;
    }

    Py_INCREF(&Vec3Type);
    if (PyModule_AddObject(m, "vec3", (PyObject *) &Vec3Type) < 0) {
        Py_DECREF(&Vec3Type);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
