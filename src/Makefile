CPP = g++
LIBS = -lgmp -lgmpxx
LIB_FLAGS = -fPIC
FLAGS = -O2

nash_objs = NashSieve.o
arg_parser_objs = arg_parser.o
mnash_app_objs = mnash.o
nash_app_objs = nash.o

libnash_so = libnash.so
libnash_objs = nash_lib.o

nash_headers = NashSieve.h
arg_parser_headers = arg_parser.h

.PHONY: all libnash clean

all: libnash nash mnash

nash: $(nash_objs) $(nash_app_objs) $(arg_parser_objs)
	$(CPP) -o $@ $^ $(LIBS)

mnash: $(nash_objs) $(mnash_app_objs) $(arg_parser_objs)
	$(CPP) -o $@ $^ $(LIBS)

libnash: $(libnash_so)

libnash.so: $(libnash_objs) $(nash_objs)
	$(CPP) $(LIB_FLAGS) -shared -o $@ $^

%.o: %.cpp $(nash_headers) $(arg_parser_headers)
	$(CPP) $(FLAGS) $(LIB_FLAGS) -c $< -o $@

%.o: %.cc $(arg_parser_headers)
	$(CPP) $(FLAGS) $(LIB_FLAGS) -c $< -o $@

clean:
	rm -f nash mnash *.o libnash.so
