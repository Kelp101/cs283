#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
void reverse_string(char *, int);
void print_words(char *, int);
void word_replace(char *, int, char*, char*);


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    char *begin = user_str;
    char *end = buff;
    int total_byte = 0;
    int user_length = 0;
    int whitespace = 0;
    
    //Checking user string until the null terminator
    while (*begin != '\0'){
        if (*begin == ' ' || *begin == '\t'){   //Checking for spaces or tabs
            if (!whitespace){                   //Checking duplicate white space
                if (total_byte >= len){         //Makes sure the buffer has space or else exits
                    return -1;
                }
                *end++ = ' ';                   //Add space to buffer
                total_byte++;                   //Increment bytes for the amount of characters in the buffer
                user_length++;                  //Keeps track of the length of the user string
                whitespace = 1;                 //Tracks whitespace
            }
        } else {                                //Checks for characters that aren't whitespaces
            if (total_byte >= len){             //Makes sure the buffer has space or else exits
                return -1;
            }
            *end++ = *begin;                    //Adds each character to the buffer
            total_byte++;                       //Increment bytes for the amount of characters in the buffer
            user_length++;                      //Keeps track of the length of the user string
            whitespace = 0;                     //Sets whitespace to 0 to repeat duplicate tracker
        }
        begin++;                                //Checks the next character in the while loop
    }

    //Makes sure user string is within 50 characters
    if (user_length > len){
        return -1;
    }

    //Remove whitespaces from the buffer, decrement bytes for whitespace
    if (whitespace && user_length > 0) {
        end--;
        total_byte--;
        user_length--;
    }

    //Fills dots for remaining buffer space
    while (total_byte < len){
        *end++ = '.';
        total_byte++;
    }

    //Returns length of user string, no padding or dots
    return user_length;
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    //YOU MUST IMPLEMENT
    int count = 0;
    int current_word = 0;

    for (int i = 0; i < str_len; i++){
        if (buff[i] == ' '){
            current_word = 0;
        } else if (!current_word){
            current_word = 1;
            count++;
        }
    }
    return count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

void reverse_string(char *str, int str_len){
    int start_idx = 0;
    int end_idx = str_len - 1;

    while (start_idx < end_idx) {
        char tmp_char = str[start_idx];
        str[start_idx] = str[end_idx];
        str[end_idx] = tmp_char;
        start_idx++;
        end_idx--;
    }
}

void print_words(char *buff, int str_len){
    printf("Word Print\n");
    printf("----------\n");

    int wc = 0;
    int wlen = 0;
    bool word_start = false;

    for (int i = 0; i < str_len; i++) {
        if (buff[i] != ' ' && !word_start) {
            word_start = true;
            wc++;
            wlen = 0;
            printf("%d. ", wc);
        }

        if (word_start){
            if (buff[i] != ' '){
                putchar(buff[i]);
                wlen++;
            }
        }

        if ((buff[i] == ' ' || i == str_len - 1) && word_start){
            printf(" (%d)\n", wlen);
            word_start = false;
        }
    }
}

//Extra credit not implemented
void word_replace(char *buff, int str_len, char *search, char *replace){
    printf("Not Implemented!\n");
}


int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    /*
    This is safe because if arv[1] does not exist there would
    still need to be at least more than two arguments for it
    to pass the if statement to let the program run.
    */
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    /*
    The purpose of this if statement below is to make sure that
    the commands of the arguments are within the operations.

    */
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ);
    if (!buff){
        printf("Error with memory allocation");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            reverse_string(buff, user_str_len);
            printf("Reversed String: ");
            for (int i = 0; i < user_str_len; i++) {
                putchar(buff[i]);
            }
            putchar('\n');
            break;

        case 'w':
            print_words(buff, user_str_len);
            break;
        
        case 'x':
            word_replace(buff, user_str_len, argv[3], argv[4]);
            break;

        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);                     //free buffer
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
/*
I think it is good practice to provide both the pointer and the length because
if we want to increase the buff variable and size of strings it takes, we could
change the exact bytes once above instead of going through the whole code manually
changing each number. This is making sure that we include encapsulation and makes
the code easier to change in the future.
*/