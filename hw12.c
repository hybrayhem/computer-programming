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

typedef enum { synonym,
               antonym } word_t;

typedef struct user {
    char *username;
    double *chance_array;
} user;

typedef struct word {
    word_t type;
    char *word;
    char **pairs;  /* a char array for antonyms/synonyms of the word */
    double chance; /* chance of being asked the word */
    struct word *next;
} word;

void print_word(word *w) {
    int i;
    printf("Word: %-12s, Chance : %.2f, Type: %d  -   ", w->word, w->chance, w->type);
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

/* dynamically scans a line from src and stores in an array */
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

void parsed_insert_word(word **head_w, char *raw_data, word_t type) {
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
    new_word->type = type;
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

void load_words(word **head_w, char *filename, int *word_count, word_t type) {
    char *line;

    FILE *word_file;
    word_file = fopen(filename, "r");
    if (word_file == NULL) printf("Couldn't open file.\n");

    while (!feof(word_file)) {

        line = dscan_line(word_file);
        /*printf("Line : %s\n", line);*/
        parsed_insert_word(head_w, line, type);
        free(line);

        (*word_count)++;
    }
    /*printf("\n\n");*/
    fclose(word_file);
}

void update_chance_array(word *head_w, double chance_array[]) {
    int i;

    for (i = 0; head_w != NULL; i++) {
        chance_array[i] = head_w->chance;
        head_w = head_w->next;
    }
    chance_array[i] = -1.0;
}

void print_chances(double chance_array[]) {
    int i;
    printf("Chance: ");
    for (i = 0; chance_array[i] != -1.0; i++) {
        printf("%3.3g ", chance_array[i]);
    }
    printf("\n");
}

void print_weightes(int weight_array[]) {
    int i;
    printf("Weight: ");
    for (i = 0; weight_array[i] != -1; i++) {
        printf("%3d ", weight_array[i]);
    }
    printf("\n");
}

int get_random_word(double chance_array[], int word_count) {
    int i, sum = 0, random_index, *indexes = (int *)calloc(word_count + 1, sizeof(int));
    double min_chance, expand_factor;

    min_chance = chance_array[0];
    for (i = 1; chance_array[i] != -1.0; i++) {
        if (chance_array[i] < min_chance) min_chance = chance_array[i];
    }
    expand_factor = 1.0 / min_chance;
    /*printf("min chance: %.5f, expand factor: %.5f\n", min_chance, expand_factor);*/

    for (i = 0; chance_array[i] != -1.0; i++) {
        sum += chance_array[i] * expand_factor + 0.1;
        indexes[i] = sum;
    }
    indexes[i] = -1;
    print_weightes(indexes);
    random_index = rand() % indexes[i - 1] + 1;
    for (i = 0; indexes[i] != -1 && indexes[i] < random_index; i++)
        ;
    /*printf("%d. selected, random index was %d\n", i, random_index);*/
    return i;
}

int check_answer(char *answer, char **pairs) {
    int i;
    for (i = 0; pairs[i] != NULL; i++) {
        if (strcmp(answer, pairs[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

word *get_word_by_index(word *word_list, int index) {
    int i;
    /*printf("%d ", index);*/
    for (i = 0; i < index; i++) {
        word_list = word_list->next;
    }
    return word_list;
}

double increase_chance(double chance) {
    if (chance >= 1.0) {
        return chance + 1;
    }
    return 1.0 / ((1.0 / chance) - 1.0);
}

double decrease_chance(double chance) {
    if (chance > 1.0) {
        return chance - 1;
    }
    return 1.0 / ((1.0 / chance) + 1.0);
}

int ask_question(word *word_list, int word_index) {
    int right_answer;
    char *answer;
    word *question = get_word_by_index(word_list, word_index);

    if (question->type == synonym)
        printf("What is the synonym of %s?\n", question->word);
    else if (question->type == antonym)
        printf("What is the antonym of %s?\n", question->word);

    printf("> ");
    answer = dscan_line(stdin);

    if (strcmp(answer, "q") == 0) return 1;

    right_answer = check_answer(answer, question->pairs);
    if (right_answer) {
        printf("Right. \n");
        (question->chance) = decrease_chance(question->chance);
    } else {
        printf("Wrong. \n");
        (question->chance) = increase_chance(question->chance);
    }

    /*printf("new chance %.2f    ", question->chance);
    printf("%s\n", answer);*/

    return 0;
}

int main() {
    int word_count = 0, word_index, quit = 0;
    user user;
    word *words = NULL;

    srand(time(NULL));

    load_words(&words, SYNFILE, &word_count, synonym);
    load_words(&words, ANTFILE, &word_count, antonym);

    printf("Print word list %d\n", word_count);
    print_words(words);

    user.chance_array = (double *)calloc(word_count + 1, sizeof(double));

    while (!quit) {
        update_chance_array(words, user.chance_array);
        print_chances(user.chance_array);

        word_index = get_random_word(user.chance_array, word_count);
        quit = ask_question(words, word_index);
        printf("\n");
    }

    return 0;
}
