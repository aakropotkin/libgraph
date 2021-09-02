ACLOCAL_AMFLAGS =
ACLOCAL_AMFLAGS += -I m4
ACLOCAL_AMFLAGS += -I gnulib/m4

SUBDIRS = gnulib

AM_CFLAGS =
AM_CFLAGS += -ggdb3
AM_CFLAGS += -Wall
AM_CFLAGS += -Werror
AM_CFLAGS += -pedantic

AM_CPPFLAGS =
AM_CPPFLAGS += -include config.h
AM_CPPFLAGS += -I$(top_srcdir)/include
AM_CPPFLAGS += -I$(top_builddir)/gnulib
AM_CPPFLAGS += -I$(top_srcdir)/gnulib

LIBADD = $(abs_top_builddir)/gnulib/libgnu.la

bin_PROGRAMS = bin/gsort
lib_LTLIBRARIES = lib/libgraph.la

EXTRA_DIST = m4/gnulib-cache.m4

include $(top_srcdir)/src/Include.am
include $(top_srcdir)/include/Include.am
include $(top_srcdir)/src/test/Include.am

CLEANFILES =

if BEAR_AM
CLEANFILES += $(BEAROUTFILE)
if NIX_CC_WRAPPER
CLEANFILES += $(BEAROUTFILE)~
endif  # NIX_CC_WRAPPER


include $(top_srcdir)/build-aux/checksum.am
.PHONY: libchecksums
libchecksums: $(LTLIBRARIES:.la=.libsums)

$(BEAROUTFILE): clean
	$(BEAR) $(BEARFLAGS) $(MAKE) all
if NIX_CC_WRAPPER
	cat $@|$(JQ) '[.[]|select(.arguments[0]|contains("nix-profile")|not)]' > $@~
	mv $@~ $@
endif  #NIX_CC_WRAPPER
endif  # BEAR_AM
