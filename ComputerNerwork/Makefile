CC = gcc
CFLAGS = -Wno-implict-function-declearation -g -MD
LFLAGS =  -lhiredis -lpthread
INC = -Iinclude

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))

.PHONY: depend clean all
DEPS:= $(OBJS:.o=.d)

all: main

-include $(DEPS)

main: $(OBJS)
	@$(CC) $^ -o $@ $(LFLAGS)

obj/%.o:src/%.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@rm obj/*.o obj/*.d ./main