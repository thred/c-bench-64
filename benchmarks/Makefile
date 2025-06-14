CURDIR := $(shell pwd)

INCLUDE = CC65P,CC65S,KICKC,SDCC

export SRCDIR = $(CURDIR)/src

export LIBS = "$(SRCDIR)/cia_timer.c" "$(SRCDIR)/main.c"

export VICEEXEC = flatpak run net.sf.VICE -config "$(SRCDIR)/vice.ini" -warp

aes256:
	$(MAKE) build-aes256
	$(MAKE) exec-aes256
build-aes256:
	PRG=aes256 EXCLUDE=KICKC $(MAKE) build
exec-aes256:
	PRG=aes256 EXCLUDE=KICKC $(MAKE) exec

crc8:
	$(MAKE) build-crc8
	$(MAKE) exec-crc8
build-crc8:
	PRG=crc8 $(MAKE) build
exec-crc8:
	PRG=crc8 $(MAKE) exec

crc16:
	$(MAKE) build-crc16
	$(MAKE) exec-crc16
build-crc16:
	PRG=crc16 EXCLUDE=KICKC $(MAKE) build
exec-crc16:
	PRG=crc16 EXCLUDE=KICKC $(MAKE) exec

crc32:
	$(MAKE) build-crc32
	$(MAKE) exec-crc32
build-crc32:
	PRG=crc32 EXCLUDE=KICKC $(MAKE) build
exec-crc32:
	PRG=crc32 EXCLUDE=KICKC $(MAKE) exec

dhrystone:
	$(MAKE) build-dhrystone
	$(MAKE) exec-dhrystone
build-dhrystone:
	PRG=dhrystone EXCLUDE=KICKC $(MAKE) build
exec-dhrystone:
	PRG=dhrystone EXCLUDE=KICKC $(MAKE) exec

fact:
	$(MAKE) build-fact
	$(MAKE) exec-fact
build-fact:
	PRG=fact EXCLUDE=KICKC $(MAKE) build
exec-fact:
	PRG=fact EXCLUDE=KICKC $(MAKE) exec

pi:
	$(MAKE) build-pi
	$(MAKE) exec-pi
build-pi:
	PRG=pi EXCLUDE=KICKC $(MAKE) build
exec-pi:
	PRG=pi EXCLUDE=KICKC $(MAKE) exec

pow:
	$(MAKE) build-pow
	$(MAKE) exec-pow
build-pow:
	PRG=pow EXCLUDE="CC65S CC65P KICKC" $(MAKE) build
exec-pow:
	PRG=pow EXCLUDE="CC65S CC65P KICKC" $(MAKE) exec

puff2:
	$(MAKE) build-puff2
	$(MAKE) exec-puff2
build-puff2:
	PRG=puff2 EXCLUDE=KICKC $(MAKE) build
exec-puff2:
	PRG=puff2 EXCLUDE=KICKC $(MAKE) exec

sieve:
	$(MAKE) build-sieve
	$(MAKE) exec-sieve
build-sieve:
	PRG=sieve $(MAKE) build
exec-sieve:
	PRG=sieve $(MAKE) exec

sieve-bit:
	$(MAKE) build-sieve-bit
	$(MAKE) exec-sieve-bit
build-sieve-bit:
	PRG=sieve_bit $(MAKE) build
exec-sieve-bit:
	PRG=sieve_bit $(MAKE) exec

build:
	if echo "$(INCLUDE)" | grep -q "CC65P" && ! echo "$(EXCLUDE)" | grep -q "CC65P" ; then \
		$(MAKE) -C cc65 build-perf; \
	fi

	if echo "$(INCLUDE)" | grep -q "CC65S" && ! echo "$(EXCLUDE)" | grep -q "CC65S" ; then \
		$(MAKE) -C cc65 build-size; \
	fi

	if echo "$(INCLUDE)" | grep -q "KICKC" && ! echo "$(EXCLUDE)" | grep -q "KICKC" ; then \
		$(MAKE) -C kickc build; \
	fi

	if echo "$(INCLUDE)" | grep -q "SDCC" && ! echo "$(EXCLUDE)" | grep -q "SDCC" ; then \
		$(MAKE) -C sdcc build; \
	fi


clean:
	if echo "$(INCLUDE)" | grep -q "CC65" && ! echo "$(EXCLUDE)" | grep -q "CC65" ; then \
		$(MAKE) -C cc65 clean; \
	fi

	if echo "$(INCLUDE)" | grep -q "KICKC" && ! echo "$(EXCLUDE)" | grep -q "KICKC" ; then \
		$(MAKE) -C kickc clean; \
	fi

	if echo "$(INCLUDE)" | grep -q "SDCC" && ! echo "$(EXCLUDE)" | grep -q "SDCC" ; then \
		$(MAKE) -C sdcc clean; \
	fi

exec:
	if echo "$(INCLUDE)" | grep -q "CC65P" && ! echo "$(EXCLUDE)" | grep -q "CC65P" ; then \
		$(MAKE) -C cc65 exec-perf; \
	fi

	if echo "$(INCLUDE)" | grep -q "CC65S" && ! echo "$(EXCLUDE)" | grep -q "CC65S" ; then \
		$(MAKE) -C cc65 exec-size; \
	fi

	if echo "$(INCLUDE)" | grep -q "KICKC" && ! echo "$(EXCLUDE)" | grep -q "KICKC" ; then \
		$(MAKE) -C kickc exec; \
	fi

	if echo "$(INCLUDE)" | grep -q "SDCC" && ! echo "$(EXCLUDE)" | grep -q "SDCC" ; then \
		$(MAKE) -C sdcc exec; \
	fi