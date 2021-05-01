#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define filein "test.txt"
#define dictfile "dictionary.txt"


/* spell checkers */
void fix_spelling_error();
void fix_spelling_error2();

/********* helper functions *********/
void find_best_correction();
float strdiff();
int not_seperator(char c);
void seperate_puncs();
void tolwr();
void match_case();
int not_to_vowel(char c1, char c2);



/********* main *********/
int main()
{
	FILE *ptrin;
	char word[50];

	/* open file */
	ptrin = fopen(filein, "r+");
	if (ptrin == NULL)
	{
		printf("Couldn't open file %s.\n'", filein);
		return 0;
	}

	printf("\n		< Spell Corrections >\n\n");

	/* scan every word */
	fscanf(ptrin, "%s", word);
	while (!feof(ptrin))
	{
		fseek(ptrin, -strlen(word), SEEK_CUR);	/* seek backwards to overwrite word */
		fix_spelling_error2(&word[0]);	/* fix misspellings and print to file */
		fprintf(ptrin, "%s ", word);

		fscanf(ptrin, "%s", word);
	}

	printf("\n");
	fclose(ptrin);
	return 0;
}



/* fixes misspellings, handles punctuations and case */
void fix_spelling_error(char *word)
{
	/* parts of word */
	char pre[20] = {0};
	char body[30] = {0};
	char post[20] = {0};
	
	char new_body[30] = {0}; /* corrected body */
	int length = 0;

	seperate_puncs(&word[0], pre, body, post); /* extract word from puncs etc. */
	length = strlen(body);

	if (length < 2) /* ignore too short words */
		return;

	tolwr(new_body, body, length); /* make comparison case independent, copy body to new_body */

	find_best_correction(&new_body[0], 2); /* apply corrections that max one character difference */

	match_case(new_body, body, length); /* match new_body letter cases with original body */

	sprintf(word, "%s%s%s", pre, new_body, post); /* assemble word */
}

/* fixes misspellings, handles punctuations and case up to two character differences */
void fix_spelling_error2(char *word)
{
	char pre[20] = {0};
	char body[30] = {0};
	char post[20] = {0};

	char new_body[30] = {0};
	int length = 0;

	seperate_puncs(&word[0], pre, body, post);
	length = strlen(body);

	if (length < 3)
		return;

	tolwr(new_body, body, length);

	find_best_correction(&new_body[0], 3); /* apply corrections that max two character difference */

	match_case(new_body, body, length);

	sprintf(word, "%s%s%s", pre, new_body, post);
}




/* gets src and writes to dest as lowercase */
void tolwr(char *dest, char *src, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		if (src[i] <= 90 && src[i] >= 65) /* make capitals lowercase */
			dest[i] = src[i] + 32;
		else
			dest[i] = src[i];
	}
}

/* gets two equal string, and makes dest's letter cases same with src */
void match_case(char *dest, char *src, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		if (src[i] <= 90 && src[i] >= 65)
			dest[i] -= 32;
	}
}

/* compare word with dictionary and return the most similar in limits of tolerance */
void find_best_correction(char *word, int tlr)
{
	float diff, diffmin = tlr + 2; /* set diffmin as max */
	char dict_word[50];
	char correct_word[50] = {0};

	FILE *dictin;
	dictin = fopen(dictfile, "r"); /* open dictionary */

	rewind(dictin); /* go to beginning of file */
	while (!(feof(dictin)))
	{
		fscanf(dictin, "%s", dict_word);
		diff = strdiff(word, dict_word);

		if (diff < tlr && diff != -1) /* diff validation */
		{
			if (diff < diffmin) /* update best match */
			{
				diffmin = diff;
				strcpy(correct_word, dict_word);
			}
		}
	}

	if (correct_word[0] != '\0'){
		if(strcmp(word, correct_word) != 0) printf("	[ %-15s to %15s ]\n", word, correct_word);
		strcpy(word, correct_word); /* return result */
	}
	fclose(dictin);
}

/* calculates word difference with conosonant to vowel priority */
float strdiff(char *word1, char *word2)
{
	int i, l1, l2;
	float diff = 0.0;
	char c1, c2;

	l1 = strlen(word1);
	l2 = strlen(word2);

	if (l1 != l2)
		return -1;

	for (i = 0; i < l1; i++)
	{
		c1 = word1[i];
		c2 = word2[i];

		if ((c1 != c2))
		{
			diff++;
			if (not_to_vowel(c1, c2))
			{
				diff = diff + 0.05; /* adds more cost to make not preferred */
			}
		}
	}
	return diff;
}

/* if c1 to c2 is not consonant to vowel returns true */
int not_to_vowel(char c1, char c2)
{
	if ((c1 != 'a' && c1 != 'e' && c1 != 'i' && c1 != 'o' && c1 != 'u') /* c1 is consonant */
		&&
		(c2 == 'a' || c2 == 'e' || c2 == 'i' || c2 == 'o' || c2 == 'u')) /* c2 is vowel */
	{
		return 0;
	}

	return 1;
}

/* splits string to three, pre is signs before word, body is word and post is signs after word */
void seperate_puncs(char *str, char *pre, char *body, char *post)
{
	int i = 0, j = 0, len = strlen(str);
	while (not_seperator(str[i]) == 0 && i < len) /* read until start of word */
	{
		pre[i] = str[i];
		i++;
	}
	while (not_seperator(str[i]) == 1 && i < len) /* read until end of word */
	{
		body[j] = str[i];
		i++;
		j++;
	}

	j = 0;
	while (i < len) /* take other stuff to post */
	{
		post[j] = str[i];
		i++;
		j++;
	}
}

/* is character part of word or not */
int not_seperator(char c)
{
	if ((c <= 90 && c >= 65) || (c <= 122 && c >= 97) || c == '&' || c == '-')
		return 1; /* takes alphabetical letters and &- signs as a part of the word */
	return 0;
}
