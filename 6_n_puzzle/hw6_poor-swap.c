#include <stdio.h>
#include <stdlib.h>

/* board size */
#define N 3

#define advanced_randomize 1 /* set to 0 for simple randomizing */

/* move directions */
typedef enum
{
    left,
    right,
    up,
    down
} direction_t;

/* say hi to user */
void welcome(){
    unsigned int seed = 0;

    if (advanced_randomize){

        printf("Welcome!\n\n");
        printf("> I need to warm up my game engine \n");
        printf("> Please press Spacebar and hold for 5s, then press Enter to start\n");
        printf("/");

        /* get randomizing seed without time */
        /* if user directly hits Enter, it works as there was no srand */
        while (1)
        {
            if (getchar() == '\n')
            {
                break;
            }
            seed++; /* number of spaces */
        }

        srand(seed);
        printf("\nThanks, I'm ready!\n\n");
    }


    printf("           _____                                             \n          /\\  __\\_          ___              __            \n");
    printf("         /  \\/ \\___\\       / _ \\__ ________ / /__        \n         \\     /___/      / ___/ // /_ /_ // / -_)          \n");
    printf("      /\\_/     \\    \\    /_/   \\_,_//__/__/_/\\__/       \n     /          \\____\\              _____                  \n");
    printf("     \\       _  /    /             / ___/__ ___ _  ___      \n      \\     /_\\/____/             / (_ / _ `/  ' \\/ -_)   \n");
    printf("      /     \\___\\                 \\___/\\_,_/_/_/_/\\__/  \n      \\_/\\  /   /                                          \n");
    printf("          \\/___/                                        v1.0.2\n\n\n");
}


void create_puzzle(int (*puzzle)[10], int *space_a, int *space_b);
void print_puzzle(int (*puzzle)[10]);
void get_space_index(int (*puzzle)[10]);
int move_left(int (*puzzle)[10], int a, int b, int *space_a, int *space_b);
int move_right(int (*puzzle)[10], int a, int b, int *space_a, int *space_b);
int move_up(int (*puzzle)[10], int a, int b, int *space_a, int *space_b);
int move_down(int (*puzzle)[10], int a, int b, int *space_a, int *space_b);
int you_won(int (*puzzle)[10]);


int main(){
    welcome();

    int puzzle[10][10] = {0};
    int success;
    int item, item_a, item_b, space_a, space_b; /* item and space locations */
    char move_c;
    create_puzzle(&puzzle[0], &space_a, &space_b);
    
    printf("> INFO: movement format is [vertical_index][horizontal_index][u, d, l or r]\n> ex: 11u, 24l, 64d, 71r\n\n> type '-1e' for exit the game");
    while(1){
        print_puzzle(&puzzle[0]);
        printf("Enter your movement: ");
        scanf("%d%c", &item, &move_c); /* gets command as one part due to be user friendly */
        while (getchar() != '\n'); /* sweep unnecessary input */

        /* exit */
        if (item == -1 && move_c == 'e'){
            break;
        }

        /* seperate to coordinates, decrease because of indexes starts from 1 for user */
        item_a = (item / 10) - 1;
        item_b = (item % 10) - 1;

        /* perform movement */
        switch (move_c)
        {
        case 'l':
            success = move_left(&puzzle[0], item_a, item_b, &space_a, &space_b);
            break;
        case 'r':
            success = move_right(&puzzle[0], item_a, item_b, &space_a, &space_b);
            break;
        case 'u':
            success = move_up(&puzzle[0], item_a, item_b, &space_a, &space_b);
            break;
        case 'd':
            success = move_down(&puzzle[0], item_a, item_b, &space_a, &space_b);
            break;
        default:
            printf("Invalid direction: '%c'\n", move_c);
            success = 1; /* except a second error */
            break;
        }
        if (!success) printf("\n> OOPS! I can't move to that direction!\n"); /* for invalid movements */

        /* check winning status */
        if(you_won(&puzzle[0])){
            break;
        }
    }
    printf("\nGame over.\n"); /* say bye to user */

    return 0;
}


/* creates random sliding puzzle 2d array */
void create_puzzle(int (*puzzle)[10], int *space_a, int *space_b)
{
    int r1, r2, r3, r4; /* random numbers */
    float difficulty = 1; /* level of shuffle between 0-1*/

    int i, j;
    int x = 1;

    /* create puzzle board */
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            puzzle[i][j] = x;
            x++;
        }
    }
    puzzle[N-1][N-1] = -1; /* last square is space */
    
    /* shuffle the puzzle randomly */
    for (i = 0; i < N*difficulty; i++)
    {
        for (j = 0; j < N*difficulty; j++)
        {
            r1 = rand() % N;
            r2 = rand() % N;
            r3 = rand() % N;
            r4 = rand() % N;

            /* make random swap */
            x = puzzle[r1][r2]; /* x used as temporary swap value */
            puzzle[r1][r2] = puzzle[r3][r4];
            puzzle[r3][r4] = x;
            x++;

            /* store location of space */
            if(puzzle[r1][r2] == -1){
                *space_a = r1;
                *space_b = r2;
            }
            else if(puzzle[r3][r4] == -1){
                *space_a = r3;
                *space_b = r4;
            }
        }
    }
}

/* print well-formatted puzzle */
void print_puzzle(int (*puzzle)[10])
{
    int i, j;

    /* header row */
    printf("\n\n\n     ");
    for (i = 0; i < N; i++){
        if(i+1 < 10) printf("  [%d] ", i+1);
        else printf(" [%d] ", i+1);
    }
    printf("\n\n     ");

    /* top line */
    for(j = 0; j < N*3 + 1; j++) printf("* ");
    printf("\n");

    for (i = 0; i < N; i++)
    {
        /* row numbers */
        if(i+1 < 10) printf("[%d]  * ", i+1);
        else printf("[%d] * ", i+1);

        /* board items */
        for (j = 0; j < N; j++)
        {
            if(puzzle[i][j] == -1){
                printf(" □  * "); /* space item */
            }
            else{
                printf(" %-3d* ", puzzle[i][j]);
            }
        }

        /* bottom lines */
        printf("\n     ");
        for(j = 0; j < N*3 + 1; j++) printf("* ");
        printf("\n");
    }
    printf("\n");
}

/* move item at [a,b] to left on board */
int move_left(int (*puzzle)[10], int a, int b, int *space_a, int *space_b){
    int i;

    if(a == *space_a && b > *space_b){
        /* slide every item left until reach space */
        for(i = (*space_b)+1; i <= b; i++){
            puzzle[a][i-1] = puzzle[a][i];
        }
        puzzle[a][b] = -1; /* set start point as space */
        /* update space location */
        *space_a = a;
        *space_b = b;
        return 1;
    }
    else{
        return 0;
    }
}

/* move item at [a,b] to right on board */
int move_right(int (*puzzle)[10], int a, int b, int *space_a, int *space_b){
    int i;
    
    if(a == *space_a && b < *space_b){
        for(i = (*space_b)-1; i >= b; i--){
            puzzle[a][i+1] = puzzle[a][i];
        }
        puzzle[a][b] = -1;
        *space_a = a;
        *space_b = b;
        return 1;
    }
    else{
        return 0;
    }
}

/* move item at [a,b] to up on board */
int move_up(int (*puzzle)[10], int a, int b, int *space_a, int *space_b){
    int i;
    
    if(b == *space_b && a > *space_a){
        /* slide every item up until reach space */
        for(i = (*space_a)+1; i <= a; i++){
            puzzle[i-1][b] = puzzle[i][b];
        }
        puzzle[a][b] = -1; /* set start point as space */
        /* update space location */
        *space_a = a;
        *space_b = b;
        return 1;
    }
    else{
        return 0;
    }
}

/* move item at [a,b] to down on board */
int move_down(int (*puzzle)[10], int a, int b, int *space_a, int *space_b){
    int i;
    
    if(b == *space_b && a < *space_a){
        for(i = (*space_a)-1; i >= a; i--){
            puzzle[i+1][b] = puzzle[i][b];
        }
        puzzle[a][b] = -1;
        *space_a = a;
        *space_b = b;
        return 1;
    }
    else{
        return 0;
    }
}

/* checks win status, 0: game continues, 1: win */
int you_won(int (*puzzle)[10]){
    int i, j;

    for (i = 0; i < N-1; i++)
    {
        for (j = 0; j < N-1; j++)
        {
            /* pass space*/
            if(puzzle[i][j+1] != -1){
                /* difference between numbers must be 1 */
                if(puzzle[i][j+1] - puzzle[i][j] != 1){
                    return 0; /* no win */
                }
            }
        }
    }
    printf("\n▧ Heyy! You won the game. ▧\n");
    return 1; /* win */
}
