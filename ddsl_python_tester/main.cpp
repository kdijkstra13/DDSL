#define DDSL_NO_EXT_LIB

#include "ddsl.hpp"
#include <Python.h>
#include"cnpy.h"
#include<complex>


// Python wrappers
static PyObject *DDSLError;

static PyObject *makeNumpy(PyObject *self, PyObject *args) {

    const char *output_path;

    if (!PyArg_ParseTuple(args, "s", &output_path))
        return NULL;

    // Example 1:
    // Converting Matrix to numpy array, and storing it to file
    /*vector<Float> tempVector(matrix.rows->begin(), matrix.rows->end());
    cnpy::npy_save(std::move(destinationpath),&tempVector[0],{depth, height, width},"w"); */


    // Example 2:
    // Throwing an exception
    /*PyErr_SetString(DDSLError, "One of the files does not exist");
    return NULL; */


    const int Nx = 4;
    const int Ny = 5;
    const int Nz = 10;

    // Create sample vector
    std::vector<int> data(Nx*Ny*Nz);
    for(int i = 0;i < Nx*Ny*Nz;i++) data[i] = i;

    //save it to file
    cnpy::npy_save(output_path,&data[0],{Nz,Ny,Nx},"w");

    return Py_BuildValue("i", 1);

}

static PyMethodDef DDSLMethods[] =
        {
                {"makeNumpy",    makeNumpy,    METH_VARARGS, "Make 10x4x5 numpy array."},
                {NULL, NULL, 0, NULL}
        };

static struct PyModuleDef DDSLDefs = {
        PyModuleDef_HEAD_INIT, "DDSL", "A Python wrapper for Dijkstra's Data Science Language.", -1, DDSLMethods
};

PyMODINIT_FUNC PyInit_DDSL(void) {
    Py_Initialize();
    PyObject * m;

    m = PyModule_Create(&DDSLDefs);
    if (m == NULL)
        return NULL;

    DDSLError = PyErr_NewException("DDSL.error", NULL, NULL);
    Py_INCREF(DDSLError);
    PyModule_AddObject(m, "error", DDSLError);
    return m;

}


