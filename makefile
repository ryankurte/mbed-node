# Helper makefile for installing components

DEVICE=EFR32FG12P433F1024GL125
OUTDIR=./BUILD
OUTPUT=${OUTDIR}/EFR32FG12_BRD4254A/GCC_ARM/mbed-node
BIN=${OUTPUT}.bin
ELF=${OUTPUT}.elf

IPS=192.168.2.58 192.168.2.87 192.168.3.16

configurefile = $(sed -ei '' 's/{DEVICE}/${DEVICE}/g' $0  $1)

build-debug:
	mbed-cli compile --profile mbed-os/tools/profiles/debug.json

build-dev:
	mbed-cli compile

build-release:
	mbed-cli compile --profile mbed-os/tools/profiles/release.json

${OUTDIR}/%.jlink: toolchain/%.jlink
	cp $< $@
	sed -i'' 's|$${DEVICE}|${DEVICE}|g' $@
	sed -i'' 's|$${BIN}|${BIN}|g' $@
	sed -i'' 's|$${ELF}|${ELF}|g' $@

flash:
	mbed-cli compile --flash

f:  build-debug ${OUTDIR}/flash.jlink
	JLinkExe -if SWD -device ${DEVICE} -Speed 4000 -CommanderScript  ${OUTDIR}/flash.jlink

ds: 
	JLinkGDBServer -if SWD -device ${DEVICE} -Speed 4000

d:  ${OUTDIR}/gdb.jlink
	arm-none-eabi-gdb --tui --se=${OUTPUT}.elf --command=${OUTDIR}/gdb.jlink

fa: build-debug ${OUTDIR}/flash-all.jlink
	JLinkExe -if SWD -device ${DEVICE} -Speed 4000 -CommanderScript ${OUTDIR}/flash-all.jlink



update:
	mbed-cli update

install:
	pip install mbed-cli

clean:
	rm -rf BUILD/

.phony: build-dev build-debug build-release flash update install clean
