/*
** wordonym.c:
**
** A game asks antonym/synonym questions to user and manages user's progress.
**
** Author: Halil Ilhan.
**
** Revision: 2021.06.15
** 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SYNFILE "synonyms_test.txt"
#define ANTFILE "antonyms_test.txt"
#define USRFILE "usernames.txt"


/* --------------------- Structure & Enum Declarations --------------------- */
typedef enum { synonym,
               antonym } word_t;

typedef struct user {
    char *username, *filename;
    int rights, wrongs;   /* number of right and wrong answers */
    double *chance_array; /* user specific ask probability of words */
} user;

/* word node */
typedef struct word {
    word_t type;
    char *word;
    char **pairs;  /* a char array for antonyms/synonyms of the word */
    double chance; /* chance of being asked the word */
    struct word *next;
} word;




/* -------------------------------------------------------------------------- */
/*                                 Prototypes                                 */
/* -------------------------------------------------------------------------- */

/* --------------------------- Question operations -------------------------- */
double increase_chance(double chance);
double decrease_chance(double chance);
int get_random_word(double chance_array[], int word_count);
int ask_question(user *user, word *word_list, int word_index);
int check_answer(char *answer, char **pairs);

/* ----------------------------- Word operations ---------------------------- */
void load_words(word **head_w, char *filename, int *word_count, word_t type);
void parsed_insert_word(word **head_w, char *raw_data, word_t type);
void store_words(word *head_w, char *filename, word_t type);
void init_word_chances(user user, word *head_w);

word *get_word_by_index(word *word_list, int index);
word *get_word_by_name(word *head_w, char *word, word_t type);
char *word_type_to_string(word_t type);

/* ----------------------------- User operations ---------------------------- */
int read_user(user *user, int word_count);
void create_user(user *user, int word_count, char **usernames);
void write_user(user user, int word_count);

/* --------------------------- Username functions --------------------------- */
int is_username_exist(char *username, char **usernames);
void get_usernames(char ***usernames);
void select_username(char **usernames, char **username);
void write_usernames(char **usernames);

/* ------------------------------ IO Operations ----------------------------- */
char *dscan_line(FILE *src);
void insert_string(char *new_str, char ***str_array);
double get_selection(const char msg[], int lower, int upper);

/* ---------------------------- Ending operations --------------------------- */
/*void free_all();*/
void terminate(char err[]);
void check_alloc(void *ptr);



/* -------------------------------------------------------------------------- */
/*                                    MAIN                                    */
/* -------------------------------------------------------------------------- */
int main() {
    /* list, counter and menu input variables */
    int quit = 0, word_count = 0, word_index, inp_selection, inp_type;
    char *inp_name, **usernames;
    user user;
    word *words = NULL, *inp_word; /* words is linked list of synonym and antonym words */

    srand(time(NULL));
    load_words(&words, SYNFILE, &word_count, synonym); /* load words and count number of word */
    load_words(&words, ANTFILE, &word_count, antonym);

    get_usernames(&usernames);
    select_username(usernames, &(user.username));
    if (!read_user(&user, word_count)) create_user(&user, word_count, usernames); /* try to fill user from bin file */
    init_word_chances(user, words); /* sync user specific chance array with words list */

    /* Game Header */
    printf("\n   _      __            __                     __    \n  | | /| / /__  _______/ /__  ___  __ ____ _  / /    \n"
           "  | |/ |/ / _ \\/ __/ _  / _ \\/ _ \\/ // /  ' \\/_/ \n  |__/|__/\\___/_/  \\_,_/\\___/_//_/\\_, /_/_/_(_)  \n"
           "                                 /___/                       v0.0.2\n\n");
    
    while (inp_selection != 4) {
        /* MENU */
        printf("\n1. Start Game\n");
        printf("2. Add synonym/antonym\n");
        printf("3. Show user info\n");
        printf("4. Save & Exit\n\n");
        inp_selection = get_selection("Please select an operation: ", 1, 4);
        printf("\n");

        switch (inp_selection) {

        /* Ask well chosen random words until quit */
        case 1:
            printf("Enter q for exit.\n");
            while (!quit) {
                word_index = get_random_word(user.chance_array, word_count);
                quit = ask_question(&user, words, word_index);
                printf("\n");
            }
            quit = 0;
            break;

        /* Adds new synonym/antonym pairs to words in list */
        case 2:
            printf("Enter the word name to add new pair: ");
            do {
                inp_name = dscan_line(stdin);
            } while (inp_name[0] == 0); /* dont read empty newlines */
            inp_type = get_selection("Synonym(0) or Antonym(1): ", 0, 1);
            inp_word = get_word_by_name(words, inp_name, inp_type);

            if (inp_word == NULL)
                printf("%s not found.\n", inp_name);
            else {
                printf("Enter the new %s of %s: ", word_type_to_string(inp_type), inp_name);
                do {
                    inp_name = dscan_line(stdin);
                } while (inp_name[0] == 0);
                insert_string(inp_name, &(inp_word->pairs));
            }
            free(inp_name);
            break;

        /* User info, stats */
        case 3:
            printf("\n+-----------------------------------------------------------------------+\n");
            printf("|                                                                       |\n");
            printf("|        Username: %-45s        |\n", user.username);
            printf("|                                                                       |\n");
            printf("|        Right/Wrong Ratio: %5d / %-5d                               |\n", user.rights, user.wrongs);
            printf("|                                                                       |\n");
            printf("+-----------------------------------------------------------------------+\n\n");
            break;
        }
    }

    /* Write all changes in word list and user to files */
    printf("\nSaving progress...\n");
    write_user(user, word_count);
    write_usernames(usernames);
    store_words(words, SYNFILE, synonym);
    store_words(words, ANTFILE, antonym);
    return 0;
}



/* -------------------------------------------------------------------------- */
/*                             Question Operations                            */
/* -------------------------------------------------------------------------- */
/* increases chance with proper method ex: 1 to 2, 5 to 6, 0.2 to 0.25, 0.3333 to 0.5 */ 
double increase_chance(double chance) {
    if (chance >= 1.0) {
        return chance + 1;
    }
    return 1.0 / ((1.0 / chance) - 1.0);
}

/* decreases chance with proper method ex: 6 to 5, 2 to 1, 0.5 to 0.3333, 0.25 to 0.2 */ 
double decrease_chance(double chance) {
    if (chance > 1.0) {
        return chance - 1;
    }
    return 1.0 / ((1.0 / chance) + 1.0);
}

/* selects a random word from items with different probability given as chance_array */
int get_random_word(double chance_array[], int word_count) {
    int i, sum = 0, random_index, *indexes = (int *)calloc(word_count + 1, sizeof(int));
    double min_chance, expand_factor;

    check_alloc(indexes);

    min_chance = chance_array[0];
    for (i = 1; chance_array[i] != -1.0; i++) {
        if (chance_array[i] < min_chance) min_chance = chance_array[i];
    }
    expand_factor = 1.0 / min_chance; /* expand factor for making all number greater than 1 */

    for (i = 0; chance_array[i] != -1.0; i++) {
        sum += chance_array[i] * expand_factor + 0.1; /* expand numbers and round to up */
        indexes[i] = sum; /* set sum of all previous numbers as current item of indexes array */
    }
    indexes[i] = -1;
    /* ex:                            */
    /* Chances: 1  2  0.5  3    0.3  */
    /* Indexes: 1  3  3.5  6.5  6.8 */
    random_index = rand() % indexes[i - 1] + 1; /* gets random number from one to sum of all probabilities */
    for (i = 0; indexes[i] != -1 && indexes[i] < random_index; i++) /* goes until random_index is below i'th items index value */
        ;
    return i;
}

/* asks question from given index and handles answer */
int ask_question(user *user, word *word_list, int word_index) {
    int right_answer;
    char *answer, *temp;
    word *question = get_word_by_index(word_list, word_index);

    printf("What is the %s of %s?\n", word_type_to_string(question->type), question->word);
    printf("> ");

    do {
        answer = dscan_line(stdin);
    } while (answer[0] == 0);

    if (strcmp(answer, "q") == 0) return 1; /* quit game */

    right_answer = check_answer(answer, question->pairs);
    if (right_answer) {
        printf("Right. \n");
        (question->chance) = decrease_chance(question->chance); /* if answer is right decrease probability of asking this word again */
        (user->rights)++;
    } else {
        printf("Wrong. \n");

        printf("Do you want to add '%s' to the %ss of %s? (y / n) ", answer, word_type_to_string(question->type), question->word); /* asks to user, is this word should be in vocabulary */
        do {
            temp = dscan_line(stdin);
        } while (temp[0] == 0 || (temp[0] != 'y' && temp[0] != 'n'));
        if (temp[0] == 'y') {
            insert_string(answer, &(question->pairs));
            return 0;
        }

        (question->chance) = increase_chance(question->chance); /* if answer is right decrease probability of asking this word again */
        (user->wrongs)++;
    }
    user->chance_array[word_index] = question->chance; /* update chance in user also */

    return 0;
}

/* if answer found in pairs return true */
int check_answer(char *answer, char **pairs) {
    int i;
    for (i = 0; pairs[i] != NULL; i++) {
        if (strcmp(answer, pairs[i]) == 0) {
            return 1;
        }
    }
    return 0;
}


/* -------------------------------------------------------------------------- */
/*                               Word Operations                              */
/* -------------------------------------------------------------------------- */
/* parses raw_data into a word struct and inserts at end of word linked list*/
void parsed_insert_word(word **head_w, char *raw_data, word_t type) {
    int i, pair_num = 5;
    word *new_word = (word *)malloc(sizeof(word)), *temp_w = *head_w;
    check_alloc(new_word);

    new_word->pairs = (char **)malloc((pair_num * sizeof(char *)));
    check_alloc(new_word->pairs);

    raw_data = strtok(raw_data, " "); /* get word name */
    new_word->word = (char *)malloc((strlen(raw_data) + 1) * sizeof(char));
    check_alloc(new_word->word);
    strcpy(new_word->word, raw_data);

    strtok(NULL, " "); /* remove = or <> sign */

    raw_data = strtok(NULL, ","); /* get first pair */
    for (i = 1; raw_data != NULL; i++) {

        if (i > pair_num) { /* expand array to double on each five words read */
            pair_num *= 2;
            new_word->pairs = (char **)realloc(new_word->pairs, pair_num * sizeof(char *));
            check_alloc(new_word->pairs);
        }

        new_word->pairs[i - 1] = (char *)malloc((strlen(raw_data) + 1) * sizeof(char)); /* allocate new element and */
        check_alloc(new_word->pairs[i - 1]);
        strcpy(new_word->pairs[i - 1], raw_data);                                       /* copy data in */

        raw_data = strtok(NULL, ","); /* get the next word */
    }

    if (i > pair_num) {
        new_word->pairs = realloc(new_word->pairs, i * sizeof(char *));
        check_alloc(new_word->pairs);
    }
    new_word->pairs[i - 1] = NULL; /* end array with a sentinel value */

    /* init word variables */
    new_word->type = type;
    new_word->chance = 1.0;
    new_word->next = NULL;

    /* insert to end */
    if (*head_w == NULL) {
        *head_w = new_word;
    } else {

        while (temp_w->next != NULL)
            temp_w = temp_w->next;
        temp_w->next = new_word;
    }
}

/* loads words from given file to given word linked list */
void load_words(word **head_w, char *filename, int *word_count, word_t type) {
    char *line;

    FILE *word_file = fopen(filename, "r");
    if (word_file == NULL) terminate("Couldn't open file.\n");

    while (!feof(word_file)) {

        line = dscan_line(word_file);
        parsed_insert_word(head_w, line, type);
        free(line);

        (*word_count)++;
    }
    fclose(word_file);
}

/* writes given word linked list into given file */
void store_words(word *head_w, char *filename, word_t type) {
    int i;
    char seperator[2];
    FILE *word_file = fopen(filename, "w");
    if (word_file == NULL) terminate("Couldn't open file.\n");

    /* synonym words seperated by '=' and antonyms by '<>'*/
    if (type == synonym) {
        seperator[0] = '=';
    } else if (type == antonym) {
        seperator[0] = '<';
        seperator[1] = '>';
    }

    while (head_w != NULL) {
        if (head_w->type == type) { /* only writing words with given type */

            fprintf(word_file, "%s %s ", head_w->word, seperator);
            for (i = 0; head_w->pairs[i + 1] != NULL; i++) {
                fprintf(word_file, "%s,", head_w->pairs[i]);
            }
            fprintf(word_file, "%s", head_w->pairs[i]);
            if (!(head_w->next == NULL || head_w->type != head_w->next->type)) fprintf(word_file, "\n"); /* add newline until last element */
        }

        head_w = head_w->next;
    }
    fclose(word_file);
}

/* writes readed user chance array to words array */
void init_word_chances(user user, word *head_w) {
    int i;

    for (i = 0; user.chance_array[i] != -1 && head_w != NULL; i++) {
        head_w->chance = user.chance_array[i];
        head_w = head_w->next;
    }
}


/* returns nth word */
word *get_word_by_index(word *word_list, int index) {
    int i;
    for (i = 0; i < index; i++) {
        word_list = word_list->next;
    }
    return word_list;
}

/* searches and returns word with given name */
word *get_word_by_name(word *head_w, char *word, word_t type) {
    while (head_w != NULL) {
        if (head_w->type == type && strcmp(head_w->word, word) == 0) {
            return head_w;
        }
        head_w = head_w->next;
    }
    return NULL; /* if user not found */
}

/* word_t enum to string converter */
char *word_type_to_string(word_t type) {
    if (type == synonym) return "synonym";
    return "antonym";
}


/* -------------------------------------------------------------------------- */
/*                               User operations                              */
/* -------------------------------------------------------------------------- */
/* if exists reads user from bin file username.worddat, else returns 0 as */
int read_user(user *user, int word_count) {
    int i = 0;
    double temp;
    FILE *usrfile;

    user->chance_array = (double *)calloc(word_count + 1, sizeof(double));
    check_alloc(user->chance_array);

    user->filename = (char *)malloc((strlen(user->username) + 9) * sizeof(char));
    check_alloc(user->filename);

    sprintf(user->filename, "%s.worddat", user->username);
    usrfile = fopen(user->filename, "rb");
    if (usrfile == NULL) {
        return 0;
    }

    /* fill user with reading user data file */
    fread(user->username, sizeof(char), strlen(user->username) + 1, usrfile);

    fread(&(user->rights), sizeof(int), 1, usrfile);
    fread(&(user->wrongs), sizeof(int), 1, usrfile);
    do {
        fread(&(user->chance_array[i]), sizeof(double), 1, usrfile);
        i++;
    } while (i < word_count);
    user->chance_array[word_count] = -1;

    fclose(usrfile);
    return 1;
}

/* initializes user struct and adds username to usernames array */
void create_user(user *user, int word_count, char **usernames) {
    int i;

    insert_string(user->username, &usernames);

    for (i = 0; i < word_count; i++) {
        user->chance_array[i] = 1.0;
    }
    user->chance_array[word_count] = -1;
    user->rights = 0;
    user->wrongs = 0;
}

/* writes user to bin file username.worddat */
void write_user(user user, int word_count) {
    int i = 0;
    FILE *usrfile = fopen(user.filename, "wb");
    if (usrfile == NULL) terminate("Couldn't open file.\n");

    fwrite(user.username, sizeof(char), strlen(user.username) + 1, usrfile);
    fwrite(&(user.rights), sizeof(int), 1, usrfile);
    fwrite(&(user.wrongs), sizeof(int), 1, usrfile);

    do {
        fwrite(&(user.chance_array[i]), sizeof(double), 1, usrfile);
        i++;
    } while (i < word_count);
    printf("\n");

    fclose(usrfile);
}


/* -------------------------------------------------------------------------- */
/*                             Username functions                             */
/* -------------------------------------------------------------------------- */
/* checks is username exists in usernames array */
int is_username_exist(char *username, char **usernames) {
    int i;
    for (i = 0; usernames[i] != NULL; i++) {
        if (strcmp(username, usernames[i]) == 0) return 1;
    }
    return 0;
}

/* read and stores usernames from file */
void get_usernames(char ***usernames) {
    int i;
    char *username;
    FILE *usrfile = fopen(USRFILE, "r");
    if (usrfile == NULL) terminate("Couldn't open file.\n");

    *usernames = (char **)calloc(1, (sizeof(char *)));
    check_alloc(*usernames);

    for (i = 0; !feof(usrfile); i++) {
        username = dscan_line(usrfile);
        if (username[0] != 0)
            insert_string(username, usernames);
        else
            i--;
    }
    (*usernames)[i] = NULL;
    fclose(usrfile);
}

/* lists usernames and returns selection as username string */
void select_username(char **usernames, char **username) {
    int i, selection, valid = 0;

    printf("\nPlease select or create a user:\n\n");
    for (i = 0; usernames[i] != NULL; i++) {
        printf("%d. %s\n", i + 1, usernames[i]);
    }
    printf("%d. Create new user\n", i + 1);

    selection = get_selection("Enter the selection: ", 1, i + 1);
    if (selection == i + 1) {
        while (!valid) {
            printf("Please enter username: ");
            do {
                *username = dscan_line(stdin);
            } while ((*username)[0] == 0);
            if (is_username_exist(*username, usernames))
                printf("Username exists, try again.\n");
            else
                valid = 1;
        }
    } else {
        *username = (char *)malloc((strlen(usernames[selection - 1]) + 1) * sizeof(char));
        check_alloc(*username);
        strcpy(*username, usernames[selection - 1]);
    }
}

/* writes usernames array to file */
void write_usernames(char **usernames) {
    int i;
    FILE *usrfile = fopen(USRFILE, "w");
    if (usrfile == NULL) terminate("Couldn't open file.\n");

    for (i = 0; usernames[i + 1] != NULL; i++) {
        fprintf(usrfile, "%s\n", usernames[i]);
    }
    fprintf(usrfile, "%s", usernames[i]);
    fclose(usrfile);
}


/* -------------------------------------------------------------------------- */
/*                                IO Operations                               */
/* -------------------------------------------------------------------------- */
/* dynamically scans a line from src and stores in an array */
char *dscan_line(FILE *src) {
    int counter = 0, size = 3;
    char input, *str = (char *)malloc((size + 1) * sizeof(char));
    check_alloc(str);

    do {
        fscanf(src, "%c", &input);
        counter++;

        if (counter >= size) {
            size *= 2;
            str = (char *)realloc(str, (size + 1) * sizeof(char));
            check_alloc(str);
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

/* expands str_array and copies new_str to end */
void insert_string(char *new_str, char ***str_array) { /* str_array: address of array of char pointer arrays */
    int i = 0;

    while ((*str_array)[i] != NULL)
        i++;
    *str_array = (char **)realloc(*str_array, (i + 2) * sizeof(char *));
    check_alloc(*str_array);

    (*str_array)[i] = (char *)malloc((strlen(new_str) + 1) * sizeof(char));
    check_alloc((*str_array)[i]);
    strcpy((*str_array)[i], new_str);
    (*str_array)[i + 1] = NULL;
}

/* secure scanf function that gets the selection for menu with bullet-proof error handling */
double get_selection(const char msg[], int lower, int upper) {
    int status, flag = 0;
    double selection = 0.0;

    while (!flag) {
        if (msg[0] != '\0') printf("%s", msg); /* print input message */
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


/* -------------------------------------------------------------------------- */
/*                              Ending operations                             */
/* -------------------------------------------------------------------------- */

void terminate(char err[]) {
    printf("\n%s\n\n", err);
    exit(0);
}

void check_alloc(void *ptr){
    if(ptr == NULL) terminate("Out of memory.\n");
}
