#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define filein "text.txt"
#define dictfile "dictionary.txt"
#define fileout "test.txt"

void fix_spelling_error();
void find_best_correction();
int strdiff(); 
int not_seperator(char c); 
void seperate_puncs(); 
void tolwr(); 
void match_case(); 


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

	/*strcpy(word, "musac");*/		
	strcpy(word, "Musac's");		
	fix_spelling_error(&word[0]);
	
	fclose(ptrin);
    return 0;
}


void fix_spelling_error(char *word){
	char pre[20] = {0};
	char body[20] = {0};
	char new_body[20] = {0}; /* corrected body */
	char post[20] = {0};

	int length = 0;

	seperate_puncs(&word[0], pre, body, post);
	length = strlen(body);
	printf("result: %s + %s + %s = [%s]\n", pre, body, post, word);

	if(length < 2) return;

	tolwr(new_body, body, length);
	printf("body to lowercase: %s\n", new_body);

	printf("body [%s] > ", body);
	find_best_correction(&body[0], 2);
	printf("[%s]\n", body);

	match_case(new_body, body, length);
	printf("body case match: %s\n", new_body);

	sprintf(word, "%s%s%s", pre, body, post);
	printf("word assemble: [%s]\n\n", word);
}

void tolwr(char *dest, char *src, int len){
	int i;
	for(i = 0; i < len; i++){
		if(src[i]<= 90 && src[i] >= 65) dest[i] = src[i] + 32; 
		else dest[i] = src[i];
	}
}

void match_case(char *dest, char *src, int len){
	int i;
	for(i = 0; i < len; i++){
		if(src[i]<= 90 && src[i] >= 65) dest[i] -= 32;
	}
}

void find_best_correction(char *word, int tlr){
	int diff, diffmin = tlr + 1;
	char dword[50]; 
	char correctw[50] = {0};

    FILE *dictin;
	dictin = fopen(dictfile, "r");

	rewind(dictin);
	while(!(feof(dictin))){
		fscanf(dictin, "%s", dword);
		diff = strdiff(word, dword);
		if(diff < 0) continue;

		if(diff < tlr){	
			/*printf("\nMATCH = %s | %s, difference: %d\n", word, dword, diff);*/
			if(diff < diffmin){
				diffmin = diff;
				strcpy(correctw, dword);
			}
		}
	}

	if(correctw[0] != '\0') strcpy(word, correctw);
	
	fclose(dictin);
}

int strdiff(char *word1, char *word2){
	int i, l, l1, l2, diff;
	l1 = strlen(word1);
	l2 = strlen(word2);
	char c1, c2;

	if(l1 != l2) return -1;
	/*if (l1 <= l2) l = l1;
	else l = l2;*/

	/*diff = abs(l1 - l2);*/
	diff = 0;

	for (i = 0; i < l1; i++){
		c1 = word1[i];
		c2 = word2[i];

		if ((c1 != c2)) diff++;
	}
	return diff;
}

void seperate_puncs(char *word, char *pre, char *body, char *post){
	int i = 0, j = 0, len = strlen(word);
	while(not_seperator(word[i]) == 0 && i < len){
		/*printf("pre + %c\n", word[i]);*/
		pre[i] = word[i];
		i++;
	}
	while(not_seperator(word[i]) == 1 && i < len){
		/*printf("body + %c\n", word[i]);*/
		body[j] = word[i];
		i++;
		j++;
	}

	j = 0;
	while(i < len){
		/*printf("post + %c\n", word[i]);*/
		post[j] = word[i];
		i++;
		j++;
	}
}

int not_seperator(char c){
	/*if(c > 127) return -1;*/
	if((c <= 90 && c >= 65) || (c <= 122 && c >= 97) || c == '&') return 1;
	return 0;
}