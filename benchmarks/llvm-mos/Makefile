CURDIR := $(shell pwd)
BINDIR := $(CURDIR)/bin

CLANG = "$(CURDIR)/llvm-mos/bin/mos-c64-clang"
CLANGTARGET = 
CLANGPERFOPTS = -Os
CLANGSIZEOPTS = -Os -flto

build-perf:
	$(MAKE) CLANGOPTS="$(CLANGPERFOPTS)" SUFFIX=llvmp build

build-size:
	$(MAKE) CLANGOPTS="$(CLANGSIZEOPTS)" SUFFIX=llvms build

build:
	mkdir -p "$(BINDIR)"
	
	$(CLANG) $(CLANGTARGET) $(CLANGOPTS) "$(SRCDIR)/$(PRG).c" $(LIBS) $(EXTRA) -o "$(BINDIR)/$(PRG)-$(SUFFIX).prg"

clean:
	rm -rf "$(BINDIR)"

exec-perf:
	$(MAKE) SUFFIX=llvmp exec

exec-size:
	$(MAKE) SUFFIX=llvms exec

exec:
	rm -f "$(BINDIR)/$(PRG)-$(SUFFIX).log"

	$(VICEEXEC) -autostart "$(BINDIR)/$(PRG)-$(SUFFIX).prg" \
		-initbreak 0xc000 \
		-moncommands "$(SRCDIR)/vice.mon" \
		-monlog \
		-monlogname "$(BINDIR)/$(PRG)-$(SUFFIX).log"

	cat "$(BINDIR)/$(PRG)-$(SUFFIX).log" 



