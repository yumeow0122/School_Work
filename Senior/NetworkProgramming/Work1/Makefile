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
	${CC} $^ -o bin/$@

obj/%.o: src/%.c
	${CC} ${CFLAGS} ${INC} -c -o $@ $<

.PHONY:
	depend clean all

all:
	./bin/shell

clean:
	@rm bin/*.o obj/*.d