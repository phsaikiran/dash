/*
 * This file contains built-in command functions
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtincmd.h"

char *PATHS[MAX_PATHS] = {"/bin"};
int PATH_LENGTH = 1;

int exec_path(char *tokens[], int num_tokens) {
    //TODO: Error checking for not a happy path
    if (num_tokens == 2) {
        PATHS[PATH_LENGTH] = strdup(tokens[1]);
        PATH_LENGTH += 1;
    } else {
        //TODO Error handling
    }
    return 1;
}