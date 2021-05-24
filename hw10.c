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

int count_lines(FILE *src){
    char c;
    int nol = 0;
    for (c = getc(src); c != EOF; c = getc(src)){
        if (c == '\n') nol++;
    }
    rewind(src);

    return nol+1;
}

void print_movies(Movie *movies, int l){
    int i;
    for(i=0; i < l; i++){
        printf("%d. movie = budget: %.1f, genre: %d, name: %s, score: %.1f, year: %d\n", i, movies[i].budget, movies[i].genre, movies[i].name, movies[i].score, movies[i].year);
    }
}

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
    int number_of_lines, l = 0, success = 0, selection;
    long int i;
    FILE *src;
    Movie *movies, movie = {0};
    char **genres;
    char headers[5][10] = {0};

    
    src = fopen(SRCFILE, "r");
    number_of_lines = count_lines(src);
    printf("%d\n",number_of_lines);
    movies = (Movie *)calloc(number_of_lines, sizeof(Movie));

    get_headers(src, headers);
    for(i = 0; i < 5; i++) printf("- %s ", headers[i]);

    while(!feof(src)){
        /*printf("\n*-------------------------\n");
        print_movies(movies, l);
        printf("-------------------------*\n");*/
        get_movie(src, &movie, genres);
        success = insert_movie(movie, movies, &l);
        if(success == 1){
            /*printf("beforins = %.1f, %d, %s, %.1f, %d;", movie.budget, movie.genre, movie.name, movie.score, movie.year);*/
            /*printf("\n%d. Movie = %.1f, %d, %s, %.1f, %d;\n", l, movies[l-1].budget, movies[l-1].genre, movies[l-1].name, movies[l-1].score, movies[l-1].year);*/
        }
    }
    printf("l = %d\n", l);
    print_movies(movies, 10);


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
        printf("1. Single Selection\n");
        printf("2. Multiple Selection\n");
        scanf("%d", &selection);
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

int numerate_genre(char **genres, const char *genre){
    return 0;
}

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