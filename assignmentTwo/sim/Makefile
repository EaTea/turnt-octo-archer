CC=g++
CFLAGS=-Wall -pedantic -lstdc++ -gdb
DEPS = sim.h
OBJ = csv.o metric.o output.o strategy.o simulation.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

simulator: $(OBJ)
	g++ -o $@ $^ $(CFLAGS)

clean:
	rm *.o
	rm simulator
