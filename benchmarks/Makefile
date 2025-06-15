CURDIR := $(shell pwd)

INCLUDE = cc65p,cc65s,kickc,llvm,o64p,o64s,sdcc,vbccp,vbccs

export SRCDIR = $(CURDIR)/src

export LIBS = "$(SRCDIR)/cia_timer.c" "$(SRCDIR)/main.c"

export VICEEXEC = flatpak run net.sf.VICE -config "$(SRCDIR)/vice.ini" -warp

all:
	$(MAKE) build-all
	$(MAKE) exec-all

build-all:	
	$(MAKE) build-aes256
	$(MAKE) build-crc16
	$(MAKE) build-crc32			
	$(MAKE) build-crc8
	$(MAKE) build-dhrystone
	$(MAKE) build-fact
	$(MAKE) build-pi
	$(MAKE) build-pow
	$(MAKE) build-puff2
	$(MAKE) build-sieve
	$(MAKE) build-sieve-bit

exec-all:
	$(MAKE) exec-aes256
	$(MAKE) exec-crc16
	$(MAKE) exec-crc32
	$(MAKE) exec-crc8
	$(MAKE) exec-dhrystone
	$(MAKE) exec-fact
	$(MAKE) exec-pi
	$(MAKE) exec-pow
	$(MAKE) exec-puff2
	$(MAKE) exec-sieve
	$(MAKE) exec-sieve-bit

aes256:
	$(MAKE) build-aes256
	$(MAKE) exec-aes256
build-aes256:
	PRG=aes256 EXCLUDE=kickc $(MAKE) build
exec-aes256:
	PRG=aes256 EXCLUDE=kickc $(MAKE) exec

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
	PRG=crc16 EXCLUDE=kickc $(MAKE) build
exec-crc16:
	PRG=crc16 EXCLUDE=kickc $(MAKE) exec

crc32:
	$(MAKE) build-crc32
	$(MAKE) exec-crc32
build-crc32:
	PRG=crc32 EXCLUDE=kickc $(MAKE) build
exec-crc32:
	PRG=crc32 EXCLUDE=kickc $(MAKE) exec

dhrystone:
	$(MAKE) build-dhrystone
	$(MAKE) exec-dhrystone
build-dhrystone:
	PRG=dhrystone EXCLUDE=kickc $(MAKE) build
exec-dhrystone:
	PRG=dhrystone EXCLUDE=kickc $(MAKE) exec

fact:
	$(MAKE) build-fact
	$(MAKE) exec-fact
build-fact:
	PRG=fact EXCLUDE=kickc $(MAKE) build
exec-fact:
	PRG=fact EXCLUDE=kickc $(MAKE) exec

pi:
	$(MAKE) build-pi
	$(MAKE) exec-pi
build-pi:
	PRG=pi EXCLUDE=kickc $(MAKE) build
exec-pi:
	PRG=pi EXCLUDE=kickc $(MAKE) exec

pow:
	$(MAKE) build-pow
	$(MAKE) exec-pow
build-pow:
	PRG=pow EXCLUDE="cc65s cc65p kickc" $(MAKE) build
exec-pow:
	PRG=pow EXCLUDE="cc65s cc65p kickc" $(MAKE) exec

puff2:
	$(MAKE) build-puff2
	$(MAKE) exec-puff2
build-puff2:
	PRG=puff2 EXCLUDE=kickc $(MAKE) build
exec-puff2:
	PRG=puff2 EXCLUDE=kickc $(MAKE) exec

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
	if echo "$(INCLUDE)" | grep -q "cc65p" && ! echo "$(EXCLUDE)" | grep -q "cc65p" ; then \
		$(MAKE) -C cc65 build-perf; \
	fi

	if echo "$(INCLUDE)" | grep -q "cc65s" && ! echo "$(EXCLUDE)" | grep -q "cc65s" ; then \
		$(MAKE) -C cc65 build-size; \
	fi

	if echo "$(INCLUDE)" | grep -q "kickc" && ! echo "$(EXCLUDE)" | grep -q "kickc" ; then \
		$(MAKE) -C kickc build; \
	fi

	if echo "$(INCLUDE)" | grep -q "llvm" && ! echo "$(EXCLUDE)" | grep -q "llvm" ; then \
		$(MAKE) -C llvm-mos build; \
	fi

	if echo "$(INCLUDE)" | grep -q "o64p" && ! echo "$(EXCLUDE)" | grep -q "o64p" ; then \
		$(MAKE) -C oscar64 build-perf; \
	fi

	if echo "$(INCLUDE)" | grep -q "o64s" && ! echo "$(EXCLUDE)" | grep -q "o64s" ; then \
		$(MAKE) -C oscar64 build-size; \
	fi

	if echo "$(INCLUDE)" | grep -q "sdcc" && ! echo "$(EXCLUDE)" | grep -q "sdcc" ; then \
		$(MAKE) -C sdcc build; \
	fi

	if echo "$(INCLUDE)" | grep -q "vbccp" && ! echo "$(EXCLUDE)" | grep -q "vbccp" ; then \
		$(MAKE) -C vbcc build-perf; \
	fi

	if echo "$(INCLUDE)" | grep -q "vbccs" && ! echo "$(EXCLUDE)" | grep -q "vbccs" ; then \
		$(MAKE) -C vbcc build-size; \
	fi


clean:
	if echo "$(INCLUDE)" | grep -q "CC65" && ! echo "$(EXCLUDE)" | grep -q "CC65" ; then \
		$(MAKE) -C cc65 clean; \
	fi

	if echo "$(INCLUDE)" | grep -q "kickc" && ! echo "$(EXCLUDE)" | grep -q "kickc" ; then \
		$(MAKE) -C kickc clean; \
	fi

	if echo "$(INCLUDE)" | grep -q "llvm" && ! echo "$(EXCLUDE)" | grep -q "llvm" ; then \
		$(MAKE) -C llvm-mos clean; \
	fi

	if echo "$(INCLUDE)" | grep -q "o64" && ! echo "$(EXCLUDE)" | grep -q "o64" ; then \
		$(MAKE) -C oscar64 clean; \
	fi

	if echo "$(INCLUDE)" | grep -q "sdcc" && ! echo "$(EXCLUDE)" | grep -q "sdcc" ; then \
		$(MAKE) -C sdcc clean; \
	fi

	if echo "$(INCLUDE)" | grep -q "vbcc" && ! echo "$(EXCLUDE)" | grep -q "vbcc" ; then \
		$(MAKE) -C vbcc clean; \
	fi

exec:
	if echo "$(INCLUDE)" | grep -q "cc65p" && ! echo "$(EXCLUDE)" | grep -q "cc65p" ; then \
		$(MAKE) -C cc65 exec-perf; \
	fi

	if echo "$(INCLUDE)" | grep -q "cc65s" && ! echo "$(EXCLUDE)" | grep -q "cc65s" ; then \
		$(MAKE) -C cc65 exec-size; \
	fi

	if echo "$(INCLUDE)" | grep -q "kickc" && ! echo "$(EXCLUDE)" | grep -q "kickc" ; then \
		$(MAKE) -C kickc exec; \
	fi

	if echo "$(INCLUDE)" | grep -q "llvm" && ! echo "$(EXCLUDE)" | grep -q "llvm" ; then \
		$(MAKE) -C llvm-mos exec; \
	fi

	if echo "$(INCLUDE)" | grep -q "o64p" && ! echo "$(EXCLUDE)" | grep -q "o64p" ; then \
		$(MAKE) -C oscar64 exec-perf; \
	fi

	if echo "$(INCLUDE)" | grep -q "o64s" && ! echo "$(EXCLUDE)" | grep -q "o64s" ; then \
		$(MAKE) -C oscar64 exec-size; \
	fi

	if echo "$(INCLUDE)" | grep -q "sdcc" && ! echo "$(EXCLUDE)" | grep -q "sdcc" ; then \
		$(MAKE) -C sdcc exec; \
	fi

	if echo "$(INCLUDE)" | grep -q "vbccp" && ! echo "$(EXCLUDE)" | grep -q "vbccp" ; then \
		$(MAKE) -C vbcc exec-perf; \
	fi
	
	if echo "$(INCLUDE)" | grep -q "vbccs" && ! echo "$(EXCLUDE)" | grep -q "vbccs" ; then \
		$(MAKE) -C vbcc exec-size; \
	fi