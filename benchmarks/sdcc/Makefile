CURDIR := $(shell pwd)
BINDIR := $(CURDIR)/bin

SDCCDIR := $(CURDIR)/sdcc
SDCC = $(SDCCDIR)/bin/sdcc
SDCCTARGET = -mmos6502 --fverbose-asm --i-code-in-asm
SDCCOPTS = --max-allocs-per-node 250000 --opt-code-speed

build:
	mkdir -p "$(BINDIR)"

	$(SDCCDIR)/bin/sdas6500 -og "$(BINDIR)/c64_crt.rel" "$(SRCDIR)/c64_crt.s"

	$(SDCC) $(SDCCTARGET) -c "$(SRCDIR)/c64lib.c" -o "$(BINDIR)/" 

	$(foreach LIB,$(LIBS),$(SDCC) $(SDCCTARGET) -c "$(LIB)" -o "$(BINDIR)/";)

	$(SDCC) $(SDCCTARGET) $(SDCCOPTS) -c "$(SRCDIR)/$(PRG).c" -o "$(BINDIR)/"

	$(SDCC) $(SDCCTARGET) --no-std-crt0 --code-loc 0x7ff -mmos6502 "$(BINDIR)/c64_crt.rel" "$(BINDIR)/c64lib.rel" \
		"$(BINDIR)/cia_timer.rel" "$(BINDIR)/main.rel" "$(BINDIR)/$(PRG).rel" -o "$(BINDIR)/$(PRG).ihx"

	$(SDCCDIR)/bin/makebin -o 2047 -p "$(BINDIR)/$(PRG).ihx"  > "$(BINDIR)/$(PRG)-sdcc.prg"

clean:
	rm -rf "$(BINDIR)"

exec:
	rm -f "$(BINDIR)/$(PRG)-sdcc.log"

	$(VICEEXEC) -autostart "$(BINDIR)/$(PRG)-sdcc.prg" \
		-initbreak 0xc000 \
		-moncommands "$(SRCDIR)/vice.mon" \
		-monlog \
		-monlogname "$(BINDIR)/$(PRG)-sdcc.log"

	cat "$(BINDIR)/$(PRG)-sdcc.log" 



