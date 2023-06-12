PWD := $(shell pwd)
BUILD_DIR ?= $(PWD)/build
BUILD_DIR_MAKEFILE ?= $(PWD)/build/Makefile
CROSS ?= aarch64-none-linux-gnu-
KERNEL ?= ../linux-apple

obj-m += el3_demo_fib.o

default: $(BUILD_DIR_MAKEFILE)
	make ARCH=arm64 CROSS_COMPILE=$(CROSS) -C $(KERNEL) M=$(BUILD_DIR) src=$(PWD) modules

$(BUILD_DIR):
	mkdir -p "$@"

$(BUILD_DIR_MAKEFILE): $(BUILD_DIR)
	touch "$@"

clean:
	make -C $(KERNEL) M=$(BUILD_DIR) src=$(PWD) clean
	rm $(BUILD_DIR_MAKEFILE)
