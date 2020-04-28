CFLAGS= -I../inc -I../util -I.
CC= gcc
OBJSDIR = ./build

.PHONY: all clean install

TARGETS=example_jsmn
OBJS=./build/jsmn_util.o ./build/jsmn.o

all: $(TARGETS) 

example_jsmn: $(OBJS) ./build/example_jsmn.o
	${CC} $^ -g -o $@


./build/jsmn.o : ../src/jsmn.c
	${CC} -g -c $^ -o $@ ${CFLAGS} 
./build/jsmn_util.o : ../util/jsmn_util.c
	${CC} -g -c $^ -o $@ ${CFLAGS} 
./build/example_jsmn.o : example_jsmn.c
	${CC} -g -c $^ -o $@ ${CFLAGS} 

install:
	@if [ ! -d $(OBJSDIR) ]; then \
		mkdir -p $(OBJSDIR); \
	fi

clean:
	$(RM) ./*.exe
	$(RM) $(TARGETS)
	$(RM) $(OBJS)
	$(RM) ./build/example_jsmn.o
	