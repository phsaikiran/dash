/*
 * This file deals with functions that parse user input
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsecommand.h"

void readUserCommand(char *input) {

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
        if (strlen(token) >= MAX_TOKEN_LENGTH) {
            // TODO: Fix error messages
            printf("%s is too big", token);
            continue;
        }
        tokens[i] = malloc((MAX_TOKEN_LENGTH + 1) * sizeof(char));
        strcpy(tokens[i], token);
        num_tokens += 1;
    }
//    printf("num_tokens: %d\n", num_tokens);
    return num_tokens;
}

