CURDIR := $(shell pwd)
SRCDIR = "$(CURDIR)/../src"

TIMER = "$(SRCDIR)/cia_timer.c" "$(SRCDIR)/main.c"

KICKASSEMBLER = KickAssembler/KickAss.jar

KICKCWORKDIR = "$(CURDIR)/KickC/bin"
KICKC = sh kickc.sh
KICKCTARGET = -t c64 -DKICKC
KICKCOPTS = -Oliverangecallpath

VICEEXEC = flatpak run net.sf.VICE -config "$(SRCDIR)/vice.ini" -warp

all:
	$(MAKE) build-all
	$(MAKE) exec-all

aes256:
	$(MAKE) build-aes256
	$(MAKE) exec-aes256

crc8:
	$(MAKE) build-crc8
	$(MAKE) exec-crc8

crc16:
	$(MAKE) build-crc16
	$(MAKE) exec-crc16

crc32:
	$(MAKE) build-crc32
	$(MAKE) exec-crc32

dhrystone:
	$(MAKE) build-dhrystone
	$(MAKE) exec-dhrystone

fact:
	$(MAKE) build-fact
	$(MAKE) exec-fact

pi:
	$(MAKE) build-pi
	$(MAKE) exec-pi

pow:
	$(MAKE) build-pow
	$(MAKE) exec-pow

puff:
	$(MAKE) build-puff
	$(MAKE) exec-puff

puff2:
	$(MAKE) build-puff2
	$(MAKE) exec-puff2

sieve-bit:
	$(MAKE) build-sieve-bit
	$(MAKE) exec-sieve-bit

sieve:
	$(MAKE) build-sieve
	$(MAKE) exec-sieve

build-all:
	$(MAKE) build-aes256
	$(MAKE) build-crc8
	$(MAKE) build-crc16
	$(MAKE) build-crc32
	$(MAKE) build-dhrystone
	$(MAKE) build-fact
	$(MAKE) build-pi
#	$(MAKE) build-pow
#	$(MAKE) build-puff
	$(MAKE) build-puff2
	$(MAKE) build-sieve-bit
	$(MAKE) build-sieve

build-aes256:
	PRG=aes256 $(MAKE) build

build-crc8:
	PRG=crc8 $(MAKE) build

build-crc16:
	PRG=crc16 $(MAKE) build

build-crc32:
	PRG=crc32 $(MAKE) build

build-dhrystone:
	PRG=dhrystone $(MAKE) build

build-fact:
	PRG=fact $(MAKE) build

build-pi:
	PRG=pi $(MAKE) build

build-pow:
	PRG=pow $(MAKE) build

build-puff:
	PRG=puff $(MAKE) build

build-puff2:
	PRG=puff2 $(MAKE) build

build-sieve-bit:
	PRG=sieve_bit $(MAKE) build

build-sieve:
	PRG=sieve $(MAKE) build

build:
	mkdir -p target

	TARGETDIR="$(CURDIR)/target" SUFFIX=kickc $(MAKE) build-prg

build-prg:
	cd $(KICKCWORKDIR); \
		$(KICKC) $(KICKCOPTS) $(KICKCTARGET) $(SRCDIR)/$(PRG).c $(TIMER) $(EXTRA) \
			-odir $(TARGETDIR) \
			-DKICKC -o $(PRG)-$(SUFFIX).asm

	java -jar $(KICKASSEMBLER) $(TARGETDIR)/$(PRG)-$(SUFFIX).asm

exec-all:
	$(MAKE) exec-aes256
	$(MAKE) exec-crc8
	$(MAKE) exec-crc16
	$(MAKE) exec-crc32
	$(MAKE) exec-dhrystone
	$(MAKE) exec-fact
	$(MAKE) exec-pi
#	$(MAKE) exec-pow
#	$(MAKE) exec-puff
	$(MAKE) exec-puff2
	$(MAKE) exec-sieve-bit
	$(MAKE) exec-sieve

exec-aes256:
	PRG=aes256 $(MAKE) exec

exec-crc8:
	PRG=crc8 $(MAKE) exec

exec-crc16:
	PRG=crc16 $(MAKE) exec

exec-crc32:
	PRG=crc32 $(MAKE) exec

exec-dhrystone:
	PRG=dhrystone $(MAKE) exec

exec-fact:
	PRG=fact $(MAKE) exec

exec-pi:
	PRG=pi $(MAKE) exec

exec-pow:
	PRG=pow $(MAKE) exec

exec-puff:
	PRG=puff $(MAKE) exec

exec-puff2:
	PRG=puff2 $(MAKE) exec
		
exec-sieve-bit:
	PRG=sieve_bit $(MAKE) exec

exec-sieve:
	PRG=sieve $(MAKE) exec

exec:
	TARGETDIR="$(CURDIR)/target" SUFFIX=kickc $(MAKE) exec-prg

exec-prg:
	rm -f "$(TARGETDIR)/$(PRG)-$(SUFFIX).log"

	$(VICEEXEC) -autostart "$(TARGETDIR)/$(PRG)-$(SUFFIX).prg" \
		-initbreak 0xc000 \
		-moncommands "$(SRCDIR)/vice.mon" \
		-monlog \
		-monlogname "$(TARGETDIR)/$(PRG)-$(SUFFIX).log"

clean:
	rm -rf def perf size
