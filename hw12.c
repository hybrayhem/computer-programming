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
#define USRFILE "usernames.txt"

typedef enum { synonym,
               antonym } word_t;

typedef struct user {
    char *username, *filename;
    int rights, wrongs;   /* number of right and wrong answers */
    double *chance_array; /* user specific ask probability of words */
} user;

typedef struct word {
    word_t type;
    char *word;
    char **pairs;  /* a char array for antonyms/synonyms of the word */
    double chance; /* chance of being asked the word */
    struct word *next;
} word;

/* secure scanf function that gets the selection for menu with a bullet-proof error handling */
double get_selection(const char msg[], int lower, int upper) {
    int status, flag = 0;
    double selection = 0.0;

    while (!flag) {
        if (msg[0] != '\0') printf("%s", msg);
        status = scanf("%lf", &selection);
        if (status < 1 || selection < lower || selection > upper) {
            while (getchar() != '\n')
                ;
            printf("Invalid.\n");
            continue;
        }
        flag = 1;
    }
    return selection;
}

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
    int counter = 0, size = 3;
    char input, *str = (char *)malloc((size + 1) * sizeof(char));
    do {
        fscanf(src, "%c", &input);
        counter++;

        if (counter >= size) {
            size *= 2;
            str = (char *)realloc(str, (size + 1) * sizeof(char));
        }

        if (input == '\n') {
            str[counter - 1] = '\0';
            break;
        }

        str[counter - 1] = input;
        input = 0;

    } while (!feof(src));

    return str;
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

    new_word->type = type;
    new_word->chance = 1.0;
    new_word->next = NULL;
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

    FILE *word_file = fopen(filename, "r");
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

void store_words(word *head_w, char *filename, word_t type) {
    int i;
    char seperator[2];
    FILE *word_file = fopen(filename, "w");

    if (type == synonym) {
        seperator[0] = '=';
    } else if (type == antonym) {
        seperator[0] = '<';
        seperator[1] = '>';
    }

    while (head_w != NULL) {
        if (head_w->type == type) {
            /*print_word(head_w);*/

            fprintf(word_file, "%s %s ", head_w->word, seperator);
            for (i = 0; head_w->pairs[i + 1] != NULL; i++) {
                fprintf(word_file, "%s,", head_w->pairs[i]);
            }
            fprintf(word_file, "%s", head_w->pairs[i]);
            if (!(head_w->next == NULL || head_w->type != head_w->next->type)) fprintf(word_file, "\n");
        }

        head_w = head_w->next;
    }
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

char *word_type_to_string(word_t type) {
    if (type == synonym) return "synonym";
    return "antonym";
}

void add_pair(char *answer, char **pairs) {
    int i = 0;

    while (pairs[i] != NULL)
        i++;
    pairs = (char **)realloc(pairs, (i + 2) * sizeof(char *));

    pairs[i] = (char *)malloc((strlen(answer) + 1) * sizeof(char));
    strcpy(pairs[i], answer);
    pairs[i + 1] = NULL;
}

int ask_question(user *user, word *word_list, int word_index) {
    int right_answer;
    char *answer, *temp;
    word *question = get_word_by_index(word_list, word_index);

    if (question->type == synonym)
        printf("What is the synonym of %s?\n", question->word);
    else if (question->type == antonym)
        printf("What is the antonym of %s?\n", question->word);
    printf("> ");

    do {
        answer = dscan_line(stdin);
    } while (answer[0] == 0);

    if (strcmp(answer, "q") == 0) return 1;

    right_answer = check_answer(answer, question->pairs);
    if (right_answer) {
        printf("Right. \n");
        (question->chance) = decrease_chance(question->chance);
        (user->rights)++;
    } else {
        printf("Wrong. \n");

        printf("Do you want to add '%s' to the %ss of %s? (y / n) ", answer, word_type_to_string(question->type), question->word);
        do {
            temp = dscan_line(stdin);
        } while (temp[0] == 0 || (temp[0] != 'y' && temp[0] != 'n'));
        if (temp[0] == 'y') {
            add_pair(answer, question->pairs);
            return 0;
        }

        (question->chance) = increase_chance(question->chance);
        (user->wrongs)++;
    }
    user->chance_array[word_index] = question->chance;

    /*printf("new chance %.2f    ", question->chance);
    printf("%s\n", answer);*/

    return 0;
}

void skip_line(FILE *src) {
    while (fgetc(src) != '\n' && !feof(src))
        ;
}

void get_username(char **username) {
    int i, selection;
    FILE *usrfile = fopen(USRFILE, "r");

    for (i = 1; !feof(usrfile); i++) {
        *username = dscan_line(usrfile);
        if ((*username)[0] != 0)
            printf("%d. %s\n", i, *username);
        else
            i--;
        free(*username);
    }
    printf("%d. Create new user\n", i);

    selection = get_selection("Enter the selection: ", 1, i);
    if (selection == i) {
        printf("Please enter username: ");
        do {
            *username = dscan_line(stdin);
        } while ((*username)[0] == 0);
    } else {
        rewind(usrfile);

        for (i = selection; i > 1; i--)
            skip_line(usrfile);
        /*printf("%d\n", i);*/

        *username = dscan_line(usrfile);
        /*printf("User: %s\n\n", *username);*/
    }

    fclose(usrfile);
}

void create_user(user *user, int word_count) {
    int i;
    FILE *usrfile = fopen(USRFILE, "a");

    fprintf(usrfile, "%s\n", user->username);

    printf("Create user %s\n", user->username);
    for (i = 0; i < word_count; i++) {
        user->chance_array[i] = 1.0;
    }
    user->chance_array[word_count] = -1;
    user->rights = 0;
    user->wrongs = 0;

    fclose(usrfile);
}

void read_user(user *user, int word_count) {
    int i = 0;
    double temp;
    FILE *usrfile;

    user->chance_array = (double *)calloc(word_count + 1, sizeof(double));

    user->filename = (char *)malloc((strlen(user->username) + 9) * sizeof(char));
    sprintf(user->filename, "%s.worddat", user->username);
    usrfile = fopen(user->filename, "rb");
    if (usrfile == NULL) {
        create_user(user, word_count);
        return;
    }

    /* fill user with reading user data file */
    fread(user->username, sizeof(char), strlen(user->username) + 1, usrfile);
    printf("Username: %s\n", user->username);

    fread(&(user->rights), sizeof(int), 1, usrfile);
    fread(&(user->wrongs), sizeof(int), 1, usrfile);
    /*fread(user->chance_array, sizeof(double), word_count + 1, usrfile);*/
    i = 0;
    do {
        fread(&(user->chance_array[i]), sizeof(double), 1, usrfile);
        /*fread(&temp, sizeof(double), 1, usrfile);
        printf("%f ", temp);*/
        i++;
    } while (i < word_count);
    user->chance_array[word_count] = -1;

    fclose(usrfile);

    printf("fread(%s) username: %s, r/w: %d/%d, ", user->filename, user->username, user->rights, user->wrongs);
    print_chances(user->chance_array);
}

void write_user(user user, int word_count) {
    int i;
    FILE *usrfile = fopen(user.filename, "wb");

    printf("fwrite(%s) username: %s, r/w: %d/%d, ", user.filename, user.username, user.rights, user.wrongs);
    print_chances(user.chance_array);
    fwrite(user.username, sizeof(char), strlen(user.username) + 1, usrfile);
    fwrite(&(user.rights), sizeof(int), 1, usrfile);
    fwrite(&(user.wrongs), sizeof(int), 1, usrfile);

    /*fwrite(user.chance_array, sizeof(double), word_count + 1, usrfile);*/
    do {
        fwrite(&(user.chance_array[i]), sizeof(double), 1, usrfile);
        /*printf("%f ", user.chance_array[i]);*/
        i++;
    } while (i < word_count);
    printf("\n");

    fclose(usrfile);
}

/* writes readed chance array to words array */
void init_word_chances(user user, word *head_w) {
    int i;

    for (i = 0; user.chance_array[i] != -1 && head_w != NULL; i++) {
        head_w->chance = user.chance_array[i];
        head_w = head_w->next;
    }
}

word *search_word(word *head_w, char *word, word_t type) {
    while (head_w != NULL) {
        if (head_w->type == type && strcmp(head_w->word, word) == 0) {
            return head_w;
        }
        head_w = head_w->next;
    }
    return NULL;
}

int main() {
    int word_count = 0, word_index, quit = 0, inp_selection, inp_type;
    char *inp_name;
    user user;
    word *words = NULL, *inp_word;

    srand(time(NULL));
    load_words(&words, SYNFILE, &word_count, synonym);
    load_words(&words, ANTFILE, &word_count, antonym);

    printf("Print word list %d\n", word_count);
    print_words(words);

    printf("Please select your user or create new one:\n");
    get_username(&(user.username));
    printf("get username: %s\n\n", user.username);
    read_user(&user, word_count);
    init_word_chances(user, words);

    print_words(words);

    /* TODO: build menu */
    while (inp_selection != 4) {

        /* MENU */
        printf("\n1. Start Game\n");
        printf("2. Add synonym/antonym\n");
        printf("3. Show user stats\n");
        printf("4. Save&Exit\n\n");
        inp_selection = get_selection("Please select an operation: ", 1, 4);

        switch (inp_selection) {

        case 1:
            printf("Enter q for exit.\n");
            while (!quit) {
                print_chances(user.chance_array);

                word_index = get_random_word(user.chance_array, word_count);
                quit = ask_question(&user, words, word_index);
                printf("\n");
            }
            quit = 0;
            break;

        case 2:
            printf("Enter the word name for adding new pair: ");
            do {
                inp_name = dscan_line(stdin);
            } while (inp_name[0] == 0);
            inp_type = get_selection("Synonym(0) or Antonym(1): ", 0, 1);
            inp_word = search_word(words, inp_name, inp_type);

            if (inp_word == NULL)
                printf("%s not found.\n", inp_name);
            else {
                printf("Enter the new %s of %s: ", word_type_to_string(inp_type), inp_name);
                do {
                    inp_name = dscan_line(stdin);
                } while (inp_name[0] == 0);
                add_pair(inp_name, inp_word->pairs);
            }
            free(inp_name);
            break;

        case 3:
            printf("\nUsername: %s\nRight/Wrong Ratio: %d/%d\n", user.username, user.rights, user.wrongs);
            break;
        }
    }

    printf("\n\nSaving progress...\n");
    write_user(user, word_count);
    store_words(words, SYNFILE, synonym);
    store_words(words, ANTFILE, antonym);
    return 0;
}
