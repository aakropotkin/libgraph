
.PHONY: all topclean
.DEFAULT_GOAL = all


TOP = $(patsubst %/,%,$(dir $(abspath $(firstword $(MAKEFILE_LIST)))))
BUILDDIR = $(TOP)/build
INSTALL_PREFIX = $(BUILDDIR)/output


$(BUILDDIR):
	mkdir -p $@

AUTORECONF_OUTPUTS =  Makefile.in configure compile config.guess config.sub
AUTORECONF_OUTPUTS += install-sh test-driver depcomp missing ltmain.sh
AUTORECONF_OUTPUTS := $(addprefix $(TOP)/,$(AUTORECONF_OUTPUTS))


$(AUTORECONF_OUTPUTS): $(TOP)/Makefile.am $(TOP)/configure.ac
	autoreconf -if $(TOP)


CONFIGURE_OUTPUTS = Makefile config.h config.status libtool src src/test
CONFIGURE_OUTPUTS += stamp-h1
CONFIGURE_OUTPUTS := $(addprefix $(BUILDDIR)/,$(CONFIGURE_OUTPUTS))

$(CONFIGURE_OUTPUTS): | $(BUILDDIR)
$(CONFIGURE_OUTPUTS): $(addprefix $(TOP)/,configure config.h.in Makefile.in)
	cd $(BUILDDIR);                                 \
	if test -x $(BUILDDIR)/config.status; then      \
	  $(BUILDDIR)/config.status --recheck;          \
	else                                            \
	  $(TOP)/configure --prefix=$(INSTALL_PREFIX);  \
	fi


topclean:
	-rm -rf $(BUILDDIR)
	-rm -rf $(TOP)/Makefile.in $(TOP)/configure


.DEFAULT all:: | $(BUILDDIR)/Makefile
	$(MAKE) -C $(BUILDDIR) -f $(BUILDDIR)/Makefile $@
