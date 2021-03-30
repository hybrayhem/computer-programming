/*
** main.c:
**
** The test/driver program for the homework.
**
** Author: Yakup Genc. (c) 2018-2021
**
** Revision: 2021.04.22.19.55
** 
*/


#include <stdio.h>
#include "hw8_lib.h"


void test_clean_file () 
{
	char *to_delete[10] = {"effort", "thereby", "robot"};
	clean_file("input.txt", "output.txt", to_delete, 3);
	printf("test_clean_file     DONE...\n\n");

	
	/*printf("is_equal: %d\n", is_equal("abc", "abc"));
	printf("includes: %d\n", includes(to_delete, "robot", 3));*/
}

void print_maze(cell_type maze[][8]){
	int i, j, val;
	for(i = 0; i < 8; i++){
		for(j = 0; j < 8; j++){
			val = maze[i][j];
			if(val == cell_wall)
				printf("# ");
			else if(val == cell_free)
				printf("  ");
			else if(val == cell_target)
				printf("+ ");
			else if(val == cell_p1)
				printf("P ");
			else if(val == cell_p2)
				printf("p ");

		}
		printf("\n");
	}
}

void test_maze_move ()
{
	int i, j, val;
	cell_type maze[8][8] = {
		{cell_wall, cell_wall, cell_wall, cell_wall, cell_wall, cell_wall, cell_wall, cell_wall},

		{cell_wall, cell_p1,   cell_free, cell_free, cell_wall, cell_free, cell_free, cell_wall},

		{cell_wall, cell_free, cell_wall, cell_free, cell_wall, cell_wall, cell_wall, cell_wall},

		{cell_wall, cell_free, cell_wall, cell_free, cell_wall, cell_wall, cell_p2,   cell_wall},

		{cell_wall, cell_free, cell_wall, cell_wall, cell_wall, cell_wall, cell_free, cell_wall},

		{cell_wall, cell_free, cell_wall, cell_wall, cell_wall, cell_wall, cell_free, cell_wall},

		{cell_wall, cell_free, cell_free, cell_target, cell_free, cell_free, cell_free, cell_wall},
		
		{cell_wall, cell_wall, cell_wall, cell_wall, cell_wall, cell_wall, cell_wall, cell_wall}
	};

	print_maze(maze);
	maze_move(maze, cell_p1, move_down);
	/*print_maze(maze);*/
	printf("test_maze_move      DONE...\n\n");
}


void test_towers_of_hanoi ()
{
	int n = 5, i, j, step = 0;
	/*int tower[6][3] = {{1,0,0},{2,0,0},{3,0,0}};*/
	int tower[5][3] = {{1,0,0},{2,0,0},{3,0,0},{4,0,0},{5,0,0}};
	int val;
	
	/* TEST PRINT */
	/* header */
	/*printf(" ");
	for(i = 0; i < n*14; i++) printf("=");
	printf("\n=%*s%s %d%*s=\n", (n*13)/2, "", "Step", 1, (n*13)/2, "");
	printf(" ");
	for(i = 0; i < n*14; i++) printf("=");
	printf("\n");


	for(i = 0; i < n; i++){
		for(j = 0; j < 3; j++){
			val = tower[i][j];
			if(val == 0) 
				printf("%*s  %*s-%*s  %*s", 2*n, "", val, "", val, "", 2*n, "");
			else 
				printf("%*s -%*s%d%*s- %*s", 2*n - val, "", val, "", val, val, "", 2*n - val, "");
		}
		printf("\n");
	}*/

	/* bottom border */
	/*printf(" ");
	for(i = 0; i < n*14; i++) printf("=");
	printf("\n= %*sA%*sB%*sC%*s=\n", n*2, "", (n*14)/3, "", (n*14)/3, "", n*2, "");
	printf(" ");
	for(i = 0; i < n*14; i++) printf("=");
	printf("\n");*/

	towers_of_hanoi('A', 'C', 'B', 5, &tower[0], &step, n);
	printf("test_towers_of_hanoi DONE...\n\n");
}


/*
** main function for testing the functions...
**
*/
int main(void) {
	test_clean_file ();
	test_maze_move ();
	test_towers_of_hanoi();
	return (0);
} /* end main */
