OS_CHECK := $(shell lsb_release -d 2>/dev/null | grep -q "Ubuntu 18.04" && echo "Ubuntu 18.04" || echo "Not Ubuntu 18.04")
# hack way to fit using at ubuntu 18.04
$(info OS ${OS_CHECK})
ifeq ($(OS_CHECK), Ubuntu 18.04)
    CROSS_COMPILE := riscv64-unknown-linux-gnu-
else
	CROSS_COMPILE := riscv64-linux-gnu-
endif

COMMON_CFLAGS := -fno-pic -march=rv64g -mcmodel=medany -mstrict-align
CFLAGS        += $(COMMON_CFLAGS) -static
ASFLAGS       += $(COMMON_CFLAGS) -O0
LDFLAGS       += -melf64lriscv

# overwrite ARCH_H defined in $(AM_HOME)/Makefile
ARCH_H := arch/riscv.h
