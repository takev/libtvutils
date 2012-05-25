
AM_CFLAGS = -Wall -W -Werror -pedantic -Wno-sign-compare -Wno-long-long -Wno-unused -std=gnu99

#dist_man3_MANS = uniq_init.3 uniq_time.3 uniq_tipc_node.3 uniq_tipc_cuid.3

DEFAULT_INCLUDES=-I$(top_srcdir)

lib_LTLIBRARIES = libtvutils.la

libtvutils_la_LDFLAGS = -version-info $(SHARED_VERSION_INFO)
libtvutils_la_SOURCES = tvutils.c
libtvutils_la_LIBADD = $(LIBS)

library_includedir=$(includedir)/uniqueid-$(VERSION)/uniqueid
library_include_HEADERS = atomic.h time.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tvutils.pc

