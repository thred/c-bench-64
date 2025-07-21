CURDIR := $(shell pwd)
BINDIR := $(CURDIR)/bin

SDCCDIR := $(CURDIR)/sdcc
SDCCINCLUDEDIR := $(SDCCDIR)/share/sdcc/include
SDCCLIBDIR := $(SDCCDIR)/share/sdcc/lib
SDAS = $(SDCCDIR)/bin/sdas6500
SDCC = $(SDCCDIR)/bin/sdcc
MAKEBIN = $(SDCCDIR)/bin/makebin
SDCCTARGET = -mmos6502 -I"$(SDCCINCLUDEDIR)" --lib-path "$(SDCCLIBDIR)"
# SDCCTARGET = -mmos6502  -I"$(SDCCINCLUDEDIR)" --lib-path "$(SDCCLIBDIR)" --fverbose-asm --i-code-in-asm
SDCCPERFOPTS = --max-allocs-per-node 250000 --opt-code-speed
SDCCSIZEOPTS = --max-allocs-per-node 250000 --opt-code-size

# Install a trap at kernal GETIN
VICETRAP = 0xffe4

version:
	@echo ""
	@echo "Collecting version information of sdcc ..."
	@echo "----------------------------------------------------------------------"

	@$(SDCC) --version | grep "SDCC" | awk '{print $$5}' > "sdcc.version" 2>&1

	@date +"%Y-%m-%d %H:%M:%S" > "sdcc.date"
	
build-perf:
	$(MAKE) SDCCOPTS="$(SDCCPERFOPTS)" SUFFIX=sdccp build

build-size:
	$(MAKE) SDCCOPTS="$(SDCCSIZEOPTS)" SUFFIX=sdccs build

build:
	@echo ""
	@echo "Building $(PRG) for $(SUFFIX) ..."
	@echo "----------------------------------------------------------------------"

	@mkdir -p "$(BINDIR)/$(SUFFIX)"

	$(SDAS) -og "$(BINDIR)/$(SUFFIX)/c64_crt.rel" "$(SRCDIR)/c64_crt.s"

	@echo ""

	$(SDCC) $(SDCCTARGET) -c "$(SRCDIR)/c64lib.c" -o "$(BINDIR)/$(SUFFIX)/" 

	@echo ""

	$(foreach LIB,$(LIBS),$(SDCC) $(SDCCTARGET) -c "$(LIB)" -o "$(BINDIR)/$(SUFFIX)/";)

	@echo ""

	$(SDCC) $(SDCCTARGET) $(SDCCOPTS) -c "$(SRCDIR)/$(PRG).c" -o "$(BINDIR)/$(SUFFIX)/"

	@echo ""

	$(SDCC) $(SDCCTARGET) --no-std-crt0 --code-loc 0x7ff -mmos6502 \
		"$(BINDIR)/$(SUFFIX)/c64_crt.rel" \
		"$(BINDIR)/$(SUFFIX)/c64lib.rel" \
		"$(BINDIR)/$(SUFFIX)/cia_timer.rel" \
		"$(BINDIR)/$(SUFFIX)/out.rel" \
		"$(BINDIR)/$(SUFFIX)/main.rel" \
		"$(BINDIR)/$(SUFFIX)/$(PRG).rel" \
		-o "$(BINDIR)/$(SUFFIX)/$(PRG).ihx"

	$(MAKEBIN) -o 2047 -p "$(BINDIR)/$(SUFFIX)/$(PRG).ihx" > "$(BINDIR)/$(PRG)-$(SUFFIX).prg"

clean:
	@echo ""
	@echo "Cleaning sdcc ..."
	@echo "----------------------------------------------------------------------"

	@$(RM) -rf "$(BINDIR)"

exec-perf:
	$(MAKE) SUFFIX=sdccp exec

exec-size:
	$(MAKE) SUFFIX=sdccs exec
	
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



