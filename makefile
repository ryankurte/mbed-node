# Helper makefile for installing components

build:
	mbed-cli compile

build-debug:
	mbed-cli --profile mbed-os/tools/profiles/debug.json

flash:
	mbed-cli compile --flash

update:
	mbed-cli update

install:
	pip install mbed-cli

