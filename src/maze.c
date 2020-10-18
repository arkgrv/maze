#include "../include/maze.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <memory.h>

void generate_maze(int** maze, int cx, int cy);

static void error(const char* message) {
    fprintf(stderr, "ERROR: %s\n", message);
}

static void end_program(const char* message) {
    error(message);
    exit(EXIT_FAILURE);
}

/* maze direction type */
typedef struct dir {
    int bit;
    int dx;
    int dy;

    struct dir* opposite;
} dir_t;

/* static incomplete maze directions */
static dir_t NORTH;
static dir_t SOUTH;
static dir_t EAST;
static dir_t WEST;

const size_t N_DIRECTIONS = 4;

/* directions array */
static dir_t* DIRECTIONS;

/* maze size */
static int X;
static int Y;

/* direction creation function */
static dir_t create_direction(int bit, int dx, int dy, dir_t* opposite) {
    dir_t tmp = { bit, dx, dy, opposite };
    return tmp;
}

/* returns a new random directions array */
static dir_t* rand_directions() {
    dir_t* rnd = (dir_t*)malloc(sizeof(dir_t) * N_DIRECTIONS);
    if (NULL == rnd) end_program("cannot create random directions");
    memcpy(rnd, DIRECTIONS, sizeof(dir_t) * N_DIRECTIONS);

    srand(time(NULL));
    int i;
    for (i = 0; i < N_DIRECTIONS - 1; i++) {
        int j = i + rand() / (RAND_MAX / (N_DIRECTIONS - i) + 1);
        dir_t tmp = rnd[j];
        rnd[j] = rnd[i];
        rnd[i] = tmp;
    }

    return rnd;
}

int** create_maze(int x, int y) {
    /* creating pointers pool */
    int** maze = (int**)malloc(sizeof(int*) * x);
    if (NULL == maze) end_program("unable to allocate maze memory");

    /* allocating maze memory */
    int i;
    for (i = 0; i < x; i++) {
        maze[i] = (int*)malloc(sizeof(int) * y);
        if (NULL == maze[i]) end_program("unable to allocate maze memory");
    }

    /* creating directions array */
    DIRECTIONS = (dir_t*)malloc(sizeof(dir_t) * N_DIRECTIONS);
    if (NULL == DIRECTIONS) end_program("cannot allocate directions array");

    /* setting directions */
    NORTH = create_direction(1, 0, -1, &SOUTH);
    SOUTH = create_direction(2, 0, 1, &NORTH);
    EAST = create_direction(4, 1, 0, &WEST);
    WEST = create_direction(8, -1, 0, &EAST);

    /* copying into array */
    DIRECTIONS[0] = NORTH;
    DIRECTIONS[1] = SOUTH;
    DIRECTIONS[2] = EAST;
    DIRECTIONS[3] = WEST;

    /* setting global size */
    X = x;
    Y = y;

    /* generating maze */
    generate_maze(maze, 0, 0);

    return maze;
}

void delete_maze(int** maze, int x) {
    int i;
    for (i = 0; i < x; i++) free(maze[i]);
    free(DIRECTIONS);
}

void write_maze(int** maze, int x, int y) {
    FILE* maze_file = fopen("maze.txt", "w");
    if (NULL == maze_file) end_program("unable to open maze output file");

    int i, j;
    for (i = 0; i < y; i++) {
        /* drawing northern edge */
        for (j = 0; j < x; j++) {
            char* maze_edge = (maze[j][i] & 1) == 0 ? "+---" : "+   ";
            fprintf(maze_file, "%s", maze_edge);
        }
        fprintf(maze_file, "\n");

        /* drawing western edge */
        for (j = 0; j < x; j++) {
            char* maze_edge = (maze[j][i] & 8) == 0 ? "|   " : "    ";
            fprintf(maze_file, "%s", maze_edge);
        }
        fprintf(maze_file, "\n");
    }
    /* drawing southern edge */
    for (j = 0; j < x; j++)
        fprintf(maze_file, "%s", "+---");
    fprintf(maze_file, "\n");

    fclose(maze_file);
}

int between(int v, int upper) {
    return (v >= 0) && (v < upper);
}

void generate_maze(int** maze, int cx, int cy) {
    dir_t* dirs = rand_directions();

    int i;
    for (i = 0; i < N_DIRECTIONS; i++) {
        int nx = cx + dirs[i].dx;
        int ny = cy + dirs[i].dy;

        if (between(nx, X) && between(ny, Y) &&
        maze[nx][ny] == 0) {
            maze[cx][cy] |= dirs[i].bit;
            maze[nx][ny] |= dirs[i].opposite->bit;
            generate_maze(maze, nx, ny);
        }
    }

    free(dirs);
}
