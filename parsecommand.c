/*
 * This file deals with functions that parse user input
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsecommand.h"
#include <unistd.h>


/*
1. Calls get_tokens() and stores list of command and arguments in tokens
2. Check if command(tokens[0]) is built-in or not

*/
int read_command(char *input) {
    char *tokens[MAX_TOKENS];
    int num_tokens = get_tokens(input," ", tokens);

    if(strcmp(tokens[0], "cd") == 0 || strcmp(tokens[0], "exit") == 0 || strcmp(tokens[0], "path") == 0){
        
        printf("Built-in command\n");
        if(strcmp(tokens[0], "exit") == 0){
            return -1;
        }
    }
    else{
        printf("Not a Built-in command\n");

        int rc = fork();
        if (rc == 0) { // child:
        printf("I am child\n");
        execv(tokens[0], tokens); // runs command
        } 
        else { 
            // parent goes down this path (main)
            wait(NULL);
            printf("I am parent\n");

        }
    }


    
    return 1;
}

/*
 * Uses strtok_r and extracts tokens from a string sing given delimiter
 * Returns List of tokens
 */
int get_tokens(char *str, char *delim, char *tokens[]) {
    char *ref = str;
    char *token;
    int num_tokens = 0;

//    *tokens = malloc(MAX_TOKENS * sizeof(char *));
    for (int i = 0; (token = strtok_r(ref, delim, &ref)); i++) {
        if (num_tokens >= MAX_TOKENS) {
            // TODO: Fix error messages
            printf("Max tokens reached");
            break;
        }
//        printf("token: %s\n", token);
        // Skip big tokens
        /*if (strlen(token) >= MAX_TOKEN_LENGTH) {
            // TODO: Fix error messages
            printf("%s is too big", token);
            continue;
        }*/
        tokens[i] = malloc((strlen(token) + 1) * sizeof(char));
        tokens[i] = strdup(token);
        num_tokens += 1;
    }

    tokens[num_tokens] = NULL;

//    printf("num_tokens: %d\n", num_tokens);
    return num_tokens;
}

