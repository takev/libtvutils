
%module tvutils
%include "typemaps.i"
%{
#include <tvutils/tvutils.h>
typedef int error_code;
%}

int tvu_init(char *argv);
long long tvu_time(void);
unsigned long long tvu_random(void);

