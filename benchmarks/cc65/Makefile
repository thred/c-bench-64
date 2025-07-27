CURDIR := $(shell pwd)
BINDIR := $(CURDIR)/bin

CC65DIR := $(CURDIR)/cc65
CC65 = $(CC65DIR)/bin/cl65
CC65TARGET = -t c64 -DCC65 -D__FLOAT_MISSING -D__MATHH_MISSING -D__LDIV_MISSING
CC65PERFOPTS = -Oisr --static-locals
CC65SIZEOPTS = -O --static-locals

# Trap at the end of the SYS call in $e147
VICETRAP = 0xe147

version:
	@echo ""
	@echo "Collecting version information of cc65 ..."
	@echo "----------------------------------------------------------------------"

	$(CC65) --version > "cc65.version" 2>&1
	@echo "$$(cat cc65.version | awk '{print $$2}')" > "cc65.version"

	@date +"%Y-%m-%d %H:%M:%S" > "cc65.date"

build-perf:
	CC65OPTS="$(CC65PERFOPTS)" SUFFIX=cc65p $(MAKE) build

build-size:
	CC65OPTS="$(CC65SIZEOPTS)" SUFFIX=cc65s $(MAKE) build

build:
	@echo ""
	@echo "Building $(PRG) for $(SUFFIX) ..."
	@echo "----------------------------------------------------------------------"

	@mkdir -p "$(BINDIR)"

	$(CC65) $(CC65OPTS) $(CC65TARGET) "$(SRCDIR)/$(PRG).c" $(LIBS) $(EXTRA) \
		-o "$(BINDIR)/$(PRG)-$(SUFFIX).prg" \
		-m "$(BINDIR)/$(PRG)-$(SUFFIX).map" \
		-l "$(BINDIR)/$(PRG)-$(SUFFIX).lst"	

clean:
	@echo ""
	@echo "Cleaning cc65 ..."
	@echo "----------------------------------------------------------------------"

	@$(RM) -rf "${BINDIR}"

exec-perf:
	SUFFIX=cc65p $(MAKE) exec

exec-size:
	SUFFIX=cc65s $(MAKE) exec

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

	cat "$(BINDIR)/$(PRG)-$(SUFFIX).log" 
