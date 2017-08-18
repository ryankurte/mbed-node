# Helper makefile for installing components

build:
	mbed-cli compile

build-debug:
	mbed-cli compile --profile mbed-os/tools/profiles/debug.json

build-release:
	mbed-cli compile --profile mbed-os/tools/profiles/release.json

flash:
	mbed-cli compile --flash

update:
	mbed-cli update

install:
	pip install mbed-cli

