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
    if (mb == NULL || mn == NULL) {
        printf("\n");
    } else {
        printf("Movies\n");
        printf("Movie budget: b: %d, y: %d, n: %s\n", mb->budget, mb->year, mb->name);
        printf("Movie name: g: %s, n: %s, s: %.2f\n\n", mn->genre, mn->name, mn->score);
        print_movies(mb->next, mn->next);
    }
}

void print_movie(movie_budget *mb, movie_name *mn) {
    printf("Movie budget = b: %d, y: %d, n: %s\n", mb->budget, mb->year, mb->name);
    printf("Movie name = g: %s, n: %s, s: %.2f\n\n", mn->genre, mn->name, mn->score);
}

void print_last_movie(movie_budget *mb, movie_name *mn) {
    if (mb->next == NULL && mn->next == NULL) {
        printf("Movie budget = b: %d, y: %d, n: %s\n", mb->budget, mb->year, mb->name);
        printf("Movie name = g: %s, n: %s, s: %.2f\n\n", mn->genre, mn->name, mn->score);
    } else {
        print_last_movie(mb->next, mn->next);
    }
}

movie_budget *sorted_insert(movie_budget **mb, movie_name **mn, int budget, int year, char *name, char *genre, double score) {
    movie_budget *ce, *ne, *n;

    if ((*mb) == NULL || (*mb)->year < year) {
        n = (movie_budget *)malloc(sizeof(movie_budget));
        n->year = year;
        n->next = (*mb);
        (*mb) = n;
    } else {
        ce = (*mb);
        ne = ce->next;
        while (ce != NULL && ne != NULL && !(ce->year > year && year > ne->year)) {
            ce = ne;
            ne = ce->next;
        }
        n = (movie_budget *)malloc(sizeof(movie_budget));
        ce->next = n;
        n->year = year;
        n->next = ne;
    }
    return (*mb);
}

movie_pack *search_movie_by_name(movie_budget *mb, movie_name *mn, char name[]) {
    int success = 0;
    movie_budget *mb_search = mb;
    movie_name *mn_search = mn;
    movie_pack *pack = (movie_pack *)calloc(2, sizeof(movie_pack));

    while (mb_search != NULL) {
        /*printf("mb search: %s vs %s\n", mb_search->name, name);*/
        if (strcmp(mb_search->name, name) == 0) {
            printf("Movie Budget exists.\n");
            pack[0].b = mb_search;
            success = 1;
            break;
        }
        mb_search = mb_search->next;
    }

    while (mn_search != NULL) {
        /*printf("mn search: %s vs %s\n", mn_search->name, name);*/
        if (strcmp(mn_search->name, name) == 0) {
            printf("Movie Name exists.\n");
            pack[1].n = mn_search;
            success = 1;
            break;
        }
        mn_search = mn_search->next;
    }

    if (success)
        return pack;
    else
        return NULL;
}

int update_movie(movie_budget **mb, movie_name **mn, int budget, int year, char *name, char *genre, double score) {
    return 0;
}

int insert_movie(movie_budget **mb, movie_name **mn, int budget, int year, char *name, char *genre, double score) {
    movie_budget *next_mb = (movie_budget *)malloc(sizeof(movie_budget)), *temp_mb = *mb;
    movie_name *next_mn = (movie_name *)malloc(sizeof(movie_name)), *temp_mn = *mn;
    movie_pack *pack;
    
    pack = search_movie_by_name(*mb, *mn, name);

    if (pack == NULL) {
        next_mb->budget = budget;
        next_mb->year = year;
        next_mb->name = (char *)calloc(strlen(name) + 1, sizeof(char));
        strcpy(next_mb->name, name);

        next_mn->name = (char *)calloc(strlen(name) + 1, sizeof(char));
        strcpy(next_mn->name, name);
        next_mn->genre = (char *)calloc(strlen(genre) + 1, sizeof(char));
        strcpy(next_mn->genre, genre);
        next_mn->score = score;

        next_mb->next = NULL;
        next_mn->next = NULL;
        /*printf("Next movie\n");
        print_last_movie(next_mb, next_mn);*/

        if (*mb != NULL && *mn != NULL) {
            
            while (temp_mb->next != NULL)
                temp_mb = temp_mb->next;
            temp_mb->next = next_mb;

            while (temp_mn->next != NULL)
                temp_mn = temp_mn->next;
            temp_mn->next = next_mn;

        } else {
            *mb = next_mb;
            *mn = next_mn;
        }
        return 1;
    } else {
        printf("Updating.\n");
        pack[0].b->budget = budget;
        pack[0].b->year = year;
        pack[0].b->name = (char *)calloc(strlen(name) + 1, sizeof(char));
        strcpy(pack[0].b->name, name);

        pack[1].n->name = (char *)calloc(strlen(name) + 1, sizeof(char));
        strcpy(pack[1].n->name, name);
        pack[1].n->genre = (char *)calloc(strlen(genre) + 1, sizeof(char));
        strcpy(pack[1].n->genre, genre);
        pack[1].n->score = score;
    }
    free(next_mb);
    free(next_mn);
    return 0;
}

void get_movie(FILE *src, int *budget, int *year, char **name, char **genre, double *score) {
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
        *genre = (char *)calloc(strlen(apb) + 1, sizeof(char));
        strcpy(*genre, apb);

        apb = strtok(NULL, ",");
        /*printf("%s; ", apb);*/
        /*if(name != NULL) free(name);*/
        *name = (char *)calloc(strlen(apb) + 1, sizeof(char));
        strcpy(*name, apb);

        apb = strtok(NULL, ",");
        /*printf("%s; ", apb);*/
        *score = strtod(apb, NULL);

        apb = strtok(NULL, ",");
        /*printf("%s.\n", apb);*/
        *year = atoi(apb);
    }

    /*printf("\nbudget: %d, year: %d, name: %s, genre: %s, score: %.1f\n", *budget, *year, *name, *genre, *score);*/
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
    int i = 0, success;
    FILE *src;
    movie_budget *head_mb;
    movie_name *head_mn;
    movie_pack *pack;
    char headers[5][10] = {0};

    int budget, year;
    char *name, *genre;
    double score;

    src = fopen(SRCFILE, "r");
    head_mb = (movie_budget *)malloc(sizeof(movie_budget));
    head_mn = (movie_name *)malloc(sizeof(movie_name));
    head_mb = NULL;
    head_mn = NULL;

    get_headers(src, headers);
    while (!feof(src)) {
        get_movie(src, &budget, &year, &name, &genre, &score);
        
        /*printf("\nbudget: %d, ", budget);
        printf("year: %d, ", year);
        printf("name: %s, ", name);
        printf("genre: %s, ", genre);
        printf("score: %.1f\n", score);*/
        
        success = insert_movie(&head_mb, &head_mn, budget, year, name, genre, score);
        if (success) {
            printf("> last movie ");
            print_last_movie(head_mb, head_mn);
            i++;
        }
    }
    printf("Program is done.\n");
    print_movies(head_mb, head_mn);
    printf("Length = %d\n\n", i);


    /*pack = search_movie_by_name(head_mb, head_mn, "kikujiro");
    if(pack != NULL){
        printf("> pack\n");
        print_movie(pack[0].b, pack[1].n);
        pack[0].b->budget = 38;
        strcpy(pack[1].n->genre, "adventure time");


        print_movies(head_mb, head_mn);
    } else {
        printf("Not found.\n");
    }*/
    return 0;
}