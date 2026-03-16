CURDIR := $(shell pwd)
BINDIR := $(CURDIR)/bin

# Trap for the VICE monitor to break into the monitor
VICETRAP = 0xcff0

CALYPSIAS = as6502
CALYPSICC = cc6502
CALYPSILN = ln6502
CALYPSITARGET = --target=c64
CALYPSICOMMONOPTS = -D VICETRAP=$(VICETRAP) -D __MATHH_LOG10_BROKEN -D NEWLINE_CR
CALYPSIPERFOPTS = -O2 --speed -D CALYPSI -D __LDIV_BROKEN $(CALYPSICOMMONOPTS)
CALYPSISIZEOPTS = -O2 --space -D CALYPSI $(CALYPSICOMMONOPTS)
CALYPSILINKEROPTS = c64-opt.scm

version:
	@echo ""
	@echo "Collecting version information of Calypsi ..."
	@echo "----------------------------------------------------------------------"

	$(CALYPSICC) --version > "calypsi.version" 2>&1
	@echo "$$(cat calypsi.version | awk '{print $$8}')" > "calypsi.version"

	@date +"%Y-%m-%d %H:%M:%S" > "calypsi.date"

build-perf:
	CALYPSIOPTS="$(CALYPSIPERFOPTS)" SUFFIX=calypsip $(MAKE) build

build-size:
	CALYPSIOPTS="$(CALYPSISIZEOPTS)" SUFFIX=calypsis $(MAKE) build

build:
	@echo ""
	@echo "Building $(PRG) for $(SUFFIX) ..."
	@echo "----------------------------------------------------------------------"

	@mkdir -p "$(BINDIR)/$(SUFFIX)"

	@echo ""
	
	$(foreach LIB,$(LIBS),$(CALYPSICC) $(CALYPSIOPTS) $(CALYPSITARGET) \
		-o "$(BINDIR)/$(SUFFIX)/$(notdir $(basename $(LIB))).o" \
		--list-file="$(BINDIR)/$(SUFFIX)/$(notdir $(basename $(LIB))).lst" \
		$(LIB);)

	@echo ""

	$(CALYPSICC) $(CALYPSIOPTS) $(CALYPSITARGET) \
		-o "$(BINDIR)/$(SUFFIX)/$(PRG).o" \
		--list-file "$(BINDIR)/$(SUFFIX)/$(PRG).lst" \
		"$(SRCDIR)/$(PRG).c"

	@echo ""

	$(CALYPSILN) $(CALYPSILINKEROPTS) $(CALYPSITARGET) \
		--output-format=prg --verbose \
		-o "$(BINDIR)/$(PRG)-$(SUFFIX).prg" \
		--list-file "$(BINDIR)/$(SUFFIX)/$(PRG).lst" \
		"$(BINDIR)/$(SUFFIX)/$(PRG).o" \
		$(foreach LIB,$(LIBS), "$(BINDIR)/$(SUFFIX)/$(notdir $(basename $(LIB))).o")	

clean:
	@echo ""
	@echo "Cleaning calypsi ..."
	@echo "----------------------------------------------------------------------"

	@$(RM) -rf "${BINDIR}"

exec-perf:
	SUFFIX=calypsip $(MAKE) exec

exec-size:
	SUFFIX=calypsis $(MAKE) exec

exec:
	@echo ""
	@echo "Executing $(PRG) for $(SUFFIX) ..."
	@echo "----------------------------------------------------------------------"

	@$(RM) -f "$(BINDIR)/$(PRG)-$(SUFFIX).log"

	$(VICEEXEC) $(VICEOPTS) -autostart "$(BINDIR)/$(PRG)-$(SUFFIX).prg" \
		-initbreak $(VICETRAP) \
		-moncommands "$(SRCDIR)/vice.mon" \
		-monlog \
		-monlogname "$(BINDIR)/$(PRG)-$(SUFFIX).log"

	@if [ -f "$(BINDIR)/vice.png" ]; then mv "$(BINDIR)/vice.png" "$(BINDIR)/$(PRG)-$(SUFFIX).png"; fi
	@if [ -f "$(BINDIR)/vice.scr" ]; then mv "$(BINDIR)/vice.scr" "$(BINDIR)/$(PRG)-$(SUFFIX).scr"; fi

	cat "$(BINDIR)/$(PRG)-$(SUFFIX).log" 
