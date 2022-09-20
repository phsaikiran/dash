/* Main file for dash shell */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsecommand.h"

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

            int is_parallel_commands = 0;
            // Check for '&' in the buffer for parallel commands
            for (int i = 0; buffer[i] != '\0'; i++) {
                if (buffer[i] == '&') {
                    is_parallel_commands = 1;
                    break;
                }
            }

            if (is_parallel_commands == 0) {
                int ret = exec_single_command(buffer); //parse user input
                if (ret == -1) break;
            } else {
                int ret = exec_parallel_commands(buffer); //parse user input
                if (ret == -1) break;
            }
        }
    } else if (argc == 2) {//Batch Mode
        FILE *f_batch;      //To read from batch file
        f_batch = fopen(argv[0], "r"); //Open batch file in read mode

        if (f_batch == NULL) {
            //Error Processing
        }

        //Till End of File
        //Read from file line by line
        //Parse the Line
        //Execute Command
    } else {
        //TODO: Error Processing
    }
}