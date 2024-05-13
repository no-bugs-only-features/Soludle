CC=g++
FLAGS=-std=c++20 -g -Wall -Werror

PRODUCT=dlx

all: ${PRODUCT}

dlx: main.cpp Noodle.o KanoodleSolver.o
	${CC} ${FLAGS} -o $@ $^

Noodle.o: Noodle.cpp Noodle.h
	${CC} ${FLAGS} -c $<

KanoodleSolver.o: KanoodleSolver.cpp KanoodleSolver.h
	${CC} ${FLAGS} -c $<

clean:
	rm -f *.o ${PRODUCT}