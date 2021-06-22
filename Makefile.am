ACLOCAL_AMFLAGS = -I m4 -I gnulib/m4

SUBIRS = gnulib

EXTRA_DIST = m4/gnulib-cache.m4

AM_CFLAGS = -ggdb3 -Wall -pedantic
AM_CPPFLAGS =  -I$(top_srcdir)/include -D_GNU_SOURCE
AM_CPPFLAGS += -I$(top_builddir)/gnulib -I$(top_srcdir)/gnulib

LIBADD = $(top_builddir)/gnulib/libgnu.la

bin_PROGRAMS = gsort
lib_LTLIBRARIES = src/libgraph.la

include $(top_srcdir)/src/Include.am
include $(top_srcdir)/include/Include.am
include $(top_srcdir)/src/test/Include.am


$(top_srcdir)/compile_commands.json: clean
	$(BEAR) --output $@ -- $(MAKE) all
