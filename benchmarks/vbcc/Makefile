CURDIR := $(shell pwd)
BINDIR := $(CURDIR)/bin

export VBCC = $(CURDIR)/vbcc
VC = vc
VBCCTARGET = +c64 -lm
VBCCPERFOPTS = -O4 -speed
VBCCSIZEOPTS = -O4 -size

version:
	@echo ""
	@echo "Collecting version information of vbcc ..."
	@echo "----------------------------------------------------------------------"

	@echo "Please, enter the version to the file vbcc.version manually!"
	
build-perf:
	$(MAKE) build VBCCOPTS="$(VBCCPERFOPTS)" SUFFIX=vbccp

build-size:
	$(MAKE) build VBCCOPTS="$(VBCCSIZEOPTS)" SUFFIX=vbccs

build:
	@echo ""
	@echo "Building $(PRG) for $(SUFFIX) ..."
	@echo "----------------------------------------------------------------------"

	@mkdir -p "$(BINDIR)"

	PATH="$(VBCC)/bin:$(PATH)"; "$(VBCC)/bin/$(VC)" $(VBCCTARGET) $(VBCCOPTS) "$(SRCDIR)/$(PRG).c" $(LIBS) $(EXTRA) -o "$(BINDIR)/$(PRG)-$(SUFFIX).prg"

clean:
	@echo ""
	@echo "Cleaning vbcc ..."
	@echo "----------------------------------------------------------------------"

	@$(RM) -rf "$(BINDIR)"

exec-perf:
	$(MAKE) exec SUFFIX=vbccp

exec-size:
	$(MAKE) exec SUFFIX=vbccs

exec:
	@echo ""
	@echo "Executing $(PRG) for $(SUFFIX) ..."
	@echo "----------------------------------------------------------------------"

	@$(RM) -f "$(BINDIR)/$(PRG)-$(SUFFIX).log"

	$(VICEEXEC) -autostart "$(BINDIR)/$(PRG)-$(SUFFIX).prg" \
		-initbreak 0xc000 \
		-moncommands "$(SRCDIR)/vice.mon" \
		-monlog \
		-monlogname "$(BINDIR)/$(PRG)-$(SUFFIX).log"

	cat "$(BINDIR)/$(PRG)-$(SUFFIX).log" 



