CURDIR := $(shell pwd)
BINDIR := $(CURDIR)/bin

KICKASSEMBLER = KickAssembler/KickAss.jar

KICKCWORKDIR = "KickC/bin"
KICKC = sh kickc.sh
KICKCTARGET = -t c64 -DKICKC
KICKCOPTS = -Oliverangecallpath

version:
	@echo ""
	@echo "Collecting version information of kickc ..."
	@echo "----------------------------------------------------------------------"

	cd $(KICKCWORKDIR); $(KICKC) --version | grep KickC > "../../kickc.version" 2>&1
	
build:
	@echo ""
	@echo "Building $(PRG) for kickc ..."
	@echo "----------------------------------------------------------------------"

	@mkdir -p "$(BINDIR)"

	cd $(KICKCWORKDIR); \
		$(KICKC) $(KICKCOPTS) $(KICKCTARGET) "$(SRCDIR)/$(PRG).c" $(LIBS) $(EXTRA) \
			-odir "$(BINDIR)" -o $(PRG)-kickc.asm

	java -jar $(KICKASSEMBLER) "$(BINDIR)/$(PRG)-kickc.asm"

clean:
	@echo ""
	@echo "Cleaning kickc ..."
	@echo "----------------------------------------------------------------------"

	@$(RM) -rf "$(BINDIR)"

exec:
	@echo ""
	@echo "Executing $(PRG) for kickc ..."
	@echo "----------------------------------------------------------------------"

	@$(RM) -f "$(BINDIR)/$(PRG)-kickc.log"

	$(VICEEXEC) -autostart "$(BINDIR)/$(PRG)-kickc.prg" \
		-initbreak 0xc000 \
		-moncommands "$(SRCDIR)/vice.mon" \
		-monlog \
		-monlogname "$(BINDIR)/$(PRG)-kickc.log"

	cat "$(BINDIR)/$(PRG)-kickc.log" 



