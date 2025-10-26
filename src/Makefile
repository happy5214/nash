CPP = g++
LIBS = -lgmp -lgmpxx
FLAGS = -O2

nash_objs = NashSieve.o
arg_parser_objs = arg_parser.o
mnash_app_objs = mnash.o
nash_app_objs = nash.o

nash_headers = NashSieve.h
arg_parser_headers = arg_parser.h

.PHONY: all clean

all: nash mnash

nash: $(nash_objs) $(nash_app_objs) $(arg_parser_objs)
	$(CPP) -o $@ $^ $(LIBS)

mnash: $(nash_objs) $(mnash_app_objs) $(arg_parser_objs)
	$(CPP) -o $@ $^ $(LIBS)

%.o: %.cpp $(nash_headers) $(arg_parser_headers)
	$(CPP) $(FLAGS) -c $< -o $@

%.o: %.cc $(arg_parser_headers)
	$(CPP) $(FLAGS) -c $< -o $@

clean:
	rm -f nash mnash *.o
