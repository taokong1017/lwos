BASE_DIR   = $(CURDIR)
BUILD_DIR  = $(BASE_DIR)/build
OUT_DIR    = $(BASE_DIR)/out
CONFIG_DIR = $(BASE_DIR)/config


include $(BUILD_DIR)/compiler.mk


.PHONY: all menuconfig run dbg help

all:
	@echo "build all success"

run:
	@echo "run success"

dbg:
	@echo "debug success"

menuconfig:
	@python  $(CONFIG_DIR)/usr_config.py

help:
	@echo "make all:	make CROSS_COMPILE=aarch64-none-elf/bin/aarch64-none-elf- -j"
	@echo "make clean:	make CROSS_COMPILE=aarch64-none-elf/bin/aarch64-none-elf- clean"
	@echo "make run:	make CROSS_COMPILE=aarch64-none-elf/bin/aarch64-none-elf- run"