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
int get_selection(int lower, int upper);
int insert_movie(Movie movie, Movie movies[], int *l);
int update_movie(Movie movie, Movie movies[], int l);
int numerate_genre(char **genres, const char *genre);
void get_movie(FILE *src, Movie *movie, char **genres);

/* counting number of lines in file */
int count_lines(FILE *src){
    char c;
    int nol = 0;
    for (c = getc(src); c != EOF; c = getc(src)){
        if (c == '\n') nol++;
    }
    rewind(src);

    return nol+1;
}

/* prints movies until l. */
void print_movies(Movie *movies, int l){
    int i;
    for(i=0; i < l; i++){
        printf("%d. movie = budget: %.1f, genre: %d, name: %s, score: %.1f, year: %d\n", i, movies[i].budget, movies[i].genre, movies[i].name, movies[i].score, movies[i].year);
    }
}

/* prints given movie */
void print_movie(Movie movie){
    printf("%-10.f, %10d, %-40s, %-8.1f, %5d;\n", movie.budget, movie.genre, movie.name, movie.score, movie.year);
}


/* extract headers from first line of file */
void get_headers(FILE *src, char headers[][10]){
    int i = 0;
    char line[30], ch, *temp;

    do{
        fscanf(src, "%c", &ch);
        line[i] = ch;
        i++;
    }while(ch != '\n');
    
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

int main(){
    int number_of_lines, l = 0, success = 0, selection, start, end;
    long int i;
    FILE *src;
    Movie *movies, movie = {0};
    char **genres;
    char headers[5][10] = {0};

    src = fopen(SRCFILE, "r");
    number_of_lines = count_lines(src);
    /*printf("%d\n",number_of_lines);*/
    movies = (Movie *)calloc(number_of_lines, sizeof(Movie)); /* allocates movies as many as line number of file */

    get_headers(src, headers);
    for(i = 0; i < 5; i++) printf("- %s ", headers[i]);

    /* scan file and fill movie arrays */
    while(!feof(src)){
        /*printf("\n*-------------------------\n");
        print_movies(movies, l);
        printf("-------------------------*\n");*/
        get_movie(src, &movie, genres);
        success = insert_movie(movie, movies, &l);
    }
    /*printf("l = %d\n", l);
    print_movies(movies, 10); printf("\n\n");*/

    /* MENU */
    selection = show_menu();
    if(selection == 1){

    }
    else if(selection == 2){

    }
    else if(selection == 3){
        
    }
    else if(selection == 4){
        
    }
    else if(selection == 5){
        printf("\n1. Single Selection\n");
        printf("2. Multiple Selection\n\n");
        selection = get_selection(1, 2);

        if(selection == 1){
            printf("\nEnter value: ");
            scanf("%d", &start);
            printf("\n%d. Movie:\n\n", start);
            print_movie(movies[start-1]);
        }
        else if(selection == 2){
            printf("\nEnter start value: ");
            scanf("%d", &start);
            printf("\nEnter end value: ");
            scanf("%d", &end);
            printf("\nMovies between %d and %d.\n\n", start, end);
            for(i = start-1; i < end-1; i++){
                print_movie(movies[i]);
            }
        }
    }
    else if(selection == 6){
        
    }
    else if(selection == 7){
        
    }
    else if(selection == 8){
        printf("Exit.\n");
        return 0;
    }


    free(movies);
    return 0;
}

/* inserts new movie to array */
int insert_movie(Movie movie, Movie movies[], int *l){
    int i;
    for(i=0; i < *l; i++){
        if(strcmp(movies[i].name, movie.name) == 0){
            /*printf("Movie is already exist: %s == %s\n", movie.name, movies[i].name);*/
            
            /*printf(".");*/
            return 0;
        }
    }
    
    movies[*l] = movie;
    (*l)++;
    return 1;
}

/* gets selection for menu with error handling */
int get_selection(int lower, int upper){
    int status, selection = 0, flag = 0;

    while(!flag){
        printf("Please select an operation: ");
        status = scanf("%d", &selection);
        if(status < 1 || selection < lower || selection > upper){
            while(getchar() != '\n');
            printf("TRY AGAIN\n");
            continue;
        }
        flag = 1;
    }

    return selection;
}

/* printing main menu */
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

/* numerates genre, assigns unique integer for each genre */
int numerate_genre(char **genres, const char *genre){
    return 0;
}

/* extracts movie struct from line of file */
void get_movie(FILE *src, Movie *movie, char **genres){
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

    if(strlen(ap) > 5){
        apb = strtok(ap, ",");
        /*printf("movie = %s; ", apb);*/
        if(strcmp(apb, "unknown") == 0){
            movie->budget = -1;
        }else{
            movie->budget = strtod(apb, NULL);
        }

        apb = strtok(NULL, ",");
        /*printf("%s; ", apb);*/
        movie->genre = numerate_genre(genres, apb);

        apb = strtok(NULL, ",");
        /*printf("%s; ", apb);*/
        movie->name = (char *)malloc((strlen(apb) + 1)*sizeof(char));
        strcpy(movie->name, apb);
        /**(movie->name) = apb;*/

        apb = strtok(NULL, ",");
        /*printf("%s; ", apb);*/
        movie->score = strtod(apb, NULL);

        apb = strtok(NULL, ",");
        /*apb[strlen(apb) - 1] = '\0';*/
        /*printf("%s.\n", apb);*/
        movie->year = atoi(apb);
    }

    /*printf("ap = %s\n", ap);*/
    free(ap);

}