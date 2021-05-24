#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numerate_genre(char *genres[], char genre[]){
	int i, j, flag =0;
	char **temp = NULL;
	printf("genre = %s\n", genre);
	
	for(i=0; genres[i] != NULL; i++){
		printf("genres %d. = %s\n", i, genres[i]);
		if(strcmp(genres[i], genre) == 0){
			flag = 1;			
		}
	}
	
	if(flag){
        printf("Found.\n");
    }
	else{
        printf("Not found.\n");

		temp = (char **)calloc(i, sizeof(char));
		for(j = 0; j < i; j++){
			temp[j] = (char *)malloc((strlen(genres[j])+1)*sizeof(char));
			strcpy(temp[j], genres[j]);
        }
    	for(i=0; temp[i] != NULL; i++){
			printf("*temp %d. = %s\n", i, temp[i]);
		}


		genres = (char **)calloc(i+2, sizeof(char));
        for(j = 0; j < i; j++){
			genres[j] = (char *)malloc((strlen(temp[j])+1)*sizeof(char));
			strcpy(genres[j], temp[j]);
        }
		genres[i] = (char *)malloc((strlen(genre)+1)*sizeof(char));
		strcpy(genres[i], genre);
        genres[i+1] = NULL;

		free(temp);
	}
	
    for(i=0; genres[i] != NULL; i++){
		printf("*genres %d. = %s\n", i, genres[i]);
	}
	printf("\n\n");
    
    return 0;
}

int main()
{
	int i;
	char **genres;
    /*genres = (char **)calloc(1, sizeof(char));
    genres[0] = NULL;*/
	genres = (char **)calloc(3, sizeof(char));
	genres[0] = "action";
	genres[1] = "dram";
	genres[2] = NULL;
	numerate_genre(genres, "dram");
	numerate_genre(genres, "comedy");
	for(i=0; genres[i] != NULL; i++){
		printf("#genres %d. = %s\n", i, genres[i]);
	}
	printf("\n");
	numerate_genre(genres, "action");
	/*numerate_genre(genres, "dram");
	numerate_genre(genres, "foreign");
	numerate_genre(genres, "dram");*/
	free(genres);
	return 0;
}