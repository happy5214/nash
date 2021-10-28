CC = gcc
LIBS = -lgmp -lm
FLAGS = -O2

objs = nash_lib.o
headers = nash.h

mnash_objs = mnash.o $(objs)
nash_objs = nash.o $(objs)

.PHONY: all

all: nash mnash

nash: $(nash_objs)
	$(CC) -o $@ $(nash_objs) $(LIBS)

mnash: $(mnash_objs)
	$(CC) -o $@ $(mnash_objs) $(LIBS)

%.o: %.c $(headers)
	$(CC) -c -o $@ $< $(FLAGS)

clean: 
	rm -f nash mnash *.o
