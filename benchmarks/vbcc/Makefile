CURDIR := $(shell pwd)
BINDIR := $(CURDIR)/bin

export VBCC = $(CURDIR)/vbcc
VC = vc
VBCCTARGET = +c64 -lm
VBCCPERFOPTS = -O4 -speed
VBCCSIZEOPTS = -O4 -size

build-perf:
	$(MAKE) build VBCCOPTS="$(VBCCPERFOPTS)" SUFFIX=vbccp

build-size:
	$(MAKE) build VBCCOPTS="$(VBCCSIZEOPTS)" SUFFIX=vbccs

build:
	mkdir -p "$(BINDIR)"

	@date +"build-%Y-%m-%d" > "$(BINDIR)/$(PRG)-$(SUFFIX).buildversion"

	@date +"%Y-%m-%d %H:%M:%S" > "$(BINDIR)/$(PRG)-$(SUFFIX).buildtime"

	PATH="$(VBCC)/bin:$(PATH)"; "$(VBCC)/bin/$(VC)" $(VBCCTARGET) $(VBCCOPTS) "$(SRCDIR)/$(PRG).c" $(LIBS) $(EXTRA) -o "$(BINDIR)/$(PRG)-$(SUFFIX).prg"

clean:
	$(RM) -rf "$(BINDIR)"

exec-perf:
	$(MAKE) exec SUFFIX=vbccp

exec-size:
	$(MAKE) exec SUFFIX=vbccs

exec:
	$(RM) -f "$(BINDIR)/$(PRG)-$(SUFFIX).log"

	$(VICEEXEC) -autostart "$(BINDIR)/$(PRG)-$(SUFFIX).prg" \
		-initbreak 0xc000 \
		-moncommands "$(SRCDIR)/vice.mon" \
		-monlog \
		-monlogname "$(BINDIR)/$(PRG)-$(SUFFIX).log"

	cat "$(BINDIR)/$(PRG)-$(SUFFIX).log" 



