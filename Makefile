CROSS_COMPILE=../toolchain/bin/aarch64-none-linux-gnu-
INC=-Idriver/ -Iutil/ -I. -Inet/
SRC=driver/*.c util/*.c
CC=${CROSS_COMPILE}gcc
BIN=pcap.o
all:
	${CC} app/pcap.c -o ${BIN} ${INC} ${SRC} -static

install:
	cp ${BIN} ../rootfs
	../make_rootfs.sh

qemu:
	qemu-system-aarch64 -M virt -cpu cortex-a57 -nographic -kernel ../linux/arch/arm64/boot/Image -initrd ../rootfs.cpio.gz -net nic,model=e1000e
