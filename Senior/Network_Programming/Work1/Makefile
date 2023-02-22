COMP = gcc

srcDest = ./src/
libDest = ./include/

SRCS = ${wildcard src/*.c}
OBJS = ${patsubst src/%.c, obj/%.o, ${SRCS}}

DEPS := $(OBJS:.o=.d)

shell.exe: shell.o
	${COMP} -o shell.exe shell.o
shell.o: ${srcDest}shell.c
	${COMP} -c ${srcDest}shell.c

.PHONY: depend clean all

clean:
	rm -rf ./*.o