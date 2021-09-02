.DEFAULT_GOAL = all

TOP = $(patsubst %/,%,$(dir $(abspath $(firstword $(MAKEFILE_LIST)))))
BUILDDIR = $(TOP)/build
INSTALL_PREFIX = $(BUILDDIR)/output

$(BUILDDIR):
	mkdir -p $@


$(TOP)/Makefile.in $(TOP)/configure: $(TOP)/Makefile.am $(TOP)/configure.ac
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

.PHONY: all topclean

.DEFAULT all:: | $(BUILDDIR)/Makefile
	$(MAKE) -C $(BUILDDIR) -f $(BUILDDIR)/Makefile $@

topclean:
	-rm -rf $(BUILDDIR)
	-rm -rf $(TOP)/Makefile.in $(TOP)/configure
