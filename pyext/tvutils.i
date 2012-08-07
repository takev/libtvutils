
%module tvutils
%{
#include <tvutils/tvutils.h>
%}

int tvu_init(char *argv);
long long tvu_time(void);

