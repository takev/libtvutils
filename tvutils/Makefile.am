
DEFAULT_INCLUDES=-I$(top_srcdir)

MACHINE_FLAGS = -mmmx -msse -msse2 -msse3 -msse4 -maes -mrdrnd
AM_CFLAGS = $(MACHINE_FLAGS) -fstrict-aliasing -O3 -Wall -W -Werror -Wstrict-aliasing=2 -pedantic -Wno-sign-compare -Wno-long-long -Wno-unused -std=gnu99 $(DEPS_CFLAGS) $(DEFAULT_INCLUDES)

#dist_man3_MANS = uniq_init.3 uniq_time.3 uniq_tipc_node.3 uniq_tipc_cuid.3


lib_LTLIBRARIES = libtvutils.la

libtvutils_la_LDFLAGS = -version-info $(SHARED_VERSION_INFO)
libtvutils_la_LIBADD = $(LIBS)
libtvutils_la_SOURCES =\
	common.c\
	path.c\
	daemon.c\
	string.c\
	time.c\
	base64.c\
	random.c\
	hash.c\
	ringbuffer.c\
	permhashtable.c\
	iopoll.c\
	aes.c\
	varint.c

check_PROGRAMS =\
	bsearch_test\
	ringbuffer_test\
	dynarg_test\
	time_test\
	random_test\
	hash_test\
	permhashtable_test\
	varint_test

TESTS =\
	bsearch_test\
	ringbuffer_test\
	dynarg_test\
	time_test\
	random_test\
	hash_test\
	permhashtable_test\
	varint_test

bsearch_test_SOURCES = bsearch_test.c
bsearch_test_LDADD = $(top_builddir)/tvutils/libtvutils.la

ringbuffer_test_SOURCES = ringbuffer_test.c
ringbuffer_test_LDADD = $(top_builddir)/tvutils/libtvutils.la

dynarg_test_SOURCES = dynarg_test.c
dynarg_test_LDADD = $(top_builddir)/tvutils/libtvutils.la

time_test_SOURCES = time_test.c
time_test_LDADD = $(top_builddir)/tvutils/libtvutils.la

random_test_SOURCES = random_test.c
random_test_LDADD = $(top_builddir)/tvutils/libtvutils.la

hash_test_SOURCES = hash_test.c
hash_test_LDADD = $(top_builddir)/tvutils/libtvutils.la

permhashtable_test_SOURCES = permhashtable_test.c
permhashtable_test_LDADD = $(top_builddir)/tvutils/libtvutils.la

varint_test_SOURCES = varint_test.c
varint_test_LDADD = $(top_builddir)/tvutils/libtvutils.la


EXTRA_DIST=\
	memory_access.h.php\
	endian.h.php\
	atomic_gcc.h.php\
	number.h.php\
	permhashtable.h.php

BUILT_SOURCES=\
	memory_access.h\
	endian.h\
	atomic_gcc.h\
	number.h\
	permhashtable.h

library_includedir=$(includedir)/tvutils-$(VERSION)/tvutils
library_include_HEADERS =\
	tvutils.h\
	target.h\
	common.h\
	assert.h\
	path.h\
	buffer.h\
	target.h\
	compiler.h\
	types.h\
	atomic.h\
	atomic_gcc.h\
	dynarg.h\
	dynarg_sysv_x86_64.h\
	time.h\
	time_bsd.h\
	time_posix1993.h\
	endian.h\
	number.h\
	string.h\
	memory_access.h\
	ringbuffer.h\
	macros.h\
	unit_test.h\
	daemon.h\
	base64.h\
	aes.h\
	aes_x86ni.h\
	aes_fallback.h\
	bsearch.h\
	random.h\
	hash.h\
	permhashtable.h\
	varint.h\
	iopoll.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tvutils.pc

CLEANFILES=\
	endian.h\
	memory_acces.h\
	atomic_gcc.h\
	number.h\
	permhashtable.h

memory_access.h: memory_access.h.php
	${PHP} $< >$@ || (cat $@; rm $@; false)

endian.h: endian.h.php
	${PHP} $< >$@ || (cat $@; rm $@; false)

atomic_gcc.h: atomic_gcc.h.php
	${PHP} $< >$@ || (cat $@; rm $@; false)

number.h: number.h.php
	${PHP} $< >$@ || (cat $@; rm $@; false)

permhashtable.h: permhashtable.h.php
	${PHP} $< >$@ || (cat $@; rm $@; false)

