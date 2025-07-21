CURDIR := $(shell pwd)
BINDIR := $(CURDIR)/bin

KICKASSEMBLER = KickAssembler/KickAss.jar

KICKCWORKDIR = "KickC/bin"
KICKC = sh kickc.sh
KICKCTARGET = -t c64 -DKICKC
KICKCOPTS = -Oliverangecallpath

# Install a trap at kernal GETIN
VICETRAP = 0xffe4

version:
	@echo ""
	@echo "Collecting version information of kickc ..."
	@echo "----------------------------------------------------------------------"

	cd $(KICKCWORKDIR); $(KICKC) --version | grep KickC > "../../kickc.version" 2>&1

	@date +"%Y-%m-%d %H:%M:%S" > "kickc.date"
	
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

	$(VICEEXEC) $(VICEOPTS) -autostart "$(BINDIR)/$(PRG)-kickc.prg" \
		-initbreak $(VICETRAP) \
		-moncommands "$(SRCDIR)/vice.mon" \
		-monlog \
		-monlogname "$(BINDIR)/$(PRG)-kickc.log"

	@if [ -f "$(BINDIR)/vice.png" ]; then mv "$(BINDIR)/vice.png" "$(BINDIR)/$(PRG)-$(SUFFIX).png"; fi

	cat "$(BINDIR)/$(PRG)-kickc.log" 



