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

/*
    Sets the path given by the user.
*/
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

/*
    Changes directory to path specified by the user.
    1. Uses chdir() system call
    2. Returns 0 if directory was changed successfully.
    3. Returns 1 if directory was not changed successfully.
*/
int exec_chdir(const char* directory){
    int cd_ret;

    cd_ret = chdir(directory);

    if(cd_ret == -1){
        return -1;
    }

    return 0;

}