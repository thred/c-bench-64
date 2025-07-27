CURDIR := $(shell pwd)

# Currently kickc is mostly incompatible and does not even recognize #ifdef.
# It is not included in the default build.

# INCLUDE = cc65,kickc,llvm,o64,sdcc,vbcc

INCLUDE_ALL = cc65,kickc,llvm,o64,sdcc,vbcc
INCLUDE = cc65,llvm,o64,sdcc,vbcc

export SRCDIR = $(CURDIR)/src

BENCH_LIBS = "$(SRCDIR)/cia_timer.c" "$(SRCDIR)/out.c" "$(SRCDIR)/main.c"
TEST_LIBS = "$(SRCDIR)/cia_timer.c" "$(SRCDIR)/test.c"

export VICEEXEC = flatpak run net.sf.VICE -config "$(SRCDIR)/vice.ini" -warp -jamaction 2

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
	$(MAKE) build-test-math
	$(MAKE) build-test-stdio-file
	$(MAKE) build-test-stdio-print
	$(MAKE) build-test-stdio-scan
	$(MAKE) build-test-stdlib
	$(MAKE) build-test-string

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
	$(MAKE) exec-test-math
	$(MAKE) exec-test-stdio-file
	$(MAKE) exec-test-stdio-print
	$(MAKE) exec-test-stdio-scan
	$(MAKE) exec-test-stdlib
	$(MAKE) exec-test-string

aes256:
	$(MAKE) build-aes256
	$(MAKE) exec-aes256
build-aes256:
	PRG=aes256 LIBS='$(BENCH_LIBS)' $(MAKE) build
exec-aes256:
	PRG=aes256 $(MAKE) exec

crc8:
	$(MAKE) build-crc8
	$(MAKE) exec-crc8
build-crc8:
	PRG=crc8 LIBS='$(BENCH_LIBS)' $(MAKE) build
exec-crc8:
	PRG=crc8 $(MAKE) exec

crc16:
	$(MAKE) build-crc16
	$(MAKE) exec-crc16
build-crc16:
	PRG=crc16 LIBS='$(BENCH_LIBS)' $(MAKE) build
exec-crc16:
	PRG=crc16 $(MAKE) exec

crc32:
	$(MAKE) build-crc32
	$(MAKE) exec-crc32
build-crc32:
	PRG=crc32 LIBS='$(BENCH_LIBS)' $(MAKE) build
exec-crc32:
	PRG=crc32 $(MAKE) exec

dhrystone:
	$(MAKE) build-dhrystone
	$(MAKE) exec-dhrystone
build-dhrystone:
	PRG=dhrystone LIBS='$(BENCH_LIBS)' $(MAKE) build
exec-dhrystone:
	PRG=dhrystone $(MAKE) exec

fact:
	$(MAKE) build-fact
	$(MAKE) exec-fact
build-fact:
	PRG=fact LIBS='$(BENCH_LIBS)' $(MAKE) build
exec-fact:
	PRG=fact $(MAKE) exec

pi:
	$(MAKE) build-pi
	$(MAKE) exec-pi
build-pi:
	PRG=pi LIBS='$(BENCH_LIBS)' $(MAKE) build
exec-pi:
	PRG=pi $(MAKE) exec

pow:
	$(MAKE) build-pow
	$(MAKE) exec-pow
build-pow:
	PRG=pow LIBS='$(BENCH_LIBS)' EXCLUDE="cc65 kickc" $(MAKE) build
exec-pow:
	PRG=pow EXCLUDE="cc65 kickc" $(MAKE) exec

puff2:
	$(MAKE) build-puff2
	$(MAKE) exec-puff2
build-puff2:
	PRG=puff2 LIBS='$(BENCH_LIBS)' $(MAKE) build
exec-puff2:
	PRG=puff2 $(MAKE) exec

sieve:
	$(MAKE) build-sieve
	$(MAKE) exec-sieve
build-sieve:
	PRG=sieve LIBS='$(BENCH_LIBS)' $(MAKE) build
exec-sieve:
	PRG=sieve $(MAKE) exec

sieve-bit:
	$(MAKE) build-sieve-bit
	$(MAKE) exec-sieve-bit
build-sieve-bit:
	PRG=sieve_bit LIBS='$(BENCH_LIBS)' $(MAKE) build
exec-sieve-bit:
	PRG=sieve_bit $(MAKE) exec

test-math:
	$(MAKE) build-test-math
	$(MAKE) exec-test-math
build-test-math:
	PRG=test_math LIBS='$(TEST_LIBS)' $(MAKE) build
exec-test-math:
	PRG=test_math $(MAKE) exec

test-stdio-file:
	$(MAKE) build-test-stdio-file
	$(MAKE) exec-test-stdio-file
build-test-stdio-file:
	PRG=test_stdio_file LIBS='$(TEST_LIBS)' $(MAKE) build
exec-test-stdio-file:
	PRG=test_stdio_file VICEOPTS='-addconfig "$(SRCDIR)/vice-true-drive-emu.ini"' $(MAKE) exec

test-stdio-print:
	$(MAKE) build-test-stdio-print
	$(MAKE) exec-test-stdio-print
build-test-stdio-print:
	PRG=test_stdio_print LIBS='$(TEST_LIBS)' $(MAKE) build
exec-test-stdio-print:
	PRG=test_stdio_print $(MAKE) exec

test-stdio-scan:
	$(MAKE) build-test-stdio-scan
	$(MAKE) exec-test-stdio-scan
build-test-stdio-scan:
	PRG=test_stdio_scan LIBS='$(TEST_LIBS)' $(MAKE) build
exec-test-stdio-scan:
	PRG=test_stdio_scan $(MAKE) exec

test-stdlib:
	$(MAKE) build-test-stdlib
	$(MAKE) exec-test-stdlib
build-test-stdlib:
	PRG=test_stdlib LIBS='$(TEST_LIBS)' $(MAKE) build
exec-test-stdlib:
	PRG=test_stdlib $(MAKE) exec

test-string:
	$(MAKE) build-test-string
	$(MAKE) exec-test-string
build-test-string:
	PRG=test_string LIBS='$(TEST_LIBS)' $(MAKE) build
exec-test-string:
	PRG=test_string $(MAKE) exec

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