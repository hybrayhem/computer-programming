#include <stdio.h>
#include <string.h>

#define filein "text.txt"
#define dictfile "dictionary.txt"
#define fileout "ftext.txt"


void fix_spelling_error();
void find_best_correction(); 


int main(){
    FILE *ptrin;
    /*FILE *dictout;
    FILE *ptrout;*/

    char word[50];
    char text[20000] = ""; /* 0.02mb */
    char ch;
    
	ptrin = fopen(filein, "r");
	if(ptrin == NULL){
		printf("Couldn't open file: %s.\n'", filein);
		return 0;
	}
	int i = 0;
	while(!feof(ptrin)){
		fscanf(ptrin, "%c", &ch);
		if (ch == ' '){
			printf("word: %-20s - loc: [%d,%d]\n", word, i, i+strlen(word));
			word[0] = '\0';
		}
		else{
			sprintf(word, "%s%c", word, ch);
		}
		i++;
	}
	fclose(ptrin);
    printf("main\n");
    return 0;
}

/*

not defteri

kelime sonlarındaki nokta virgülleri at
tek karakter kelimeleri atla

kelimeyi al, sözlükle karşılaştır

*/