CURDIR := $(shell pwd)
BINDIR := $(CURDIR)/bin

CLANG = "$(CURDIR)/llvm-mos/bin/mos-c64-clang"
CLANGTARGET = -DLLVM -D__MATHH_MISSING
CLANGPERFOPTS = -O2 -flto -fnonreentrant
CLANGSIZEOPTS = -Os -flto -fnonreentrant

# Trap at the end of the SYS call in $e147
VICETRAP = 0xc000

version:
	@echo ""
	@echo "Collecting version information of llvm ..."
	@echo "----------------------------------------------------------------------"
	
	@$(CLANG) --version | grep "clang version" | awk '{print $$3}' > "llvm.version" 2>&1

	@date +"%Y-%m-%d %H:%M:%S" > "llvm.date"
	
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

	$(VICEEXEC) $(VICEOPTS) -autostart "$(BINDIR)/$(PRG)-$(SUFFIX).prg" \
		-initbreak $(VICETRAP) \
		-moncommands "$(SRCDIR)/vice.mon" \
		-monlog \
		-monlogname "$(BINDIR)/$(PRG)-$(SUFFIX).log"

	@if [ -f "$(BINDIR)/vice.png" ]; then mv "$(BINDIR)/vice.png" "$(BINDIR)/$(PRG)-$(SUFFIX).png"; fi

	cat "$(BINDIR)/$(PRG)-$(SUFFIX).log" 



