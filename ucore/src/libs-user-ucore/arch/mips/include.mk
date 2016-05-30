ifeq  ($(ON_FPGA), y)
MACH_DEF := -DMACH_FPGA
else
MACH_DEF := -DMACH_QEMU
endif

ARCH_CFLAGS := -mips1 -fno-builtin -nostdlib  -nostdinc -g  -EL -G0 -Wformat -O0 -mno-float $(MACH_DEF)
ARCH_LDFLAGS := 
ARCH_OBJS := syscall.o initcode.o intr.o clone.o udivmod.o udivmodsi4.o divmod.o
ARCH_INITCODE_OBJ := initcode.o
