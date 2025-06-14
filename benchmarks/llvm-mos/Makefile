CURDIR := $(shell pwd)
BINDIR := $(CURDIR)/bin

CLANG = "$(CURDIR)/llvm-mos/bin/mos-c64-clang"
#CLANGTARGET = --config /mnt/media/build/llvm-mos-sdk/build/commodore/64.cfg
CLANGTARGET = 
CLANGOPTS = -Os

build:
	mkdir -p "$(BINDIR)"
	
	$(CLANG) $(CLANGTARGET) $(CLANGOPTS) "$(SRCDIR)/$(PRG).c" $(LIBS) $(EXTRA) -o "$(BINDIR)/$(PRG)-llvm.prg"

clean:
	rm -rf "$(BINDIR)"

exec:
	rm -f "$(BINDIR)/$(PRG)-llvm.log"

	$(VICEEXEC) -autostart "$(BINDIR)/$(PRG)-llvm.prg" \
		-initbreak 0xc000 \
		-moncommands "$(SRCDIR)/vice.mon" \
		-monlog \
		-monlogname "$(BINDIR)/$(PRG)-llvm.log"

	cat "$(BINDIR)/$(PRG)-llvm.log" 



