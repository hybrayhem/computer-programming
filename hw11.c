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

typedef union movie_pack {
    movie_budget *b;
    movie_name *n;
} movie_pack;

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
        printf("Movie budget = b: %d, y: %d, n: %s\n", mb->budget, mb->year, mb->name);
        printf("Movie name = g: %s, n: %s, s: %.2f\n\n", mn->genre, mn->name, mn->score);
    } else {
        print_last_movie(mb->next, mn->next);
    }
}

movie_pack *movie_exists(movie_budget *mb_search, movie_name *mn_search, char name[]) {
    int success = 0;
    /*movie_budget *mb_search = mb;
    movie_name *mn_search = mn;*/
    movie_pack *pack = (movie_pack *)calloc(2, sizeof(movie_pack));

    /*while (mb_search != NULL) {
        printf("mb search: %s vs %s\n", mb_search->name, name);
        if (strcmp(mb_search->name, name) == 0) {
            printf("Movie Budget exists.\n");
            pack[0].b = mb_search;
            success = 1;
            break;
        }
        mb_search = mb_search->next;
    }

    while (mn_search != NULL) {
        printf("mn search: %s vs %s\n", mn_search->name, name);
        if (strcmp(mn_search->name, name) == 0) {
            printf("Movie Name exists.\n");
            pack[1].n = mn_search;
            success = 1;
            break;
        }
        mn_search = mn_search->next;
    }*/

    if (success)
        return pack;
    else
        return NULL;
}

int insert_movie(movie_budget *mb, movie_name *mn, movie_pack *pack) {
    movie_budget *next_mb = (movie_budget *)malloc(sizeof(movie_budget));
    movie_name *next_mn = (movie_name *)malloc(sizeof(movie_name));
    movie_pack *ex_pack; /* existing movie's pack */

    ex_pack = movie_exists(mb, mn, pack[0].b->name);
    if (ex_pack == NULL) {
        next_mb->budget = pack[0].b->budget;
        next_mb->year = pack[0].b->year;
        next_mb->name = (char *)calloc(strlen(pack[0].b->name) + 1, sizeof(char));
        strcpy(next_mb->name, pack[0].b->name);

        next_mn->name = (char *)calloc(strlen(pack[1].n->name) + 1, sizeof(char));
        strcpy(next_mn->name, pack[1].n->name);
        next_mn->genre = (char *)calloc(strlen(pack[1].n->genre) + 1, sizeof(char));
        strcpy(next_mn->genre, pack[1].n->genre);
        next_mn->score = pack[1].n->score;

        next_mb->next = NULL;
        next_mn->next = NULL;
        if (mb != NULL && mn != NULL) {
            mb->next = next_mb;
            mn->next = next_mn;
        } else {
            mb = next_mb;
            mn = next_mn;
        }
        return 1;
    } else {
        /*update movie*/
        ex_pack[0].b->budget = pack[0].b->budget;
        ex_pack[0].b->year = pack[0].b->year;
        strcpy(ex_pack[0].b->name, pack[0].b->name);

        strcpy(ex_pack[1].n->name, pack[1].n->name);
        strcpy(ex_pack[1].n->genre, pack[1].n->genre);
        ex_pack[1].n->score = pack[1].n->score;
    }
    return 0;
}

movie_pack *get_movie(FILE *src) {
    int i, counter = 0;
    char input, *ap, *apb = NULL;

    movie_pack *pack = (movie_pack *)calloc(2, sizeof(movie_pack));
    pack[0].b = (movie_budget *)malloc(sizeof(movie_budget));
    pack[1].n = (movie_name *)malloc(sizeof(movie_name));

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
            pack[0].b->budget = -1;
        } else {
            pack[0].b->budget = strtod(apb, NULL);
        }

        apb = strtok(NULL, ",");
        /*printf("%s; ", apb);*/
        /*if(genre != NULL) free(genre);*/
        pack[1].n->genre = (char *)calloc(strlen(apb) + 1, sizeof(char));
        strcpy(pack[1].n->genre, apb);

        apb = strtok(NULL, ",");
        /*printf("%s; ", apb);*/
        /*if(name != NULL) free(name);*/
        pack[0].b->name = (char *)calloc(strlen(apb) + 1, sizeof(char));
        strcpy(pack[0].b->name, apb);
        pack[1].n->name = (char *)calloc(strlen(apb) + 1, sizeof(char));
        strcpy(pack[1].n->name, apb);

        apb = strtok(NULL, ",");
        /*printf("%s; ", apb);*/
        pack[1].n->score = strtod(apb, NULL);

        apb = strtok(NULL, ",");
        /*printf("%s.\n", apb);*/
        pack[0].b->year = atoi(apb);
    }
    /*printf("\nbudget: %d, year: %d, name: %s, genre: %s, score: %.1f\n", *budget, *year, *name, *genre, *score);*/
    /*printf("ap = %s\n", ap);*/
    free(ap);

    return pack;
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
    int i = 0, success;
    FILE *src;
    movie_budget *head_mb;
    movie_name *head_mn;
    movie_pack *pack;
    char headers[5][10] = {0};

    src = fopen(SRCFILE, "r");
    head_mb = (movie_budget *)malloc(sizeof(movie_budget));
    head_mn = (movie_name *)malloc(sizeof(movie_name));
    get_headers(src, headers);
    while (!feof(src) && i < 3) {
        pack = get_movie(src);
        printf("\nbudget: %d, ", pack[0].b->budget);
        printf("year: %d, ", pack[0].b->year);
        printf("name: %s, ", pack[0].b->name);
        if (strcmp(pack[0].b->name, pack[1].n->name) != 0) printf("CONFLICT ");
        printf("genre: %s, ", pack[1].n->genre);
        printf("score: %.1f\n", pack[1].n->score);

        success = insert_movie(head_mb, head_mn, pack);
        if (success) {
            print_last_movie(head_mb, head_mn);
            i++;
        }
    }
    printf("\nNumber of unique movies = %d\n", i);
    print_movies(head_mb, head_mn);
    return 0;
}