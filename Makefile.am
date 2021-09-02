ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS = -ggdb3 -Wall -pedantic
AM_CPPFLAGS = -I$(top_srcdir)/include -D_GNU_SOURCE

bin_PROGRAMS = gsort
lib_LTLIBRARIES = src/libgraph.la

include $(top_srcdir)/src/Include.am
include $(top_srcdir)/include/Include.am
include $(top_srcdir)/src/test/Include.am

if BEAR_AM
.PHONY: bearcc
bearcc: $(BEAROUTFILE)

$(BEAROUTFILE): clean
	$(BEAR) $(BEARFLAGS) $(MAKE) $(AM_MAKEFLAGS) all
endif
