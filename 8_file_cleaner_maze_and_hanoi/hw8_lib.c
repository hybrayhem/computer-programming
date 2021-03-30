/*
** hw8_lib.c:
**
** The source file implementing library functions.
**
** Author: Yakup Genc. (c) 2018-2021
**
** Revision: 2021.04.22.19.55
** 
*/

#include <stdio.h>
#include "hw8_lib.h"

/*#define WORDSIZE 10	moved to hw8_lib.h */


/* compares two string, returns 1 in equality */
int is_equal(char word1[], char word2[]){
  
    if((word1[0] == '\0' || word1[0] == '.') && (word2[0] == '\0' || word2[0] == '.')) return 1;

    if(word1[0] == '\0' || word2[0] == '\0') return 0;

    if(word1[0] == word2[0]){
        return is_equal(&word1[1], &word2[1]);
    }
    else return 0;

}

/* searches word in words_to_delete array, if exists returning 1 */
int is_to_delete(char* words_to_delete[3], char word[], int number_of_words){
	if(number_of_words == 0){
		return 0;
	}
	/*printf("search: %d %s %s", number_of_words, words_to_delete[0], word);*/

	if(is_equal(words_to_delete[0], word)){
		/*printf("found\n\n");*/
		return 1;
	}else{
		/*printf("\n");*/
		return is_to_delete(&words_to_delete[1], word, number_of_words-1);
	}
}

/* gets array of words, reads from infid and writes to outfid except words_to_delete */
void delete_words (FILE* infid, FILE* outfid, char* words_to_delete[WORDSIZE],  int number_of_words)
{
	char word[30];

	if(number_of_words == 0 || feof(infid)){
		return;
	}
	
	fscanf(infid, "%s", word);
	if(is_to_delete(words_to_delete, word, number_of_words)){
		/*printf("%s DELETE\n", word);*/
	}else{
		fprintf(outfid, "%s ", word);
	}

	delete_words(infid, outfid, words_to_delete, number_of_words);
}

/* gets two file name, one as input and one as output, writes input as cleaned from words_to_delete to output*/
void clean_file(char* infile, char * outfile, char* words_to_delete[WORDSIZE], int number_of_words)
{
	FILE *infid, *outfid;
	infid = fopen(infile, "r");
	outfid = fopen(outfile, "w");

	delete_words (infid, outfid, words_to_delete, number_of_words);
}

int player_location(cell_type maze[][8], cell_type player){
	int i, j;
	
	return 12;
}
/* moves player in a 8x8 maze regarding walls and finish state (TO BE IMPLEMENTED)*/
int maze_move(cell_type maze[][8], cell_type player, move_type move)
{
	int loc;
	loc = player_location(maze, player);
	printf("Player at (%d,%d)\n", loc/10, loc%10);
	printf("maze_move TO BE IMPLEMENTED\n");
	return -1;
}


/* printing */
void print_hanoi(int (*hanoi)[3], int step, int n){
	/* well formatted print */
	int i, j;
	int val;
	
	/* header */
	printf(" ");
	for(i = 0; i < n*14; i++) printf("=");
	printf("\n=%*s%s %d%*s=\n", (n*13)/2, "", "Step", step, (n*13)/2, "");
	printf(" ");
	for(i = 0; i < n*14; i++) printf("=");
	printf("\n");


	for(i = 0; i < n; i++){
		for(j = 0; j < 3; j++){
			val = hanoi[i][j];
			/*printf("%d ", tower[i][j]);*/
			if(val == 0) 
				printf("%*s  %*s-%*s  %*s", 2*n, "", val, "", val, "", 2*n, "");
			else 
				printf("%*s -%*s%d%*s- %*s", 2*n - val, "", val, "", val, val, "", 2*n - val, "");
		}
		printf("\n");
	}

	/* bottom border */
	printf(" ");
	for(i = 0; i < n*14; i++) printf("=");
	printf("\n= %*sA%*sB%*sC%*s=\n", n*2, "", (n*14)/3, "", (n*14)/3, "", n*2, "");
	printf(" ");
	for(i = 0; i < n*14; i++) printf("=");
	printf("\n\n\n");
}

/* finds index of disk in peg */
int find_disk(int disk, int peg, int (*hanoi)[3], int n){
	int i;

	for(i = 0; i < n; i++){
		if(hanoi[i][peg] == disk){
			return i;
		}
	}
	return -1;
}

/* finds last 0's index in target_peg column */
int floor_index(int target_peg, int (*hanoi)[3], int n){
	int i;

	for(i = 0; i < n; i++){
		if(hanoi[i][target_peg] != 0){
			return (i-1);
		}
	}
	return (i-1);
}

void towers_of_hanoi(char start_peg, char end_peg, char aux_peg, int n, int (*hanoi)[3], int *step, int size)
{
	int startx, starty, endx, endy;
	
	starty = start_peg - 'A';
	startx = find_disk(n, starty, &hanoi[0], size);
	/*if(startx == -1) printf("cant find_disk(disk: %d, column: %d) = %d\n", n, starty, startx);*/
	endy = end_peg - 'A';
	endx = floor_index(endy, &hanoi[0], size);

	if(n == 1){
		/*printf("[%d][%d] --> [%d][%d]	", startx, starty, endx, endy);*/
		printf("Move disk 1 from peg %c to peg %c \n\n", start_peg, end_peg);
		/*printf(">ove disk 1 from [%d][%d] to [%d][%d]\n\n", startx, starty, endx, endy);*/

		hanoi[endx][endy] = hanoi[startx][starty];
		hanoi[startx][starty] = 0;
		(*step)++;
		print_hanoi(&hanoi[0], *step, size);
	}
	else{
		towers_of_hanoi(start_peg, aux_peg, end_peg, n - 1, &hanoi[0], step, size);
		/*printf("[%d][%d] --> [%d][%d]	", startx, starty, endx, endy);*/
		printf("Move disk %d from peg %c to peg %c \n", n, start_peg, end_peg);
		/*printf(">ove disk %d from [%d][%d] to [%d][%d]\n\n", hanoi[find_disk(n, start_peg, &hanoi[0])][start_peg - 'A'], find_disk(n, start_peg, &hanoi[0]), start_peg - 'A', floor_index(end_peg, &hanoi[0]), end_peg - 'A');*/

		hanoi[endx][endy] = hanoi[startx][starty];
		hanoi[startx][starty] = 0;
		(*step)++;
		print_hanoi(&hanoi[0], *step, size);

		towers_of_hanoi(aux_peg, end_peg, start_peg, n - 1, &hanoi[0], step, size);
	}
}

