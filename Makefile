
.PHONY: all topclean
.DEFAULT_GOAL = all


TOP = $(patsubst %/,%,$(dir $(abspath $(firstword $(MAKEFILE_LIST)))))
BUILDDIR = $(TOP)/build
INSTALL_PREFIX = $(BUILDDIR)/output


$(BUILDDIR):
	mkdir -p $@

AUTORECONF_OUTPUTS =  Makefile.in configure compile config.guess config.sub
AUTORECONF_OUTPUTS += install-sh test-driver depcomp missing ltmain.sh
AUTORECONF_OUTPUTS += config.h.in
AUTORECONF_OUTPUTS += $(addprefix m4/,libtool.m4 ltoptions.m4 ltsugar.m4)
AUTORECONF_OUTPUTS += $(addprefix m4/,ltversion.m4 lt~obsolete.m4)
AUTORECONF_OUTPUTS := $(addprefix $(TOP)/,$(AUTORECONF_OUTPUTS))


$(AUTORECONF_OUTPUTS): $(addprefix $(TOP)/,Makefile.am configure.ac m4/bear.m4)
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
	-rm -f $(TOP)/Makefile.in $(TOP)/configure
	-rm -f $(addsuffix ~,$(AUTORECONF_OUTPUTS))


compile_commands.json: $(BUILDDIR)/compile_commands.json
	$(MAKE) -C $(BUILDDIR) -f $(BUILDDIR)/Makefile $@;  \
	cp $< $@


.DEFAULT all:: | $(BUILDDIR)/Makefile
	$(MAKE) -C $(BUILDDIR) -f $(BUILDDIR)/Makefile $@
