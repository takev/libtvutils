
%module tvutils
%include "typemaps.i"
%{
#include <tvutils/tvutils.h>
typedef int error_code;
%}

%typemap(default) int flags {
    $1 = 0;
}

%typemap(in) (void *wbuffer, unsigned long long wbuffer_len) {
    if (!PyString_Check($input)) {
        PyErr_SetString(PyExc_ValueError, "Expecting a string");
        return NULL;
    }
    $1 = (void *)PyString_AsString($input);
    $2 = PyString_Size($input);
}

%typemap(in, numinputs=0) (long long *service_nr) {
    $1 = (long long *)malloc(sizeof (long long));
}

%typemap(argout) (long long *service_nr) {
    Py_XDECREF($result);
    if (result == -1 ) {
        free($1);
        PyErr_SetFromErrno(PyExc_IOError);
        return NULL;
    }

    $result = PyLong_FromLongLong(*(long long *)$1);
    free($1);
}

%typemap(in) (long long *service_nr, void *rbuffer, unsigned long long rbuffer_len) {
    if (!PyInt_Check($input)) {
        PyErr_SetString(PyExc_ValueError, "Expecting an integer");
        return NULL;
    }
    $3 = PyInt_AsLong($input);
    if ($3 < 0) {
        PyErr_SetString(PyExc_ValueError, "Positive integer expected");
        return NULL;
    }
    $2 = (void *)malloc($3);
    $1 = (long long *)malloc(sizeof (long long));
}

%typemap(argout) (long long *service_nr, void *rbuffer, unsigned long long rbuffer_len) {
    Py_XDECREF($result);
    if (result < 0) {
        free($2);
        free($1);
        PyErr_SetFromErrno(PyExc_IOError);
        return NULL;
    }

    $result = PyTuple_New(2);
    PyTuple_SET_ITEM($result, 0, PyString_FromStringAndSize($2, result));
    PyTuple_SET_ITEM($result, 1, PyLong_FromLongLong(*(long long *)$1));
    free($2);
    free($1);
}

typedef int error_code;

int tvu_init(char *argv);
long long tvu_time(void);

int tvu_service_socket(void);
error_code tvu_service_bind(int socket, long long service_nr);
error_code tvu_service_client_bind(int socket, long long *service_nr);
error_code tvu_service_unbind(int socket, long long service_nr);
error_code tvu_service_close(int socket, long long service_nr);
int tvu_service_sendto(int socket, long long service_nr, void *wbuffer, unsigned long long wbuffer_len, int flags);
int tvu_service_recvfrom(int socket, long long *service_nr, void *rbuffer, unsigned long long rbuffer_len, int flags);
