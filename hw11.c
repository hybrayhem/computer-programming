#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    if (mb == NULL || mn == NULL) {
        printf("\n");
    } else {
        printf("Movie budget: b: %d, y: %d, n: %s\n", mb->budget, mb->year, mb->name);
        printf("Movie name: g: %s, n: %s, s: %.2f\n\n", mn->genre, mn->name, mn->score);
        print_movies(mb->next, mn->next);
    }
}

movie_budget *get_movie() {
    char name[50];
    movie_budget *mb;
    printf("Enter name: ");
    scanf("%s", name);
    printf("%s\n",name);
    if (strcmp(name, "-1") == 0) {
        /*mb = NULL;*/
        return NULL;
    } else {
        mb = (movie_budget *)malloc(sizeof(movie_budget));
        mb->name = (char *)malloc((strlen(name) + 1) * sizeof(char));
        strcpy(mb->name, name);
        mb->next = get_movie();
    }
    return (mb);
}

int main() {
    movie_budget *mb;
    movie_name *mn;
    mb = (movie_budget *)malloc(sizeof(movie_budget));
    mn = (movie_name *)malloc(sizeof(movie_name));

    mb = get_movie();
    print_movies(mb, mn);
    return 0;
}