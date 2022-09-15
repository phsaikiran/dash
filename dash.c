/* Main file for dash shell */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* f_batch;      //To read from batch file
char* user_command; //to store user command

int
main(int argc, char *argv[])
{
    //Defining the buffer
    char* buffer;
    size_t bufsize = 1024;
    size_t inputlen;

    buffer = (char *)malloc(bufsize * sizeof(char)); //Allocating memory for buffer

    //Interactive Mode
    if (argc == 0){
        //get command from user
        while(1){
		    printf("dash>");
		    inputlen = getline(&buffer, &bufsize, stdin);
            readUserCommand(buffer);
        }
        //parse input
    }
    //Batch Mode
    else if(argc == 1){
        f_batch = fopen(argv[0], "r"); //Open batch file in read mode

        if(f_batch == NULL){
            //Error Processing
        }

        //Till End of File
        //Read from file line by line
        //Parse the Line
        //Execute Command

    }
    else{
        //Error Processing
    }
    

}