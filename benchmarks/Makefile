CURDIR := $(shell pwd)

# Currently kickc is mostly incompatible and does not even recognize #ifdef.
# It is not included in the default build.

# Removed sdcc, because it currently generates invalid code. Maybe it's my fault, but documentation is sparse.

# INCLUDE = cc65,kickc,llvm,o64,sdcc,vbcc

INCLUDE_ALL = cc65,kickc,llvm,o64,sdcc,vbcc
INCLUDE = cc65,llvm,o64,vbcc

export SRCDIR = $(CURDIR)/src

export LIBS = "$(SRCDIR)/cia_timer.c" "$(SRCDIR)/out.c" "$(SRCDIR)/main.c"

export VICEEXEC = flatpak run net.sf.VICE -config "$(SRCDIR)/vice.ini" -warp

all:
	$(MAKE) version
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
	PRG=aes256 $(MAKE) build
exec-aes256:
	PRG=aes256 $(MAKE) exec

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
	PRG=crc16 $(MAKE) build
exec-crc16:
	PRG=crc16 $(MAKE) exec

crc32:
	$(MAKE) build-crc32
	$(MAKE) exec-crc32
build-crc32:
	PRG=crc32 $(MAKE) build
exec-crc32:
	PRG=crc32 $(MAKE) exec

dhrystone:
	$(MAKE) build-dhrystone
	$(MAKE) exec-dhrystone
build-dhrystone:
	PRG=dhrystone $(MAKE) build
exec-dhrystone:
	PRG=dhrystone $(MAKE) exec

fact:
	$(MAKE) build-fact
	$(MAKE) exec-fact
build-fact:
	PRG=fact $(MAKE) build
exec-fact:
	PRG=fact $(MAKE) exec

pi:
	$(MAKE) build-pi
	$(MAKE) exec-pi
build-pi:
	PRG=pi $(MAKE) build
exec-pi:
	PRG=pi $(MAKE) exec

pow:
	$(MAKE) build-pow
	$(MAKE) exec-pow
build-pow:
	PRG=pow EXCLUDE="cc65 kickc" $(MAKE) build
exec-pow:
	PRG=pow EXCLUDE="cc65 kickc" $(MAKE) exec

puff2:
	$(MAKE) build-puff2
	$(MAKE) exec-puff2
build-puff2:
	PRG=puff2 $(MAKE) build
exec-puff2:
	PRG=puff2 $(MAKE) exec

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

version:
	@if echo "$(INCLUDE)" | grep -q "cc65" && ! echo "$(EXCLUDE)" | grep -q "cc65" ; then \
		$(MAKE) -C cc65 version; \
	fi

	@if echo "$(INCLUDE)" | grep -q "kickc" && ! echo "$(EXCLUDE)" | grep -q "kickc" ; then \
		$(MAKE) -C kickc version; \
	fi

	@if echo "$(INCLUDE)" | grep -q "llvm" && ! echo "$(EXCLUDE)" | grep -q "llvm" ; then \
		$(MAKE) -C llvm-mos version; \
	fi

	@if echo "$(INCLUDE)" | grep -q "o64" && ! echo "$(EXCLUDE)" | grep -q "o64" ; then \
		$(MAKE) -C oscar64 version; \
	fi

	@if echo "$(INCLUDE)" | grep -q "sdcc" && ! echo "$(EXCLUDE)" | grep -q "sdcc" ; then \
		$(MAKE) -C sdcc version; \
	fi

	@if echo "$(INCLUDE)" | grep -q "vbcc" && ! echo "$(EXCLUDE)" | grep -q "vbcc" ; then \
		$(MAKE) -C vbcc version; \
	fi

build:
	@if echo "$(INCLUDE)" | grep -q "cc65" && ! echo "$(EXCLUDE)" | grep -q "cc65" ; then \
		$(MAKE) -C cc65 build-perf; \
		$(MAKE) -C cc65 build-size; \
	fi

	@if echo "$(INCLUDE)" | grep -q "kickc" && ! echo "$(EXCLUDE)" | grep -q "kickc" ; then \
		$(MAKE) -C kickc build; \
	fi

	@if echo "$(INCLUDE)" | grep -q "llvm" && ! echo "$(EXCLUDE)" | grep -q "llvm" ; then \
		$(MAKE) -C llvm-mos build-perf; \
		$(MAKE) -C llvm-mos build-size; \
	fi

	@if echo "$(INCLUDE)" | grep -q "o64" && ! echo "$(EXCLUDE)" | grep -q "o64" ; then \
		$(MAKE) -C oscar64 build-perf; \
		$(MAKE) -C oscar64 build-size; \
	fi

	@if echo "$(INCLUDE)" | grep -q "sdcc" && ! echo "$(EXCLUDE)" | grep -q "sdcc" ; then \
		$(MAKE) -C sdcc build-perf; \
		$(MAKE) -C sdcc build-size; \
	fi

	@if echo "$(INCLUDE)" | grep -q "vbcc" && ! echo "$(EXCLUDE)" | grep -q "vbcc" ; then \
		$(MAKE) -C vbcc build-perf; \
		$(MAKE) -C vbcc build-size; \
	fi

clean-all:
	$(MAKE) INCLUDE=$(INCLUDE_ALL) clean
	
clean:
	@if echo "$(INCLUDE)" | grep -q "cc65" && ! echo "$(EXCLUDE)" | grep -q "cc65" ; then \
		$(MAKE) -C cc65 clean; \
	fi

	@if echo "$(INCLUDE)" | grep -q "kickc" && ! echo "$(EXCLUDE)" | grep -q "kickc" ; then \
		$(MAKE) -C kickc clean; \
	fi

	@if echo "$(INCLUDE)" | grep -q "llvm" && ! echo "$(EXCLUDE)" | grep -q "llvm" ; then \
		$(MAKE) -C llvm-mos clean; \
	fi

	@if echo "$(INCLUDE)" | grep -q "o64" && ! echo "$(EXCLUDE)" | grep -q "o64" ; then \
		$(MAKE) -C oscar64 clean; \
	fi

	@if echo "$(INCLUDE)" | grep -q "sdcc" && ! echo "$(EXCLUDE)" | grep -q "sdcc" ; then \
		$(MAKE) -C sdcc clean; \
	fi

	@if echo "$(INCLUDE)" | grep -q "vbcc" && ! echo "$(EXCLUDE)" | grep -q "vbcc" ; then \
		$(MAKE) -C vbcc clean; \
	fi

exec:
	@if echo "$(INCLUDE)" | grep -q "cc65" && ! echo "$(EXCLUDE)" | grep -q "cc65" ; then \
		$(MAKE) -C cc65 exec-perf; \
		$(MAKE) -C cc65 exec-size; \
	fi

	@if echo "$(INCLUDE)" | grep -q "kickc" && ! echo "$(EXCLUDE)" | grep -q "kickc" ; then \
		$(MAKE) -C kickc exec; \
	fi

	@if echo "$(INCLUDE)" | grep -q "llvm" && ! echo "$(EXCLUDE)" | grep -q "llvm" ; then \
		$(MAKE) -C llvm-mos exec-perf; \
		$(MAKE) -C llvm-mos exec-size; \
	fi

	@if echo "$(INCLUDE)" | grep -q "o64" && ! echo "$(EXCLUDE)" | grep -q "o64" ; then \
		$(MAKE) -C oscar64 exec-perf; \
		$(MAKE) -C oscar64 exec-size; \
	fi

	@if echo "$(INCLUDE)" | grep -q "sdcc" && ! echo "$(EXCLUDE)" | grep -q "sdcc" ; then \
		$(MAKE) -C sdcc exec-perf; \
		$(MAKE) -C sdcc exec-size; \
	fi

	@if echo "$(INCLUDE)" | grep -q "vbcc" && ! echo "$(EXCLUDE)" | grep -q "vbcc" ; then \
		$(MAKE) -C vbcc exec-perf; \
		$(MAKE) -C vbcc exec-size; \
	fi