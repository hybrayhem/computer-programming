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

void print_movie(movie_budget *mb, movie_name *mn) {
    printf("movie\n");
    printf("Movie budget: b: %d, y: %d, n: %s\n", mb->budget, mb->year, mb->name);
    printf("Movie name: g: %s, n: %s, s: %.2f\n\n", mn->genre, mn->name, mn->score);
}

void insert_movie(movie_budget *mb_list, movie_name *mn_list, int budget, int year, char *name, char *genre, double score) {
    movie_budget *next_mb = (movie_budget *)malloc(sizeof(movie_budget));
    movie_name *next_mn = (movie_name *)malloc(sizeof(movie_name));
    

}

void get_movie(FILE *src, movie_budget *mb, movie_name *mn) {
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
            mb->budget = -1;
        } else {
            mb->budget = strtod(apb, NULL);
        }

        apb = strtok(NULL, ",");
        /*printf("%s; ", apb);*/
        mn->genre = (char *)malloc((strlen(apb) + 1) * sizeof(char));
        strcpy(mn->genre, apb);

        apb = strtok(NULL, ",");
        /*printf("%s; ", apb);*/
        mb->name = (char *)malloc((strlen(apb) + 1) * sizeof(char));
        mn->name = (char *)malloc((strlen(apb) + 1) * sizeof(char));
        strcpy(mb->name, apb);
        strcpy(mn->name, apb);

        apb = strtok(NULL, ",");
        /*printf("%s; ", apb);*/
        mn->score = strtod(apb, NULL);

        apb = strtok(NULL, ",");
        /*printf("%s.\n", apb);*/
        mb->year = atoi(apb);
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
    movie_budget *temp_mb, *head_mb;
    movie_name *temp_mn, *head_mn;
    char headers[5][10] = {0};

    src = fopen(SRCFILE, "r");
    head_mb = (movie_budget *)malloc(sizeof(movie_budget));
    head_mn = (movie_name *)malloc(sizeof(movie_name));
    temp_mb = (movie_budget *)malloc(sizeof(movie_budget));
    temp_mn = (movie_name *)malloc(sizeof(movie_name));

    get_headers(src, headers);
    while (!feof(src)) {
        get_movie(src, temp_mb, temp_mn);
        print_movie(temp_mb, temp_mn);
        insert_movie(head_mb, head_mn, temp_mb, temp_mn);
    }

    /*print_movies(head_mb, head_mn);*/
    return 0;
}