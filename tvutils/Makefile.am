
AM_CFLAGS = -fstrict-aliasing -O3 -Wall -W -Werror -Wstrict-aliasing=2 -pedantic -Wno-sign-compare -Wno-long-long -Wno-unused -std=gnu99

#dist_man3_MANS = uniq_init.3 uniq_time.3 uniq_tipc_node.3 uniq_tipc_cuid.3

DEFAULT_INCLUDES=-I$(top_srcdir)

lib_LTLIBRARIES = libtvutils.la

libtvutils_la_LDFLAGS = -version-info $(SHARED_VERSION_INFO)
libtvutils_la_SOURCES = tvutils.c
libtvutils_la_LIBADD = $(LIBS)

EXTRA_DIST=\
	memory_access.h.php\
	endian.h.php\
	atomic_gcc.h.php

BUILT_SOURCES=\
	memory_access.h\
	endian.h\
	atomic_gcc.h

library_includedir=$(includedir)/uniqueid-$(VERSION)/uniqueid
library_include_HEADERS =\
	 tvutils.h\
	 target.h\
	 stdfloat.h\
	 atomic.h\
	 atomic_gcc.h\
	 time.h\
	 time_bsd.h\
	 time_posix1993.h\
	 endian.h\
	 integer.h\
	 memory_access.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tvutils.pc

CLEANFILES=\
	endian.h\
	memory_acces.h\
	atomic_gcc.h

memory_access.h: memory_access.h.php
	${PHP} $< >$@

endian.h: endian.h.php
	${PHP} $< >$@

atomic_gcc.h: atomic_gcc.h.php
	${PHP} $< >$@

