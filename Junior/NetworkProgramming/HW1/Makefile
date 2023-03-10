CC = gcc
CFLAGS = -Wno-implicit-function-declaration -g -MD
LFLAGS = -lreadline
INC = -Iinclude

SRCS = ${wildcard src/*.c}

OBJS = ${patsubst src/%.c, obj/%.c, ${SRCS}}
DEPENDS = ${wildcard include/*.h}
DEPS := ${OBJS:.o=.d}

.PHONY: depend clean all

all: shell
-include ${DEPS}

shell: ${OBJS}
	${CC} $^ -o bin/$@ ${LFLAGS}

obj/%.o: src/%.c
	${CC} ${CFLAGS} ${INC} -c -o $@ $<

.PHONY:
	depend clean all

clean:
	@rm -f bin/* obj/*.o obj/*.d