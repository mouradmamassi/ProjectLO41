CC = gcc
CFLAGS = -W -Wall -pedantic -lpthread -lrt 
EXEC = project
all: $(EXEC)

project: main.o Drone.o Vaisseau.o Garage.o List.o
	$(CC) -o  $@ $^ $(CFLAGS)
Drone.o : Drone.c Drone.h
	$(CC) -c $^ $(CFLAGS)
Vaisseau.o : Vaisseau.c  Drone.h
	$(CC) -c $^ $(CFLAGS)
Garage.o : Garage.c Drone.h
	$(CC) -c $^ $(CFLAGS)
List.o : List.c Drone.h
	$(CC) -c $^ $(CFLAGS)
main.o : main.c Drone.h
	$(CC) -c $^ $(CFLAGS)
clean:
	rm -f project *.o *.h.gch

