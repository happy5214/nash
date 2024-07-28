MAKE = make

.PHONY: all

all: c cpp

c:
	$(MAKE) -C ./c all

cpp:
	$(MAKE) -C ./cpp all

clean:
	$(MAKE) -C ./c clean
	$(MAKE) -C ./cpp clean
