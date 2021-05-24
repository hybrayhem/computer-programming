#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    double budget;
    int genre;
    char* name; /* dynamic string */
    double score;
    int year;
}Movie;



int insert_movie(Movie movie, Movie *movies, int l){
    int i;
    Movie *temp = (Movie *)calloc(l, sizeof(Movie));

    for(i=0; i < l; i++){
        /*memcpy(&temp[i], &movies[i], sizeof(Movie));*/
        temp[i] = movies[i];
    }

    movies = (Movie *)calloc(l+1, sizeof(Movie));

    for(i=0; i < l; i++){
        movies[i] = temp[i];
    }
    /*if(temp != NULL) free(temp);*/

    movies[l] = movie;
    /*movies[l].budget = movie.budget;
    movies[l].genre = movie.genre;
    strcpy(movies[l].name, movie.name);
    movies[l].score = movie.score;
    movies[l].year = movie.year;*/

    for(i = 0; i < l+1; i++){
        printf("Movie %d : %.1f, %d, %s, %.1f, %d\n", i, movies[i].budget, movies[i].genre, movies[i].name, movies[i].score, movies[i].year);
    }
    printf("\n");

    free(temp);
    return 0;
}

void print_movies(Movie *movies, int l){
    int i;
    for(i=0; i < l; i++){
        printf("%d. movie = budget: %.1f, genre: %d, name: %s, score: %.1f, year: %d\n\n", i, movies[i].budget, movies[i].genre, movies[i].name, movies[i].score, movies[i].year);
    }
}

int main(){
    int i, l = 0, success;

    Movie temp = {50, 1, "Star", 2.5, 2025};
    Movie temp2 = {100, 2, "Wars", 7, 1995};

    Movie *movies = (Movie *)calloc(1, sizeof(Movie));

    free(movies);
    return 0;
}

/*
    int i;
    Movie *apb = NULL;

    movies = (Movie *)calloc(l+1, sizeof(Movie));
    for (i = 0; i < l; i++)
    {
        movies[i] = apb[i];
    }
    movies[l] = movie;

    if (apb != NULL) free(apb);
    apb = movies;
*/