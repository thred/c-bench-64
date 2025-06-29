CURDIR := $(shell pwd)
BINDIR := $(CURDIR)/bin

SDCCDIR := $(CURDIR)/sdcc
SDAS = $(SDCCDIR)/bin/sdas6500
SDCC = $(SDCCDIR)/bin/sdcc
MAKEBIN = $(SDCCDIR)/bin/makebin
SDCCTARGET = -mmos6502 --fverbose-asm --i-code-in-asm
SDCCPERFOPTS = --max-allocs-per-node 250000 --opt-code-speed
SDCCSIZEOPTS = --max-allocs-per-node 250000

build-perf:
	$(MAKE) SDCCOPTS="$(SDCCPERFOPTS)" SUFFIX=sdccp build

build-size:
	$(MAKE) SDCCOPTS="$(SDCCSIZEOPTS)" SUFFIX=sdccs build

build:
	mkdir -p "$(BINDIR)/$(SUFFIX)"

	$(SDCC) --version | grep "SDCC" | awk '{print $$5}' > "$(BINDIR)/$(PRG)-$(SUFFIX).buildversion" 2>&1

	@date +"%Y-%m-%d %H:%M:%S" > "$(BINDIR)/$(PRG)-$(SUFFIX).buildtime"

	$(SDAS) -og "$(BINDIR)/$(SUFFIX)/c64_crt.rel" "$(SRCDIR)/c64_crt.s"

	$(SDCC) $(SDCCTARGET) -c "$(SRCDIR)/c64lib.c" -o "$(BINDIR)/$(SUFFIX)/" 

	$(foreach LIB,$(LIBS),$(SDCC) $(SDCCTARGET) -c "$(LIB)" -o "$(BINDIR)/$(SUFFIX)/";)

	$(SDCC) $(SDCCTARGET) $(SDCCOPTS) -c "$(SRCDIR)/$(PRG).c" -o "$(BINDIR)/$(SUFFIX)/"

	$(SDCC) $(SDCCTARGET) --no-std-crt0 --code-loc 0x7ff -mmos6502 "$(BINDIR)/$(SUFFIX)/c64_crt.rel" "$(BINDIR)/$(SUFFIX)/c64lib.rel" \
		"$(BINDIR)/$(SUFFIX)/cia_timer.rel" "$(BINDIR)/$(SUFFIX)/main.rel" "$(BINDIR)/$(SUFFIX)/$(PRG).rel" -o "$(BINDIR)/$(SUFFIX)/$(PRG).ihx"

	$(MAKEBIN) -o 2047 -p "$(BINDIR)/$(SUFFIX)/$(PRG).ihx" > "$(BINDIR)/$(PRG)-$(SUFFIX).prg"

clean:
	$(RM) -rf "$(BINDIR)"

exec-perf:
	$(MAKE) SUFFIX=sdccp exec

exec-size:
	$(MAKE) SUFFIX=sdccs exec
	
exec:
	$(RM) -f "$(BINDIR)/$(PRG)-$(SUFFIX).log"

	$(VICEEXEC) -autostart "$(BINDIR)/$(PRG)-$(SUFFIX).prg" \
		-initbreak 0xc000 \
		-moncommands "$(SRCDIR)/vice.mon" \
		-monlog \
		-monlogname "$(BINDIR)/$(PRG)-$(SUFFIX).log"

	cat "$(BINDIR)/$(PRG)-$(SUFFIX).log" 



