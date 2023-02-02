/*
** movie_browser.c:
**
** A data browser works on formatted movie file.
**
** Author: Halil Ilhan.
**
** Revision: 2021.06.05
** 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SRCFILE "Movies.txt"

/* --------------------- Structure & Union Declarations --------------------- */
/* movie_budget node */
typedef struct movie_budget {
    int budget, year;
    char *name;
    struct movie_budget *next;
} movie_budget;

/* movie_name node */
typedef struct movie_name {
    char *genre, *name;
    double score;
    struct movie_name *next;
} movie_name;

/* union for packing movie_name and movie_budgets in an array */
typedef union movie_pack {
    movie_budget *b;
    movie_name *n;
} movie_pack;

/* linked list node for unique movie genres */
typedef struct genre_n {
    int num;
    char *name;
    struct genre_n *next;
} genre_n;



/* -------------------------------------------------------------------------- */
/*                                 Prototypes                                 */
/* -------------------------------------------------------------------------- */

/* -------------------------- Comparison & Sorting functions -------------------------- */
int compare_movies_by_year_and_budget(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head);
int compare_movies_by_budget(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head);
int compare_movies_by_genre(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head);
int compare_movies_by_name(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head);
int compare_movies_by_score(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head);
int compare_movies_by_year(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head);
void sort_movies(movie_budget **head_mb, movie_name **head_mn, int (*compare)(movie_budget *, movie_name *, movie_budget *, movie_name *, int));

/* ---------------------------- Movie operations ---------------------------- */
void parse_movie(FILE *src, int *budget, int *year, char **name, char **genre, double *score);
movie_pack *search_movie_by_name(movie_budget *mb, movie_name *mn, char name[]);
void insert_movie(movie_budget **mb, movie_name **mn, int budget, int year, char *name, char *genre, double score, int (*compare)(movie_budget *, movie_name *, movie_budget *, movie_name *, int));
void remove_movie(movie_budget **mb, movie_name **mn, movie_pack *pack);

/* ---------------------------- Genre operations ---------------------------- */
void init_genres(genre_n **head_g, movie_name *mn);
genre_n *search_genre(genre_n *head_g, const char *genre);
void print_genres(genre_n *genre, int withnum);

/* -------------------------- Movie print functions ------------------------- */
void print_movie(movie_budget *mb, movie_name *mn);
void print_movies_by_interval(movie_budget *mb, movie_name *mn, int start, int end);
void print_movies_by_budget_interval(movie_budget *mb, movie_name *mn, int start, int end);
void print_movies_by_score_interval(movie_budget *mb, movie_name *mn, double start, double end);
void print_movies_by_year_interval(movie_budget *mb, movie_name *mn, int start, int end);
void print_movies_by_year_index(movie_budget *mb, movie_name *mn, int year, int base, int *success);
void print_movies_by_score_index(movie_budget *mb, movie_name *mn, double score, int base, int *success);

/* ------------------------------ IO Functions ------------------------------ */
void get_headers(FILE *src, char headers[][10]);
double get_selection(const char msg[], int lower, int upper);
void init_average(double *average, movie_name *mn);

/* ---------------------------- Ending operations --------------------------- */
void free_all(movie_budget *mb, movie_name *mn, genre_n *genres);
void terminate(char err[]);



/* -------------------------------------------------------------------------- */
/*                                    MAIN                                    */
/* -------------------------------------------------------------------------- */
int main() {
    /* linked list and file declarations */
    FILE *src;
    movie_budget *head_mb = NULL;
    movie_name *head_mn = NULL;
    movie_pack *pack;
    genre_n *genres = NULL;
    int i = 0, success, inp_selection, inp_sort_type, inp_year, inp_base, inp_start, inp_end, max_budget = 0; /* temporary input variables */
    double imdb_average = -1.0, inp_score, inp_dstart, inp_dend;
    char headers[5][10] = {0}, msg[100];

    /* temporary variables for keeping a movie in */
    int budget, year;
    char *name, *genre;
    double score;

    src = fopen(SRCFILE, "r");
    if (src == NULL) terminate("Couldn't open file.");  /* if file not exists terminate */
    printf("\nLoading movies from the storage...\n\n");
    get_headers(src, headers);
    while (!feof(src)) {
        /* parse movie datas from file */
        parse_movie(src, &budget, &year, &name, &genre, &score);
        if (budget > max_budget) max_budget = budget;

        /* remove existing movies for avoid duplicates */
        pack = search_movie_by_name(head_mb, head_mn, name);
        if (pack != NULL) {
            remove_movie(&head_mb, &head_mn, pack);
            i--;
        }

        /* insert movie to list */
        insert_movie(&head_mb, &head_mn, budget, year, name, genre, score, compare_movies_by_year_and_budget);
        i++;
    }
    if (i == 0) terminate("Source file is empty."); /* if any movie not found */
    printf("%d movies loaded to the memory.\n\n", i);

    while (inp_selection != 8) {

        /* MENU */
        printf("\n1. List of the Sorted Data\n");
        printf("2. List of the Genres\n");
        printf("3. List of the Movie Through the Years\n");
        printf("4. List of the Movie Through the Scores\n");
        printf("5. All Informations of a Single Movie\n");
        printf("6. Average of the IMDB Scores\n");
        printf("7. Frequence of the Genres\n");
        printf("8. Exit\n\n");
        inp_selection = get_selection("Please select and operation: ", 1, 8);

        switch (inp_selection) {

        /* Sorted Data Sub-menu */
        case 1:
            printf("\n1. Budget\n2. Genre\n3. Name\n4. Score\n5. Year\n\n");
            inp_sort_type = get_selection("Please select and operation: ", 1, 5);

            printf("\nSorting... ");
            if (inp_sort_type == 1)
                sort_movies(&head_mb, &head_mn, compare_movies_by_budget);
            else if (inp_sort_type == 2)
                sort_movies(&head_mb, &head_mn, compare_movies_by_genre);
            else if (inp_sort_type == 3)
                sort_movies(&head_mb, &head_mn, compare_movies_by_name);
            else if (inp_sort_type == 4)
                sort_movies(&head_mb, &head_mn, compare_movies_by_score);
            else if (inp_sort_type == 5)
                sort_movies(&head_mb, &head_mn, compare_movies_by_year);
            printf("Done\n");

            printf("\n1. Single Selection\n2. Multiple Selection\n\n");
            inp_selection = get_selection("Please select and operation: ", 1, 2);
            if (inp_selection == 1) {
                inp_start = get_selection("Enter a value: ", 1, i);
                printf("\n%d. Movie\n\n", inp_start);
                print_movies_by_interval(head_mb, head_mn, inp_start, inp_start);
            } else {

                if (inp_sort_type == 1) {

                    inp_start = get_selection("Enter the start value: ", 0, max_budget);
                    inp_end = get_selection("Enter the end value: ", 0, max_budget);
                    printf("\nMovies between %d and %d, sorted by budget.\n\n", inp_start, inp_end);
                    print_movies_by_budget_interval(head_mb, head_mn, inp_start, inp_end);

                } else if (inp_sort_type == 2) {

                    inp_start = get_selection("Enter the start value: ", 0, i);
                    inp_end = get_selection("Enter the end value: ", 0, i);
                    printf("\nMovies between %d and %d, sorted by genre.\n\n", inp_start, inp_end);
                    print_movies_by_interval(head_mb, head_mn, inp_start, inp_end);

                } else if (inp_sort_type == 3) {

                    inp_start = get_selection("Enter the start value: ", 0, i);
                    inp_end = get_selection("Enter the end value: ", 0, i);
                    printf("\nMovies between %d and %d, sorted by name.\n\n", inp_start, inp_end);
                    print_movies_by_interval(head_mb, head_mn, inp_start, inp_end);

                } else if (inp_sort_type == 4) {

                    inp_dstart = get_selection("Enter the start value: ", 0, 10);
                    inp_dend = get_selection("Enter the end value: ", 0, 10);
                    printf("\nMovies between %.1f and %.1f, sorted by score.\n\n", inp_dstart, inp_dend);
                    print_movies_by_score_interval(head_mb, head_mn, inp_dstart, inp_dend);

                } else if (inp_sort_type == 5) {

                    inp_start = get_selection("Enter the start value: ", 0, 2021);
                    inp_end = get_selection("Enter the end value: ", 0, 2021);
                    printf("\nMovies between %d and %d, sorted by year.\n\n", inp_start, inp_end);
                    print_movies_by_year_interval(head_mb, head_mn, inp_start, inp_end);

                } else {
                    printf(".\n\n");
                }
            }
            break;

        /* Genre List Sub-menu */
        case 2:
            printf("\n");
            if (genres == NULL) init_genres(&genres, head_mn);
            print_genres(genres, 0);
            break;
        
        
        case 3:
            success = 0;
            do {
                inp_year = get_selection("Enter a year: ", 0, 2021);
                sprintf(msg, "Until(0) or Since(1) %d:", inp_year);
                inp_base = get_selection(msg, 0, 1);

                sort_movies(&head_mb, &head_mn, compare_movies_by_year);
                printf("\n");

                print_movies_by_year_index(head_mb, head_mn, inp_year, inp_base, &success);
                if (!success) printf("Movie not found.\n\n");
            } while (!success);
            break;
        
        case 4:
            success = 0;
            do {
                inp_score = get_selection("Enter a score: ", 0, 10);
                sprintf(msg, "Less(0) or Greater(1) %.1f:", inp_score);
                inp_base = get_selection(msg, 0, 1);

                sort_movies(&head_mb, &head_mn, compare_movies_by_score);
                printf("\n");

                print_movies_by_score_index(head_mb, head_mn, inp_score, inp_base, &success);
                if (!success) printf("Movie not found.\n\n");
            } while (!success);
            break;
        
        case 5:
            printf("Please enter the name of the movie: ");
            fflush(stdin);
            scanf(" %[^\n]%*c", msg);
            pack = search_movie_by_name(head_mb, head_mn, msg);
            if (pack != NULL) {
                printf("\nBudget: ");
                if (pack[0].b->budget == -1)
                    printf("Unknown\n");
                else
                    printf("%d\n", pack[0].b->budget);
                printf("Genre: %s\n", pack[1].n->genre);
                printf("Name: %s\n", pack[0].b->name);
                printf("Score: %.1f\n", pack[1].n->score);
                printf("Year: %d\n\n", pack[0].b->year);
            } else {
                printf("\nMovie not found.\n\n");
            }
            break;
        
        case 6:
            if (imdb_average == -1.0) init_average(&imdb_average, head_mn);
            printf("\nAverage: %g\n", imdb_average);
            break;
        
        case 7:
            printf("\n");
            if (genres == NULL) init_genres(&genres, head_mn);
            print_genres(genres, 1);
            break;

        default:
            break;
        }
    }

    free_all(head_mb, head_mn, genres);
    fclose(src);
    return 0;
}



/* -------------------------------------------------------------------------- */
/*                       Comparison & Sorting functions                       */
/* -------------------------------------------------------------------------- */
/* returning a bool value if insert at head or not */
int compare_movies_by_year_and_budget(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head) {
    if (head) return (mb->year < next_mb->year || (mb->year == next_mb->year && mb->budget < next_mb->budget));
    return ((mb->year > next_mb->year) || (mb->year == next_mb->year && mb->budget >= next_mb->budget));
}
int compare_movies_by_budget(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head) {
    if (head) return (mb->budget > next_mb->budget);
    return (mb->budget <= next_mb->budget);
}
int compare_movies_by_genre(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head) {
    if (head) return (strcmp(mn->genre, next_mn->genre) > 0);
    return (strcmp(mn->genre, next_mn->genre) <= 0);
}
int compare_movies_by_name(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head) {
    if (head) return (strcmp(mb->name, next_mb->name) > 0);
    return (strcmp(mb->name, next_mb->name) <= 0);
}
int compare_movies_by_score(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head) {
    if (head) return (mn->score > next_mn->score);
    return (mn->score <= next_mn->score);
}
int compare_movies_by_year(movie_budget *mb, movie_name *mn, movie_budget *next_mb, movie_name *next_mn, int head) {
    if (head) return (mb->year > next_mb->year);
    return (mb->year <= next_mb->year);
}

/* insertions sort based on comparator func */
void sort_movies(movie_budget **head_mb, movie_name **head_mn, int (*compare)(movie_budget *, movie_name *, movie_budget *, movie_name *, int)) {
    movie_budget *sorted_mb = NULL, *temp_mb, *current_mb = *head_mb;
    movie_name *sorted_mn = NULL, *temp_mn, *current_mn = *head_mn;

    while (current_mb != NULL && current_mn != NULL) {
        insert_movie(&sorted_mb, &sorted_mn, current_mb->budget, current_mb->year, current_mb->name, current_mn->genre, current_mn->score, compare);

        temp_mb = current_mb;
        temp_mn = current_mn;

        current_mb = current_mb->next;
        current_mn = current_mn->next;

        if (temp_mb->name != NULL) free(temp_mb->name);
        if (temp_mn->name != NULL) free(temp_mn->name);
        if (temp_mn->genre != NULL) free(temp_mn->genre);
        free(temp_mb);
        free(temp_mn);
    }
    *head_mb = sorted_mb;
    *head_mn = sorted_mn;
}


/* -------------------------------------------------------------------------- */
/*                              Movie Operations                              */
/* -------------------------------------------------------------------------- */
void parse_movie(FILE *src, int *budget, int *year, char **name, char **genre, double *score) {
    int i, counter = 0;
    char input, *ap, *apb = NULL;

    do {
        fscanf(src, "%c", &input);
        counter++;

        ap = (char *)malloc((counter + 1) * sizeof(char));
        if(ap == NULL) terminate("Out of memory.\n");
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
        if(*genre == NULL) terminate("Out of memory.\n");
        strcpy(*genre, apb);

        apb = strtok(NULL, ",");
        *name = (char *)calloc(strlen(apb) + 1, sizeof(char));
        if(*name == NULL) terminate("Out of memory.\n");        
        strcpy(*name, apb);

        apb = strtok(NULL, ",");
        *score = strtod(apb, NULL);

        apb = strtok(NULL, ",");
        *year = atoi(apb);
        while((*year) > 9999) (*year) /= 10; 
    }

    free(ap);
}

/* returns a movie_pack union array which including [mb_found, mn_found, mb_prev, mn_prev] */
movie_pack *search_movie_by_name(movie_budget *mb, movie_name *mn, char name[]) {
    int success = 0;
    movie_budget *mb_found = mb, *mb_prev = NULL;
    movie_name *mn_found = mn, *mn_prev = NULL;
    movie_pack *pack = (movie_pack *)calloc(4, sizeof(movie_pack));
    if(pack == NULL) terminate("Out of memory.\n");

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

void insert_movie(movie_budget **mb, movie_name **mn, int budget, int year, char *name, char *genre, double score, int (*compare)(movie_budget *, movie_name *, movie_budget *, movie_name *, int)) {
    movie_budget *next_mb = (movie_budget *)malloc(sizeof(movie_budget)), *current_mb = *mb, *prev_mb;
    movie_name *next_mn = (movie_name *)malloc(sizeof(movie_name)), *current_mn = *mn, *prev_mn;

    if(next_mb == NULL) terminate("Out of memory.\n");
    if(next_mn == NULL) terminate("Out of memory.\n");

    next_mb->budget = budget;
    next_mb->year = year;
    next_mb->name = (char *)calloc(strlen(name) + 1, sizeof(char));
    if(next_mb->name == NULL) terminate("Out of memory.\n");
    strcpy(next_mb->name, name);

    next_mn->name = (char *)calloc(strlen(name) + 1, sizeof(char));
    if(next_mn->name == NULL) terminate("Out of memory.\n");
    strcpy(next_mn->name, name);
    next_mn->genre = (char *)calloc(strlen(genre) + 1, sizeof(char));
    if(next_mn->genre == NULL) terminate("Out of memory.\n");
    strcpy(next_mn->genre, genre);
    next_mn->score = score;

    if (!(*mb == NULL && *mn == NULL) && (*mb == NULL || *mn == NULL)) terminate("Linked lists sync is broken.\nTerminating...");

    if ((*mb == NULL && *mn == NULL) || (*compare)(*mb, *mn, next_mb, next_mn, 1)) {
        next_mb->next = *mb;
        next_mn->next = *mn;

        *mb = next_mb;
        *mn = next_mn;
    } else {
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


/* -------------------------------------------------------------------------- */
/*                              Genre Operations                              */
/* -------------------------------------------------------------------------- */

void init_genres(genre_n **head_g, movie_name *mn) {
    genre_n *new_genre, *current_g, *temp_g;

    while (mn != NULL) {

        current_g = search_genre(*head_g, mn->genre);
        if (current_g == NULL) {
            new_genre = (genre_n *)malloc(sizeof(genre_n));
            if(new_genre == NULL) terminate("Out of memory.\n");

            (new_genre->num) = 1;
            new_genre->name = (char *)calloc(strlen(mn->genre) + 1, sizeof(char));
            if(new_genre->name == NULL) terminate("Out of memory.\n");
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

genre_n *search_genre(genre_n *head_g, const char *genre) {
    genre_n *g_search = head_g;

    while (g_search != NULL) {
        if (strcmp(g_search->name, genre) == 0) {
            return g_search;
        }
        g_search = g_search->next;
    }
    return NULL;
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


/* -------------------------------------------------------------------------- */
/*                            Movie print functions                           */
/* -------------------------------------------------------------------------- */
void print_movie(movie_budget *mb, movie_name *mn) {
    if (mb->budget == -1)
        printf("%-15s ", "-");
    else
        printf("%-15d ", mb->budget);

    printf("%-12s %-45s %1.1f %10d\n", mn->genre, mb->name, mn->score, mb->year);
}

void print_movies_by_interval(movie_budget *mb, movie_name *mn, int start, int end) {
    int i;

    for (i = 1; i < start; i++) {
        mb = mb->next;
        mn = mn->next;
    }

    for (i = start; i <= end && mb != NULL && mn != NULL; i++) {
        print_movie(mb, mn);

        mb = mb->next;
        mn = mn->next;
    }
}

void print_movies_by_budget_interval(movie_budget *mb, movie_name *mn, int start, int end) {
    while (mb != NULL && mn != NULL && mb->budget < start) {
        mb = mb->next;
        mn = mn->next;
    }
    while (mb != NULL && mn != NULL && mb->budget <= end) {
        print_movie(mb, mn);
        mb = mb->next;
        mn = mn->next;
    }
}

void print_movies_by_score_interval(movie_budget *mb, movie_name *mn, double start, double end) {
    while (mb != NULL && mn != NULL && mn->score < start) {
        mb = mb->next;
        mn = mn->next;
    }
    while (mb != NULL && mn != NULL && mn->score <= end) {
        print_movie(mb, mn);
        mb = mb->next;
        mn = mn->next;
    }
}

void print_movies_by_year_interval(movie_budget *mb, movie_name *mn, int start, int end) {
    while (mb != NULL && mn != NULL && mb->year < start) {
        mb = mb->next;
        mn = mn->next;
    }
    while (mb != NULL && mn != NULL && mb->year <= end) {
        print_movie(mb, mn);
        mb = mb->next;
        mn = mn->next;
    }
}

void print_movies_by_year_index(movie_budget *mb, movie_name *mn, int year, int base, int *success) {
    if (mb == NULL && mn == NULL) {
        printf("\n");
    } else {
        if ((!base && mb->year < year) || (base && mb->year >= year)) {
            *success = 1;
            printf("%s\n", mb->name);
        }
        print_movies_by_year_index(mb->next, mn->next, year, base, success);
    }
}

void print_movies_by_score_index(movie_budget *mb, movie_name *mn, double score, int base, int *success) {
    if (mb == NULL && mn == NULL) {
        printf("\n");
    } else {
        if ((!base && mn->score < score) || (base && mn->score >= score)) {
            *success = 1;
            printf("%s\n", mn->name);
        }
        print_movies_by_score_index(mb->next, mn->next, score, base, success);
    }
}

/* -------------------------------------------------------------------------- */
/*                                IO Functions                               */
/* -------------------------------------------------------------------------- */
/* extracting headers from the first line of file */
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

/* secure scanf function that gets the selection for menu with a bullet-proof error handling */
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


/* -------------------------------------------------------------------------- */
/*                              Ending Operations                             */
/* -------------------------------------------------------------------------- */
void free_all(movie_budget *mb, movie_name *mn, genre_n *genres) {
    movie_budget *temp_mb;
    movie_name *temp_mn;
    genre_n *temp_g;

    while (mb != NULL && mn != NULL) {
        temp_mb = mb;
        temp_mn = mn;
        mb = mb->next;
        mn = mn->next;

        if (temp_mb->name != NULL) free(temp_mb->name);
        if (temp_mn->name != NULL) free(temp_mn->name);
        if (temp_mn->genre != NULL) free(temp_mn->genre);
        free(temp_mb);
        free(temp_mn);
    }

    while (genres != NULL) {
        temp_g = genres;
        genres = genres->next;

        if (temp_g->name != NULL) free(temp_g->name);
        free(temp_g);
    }
}

void terminate(char err[]) {
    printf("\n%s\n\n", err);
    exit(0);
}
