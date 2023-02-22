CC = gcc
CFLAGS = -Wno-implicit-function-declaration -g -MD
LFLAGS = -lreadline
INC = -Iinclude

SRCS = ${wildcard src/*.c}

OBJS = ${patsubst src/%.c, obj/%.o, ${SRCS}}
DEPENDS = ${wildcard include/*.h}

DEPS := ${OBJS:.o=.d}

-include ${DEPS}

shell: ${OBJS}
	${CC} $^ -o bin/$@ # ${LFLAGS}

obj/%.o: src/%.c
	${CC} ${CFLAGS} ${INC} -c -p $@ $<

.PHONY:
	depend clean all

all:
	shell

clean:
	rm bin/*.o obj/*.d
