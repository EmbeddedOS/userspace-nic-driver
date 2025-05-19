INC=-Idriver/ -Iutil/ -I.
SRC=driver/*.c util/*.c

all:
	gcc main.c -o app ${INC} ${SRC}