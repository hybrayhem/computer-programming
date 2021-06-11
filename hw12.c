/*
** word_practice.c:
**
** A game asks antonym/synonym questions to user and manages user's progress.
**
** Author: Halil Ilhan.
**
** Revision: 2021.06.05
** 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SYNFILE "synonyms_test.txt"
#define ANTFILE "antonyms_test.txt"

typedef struct word {
    char *word;
    char **pairs; /* a char array for antonyms/synonyms of the word */
    double chance;
    struct word *next;
} word;

void print_word(word *w) {
    int i;
    printf("Word: %-12s, Chance : %.1f  -   ", w->word, w->chance);
    printf("Pairs: ");
    for (i = 0; w->pairs[i] != NULL; i++) {
        printf("%s ", w->pairs[i]);
    }
    printf("\n");
}

void print_words(word *head_w) {
    if (head_w == NULL) {
        printf("\n");
    } else {
        print_word(head_w);
        print_words(head_w->next);
    }
}

/* scans a line from src and stores it in a dynamic array */
char *dscan_line(FILE *src) {
    int i, counter = 0;
    char input, *ap, *apb = NULL;
    do {
        fscanf(src, "%c", &input);
        counter++;

        ap = (char *)calloc(counter + 1, sizeof(char));
        for (i = 0; i < counter - 1; i++) {
            ap[i] = apb[i];
        }
        ap[counter - 1] = input;

        if (apb != NULL)
            free(apb);
        apb = ap;

    } while (input != '\n' && !feof(src));
    if (input == '\n') ap[strlen(ap) - 1] = '\0';
    return ap;
}

void parsed_insert_word(char *raw_data, word **head_w) {
    int i, pair_num = 5;
    word *new_word = (word *)malloc(sizeof(word)), *temp_w = *head_w;

    new_word->pairs = (char **)malloc((pair_num * sizeof(char *)));

    raw_data = strtok(raw_data, " "); /* get word */
    new_word->word = (char *)malloc((strlen(raw_data) + 1) * sizeof(char));
    strcpy(new_word->word, raw_data);

    /*printf("\nword: %s, pairs: ", raw_data);*/
    strtok(NULL, " "); /* remove = or <> sign */

    raw_data = strtok(NULL, ",");
    for (i = 1; raw_data != NULL; i++) {

        if (i > pair_num) {
            pair_num *= 2;
            new_word->pairs = (char **)realloc(new_word->pairs, pair_num * sizeof(char *));
        }

        new_word->pairs[i - 1] = (char *)malloc((strlen(raw_data) + 1) * sizeof(char));
        strcpy(new_word->pairs[i - 1], raw_data);
        /*printf("%s,", new_word->pairs[i - 1]);*/

        raw_data = strtok(NULL, ",");
    }

    if (i > pair_num) {
        new_word->pairs = realloc(new_word->pairs, i * sizeof(char *));
    }
    new_word->pairs[i - 1] = NULL;

    new_word->next = NULL;
    new_word->chance = 1.0;
    /*print_word(new_word);*/

    /* insert to end */
    if (*head_w == NULL) {
        *head_w = new_word;
    } else {

        while (temp_w->next != NULL)
            temp_w = temp_w->next;
        temp_w->next = new_word;
    }
}

void load_words(word **head_w, char *filename) {
    int i = 0;
    char *line, *element;
    word *current_word = *head_w;

    FILE *word_file;
    word_file = fopen(filename, "r");
    if (word_file == NULL) printf("Couldn't open file.\n");

    while (!feof(word_file)) {

        line = dscan_line(word_file);
        printf("Line : %s\n", line);
        parsed_insert_word(line, head_w);

        free(line);

        /*current_word = current_word->next;*/
    }
    printf("\n\n");
}

int main() {
    word *synonym_list = NULL;
    word *antonym_list = NULL;

    load_words(&synonym_list, SYNFILE);
    /*load_words(antonym_list, ANTFILE);*/

    printf("Print synonym_list\n");
    print_words(synonym_list);
    printf("hw12\n");
    return 0;
}
