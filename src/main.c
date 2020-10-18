#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#include "../include/maze.h"

int main(int argc, char* const argv[]) {
    srand(time(NULL));

    printf("Please insert width: ");
    int w;
    scanf("%d", &w);
    printf("Please insert height: ");
    int h;
    scanf("%d", &h);

    /* generating maze */
    int** maze = create_maze(w, h);
    write_maze(maze, w, h);

    return 0;
}