CURDIR := $(shell pwd)
BINDIR := $(CURDIR)/bin

KICKASSEMBLER = KickAssembler/KickAss.jar

KICKCWORKDIR = "KickC/bin"
KICKC = sh kickc.sh
KICKCTARGET = -t c64 -DKICKC
KICKCOPTS = -Oliverangecallpath

build:
	mkdir -p "$(BINDIR)"

	cd $(KICKCWORKDIR); $(KICKC) --version | grep KickC > "$(BINDIR)/$(PRG)-kickc.buildversion"

	@date +"%Y-%m-%d %H:%M:%S" > "$(BINDIR)/$(PRG)-kicks.buildtime"

	cd $(KICKCWORKDIR); \
		$(KICKC) $(KICKCOPTS) $(KICKCTARGET) "$(SRCDIR)/$(PRG).c" $(LIBS) $(EXTRA) \
			-odir "$(BINDIR)" -o $(PRG)-kickc.asm

	java -jar $(KICKASSEMBLER) "$(BINDIR)/$(PRG)-kickc.asm"

clean:
	$(RM) -rf "$(BINDIR)"

exec:
	$(RM) -f "$(BINDIR)/$(PRG)-kickc.log"

	$(VICEEXEC) -autostart "$(BINDIR)/$(PRG)-kickc.prg" \
		-initbreak 0xc000 \
		-moncommands "$(SRCDIR)/vice.mon" \
		-monlog \
		-monlogname "$(BINDIR)/$(PRG)-kickc.log"

	cat "$(BINDIR)/$(PRG)-kickc.log" 



