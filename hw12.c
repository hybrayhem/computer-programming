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
    char **pairs;  /* a char array for antonyms/synonyms of the word */
    double chance; /* chance of being asked the word */
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
        if (apb != NULL) free(apb);

        if (input == '\n') {
            ap[counter - 1] = '\0';
            break;
        }

        ap[counter - 1] = input;
        apb = ap;

    } while (!feof(src));

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
    new_word->chance = (rand() % 100 + 1) / 50.0;
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

void load_words(word **head_w, char *filename, int *word_count) {
    char *line;

    FILE *word_file;
    word_file = fopen(filename, "r");
    if (word_file == NULL) printf("Couldn't open file.\n");

    while (!feof(word_file)) {

        line = dscan_line(word_file);
        /*printf("Line : %s\n", line);*/
        parsed_insert_word(line, head_w);
        free(line);

        (*word_count)++;
        /*current_word = current_word->next;*/
    }
    /*printf("\n\n");*/
    fclose(word_file);
}

void update_chance_array(word *head_syn, word *head_ant, double chance_array[]) {
    int i;

    for (i = 0; head_syn != NULL; i++) {
        chance_array[i] = head_syn->chance;
        head_syn = head_syn->next;
    }
    for (; head_ant != NULL; i++) {
        chance_array[i] = head_ant->chance;
        head_ant = head_ant->next;
    }
    chance_array[i] = -1.0;
}

void print_chances(double chance_array[]) {
    int i;
    for (i = 0; chance_array[i] != -1.0; i++) {
        printf("%.5f ", chance_array[i]);
    }
    printf("\n\n");
}

void print_weightes(int weight_array[]) {
    int i;
    for (i = 0; weight_array[i] != -1; i++) {
        printf("%d ", weight_array[i]);
    }
    printf("\n\n");
}

int get_random_word(double chance_array[], int len) {
    int i, sum = 0, *chance_weights = (int *)calloc(len, sizeof(int)), *indexes = (int *)calloc(len, sizeof(int));
    double min_chance = chance_array[0], expand_factor;

    for (i = 1; chance_array[i] != -1.0; i++) {
        if (chance_array[i] < min_chance) min_chance = chance_array[i];
    }
    expand_factor = 1.0 / min_chance;
    printf("min chance: %.5f, expand factor: %.5f\n", min_chance, expand_factor);

    for (i = 0; chance_array[i] != -1.0; i++) {
        chance_weights[i] = chance_array[i] * expand_factor + 0.1;
        sum += chance_weights[i];
        indexes[i] = sum;
    }
    chance_weights[i] = -1;
    indexes[i] = -1;
    print_weightes(chance_weights);
    print_weightes(indexes);
}

int check_answer(char *answer, char **pairs){

}

void ask_question(word *syn_list, word *ant_list, int syn_count, int ant_count, int word_index) {
    int i;
    word *search_word;

    if(word_index > syn_count) search_word = ant_list;
    else search_word = syn_list;

    for(i = 0; search_word != NULL && i < syn_count; i++){

    }
}

int main() {
    int syn_count = 0, ant_count = 0, word_len, word_index;
    double *chance_array;
    word *synonym_list = NULL;
    word *antonym_list = NULL;

    srand(time(NULL));

    load_words(&synonym_list, SYNFILE, &syn_count);
    load_words(&antonym_list, ANTFILE, &ant_count);
    word_len = syn_count + ant_count + 1;

    printf("Print synonym_list %d\n", syn_count);
    print_words(synonym_list);
    printf("Print antonym_list %d\n", ant_count);
    print_words(antonym_list);

    chance_array = (double *)calloc(word_len, sizeof(double));
    update_chance_array(synonym_list, antonym_list, chance_array);
    print_chances(chance_array);

    word_index = get_random_word(chance_array, word_len);
    /*ask_question(synonym_list, antonym_list, word_index);*/

    return 0;
}
