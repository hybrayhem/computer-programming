#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SRCFILE "Movies.txt"

typedef struct movie_budget {
    int budget, year;
    char *name;
    struct movie_budget *next;
} movie_budget;

typedef struct movie_name {
    char *genre, *name;
    double score;
    struct movie_name *next;
} movie_name;

void print_movies(movie_budget *mb, movie_name *mn) {
    printf("Movies\n");
    if (mb == NULL || mn == NULL) {
        printf("\n");
    } else {
        printf("Movie budget: b: %d, y: %d, n: %s\n", mb->budget, mb->year, mb->name);
        printf("Movie name: g: %s, n: %s, s: %.2f\n\n", mn->genre, mn->name, mn->score);
        print_movies(mb->next, mn->next);
    }
}

void print_last_movie(movie_budget *mb, movie_name *mn) {
    if (mb->next == NULL && mn->next == NULL) {
        printf("movie\n");
        printf("Movie budget: b: %d, y: %d, n: %s\n", mb->budget, mb->year, mb->name);
        printf("Movie name: g: %s, n: %s, s: %.2f\n\n", mn->genre, mn->name, mn->score);
    } else {
        print_movies(mb->next, mn->next);
    }
}

void insert_movie(movie_budget *mb, movie_name *mn, int budget, int year, char *name, char *genre, double score) {
    movie_budget *next_mb = (movie_budget *)malloc(sizeof(movie_budget));
    movie_name *next_mn = (movie_name *)malloc(sizeof(movie_name));

    next_mb->budget = budget;
    next_mb->year = year;
    next_mb->name = (char *)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(next_mb->name, name);

    next_mn->name = (char *)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(next_mn->name, name);
    next_mn->genre = (char *)malloc((strlen(genre) + 1) * sizeof(char));
    strcpy(next_mn->genre, genre);
    next_mn->score = score;

    /* TODO: search if exists update, else insert to end */

    next_mb->next = NULL;
    next_mn->next = NULL;
    if(mb != NULL && mn != NULL){
        mb->next = next_mb;
        mn->next = next_mn;
    }else{
        mb = next_mb;
        mn = next_mn;
    }
}

void get_movie(FILE *src, int *budget, int *year, char *name, char *genre, double *score) {
    int i, counter = 0;
    char input, *ap, *apb = NULL;

    do {
        fscanf(src, "%c", &input);
        counter++;

        ap = (char *)malloc((counter + 1) * sizeof(char));
        for (i = 0; i < counter - 1; i++) {
            ap[i] = apb[i];
        }
        ap[counter - 1] = input;

        if (apb != NULL)
            free(apb);
        apb = ap;

    } while (input != '\n' && !feof(src));

    if (strlen(ap) > 5) {
        apb = strtok(ap, ",");
        /*printf("movie = %s; ", apb);*/
        if (strcmp(apb, "unknown") == 0) {
            *budget = -1;
        } else {
            *budget = strtod(apb, NULL);
        }

        apb = strtok(NULL, ",");
        /*printf("%s; ", apb);*/
        /*if(genre != NULL) free(genre);*/
        genre = (char *)malloc((strlen(apb) + 1) * sizeof(char));
        strcpy(genre, apb);

        apb = strtok(NULL, ",");
        /*printf("%s; ", apb);*/
        /*if(name != NULL) free(name);*/
        name = (char *)malloc((strlen(apb) + 1) * sizeof(char));
        strcpy(name, apb);

        apb = strtok(NULL, ",");
        /*printf("%s; ", apb);*/
        *score = strtod(apb, NULL);

        apb = strtok(NULL, ",");
        /*printf("%s.\n", apb);*/
        *year = atoi(apb);
    }

    /*printf("ap = %s\n", ap);*/
    free(ap);
}

/* extract headers from first line of file */
void get_headers(FILE *src, char headers[][10]) {
    int i = 0;
    char line[30], ch, *temp;

    do {
        fscanf(src, "%c", &ch);
        line[i] = ch;
        i++;
    } while (ch != '\n');

    temp = strtok(line, ",");
    strcpy(headers[0], temp);

    temp = strtok(NULL, ",");
    strcpy(headers[1], temp);

    temp = strtok(NULL, ",");
    strcpy(headers[2], temp);

    temp = strtok(NULL, ",");
    strcpy(headers[3], temp);

    temp = strtok(NULL, ",");
    strcpy(headers[4], temp);
}

int main() {
    FILE *src;
    movie_budget *head_mb;
    movie_name *head_mn;
    char headers[5][10] = {0};

    int budget, year;
    char *name, *genre;
    double score;

    src = fopen(SRCFILE, "r");
    head_mb = (movie_budget *)malloc(sizeof(movie_budget));
    head_mn = (movie_name *)malloc(sizeof(movie_name));
    get_headers(src, headers);
    /*while (!feof(src)) {*/
        get_movie(src, &budget, &year, name, genre, &score);
        insert_movie(head_mb, head_mn, budget, year, name, genre, score);
        print_last_movie(head_mb, head_mn);
    /*}*/

    /*print_movies(head_mb, head_mn);*/
    return 0;
}