CPP = g++
LIBS = -lgmp -lgmpxx
FLAGS = -O2

objs = NashSieve.o arg_parser.o
headers = NashSieve.h

mnash_objs = mnash.o $(objs)
nash_objs = nash.o $(objs)

.PHONY: all clean

all: nash mnash

nash: $(nash_objs)
	$(CPP) -o $@ $(nash_objs) $(LIBS)

mnash: $(mnash_objs)
	$(CPP) -o $@ $(mnash_objs) $(LIBS)

%.o: %.cpp $(headers)
	$(CPP) -c -o $@ $< $(FLAGS)

%.o: %.cc
	$(CPP) -c -o $@ $< $(FLAGS)

clean: 
	rm -f nash mnash *.o
