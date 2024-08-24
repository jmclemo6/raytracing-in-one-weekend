#ifndef VEC3_H
#define VEC3_H

#include <Python.h>
#include <stddef.h>

// Vector 3 Definition
typedef struct {
    PyObject_HEAD
    double ob_xval;
    double ob_yval;
    double ob_zval;
} Vec3Object;

extern PyTypeObject Vec3Type;

// Vec3 methods
static void Vec3_dealloc(Vec3Object* self);
static int Vec3_init(Vec3Object* self, PyObject* args);
static PyObject* Vec3_length(PyObject* self, PyObject* Py_UNUSED(ignored));
static PyObject* Vec3_ppm_color(PyObject* self, PyObject* args);
static PyObject* Vec3_normalize(PyObject* self, PyObject* Py_UNUSED(ignored));
static PyObject* Vec3_get_normalized(PyObject* self, PyObject* Py_UNUSED(ignored));
static PyObject* Vec3_add(Vec3Object* lhs, Vec3Object* rhs);
static PyObject* Vec3_subtract(Vec3Object* lhs, Vec3Object* rhs);
static PyObject* Vec3_multiply(PyObject* lhs, PyObject* rhs);
static PyObject* Vec3_negative(Vec3Object* self);
static PyObject* Vec3_str(Vec3Object* self);

#endif // VEC3_H
