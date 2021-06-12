ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src src/test include

AM_CFLAGS = -ggdb -Wall -pedantic


$(top_srcdir)/compile_commands.json: clean
	$(BEAR) --output $@ -- $(MAKE) all
