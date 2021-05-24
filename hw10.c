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

int numerate_genre(char **genres, char genre[]){
    int i;
    
    printf("genre = %s\n", genre);
    for(i=0; genres[i] != NULL; i++){
        printf("genres %d. = %s\n", i, genres[i]);
    }
}

int parse_movie(FILE *src, Movie *movie, char **genres){
    int i, counter = 0;
    char input, *ap, *apb = NULL;

    do
    {
        fscanf(src, "%c", &input);
        counter++;

        ap = (char *)calloc(counter, sizeof(char));
        for (i = 0; i < counter - 1; i++)
        {
            ap[i] = apb[i];
        }
        ap[counter - 1] = input;

        if (apb != NULL)
            free(apb);
        apb = ap;

    } while (input != '\n' && !feof(src));

    apb = strtok(ap, ",");
    printf("movie = %s; ", apb);
    if(strcmp(apb, "unknown") == 0){
        movie->budget = -1;
    }else{
        movie->budget = strtod(apb, NULL);
    }

    apb = strtok(NULL, ",");
    printf("%s; ", apb);
    movie->genre = numerate_genre(genres, apb);

    apb = strtok(NULL, ",");
    printf("%s; ", apb);
    movie->name = apb;

    apb = strtok(NULL, ",");
    printf("%s; ", apb);
    movie->score = strtod(apb, NULL);

    apb = strtok(NULL, ",");
    apb[strlen(apb) - 1] = '\0';
    printf("%s\n", apb);
    movie->year = atoi(apb);

    /*printf("ap = %s\n", ap);*/
    /*free(ap);*/

    return 1;
}

int main(){
    int l = 0, success;
    FILE *src;

    Movie *movies = (Movie *)calloc(20, sizeof(Movie));
    char *genres = (char *)calloc(1, sizeof(char));
    genres[0] = NULL;
    
    src = fopen(TESTFILE, "r");
    while(!feof(src)){
        success = parse_movie(src, &movies[l], &genres);
        printf("Movie: %.1f, %d, %s, %.1f, %d\n\n", movies[l].budget, movies[l].genre, movies[l].name, movies[l].score, movies[l].year);
        if(success){
            l++;
        }
    }

    show_menu();
    free(movies);
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