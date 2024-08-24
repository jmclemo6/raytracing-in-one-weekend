#ifndef VECTORS_MODULE_H
#define VECTORS_MODULE_H

#include <Python.h>
#include "vec3.h"

// Module methods
static PyObject* vectors_dot(PyObject* self, PyObject* args);
static PyObject* vectors_cross(PyObject* self, PyObject* args);

#endif // VECTORS_MODULE_H
