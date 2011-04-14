LDFLAGS=-lglfw -lGLU 
CPPFLAGS=-Wall -g

all: racetrack 

racetrack: main.o genetic_algorithm.o genome.o game.o coordinate.o level.o graphics.o config.o
	g++ ${CPPFLAGS} ${LDFLAGS} -o racetrack main.o genetic_algorithm.o \
		genome.o game.o coordinate.o level.o graphics.o config.o

main.o: main.cpp
genetic_algorithm.o: genetic_algorithm.h genetic_algorithm.cpp
genome.o: genome.h genome.cpp game.h coordinate.h
game.o: game.h game.cpp coordinate.h level.h
coordinate.o: coordinate.cpp coordinate.h game.h
level.o: level.cpp level.h 
graphics.o: graphics.cpp graphics.h
config.o: config.cpp config.h

clean:
	rm *.o racetrack
