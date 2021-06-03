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

void print_movie(movie_budget *mb, movie_name *mn) {
    /*printf("Movie budget = b: %d, y: %d, n: %s\n", mb->budget, mb->year, mb->name);
    printf("Movie name = g: %s, n: %s, s: %.2f\n\n", mn->genre, mn->name, mn->score);*/
    printf("%-15d %-12s %-45s %1.1f %10d\n", mb->budget, mn->genre, mb->name, mn->score, mb->year);
}
void print_movies(movie_budget *mb, movie_name *mn) {
    if (mb == NULL || mn == NULL) {
        printf("\n");
    } else {
        print_movie(mb, mn);
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

int compare_movies(movie_budget *mb, int year, int budget){
    if(year > mb->year) return 1;
    if(year == mb->year && budget > mb->budget) return 0;
    return -1;
}

movie_budget *sorted_insert(movie_budget **mb, int year) {
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

/* returns a movie_pack union array which including [mb_found, mn_found, mb_prev, mn_prev] */
movie_pack *search_movie_by_name(movie_budget *mb, movie_name *mn, char name[]) {
    int success = 0;
    movie_budget *mb_found = mb, *mb_prev = NULL;
    movie_name *mn_found = mn, *mn_prev = NULL;
    movie_pack *pack = (movie_pack *)calloc(4, sizeof(movie_pack));

    while (mb_found != NULL) {
        /*printf("mb search: %s vs %s\n", mb_search->name, name);*/
        if (strcmp(mb_found->name, name) == 0) {
            printf("Movie Budget exists.\n");
            pack[0].b = mb_found;
            pack[2].b = mb_prev;
            success = 1;
            break;
        }
        mb_prev = mb_found;
        mb_found = mb_found->next;
    }

    while (mn_found != NULL) {
        /*printf("mn search: %s vs %s\n", mn_search->name, name);*/
        if (strcmp(mn_found->name, name) == 0) {
            printf("Movie Name exists.\n");
            pack[1].n = mn_found;
            pack[3].n = mn_prev;
            success = 1;
            break;
        }
        mn_prev = mn_found;
        mn_found = mn_found->next;
    }

    if (success)
        return pack;
    else
        return NULL;
}

void remove_movie(movie_budget **mb, movie_name **mn, movie_pack *pack) {
    movie_budget *current_budget = pack[0].b, *prev_budget = pack[2].b;
    movie_name *current_name = pack[1].n, *prev_name = pack[3].n;

    /* if prev == current */
    if(current_budget != NULL){
        if(prev_budget == NULL/*current_budget == prev_budget*/) *mb = current_budget->next;
        else prev_budget->next = current_budget->next;
        free(current_budget);
    }

    if(current_name != NULL){
        if(prev_name == NULL/*current_name == prev_name*/) *mn = current_name->next;
        else prev_name->next = current_name->next;
        free(current_name);
    }
}

void insert_movie(movie_budget **mb, movie_name **mn, int budget, int year, char *name, char *genre, double score) {
    movie_budget *next_mb = (movie_budget *)malloc(sizeof(movie_budget)), *current_mb = *mb, *prev_mb;
    movie_name *next_mn = (movie_name *)malloc(sizeof(movie_name)), *current_mn = *mn, *prev_mn;

    next_mb->budget = budget;
    next_mb->year = year;
    next_mb->name = (char *)calloc(strlen(name) + 1, sizeof(char));
    strcpy(next_mb->name, name);

    next_mn->name = (char *)calloc(strlen(name) + 1, sizeof(char));
    strcpy(next_mn->name, name);
    next_mn->genre = (char *)calloc(strlen(genre) + 1, sizeof(char));
    strcpy(next_mn->genre, genre);
    next_mn->score = score;

    if (!(*mb == NULL && *mn == NULL) && (*mb == NULL || *mn == NULL)) {
        printf("\n\n Linked lists sync is broken.\nTerminating...\n");
        exit(1);
    }

    if((*mb == NULL && *mn == NULL) || (*mb)->year < year || ((*mb)->year == year && (*mb)->budget < budget)){
        next_mb->next = *mb;
        next_mn->next = *mn;

        *mb = next_mb;
        *mn = next_mn;
    } else {
        while((current_mb != NULL && current_mn != NULL) && ((current_mb->year > year) || (current_mb->year == year && current_mb->budget >= budget))) {
        /*while((current_mb != NULL && current_mn != NULL) && compare_movies(current_mb, year, budget) == -1) {*/
            /*printf("%d ", compare_movies(current_mb, year, budget));*/
            prev_mb = current_mb;
            current_mb = current_mb->next;

            prev_mn = current_mn;
            current_mn = current_mn->next;
        }
        printf("<locating after\n");
        if(prev_mb != NULL && prev_mn != NULL && current_mb != NULL && current_mn != NULL){ 
            /*printf("%d ", compare_movies(current_mb, year, budget));*/
            printf("prev = "); print_movie(prev_mb, prev_mn);
            printf("curr = "); print_movie(next_mb, next_mn);
            printf(">\n");
        }
        /* 4 -> 6 55* 444 3 22 1 */

        next_mb->next = current_mb;
        prev_mb->next = next_mb;

        next_mn->next = current_mn;
        prev_mn->next = next_mn;
        

    }

    /*next_mb->budget = budget;
    next_mb->year = year;
    next_mb->name = (char *)calloc(strlen(name) + 1, sizeof(char));
    strcpy(next_mb->name, name);

    next_mn->name = (char *)calloc(strlen(name) + 1, sizeof(char));
    strcpy(next_mn->name, name);
    next_mn->genre = (char *)calloc(strlen(genre) + 1, sizeof(char));
    strcpy(next_mn->genre, genre);
    next_mn->score = score;

    if (!(*mb == NULL && *mn == NULL) && (*mb == NULL || *mn == NULL)) {
        printf("\n\n Linked lists sync is broken.\nTerminating...\n");
        exit(1);
    }

    if ((*mb == NULL && *mn == NULL) || (((*mb)->year < year) || ((*mb)->year == year && (*mb)->budget < budget))) {
        printf("adding to head\n");
        next_mb->next = *mb;
        next_mn->next = *mn;

        *mb = next_mb;
        *mn = next_mn;
    } else {
        printf("adding to middle\n");while (temp_mb->next != NULL && !((temp_mb->year > year) || (temp_mb->year == year && (temp_mb->budget > budget)))) {
            temp_mb = temp_mb->next;
            temp_mn = temp_mn->next;
        }
        next_mb->next = temp_mb->next;
        temp_mb->next = next_mb;

        next_mn->next = temp_mn->next;
        temp_mn->next = next_mn;
    }
    return 1;*/
}

void parse_movie(FILE *src, int *budget, int *year, char **name, char **genre, double *score) {
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
            *budget = 0;
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
    int i = 0;
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
    while (!feof(src) /*&& i < 1000*/) {
        /*printf("< %d. list\n", i);
        print_movies(head_mb, head_mn);
        printf(">\n");*/
        
        /* parse movie datas from file */
        parse_movie(src, &budget, &year, &name, &genre, &score);

        /* remove existing movie for avoid duplicates */
        pack = search_movie_by_name(head_mb, head_mn, name);
        if(pack != NULL){
            /*print_movie(pack[0].b, pack[1].n);*/
            remove_movie(&head_mb, &head_mn, pack);
            i--;
        }

        /* insert movie to list */
        insert_movie(&head_mb, &head_mn, budget, year, name, genre, score);
        i++;
    }
    printf("Program is done.\n");
    print_movies(head_mb, head_mn);
    printf("Length = %d, end name = %s\n\n", i, name);

    /*pack = search_movie_by_name(head_mb, head_mn, "far and away");
    if(pack != NULL){
        printf("> pack\n");
        print_movie(pack[0].b, pack[1].n);
        print_movie(pack[2].b, pack[3].n);
        pack[0].b->budget = 38;
        strcpy(pack[1].n->genre, "adventure time");


        print_movies(head_mb, head_mn);
    } else {
        printf("Not found.\n");
    }*/

    return 0;
}