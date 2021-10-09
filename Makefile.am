ACLOCAL_AMFLAGS =
ACLOCAL_AMFLAGS += -I m4
ACLOCAL_AMFLAGS += -I gnulib/m4

SUBDIRS = gnulib

AM_CFLAGS =
AM_CFLAGS += -ggdb3
AM_CFLAGS += -Wall
AM_CFLAGS += -pedantic

AM_CPPFLAGS =
AM_CPPFLAGS += -include config.h
AM_CPPFLAGS += -I$(top_srcdir)/include
AM_CPPFLAGS += -I$(top_builddir)/gnulib
AM_CPPFLAGS += -I$(top_srcdir)/gnulib

LIBADD = $(abs_top_builddir)/gnulib/libgnu.la

bin_PROGRAMS = gsort
lib_LTLIBRARIES = src/libgraph.la

include $(top_srcdir)/src/Include.am
include $(top_srcdir)/include/Include.am
include $(top_srcdir)/src/test/Include.am

if BEAR_AM
$(BEAROUTFILE): clean
	$(BEAR) $(BEARFLAGS) $(MAKE) all
endif
