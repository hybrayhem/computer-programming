#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define filein "text.txt"
#define dictfile "dictionary.txt"
#define fileout "test.txt"

void fix_spelling_error();
void fix_spelling_error2();
void find_best_correction();
float strdiff(); 
int not_seperator(char c); 
void seperate_puncs(); 
void tolwr(); 
void match_case(); 
int not_to_vowel(char c1, char c2);

int main(){
    FILE *ptrin;

    char word[50];
    char word2[50];
    
	ptrin = fopen(fileout, "r+");
	if(ptrin == NULL){
		printf("Couldn't open file %s.\n'", filein);
		return 0;
	}
	
	fscanf(ptrin, "%s", word);
	while(!feof(ptrin)){
		fseek(ptrin, -strlen(word), SEEK_CUR);		
		fix_spelling_error2(&word[0]);
		fprintf(ptrin, "%s ", word);
		
		fscanf(ptrin, "%s", word);
	}

	/*strcpy(word, "musac");*/		
	strcpy(word, "Musac's");		
	fix_spelling_error(&word[0]);	

	strcpy(word, "ssarch");		
	fix_spelling_error(&word[0]);
	
	/*strcpy(word, "ssarch");
	strcpy(word2, "search");
	printf("\n%s and %s difference: %.2f\n", word, word2, strdiff(word, word2));

	strcpy(word, "ssarch");
	strcpy(word2, "starch");
	printf("\n%s and %s difference: %.2f\n", word, word2, strdiff(word, word2));*/

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

	printf("body [%s] > ", new_body);
	find_best_correction(&new_body[0], 2);
	printf("[%s]\n", new_body);

	match_case(new_body, body, length);
	printf("body case match: %s\n", new_body);

	sprintf(word, "%s%s%s", pre, new_body, post);
	printf("word assemble: [%s]\n\n", word);
}

void fix_spelling_error2(char *word){
	char pre[20] = {0};
	char body[20] = {0};
	char new_body[20] = {0}; /* corrected body */
	char post[20] = {0};

	int length = 0;

	seperate_puncs(&word[0], pre, body, post);
	length = strlen(body);
	printf("result: %s + %s + %s = [%s]\n", pre, body, post, word);

	if(length < 3) return;

	tolwr(new_body, body, length);
	printf("body to lowercase: %s\n", new_body);

	printf("body [%s] > ", new_body);
	find_best_correction(&new_body[0], 3);
	printf("[%s]\n", new_body);

	match_case(new_body, body, length);
	printf("body case match: %s\n", new_body);

	sprintf(word, "%s%s%s", pre, new_body, post);
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
	float diff, diffmin = tlr + 2;
	char dword[50]; 
	char correctw[50] = {0};

    FILE *dictin;
	dictin = fopen(dictfile, "r");

	rewind(dictin);
	while(!(feof(dictin))){
		fscanf(dictin, "%s", dword);
		diff = strdiff(word, dword);
		
		if(diff < tlr && diff != -1){	
			printf("\nMATCH = %s | %s, difference: %.2f\n", word, dword, diff);
			if(diff < diffmin){
				diffmin = diff;
				strcpy(correctw, dword);
			}
		}
	}

	if(correctw[0] != '\0') strcpy(word, correctw);
	
	fclose(dictin);
}

float strdiff(char *word1, char *word2){
	int i, l1, l2;
	float diff = 0.0;
	char c1, c2;

	l1 = strlen(word1);
	l2 = strlen(word2);

	if(l1 != l2) return -1;

	for (i = 0; i < l1; i++){
		c1 = word1[i];
		c2 = word2[i];

		if ((c1 != c2)){
			diff++;
			if (not_to_vowel(c1, c2)){
				diff = diff + 0.05; /* adds more cost to make not preferred */
				/*printf("%c to %c +0.05\n", c1, c2);*/
			}/*else 
				printf("%c to %c nothing\n", c1, c2);*/
		}
	}
	return diff;
}

int not_to_vowel(char c1, char c2)
{
	/*printf("%c to %c ", c1, c2);*/
	/*if ((c1 == 'a' || c1 == 'e' || c1 == 'i' || c1 == 'o' || c1 == 'u') &&
		(c2 != 'a' && c2 != 'e' && c2 != 'i' && c2 != 'o' && c2 != 'u'))
	{*/
	if ((c1 != 'a' && c1 != 'e' && c1 != 'i' && c1 != 'o' && c1 != 'u') /* c1 is consonant */
		&&
		(c2 == 'a' || c2 == 'e' || c2 == 'i' || c2 == 'o' || c2 == 'u')) /* c2 is vowel */
	{
		/*printf("1");*/
		return 0;
	}else{
		/*printf("0\n");*/
	}

	return 1;
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
	if((c <= 90 && c >= 65) || (c <= 122 && c >= 97) || c == '&' || c == '-') return 1; /* takes alphabetical letters and &- signs as part of word */
	return 0;
}