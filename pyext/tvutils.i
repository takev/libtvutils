
%module tvutils
%{
#include <tvutils/tvutils.h>
%}

int tvu_init(void);
long long tvu_time(void);

