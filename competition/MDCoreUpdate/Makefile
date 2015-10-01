
FC=ifort
LD= ifort
FFLAGS= -g -O2

main: main.o complexproc1.o complexproc.o
	$(FC) $^ -o $@

clean:
	rm -rf *.o *.i main
