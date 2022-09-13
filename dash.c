#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* f_batch;      //To read from batch file
char* user_command; //to store user command

int
main(int argc, char *argv[])
{
    if (argc == 0){//Interactive Mode
        //get command from user
        //parse input
    }
    else if(argc == 1){//Batch Mode
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