/*
 * This file deals with functions that parse user input
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "parsecommand.h"
#include "builtincmd.h"

/*
 * read_command
 * 1. Calls get_tokens() and stores list of command and arguments in tokens
 * 2. Check if command(tokens[0]) is built-in or not
 */
int exec_single_command(char *input) {
    char *tokens[MAX_TOKENS];
    int num_tokens = get_tokens(input, " ", tokens);
    char execpath[MAX_TOKEN_LENGTH];

    if (strcmp(tokens[0], "cd") == 0 || strcmp(tokens[0], "exit") == 0 || strcmp(tokens[0], "path") == 0) {
        //printf("Built-in command\n");
        if (strcmp(tokens[0], "exit") == 0) {
            return -1;
        } 
        else if (strcmp(tokens[0], "cd") == 0) {
            //if cd has no arguments - return error
            //if cd has more than 1 argument - return error
            int cdret = exec_chdir(tokens[1]);

            if(cdret == -1){
                //error processing
                write_error();
            }
        } 
        else if (strcmp(tokens[0], "path") == 0) {
            return exec_path(tokens, num_tokens);
        }
    } else {
        //printf("Not a Built-in command\n");
        strcpy(execpath, tokens[0]);
        int cmdexist = get_path(execpath);
        //printf("%s \n", execpath);

        int rc = fork();
        if (rc < 0) {
            //TODO: Handle error
        } 
        else if (rc == 0) { // child:
            //printf("I am child\n");
            execv(execpath, tokens); // runs command
        } 
        else {
            // parent goes down this path (main)
            wait(NULL);
            //printf("I am parent\n");
        }
    }

    return 1;
}

int exec_parallel_commands(char *input) {
    // TODO: Check for max parallel commands length
    char *commands[MAX_PARALLEL_COMMANDS];
    int num_commands = get_tokens(input, "&", commands);
    for (int i = 0; i < num_commands; i++) {
        // TODO: ALl forks check for error
        int rc = fork();
        if (rc < 0) {
            //TODO: Handle error
        } else if (rc == 0) {
            exec_single_command(commands[i]);
            exit(0);
        }
    }

    // Wait for all children to exit
    while (num_commands > 0) {
        wait(NULL);
        --num_commands;
    }
    return 1;
}

/*
 * get_tokens
 1. Uses strtok_r and extracts tokens from a string sing given delimiter
 2. Returns List of tokens
 */
int get_tokens(char *str, char *delim, char *tokens[]) {
    char *ref = str;
    char *token;
    int num_tokens = 0;

    for (int i = 0; (token = strtok_r(ref, delim, &ref)); i++) {
        if (num_tokens >= MAX_TOKENS) {
            // TODO: Fix error messages
            printf("Max tokens reached");
            break;
        }
        tokens[i] = malloc((strlen(token) + 1) * sizeof(char));
        tokens[i] = strdup(token);
        num_tokens += 1;
    }
    tokens[num_tokens] = NULL;
    return num_tokens;
}

/*
 * get_path
 * Returns string with correct path
 * eg: for command ls, will return "/bin/ls"
 */
int get_path(char *command) {
    char *path = strdup("/bin/");

    strcat(path, command);
    //printf("%s", path);
    strcpy(command, path);

    return 1;
}

/* Error Processing */

void write_error(void){
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));

}