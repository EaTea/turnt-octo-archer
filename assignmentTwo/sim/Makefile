CC=g++
CFLAGS=-Wall -pedantic
DEPS = sim.h
OBJ = csv.o simulation.o output.o strategy.o metric.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

simulator: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
