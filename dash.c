/* Main file for dash shell */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsecommand.h"
#include "dash.h"

int main(int argc, char *argv[]) {
    
    //Interactive Mode
    if (argc == 1) {
        
        //Defining the buffer
        size_t bufsize = 1024;
        char *buffer = (char *) malloc(bufsize * sizeof(char)); //Allocating memory for buffer
        size_t inputlen;
       
        //get command from user
        while (1) {
            printf("dash> ");
            // TODO: Check for buffer length limit
            inputlen = getline(&buffer, &bufsize, stdin);
            int buffer_length = strlen(buffer);
            if (buffer[buffer_length - 1] == '\n') {
                buffer[buffer_length - 1] = '\0';
            }
            
            //Parse User Command
            int pret = parse_command(buffer);
            if(pret == -1){
                break;
            }

        }
    }
    //Batch Mode
    else if (argc == 2) {
        FILE *fb;
        ssize_t read;
        size_t len;
        char line[MAX_LINE_LENGTH];

        //printf("%s", argv[1]);

        fb = fopen(argv[1], "r"); //Open batch file in read mode

        if (fb == NULL) {
            //Error Processing
            write_error();
        }

        //Till End of File, Read line by line
        while(fgets(line, MAX_LINE_LENGTH, fb) != NULL){
            int line_len = strlen(line);

            if (line[line_len - 1] == '\n') {
                line[line_len - 1] = '\0';
            }

            int pret = parse_command(line);
            if(pret == -1){
                break;
            }
        }
    } 
    else {
        //Error Processing
        write_error();
    }

    exit(0);
}

/*
    Parses User Command
    1. Calls exec_single_command() if user entered only 1 command
    2. Calls exec_parallel_commands() if user entered parallel commands
    3. Returns -1 if exit is encountered, returns 0 otherwise.

*/
int parse_command(char* command){

    int is_parallel_commands = 0;
    int ret = 0;

    // Check for '&' in the buffer for parallel commands
    for(int i = 0; command[i] != '\0'; i++){
        if (command[i] == '&') {
            is_parallel_commands = 1;
            break;
        }
    }

    if(is_parallel_commands == 0){
        ret = exec_single_command(command); //parse user input
    } 
    else{
        ret = exec_parallel_commands(command); //parse user input
    }

    return ret;
    
}