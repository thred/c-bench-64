CURDIR := $(shell pwd)
BINDIR := $(CURDIR)/bin

OSCAR64 = "$(CURDIR)/oscar64/bin/oscar64"
OSCAR64TARGET = -tf=prg -tm=c64 -psci -D OSCAR64 
OSCAR64PERFOPTS = -Os
OSCAR64SIZEOPTS = -O3

build-perf:
	$(MAKE) build OSCAR64OPTS="$(OSCAR64PERFOPTS)" SUFFIX=o64p

build-size:
	$(MAKE) build OSCAR64OPTS="$(OSCAR64SIZEOPTS)" SUFFIX=o64s

build:
	mkdir -p "$(BINDIR)"
	
	$(OSCAR64) $(OSCAR64TARGET) $(OSCAR64OPTS) "$(SRCDIR)/$(PRG).c" $(LIBS) $(EXTRA) -o="$(BINDIR)/$(PRG)-$(SUFFIX).prg"

clean:
	rm -rf "$(BINDIR)"

exec-perf:
	$(MAKE) exec SUFFIX=o64p

exec-size:
	$(MAKE) exec SUFFIX=o64s

exec:
	rm -f "$(BINDIR)/$(PRG)-$(SUFFIX).log"

	$(VICEEXEC) -autostart "$(BINDIR)/$(PRG)-$(SUFFIX).prg" \
		-initbreak 0xc000 \
		-moncommands "$(SRCDIR)/vice.mon" \
		-monlog \
		-monlogname "$(BINDIR)/$(PRG)-$(SUFFIX).log"

	cat "$(BINDIR)/$(PRG)-$(SUFFIX).log" 



