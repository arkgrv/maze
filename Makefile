CC = gcc
CFLAGS = -std=c11 -g
SRC = src/main.c src/maze.c
OBJ = main.o maze.o
PROGNAME = maze

maze: ${OBJ}
	${CC} ${CFLAGS} -o $@ ${OBJ}

all:
	${CC} ${CFLAGS} ${SRC} -o ${PROGNAME}

${OBJ}:
	${CC} ${CFLAGS} ${SRC} -c

clean:
	rm -rf *.o *.txt maze
