SHELL := /bin/sh

ARM_CC ?= arm-none-eabi-gcc
ARM_CXX ?= arm-none-eabi-g++
NIX_BUILD ?= nix-build

.PHONY: all compile compile-arm-chainloader static-test test

all: test

compile: compile-arm-chainloader

compile-arm-chainloader:
	@if ! command -v "$(NIX_BUILD)" >/dev/null 2>&1; then \
		echo "error: $(NIX_BUILD) is required for compile-arm-chainloader" >&2; \
		exit 127; \
	fi
	$(NIX_BUILD) -A arm.chainloader --no-out-link

static-test:
	$(ARM_CC) -std=c11 -ffreestanding -fsyntax-only -I. tests/firmware_abi.c
	@test "$$(tr -d '\r' < arm_chainloader/start.s | grep -Ec '^  \.space 40, 0$$')" -eq 1
	@test "$$(tr -d '\r' < arm_chainloader/start.s | grep -Ec '^  \.long 0 /\* UART clock / reserved\[0\] \*/$$')" -eq 1
	@echo "static ABI checks passed"

test: static-test compile-arm-chainloader
