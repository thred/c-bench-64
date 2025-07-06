CURDIR := $(shell pwd)
BINDIR := $(CURDIR)/bin

OSCAR64 = "$(CURDIR)/oscar64/bin/oscar64"
OSCAR64TARGET = -tf=prg -tm=c64 -psci -dOSCAR64 -dNOLONG -dNOFLOAT -dHEAPCHECK -dNOBSSCLEAR -dNOZPCLEAR
OSCAR64PERFOPTS = -O3 
OSCAR64SIZEOPTS = -O -Os -Oo -Oz -Op

version: 
	@echo ""
	@echo "Collecting version information of o64 ..."
	@echo "----------------------------------------------------------------------"

	@$(OSCAR64) -v | grep "Starting oscar64" | awk '{print $$3}' > "o64.version" 2>&1

	@date +"%Y-%m-%d %H:%M:%S" > "o64.date"
	
build-perf:	
	$(MAKE) build OSCAR64OPTS="$(OSCAR64PERFOPTS)" SUFFIX=o64p

build-size:
	$(MAKE) build OSCAR64OPTS="$(OSCAR64SIZEOPTS)" SUFFIX=o64s

build:
	@echo ""
	@echo "Building $(PRG) for $(SUFFIX) ..."
	@echo "----------------------------------------------------------------------"

	@mkdir -p "$(BINDIR)"

	$(OSCAR64) $(OSCAR64TARGET) $(OSCAR64OPTS) "$(SRCDIR)/$(PRG).c" $(LIBS) $(EXTRA) -o="$(BINDIR)/$(PRG)-$(SUFFIX).prg"

clean:
	@echo ""
	@echo "Cleaning o64 ..."
	@echo "----------------------------------------------------------------------"

	@$(RM) -rf "$(BINDIR)"

exec-perf:
	$(MAKE) exec SUFFIX=o64p

exec-size:
	$(MAKE) exec SUFFIX=o64s

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



