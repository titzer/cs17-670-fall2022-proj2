.PHONY: all clean

all: weerun

clean:
	rm -f weerun *.o

test: weerun
	./weerun -test

weerun: vm.h weerun.c common.h common.c test.h test.c ir.h ir.c
	cc -o weerun weerun.c common.c test.c ir.c
