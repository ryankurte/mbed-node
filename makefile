# Helper makefile for installing components

DEVICE=EFR32FG12P433F1024GL125
OUTPUT=./BUILD/EFR32FG12_SLWRB4254A/GCC_ARM/node-mbed

build-debug:
	mbed-cli compile --profile mbed-os/tools/profiles/debug.json

build-dev:
	mbed-cli compile

build-release:
	mbed-cli compile --profile mbed-os/tools/profiles/release.json

flash:
	mbed-cli compile --flash

f:
	JLinkExe -if SWD -device ${DEVICE} -Speed 4000 -CommanderScript toolchain/flash.jlink

ds:
	JLinkGDBServer -if SWD -device ${DEVICE} -Speed 4000

d:
	arm-none-eabi-gdb --tui --se=${OUTPUT}.elf --command=toolchain/gdb.jlink

update:
	mbed-cli update

install:
	pip install mbed-cli

.phony: build-dev build-debug build-release flash update install
