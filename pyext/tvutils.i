
%module tvutils
%include "typemaps.i"
%{
#include <tvutils/tvutils.h>
typedef int error_code;
%}

int tvu_init(char *argv);
long long tvu_time(void);
long long tvu_random(void);
long long tvu_cuid(void);

