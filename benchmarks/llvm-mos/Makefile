CURDIR := $(shell pwd)
BINDIR := $(CURDIR)/bin

CLANG = "$(CURDIR)/llvm-mos/bin/mos-c64-clang"
CLANGTARGET = 
CLANGPERFOPTS = -Os
CLANGSIZEOPTS = -Os -flto

version:
	@echo ""
	@echo "Collecting version information of llvm ..."
	@echo "----------------------------------------------------------------------"
	
	@$(CLANG) --version | grep "clang version" | awk '{print $$3}' > "llvm.version" 2>&1
	
build-perf:
	$(MAKE) CLANGOPTS="$(CLANGPERFOPTS)" SUFFIX=llvmp build

build-size:
	$(MAKE) CLANGOPTS="$(CLANGSIZEOPTS)" SUFFIX=llvms build

build:
	@echo ""
	@echo "Building $(PRG) for $(SUFFIX) ..."
	@echo "----------------------------------------------------------------------"

	@mkdir -p "$(BINDIR)"

	$(CLANG) $(CLANGTARGET) $(CLANGOPTS) "$(SRCDIR)/$(PRG).c" $(LIBS) $(EXTRA) -o "$(BINDIR)/$(PRG)-$(SUFFIX).prg"

clean:
	@echo ""
	@echo "Cleaning llvm ..."
	@echo "----------------------------------------------------------------------"

	@$(RM) -rf "$(BINDIR)"

exec-perf:
	$(MAKE) SUFFIX=llvmp exec

exec-size:
	$(MAKE) SUFFIX=llvms exec

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



