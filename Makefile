CC = gcc
CPP = g++
LIBS = -lgmp
LIBS_CPP = -lgmpxx
FLAGS = -O2

objs = nash_lib.o
headers = nash.h

mnash_objs = mnash.o $(objs)
nash_objs = nash.o $(objs)
nash_cpp_objs = nash.opp NashSieve.opp
mnash_cpp_objs = mnash.opp NashSieve.opp

.PHONY: all

all: nash mnash nash_cpp

nash: $(nash_objs)
	$(CC) -o $@ $(nash_objs) $(LIBS)

nash_cpp: $(nash_cpp_objs)
	$(CPP) -o $@ $(nash_cpp_objs) $(LIBS) $(LIBS_CPP)

mnash: $(mnash_objs)
	$(CC) -o $@ $(mnash_objs) $(LIBS)

mnash_cpp: $(mnash_cpp_objs)
	$(CPP) -o $@ $(mnash_cpp_objs) $(LIBS) $(LIBS_CPP)

%.o: %.c $(headers)
	$(CC) -c -o $@ $< $(FLAGS)

%.opp: %.cpp $(headers)
	$(CPP) -c -o $@ $< $(FLAGS)

clean: 
	rm -f nash mnash *.o
