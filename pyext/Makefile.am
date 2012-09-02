
#AM_CFLAGS = -Wall -W -Werror -pedantic -Wno-sign-compare -Wno-long-long -Wno-unused -std=gnu99
AM_CFLAGS = -fstrict-aliasing -O3 -Wall -W -Wstrict-aliasing=2 -pedantic -Wno-sign-compare -Wno-long-long -Wno-unused -std=gnu99

DEFAULT_INCLUDES=-I$(top_srcdir)

BUILT_SOURCES = tvutils_wrap.c
SWIG_SOURCES = tvutils.i

pkgpython_PYTHON = tvutils.py __init__.py
pkgpython_LTLIBRARIES = _tvutils.la
_tvutils_la_SOURCES = tvutils_wrap.c $(SWIG_SOURCES)
_tvutils_la_CFLAGS = $(AX_SWIG_PYTHON_CPPFLAGS) $(AM_CFLAGS)
_tvutils_la_LDFLAGS = -module
_tvutils_la_LIBADD = $(top_builddir)/tvutils/libtvutils.la

tvutils_wrap.c: $(SWIG_SOURCES)
	$(SWIG) $(AX_SWIG_PYTHON_OPT) -o $@ $<

