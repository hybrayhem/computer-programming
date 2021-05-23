#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SRCFILE "Movies.txt"
#define TESTFILE "Movies_test.txt"


typedef struct{
    double budget;
    int genre;
    char* name; /* dynamic string */
    double score;
    int year;
}Movie;



int show_menu();
int insert_movie(Movie movie);
int update_movie(Movie movie);


int main(){
    Movie *movies;
    FILE *src;
    char muyuk[20];

    src = fopen(TESTFILE, "r"); 

    while(!feof(src)){
        fscanf(src, "%20[^,]s", muyuk);
        printf("'%s'\n", muyuk);
    }

    show_menu();
    return 0;
}


int show_menu(){
    int status, selection = 0, flag = 0;

    printf("1. List of the Sorted\n");
    printf("2. List of the Genres\n");
    printf("3. List of the Movie Through the Years\n");
    printf("4. List of the Movie Through the Scores\n");
    printf("5. All Informations of a Single Movie\n");
    printf("6. Average of the IMDB Scores\n");
    printf("7. Frequence of the Genres\n");
    printf("8. Exit\n\n");

    while(!flag){
        printf("Please select an operation: ");
        status = scanf("%d", &selection);
        if(status < 1 || selection < 1 || selection > 8){
            while(getchar() != '\n');
            printf("TRY AGAIN\n");
            continue;
        }
        flag = 1;
    }

    return selection;
}