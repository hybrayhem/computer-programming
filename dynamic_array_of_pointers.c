#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numerate_genre(char **genres, char *genre){
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

        genres = (char **)calloc(i+2, sizeof(char));
        
        for(j = 0; j < i; j++){
            genres[j] = temp[j];
        }
        genres[i] = genre;
        genres[i+1] = NULL;
        
        if(temp != NULL) free(temp);

        temp = genres;

    }
	
	/*if(!flag){
		printf("save genre to genres[%d], genres[%d] = NULL\n\n", i, i+1);
		temp = genres;
		
		genres = (char **)calloc(i+2, sizeof(char));
		
		for(i=0; temp[i] != NULL; i++){
		printf("temp %d. = %s\n", i, temp[i]);
		genres[i] = temp[i];
	}
		
		genres[i] = genre;
		genres[i+1] = NULL;
	}*/

    for(i=0; genres[i] != NULL; i++){
		printf("*genres %d. = %s\n", i, genres[i]);
		if(strcmp(genres[i], genre) == 0){
			flag = 1;			
		}
	}
	printf("\n\n");
    
    return 0;
}

int main()
{
	char **genres;
    genres = (char **)calloc(1, sizeof(char));
    genres[0] = NULL;
	/*genres[0] = "action";
	genres[1] = "dram";
	genres[2] = NULL;*/
	numerate_genre(genres, "dram");
	numerate_genre(genres, "comedy");
	numerate_genre(genres, "action");
	free(genres);
	return 0;
}