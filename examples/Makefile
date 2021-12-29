CFLAGS = -I../jsmn-1.0.0 -I../util -I. -DJSMN_PARENT_LINKS
CC = gcc
OBJSDIR = ./build

.PHONY: all clean

TARGETS = example_jsmn
OBJS = ./build/jsmn_util.o ./build/jsmn.o ./build/example_jsmn.o

all : $(TARGETS)

example_jsmn : $(OBJS)
	${CC} $^ -g -o $@

./build/jsmn.o : ../jsmn-1.0.0/jsmn.c
	${CC} -g -c $^ -o $@ ${CFLAGS}

./build/jsmn_util.o : ../util/jsmn_util.c
	@if [ ! -d $(OBJSDIR) ]; then \
		mkdir -p $(OBJSDIR); \
	fi
	${CC} -g -c $^ -o $@ ${CFLAGS}

./build/example_jsmn.o : example_jsmn.c
	${CC} -g -c $^ -o $@ ${CFLAGS}

clean:
	$(RM) ./*.exe
	$(RM) $(TARGETS)
	$(RM) $(OBJS)
