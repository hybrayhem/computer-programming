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

typedef struct genre_n {
    int num;
    char *name;
    struct genre_n *next;
} genre_n;

void print_movie(movie_budget *mb, movie_name *mn);
void print_movies(movie_budget *mb, movie_name *mn);
void print_last_movie(movie_budget *mb, movie_name *mn);
movie_pack *search_movie_by_name(movie_budget *mb, movie_name *mn, char name[]);
void remove_movie(movie_budget **mb, movie_name **mn, movie_pack *pack);
int compare_movies_by_year_budget(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head);
int compare_movies_by_budget(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head);
int compare_movies_by_genre(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head);
int compare_movies_by_name(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head);
int compare_movies_by_score(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head);
int compare_movies_by_year(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head);
void insert_movie(movie_budget **mb, movie_name **mn, int budget, int year, char *name, char *genre, double score, int (*compare)(movie_budget *, movie_name *, movie_budget *, movie_name *, int));
void parse_movie(FILE *src, int *budget, int *year, char **name, char **genre, double *score);
void get_headers(FILE *src, char headers[][10]);

/* gets selection for menu with error handling */
double get_selection(const char msg[], int lower, int upper) {
    int status, flag = 0;
    double selection = 0.0;

    while (!flag) {
        if (msg[0] != '\0') printf("%s", msg);
        status = scanf("%lf", &selection);
        if (status < 1 || selection < lower || selection > upper) {
            while (getchar() != '\n')
                ;
            printf("TRY AGAIN\n");
            continue;
        }
        flag = 1;
    }

    return selection;
}

genre_n *search_genre(genre_n *head_g, const char *genre) {
    genre_n *g_search = head_g;

    while (g_search != NULL) {
        if (strcmp(g_search->name, genre) == 0) {
            /*printf("genre exists (%s)\n", genre);*/
            return g_search;
        }
        g_search = g_search->next;
    }
    return NULL;
}

void init_average(double *average, movie_name *mn) {
    int i = 0;
    *average = 0.0;

    if (mn == NULL) {
        printf("List is empty.\n");
        return;
    }

    while (mn != NULL) {
        (*average) += mn->score;
        i++;
        mn = mn->next;
    }
    *average = (double)(*average) / i;
}

void init_genres(genre_n **head_g, movie_name *mn) {
    genre_n *new_genre, *current_g, *temp_g;

    while (mn != NULL) {
        /*printf("%s\n", mn->genre);*/

        current_g = search_genre(*head_g, mn->genre);
        if (current_g == NULL) {
            new_genre = (genre_n *)malloc(sizeof(genre_n));

            (new_genre->num) = 1;
            new_genre->name = (char *)calloc(strlen(mn->genre) + 1, sizeof(char)); /* TODO: Null allocation check */
            strcpy(new_genre->name, mn->genre);
            new_genre->next = NULL;

            if (*head_g == NULL)
                *head_g = new_genre;
            else {
                temp_g = *head_g;
                while (temp_g->next != NULL)
                    temp_g = temp_g->next;
                temp_g->next = new_genre;
            }
        } else {
            (current_g->num)++;
        }

        mn = mn->next;
    }
    printf("\n");
}

void print_genres(genre_n *genre, int withnum) {
    if (genre == NULL) {
        printf("\n");
    } else {
        printf("%-12s ", genre->name);
        if (withnum) printf("%d", genre->num);
        printf("\n");

        print_genres(genre->next, withnum);
    }
}

void print_movies_by_year(movie_budget *mb, movie_name *mn, int year, int base) { /* base = 0 if(mb.year <= year) */ /* base = 1 if(mb.year > year) */
    if (mb == NULL && mn == NULL) {
        printf("\n");
    } else {
        if ((!base && mb->year < year) || (base && mb->year >= year))
            print_movie(mb, mn); /* TODO: change with printf("%s\n", mb->name); */
        print_movies_by_year(mb->next, mn->next, year, base);
    }
}

void print_movies_by_score(movie_budget *mb, movie_name *mn, int score, int base) {
    if (mb == NULL && mn == NULL) {
        printf("\n");
    } else {
        if ((!base && mn->score < score) || (base && mn->score >= score))
            print_movie(mb, mn); /* TODO: change with printf("%s\n", mn->name); */
        print_movies_by_score(mb->next, mn->next, score, base);
    }
}

void sort_movies(movie_budget **head_mb, movie_name **head_mn, int (*compare)(movie_budget *, movie_name *, movie_budget *, movie_name *, int)) {
    movie_budget *sorted_mb = NULL, *temp_mb, *current_mb = *head_mb;
    movie_name *sorted_mn = NULL, *temp_mn, *current_mn = *head_mn;

    while (current_mb != NULL && current_mn != NULL) {
        insert_movie(&sorted_mb, &sorted_mn, current_mb->budget, current_mb->year, current_mb->name, current_mn->genre, current_mn->score, compare);

        temp_mb = current_mb;
        temp_mn = current_mn;

        current_mb = current_mb->next;
        current_mn = current_mn->next;

        free(temp_mb);
        free(temp_mn);
    }
    *head_mb = sorted_mb;
    *head_mn = sorted_mn;
}

int main() {
    FILE *src;
    movie_budget *head_mb;
    movie_name *head_mn;
    movie_pack *pack;
    genre_n *genres;
    int i = 0, inp_selection, inp_sort_type, inp_year, inp_base;
    double imdb_average, inp_score;
    char headers[5][10] = {0}, msg[100];

    int budget, year;
    char *name, *genre;
    double score;

    src = fopen(SRCFILE, "r");

    genres = (genre_n *)malloc(sizeof(genre_n)); /* TODO: check is necessary or not */
    genres = NULL;

    head_mb = (movie_budget *)malloc(sizeof(movie_budget)); /* TODO: check is necessary or not */
    head_mn = (movie_name *)malloc(sizeof(movie_name));     /* TODO: check is necessary or not */
    head_mb = NULL;
    head_mn = NULL;

    printf("\nLoading movies from storage...\n\n");
    get_headers(src, headers);
    while (!feof(src)) {
        /* parse movie datas from file */
        parse_movie(src, &budget, &year, &name, &genre, &score);

        /* remove existing movie for avoid duplicates */
        pack = search_movie_by_name(head_mb, head_mn, name);
        if (pack != NULL) {
            remove_movie(&head_mb, &head_mn, pack);
            i--;
        }

        /* insert movie to list */
        insert_movie(&head_mb, &head_mn, budget, year, name, genre, score, compare_movies_by_year_budget);
        i++;
    }
    printf("Program is done.\n");
    /*print_movies(head_mb, head_mn);*/
    printf("%d movies found. end name = %s\n\n", i, name);
    init_genres(&genres, head_mn); /* TODO if null init in cases, made this for also imdb */
    init_average(&imdb_average, head_mn);

    /*goto end;*/
    /* MENU */
    printf("1. List of the Sorted Data\n");
    printf("2. List of the Genres\n");
    printf("3. List of the Movie Through the Years\n");
    printf("4. List of the Movie Through the Scores\n");
    printf("5. All Informations of a Single Movie\n");
    printf("6. Average of the IMDB Scores\n");
    printf("7. Frequence of the Genres\n");
    printf("8. Exit\n\n");

    while (inp_selection != 8) {
        inp_selection = get_selection("Please select and operation: ", 1, 8);
        switch (inp_selection) {
        case 1:
            printf("1. Budget\n2. Genre\n3. Name\n4. Score\n5.Year\n");
            inp_selection = get_selection("Please select and operation: ", 1, 5);
            
            printf("Sorting... ");
            if(inp_selection == 1) sort_movies(&head_mb, &head_mn, compare_movies_by_budget);
            else if(inp_selection == 2) sort_movies(&head_mb, &head_mn, compare_movies_by_genre);
            else if(inp_selection == 4) sort_movies(&head_mb, &head_mn, compare_movies_by_name);
            else if(inp_selection == 3) sort_movies(&head_mb, &head_mn, compare_movies_by_score);
            else if(inp_selection == 5) sort_movies(&head_mb, &head_mn, compare_movies_by_year);
            printf("Done\n");

            printf("1. Single Selection\n2. Multiple Selection\n");
            inp_selection = get_selection("Please select and operation: ", 1, 2);
            /* TODO: handle selections */
            break;
        case 2:
            printf("\n");
            print_genres(genres, 0);
            break;
        case 3:
            inp_year = get_selection("Enter a year: ", 0, 2021);
            sprintf(msg, "Until(0) or Since(1) %d:", inp_year);
            inp_base = get_selection(msg, 0, 1);
            print_movies_by_year(head_mb, head_mn, inp_year, inp_base); /* TODO: if no movie print try again */
            break;
        case 4:
            inp_score = get_selection("Enter a score: ", 0, 10);
            sprintf(msg, "Less(0) or Greater(1) %.1f:", inp_score);
            inp_base = get_selection(msg, 0, 1);
            print_movies_by_score(head_mb, head_mn, inp_score, inp_base); /* TODO: if no movie print try again */
            break;
        case 5:
            printf("Please enter the name of the movie: ");
            fflush(stdin);
            scanf(" %[^\n]%*c", msg);
            pack = search_movie_by_name(head_mb, head_mn, msg);
            if (pack != NULL) {
                printf("Budget: ");
                if (pack[0].b->budget == -1)
                    printf("Unknown\n");
                else
                    printf("%d\n", pack[0].b->budget);
                printf("Genre: %s\n", pack[1].n->genre);
                printf("Name: %s\n", pack[0].b->name);
                printf("Score: %.1f\n", pack[1].n->score);
                printf("Year: %d\n\n", pack[0].b->year);
            } else {
                printf("Movie not found.\n\n");
            }
            break;
        case 6:
            printf("Average: %g\n", imdb_average);
            break;
        case 7:
            printf("\n");
            print_genres(genres, 1);
            break;

        default:
            break;
        }
    }
end:
    free(head_mb);
    free(head_mn);
    return 0;
}

void print_movie(movie_budget *mb, movie_name *mn) {
    if (mb->budget == -1)
        printf("%-15s ", "-");
    else
        printf("%-15d ", mb->budget);

    printf("%-12s %-45s %1.1f %10d\n", mn->genre, mb->name, mn->score, mb->year);
}

void print_movies(movie_budget *mb, movie_name *mn) {
    if (mb == NULL && mn == NULL) {
        printf("\n");
    } else {
        print_movie(mb, mn);
        print_movies(mb->next, mn->next);
    }
}

void print_last_movie(movie_budget *mb, movie_name *mn) {
    if (mb->next == NULL && mn->next == NULL) {
        print_movie(mb, mn);
    } else {
        print_last_movie(mb->next, mn->next);
    }
}

/* returns a movie_pack union array which including [mb_found, mn_found, mb_prev, mn_prev] */
/* TODO: increase algorithm efficiency */
movie_pack *search_movie_by_name(movie_budget *mb, movie_name *mn, char name[]) {
    int success = 0;
    movie_budget *mb_found = mb, *mb_prev = NULL;
    movie_name *mn_found = mn, *mn_prev = NULL;
    movie_pack *pack = (movie_pack *)calloc(4, sizeof(movie_pack));

    while (mb_found != NULL) {
        if (strcmp(mb_found->name, name) == 0) {
            pack[0].b = mb_found;
            pack[2].b = mb_prev;
            success = 1;
            break;
        }
        mb_prev = mb_found;
        mb_found = mb_found->next;
    }

    while (mn_found != NULL) {
        if (strcmp(mn_found->name, name) == 0) {
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

    if (current_budget != NULL) {
        if (prev_budget == NULL)
            *mb = current_budget->next;
        else
            prev_budget->next = current_budget->next;
        free(current_budget);
    }

    if (current_name != NULL) {
        if (prev_name == NULL)
            *mn = current_name->next;
        else
            prev_name->next = current_name->next;
        free(current_name);
    }
}

int compare_movies_by_year_budget(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head) {
    if (head) return (mb->year < next_mb->year || (mb->year == next_mb->year && mb->budget < next_mb->budget));
    return ((mb->year > next_mb->year) || (mb->year == next_mb->year && mb->budget >= next_mb->budget));
}
/* printf("1. Budget\n2. Genre\n3. Name\n4. Score\n5.Year\n"); */
int compare_movies_by_budget(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head) {
    if (head) return (mb->budget < next_mb->budget);
    return (mb->budget >= next_mb->budget);
}

int compare_movies_by_genre(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head) {
    if (head) return (strcmp(mn->genre, next_mn->genre) < 0);
    return (strcmp(mn->genre, next_mn->genre) >= 0);
}

int compare_movies_by_name(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head) {
    if (head) return (strcmp(mb->name, next_mb->name) < 0);
    return (strcmp(mb->name, next_mb->name) >= 0);
}

int compare_movies_by_score(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head) {
    if (head) return (mn->score < next_mn->score);
    return (mn->score >= next_mn->score);
}

int compare_movies_by_year(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head) {
    if (head) return (mb->year < next_mb->year);
    return (mb->year >= next_mb->year);
}

void insert_movie(movie_budget **mb, movie_name **mn, int budget, int year, char *name, char *genre, double score, int (*compare)(movie_budget *, movie_name *, movie_budget *, movie_name *, int)) {
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

    /*if ((*mb == NULL && *mn == NULL) || (*mb)->year < year || ((*mb)->year == year && (*mb)->budget < budget)) {*/
    if ((*mb == NULL && *mn == NULL) || (*compare)(*mb, *mn, next_mb, next_mn, 1)) {
        next_mb->next = *mb;
        next_mn->next = *mn;

        *mb = next_mb;
        *mn = next_mn;
    } else {
        /*while ((current_mb != NULL && current_mn != NULL) && ((current_mb->year > year) || (current_mb->year == year && current_mb->budget >= budget))) {*/
        while ((current_mb != NULL && current_mn != NULL) && (*compare)(current_mb, current_mn, next_mb, next_mn, 0)) {
            prev_mb = current_mb;
            current_mb = current_mb->next;

            prev_mn = current_mn;
            current_mn = current_mn->next;
        }

        next_mb->next = current_mb;
        prev_mb->next = next_mb;

        next_mn->next = current_mn;
        prev_mn->next = next_mn;
    }
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
        if (strcmp(apb, "unknown") == 0) {
            *budget = -1;
        } else {
            *budget = strtod(apb, NULL);
        }

        apb = strtok(NULL, ",");
        *genre = (char *)calloc(strlen(apb) + 1, sizeof(char));
        strcpy(*genre, apb);

        apb = strtok(NULL, ",");
        *name = (char *)calloc(strlen(apb) + 1, sizeof(char));
        strcpy(*name, apb);

        apb = strtok(NULL, ",");
        *score = strtod(apb, NULL);

        apb = strtok(NULL, ",");
        *year = atoi(apb);
    }

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
