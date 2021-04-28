#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define filein "text.txt"
#define dictfile "dictionary.txt"
#define fileout "test.txt"


void fix_spelling_error();
void find_best_correction();
int strdiff(); 
int is_alphabetic(char c); 
void seperate_puncs(); 


int main(){
    FILE *ptrin;

    char word[50];
    
	ptrin = fopen(fileout, "r+");
	if(ptrin == NULL){
		printf("Couldn't open file %s.\n'", filein);
		return 0;
	}
	
	fscanf(ptrin, "%s", word);
	while(!feof(ptrin)){
		fseek(ptrin, -strlen(word), SEEK_CUR);		
		fix_spelling_error(&word[0]);
		fprintf(ptrin, "%s ", word);
		
		fscanf(ptrin, "%s", word);
	}

	fclose(ptrin);
    return 0;
}


void fix_spelling_error(char *word){
	char pre[20] = {0};
	char body[20] = {0};
	char post[20] = {0};

	if(strlen(word) < 2) return;

	seperate_puncs(&word[0], pre, body, post);
	printf("result: %s + %s + %s = [%s]\n", pre, body, post, word);

	printf("body [%s] > ", body);
	find_best_correction(&body[0]);
	printf("[%s]\n", body);

	sprintf(word, "%s%s%s", pre, body, post);
	printf("word assemble: [%s]\n\n", word);
}

void tolwr(){}
void match_case(){}

void find_best_correction(char *word){
	int diff, diffmin = 8;
	char dword[50]; 
	char correctw[50];

    FILE *dictin;
	dictin = fopen(dictfile, "r");

	rewind(dictin);
	while(!(feof(dictin))){
		fscanf(dictin, "%s", dword);
		diff = strdiff(word, dword);
		if(diff < 2){		
			/*printf("\nMATCH = %s | %s, difference: %d\n", word, dword, d);*/
			if(diff < diffmin){
				diffmin = diff;
				strcpy(correctw, dword);
			}
		}
	}

	strcpy(word, correctw);
	
	fclose(dictin);
}

int strdiff(char *word1, char *word2){
	int i, l, l1, l2, diff;
	l1 = strlen(word1);
	l2 = strlen(word2);
	char c1, c2;

	if (l1 < l2) l = l1;
	else l = l2;

	diff = abs(l1 - l2);

	for (i = 0; i < l; i++){
		c1 = word1[i];
		c2 = word2[i];

		if ((c1 != c2)) diff++;
	}
	return diff;
}

void seperate_puncs(char *word, char *pre, char *body, char *post){
	int i = 0, j = 0;
	while(is_alphabetic(word[i]) == 0){
		/*printf("pre + %c\n", word[i]);*/
		pre[i] = word[i];
		i++;
	}
	while(is_alphabetic(word[i]) == 1){
		/*printf("body + %c\n", word[i]);*/
		body[j] = word[i];
		i++;
		j++;
	}

	j = 0;
	while(i < strlen(word)){
		/*printf("post + %c\n", word[i]);*/
		post[j] = word[i];
		i++;
		j++;
	}
}

int is_alphabetic(char c){
	/*if(c > 127) return -1;*/
	if((c <= 90 && c >= 65) || (c <= 122 && c >= 97)) return 1;
	return 0;
}