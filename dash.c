/* Main file for dash shell */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

/* Declarations and Definitions */

#define MAX_PATHS 100
#define MAX_TOKENS 100

char *PATHS[MAX_PATHS] = {"/bin"};//think about max path length
int PATH_LENGTH = 1;

int exec_path(char *tokens[], int num_tokens);

int exec_chdir(const char *directory);

int parse_command(char *command);

int get_tokens(char *str, char *delim, char *tokens[]);

int exec_single_command(char *input);

int exec_parallel_commands(char *input);

int get_path(char *command);

void write_error(char error[]);

/********************************* MAIN *************************************************/
int main(int argc, char *argv[]) {
    
    //Interactive Mode
    if (argc == 1) {
        //get command from user
        while (1) {
            char *buffer;
            ssize_t buffer_length = 0;
            size_t buffer_size = 0;

            printf("dash> ");
            buffer_length = getline(&buffer, &buffer_size, stdin);
            if (buffer[buffer_length - 1] == '\n') {
                buffer[buffer_length - 1] = '\0';
            }

            //Parse User Command
            int pret = parse_command(buffer);
            if (pret == -1) {
                break;
            }
        }
    }
    //Batch Mode
    else if (argc == 2) {
        FILE *fb;
        char *line;

        fb = fopen(argv[1], "r"); //Open batch file in read mode

        if (fb == NULL) {
            //Error Processing
            write_error("fb is NULL");
            exit(0);
        }

        ssize_t line_length = 0;
        size_t buffer_size = 0;
        //Till End of File, Read line by line
        while ((line_length = getline(&line, &buffer_size, fb)) > 0) {
            if (line[line_length - 1] == '\n') {
                line[line_length - 1] = '\0';
            }

            int pret = parse_command(line);
            if (pret == -1) {
                break;
            }
        }
    } 
    else {
        //Error Processing
        write_error("argc > 2");
        exit(0);
    }

    exit(0);
}

/****************************** FUNCTIONS **********************************************/

/* parse_command
 * Parses User Command
 * 1. Calls exec_single_command() if user entered only 1 command
 * 2. Calls exec_parallel_commands() if user entered parallel commands
 * 3. Returns -1 if exit is encountered, returns 0 otherwise.
 */
int parse_command(char *command) {
    int is_parallel_commands = 0;
    int ret = 0;

    // Check for '&' in the buffer for parallel commands
    for (int i = 0; command[i] != '\0'; i++) {
        if (command[i] == '&') {
            is_parallel_commands = 1;
            break;
        }
    }

    if (is_parallel_commands == 0) {
        ret = exec_single_command(command); //parse user input
    } 
    else {
        ret = exec_parallel_commands(command); //parse user input
    }

    return ret;
}

/* exec_single_command: Executes a single command
 * 1. Checks for any redirection. Extracts command in case of redirection
 * 2. Calls get_tokens() and stores list of command and arguments in tokens
 * 3. Check if command(tokens[0]) is built-in or not
 * 4. Executes the command
*/
int exec_single_command(char *input) {
    char *input_dup = strdup(input);
    char *reds[MAX_TOKENS];
    int num_red_tokens = get_tokens(input, ">", reds);
    char *redirection_file;

    //If there is an empty
    if (num_red_tokens == 0) {
        return 0;
    } 
    else if (num_red_tokens == 1) {
        // Check if > is present. If it was present and tokens are 1, that means
        // Either no file was mentioned or no command was mentioned
        for (int i = 0; input_dup[i] != '\0'; i++) {
            if (input_dup[i] == '>') {
                write_error("No input file mentioned or no command was present");
                return 0;
            }
        }
    } 
    else if (num_red_tokens == 2) {
        // Redirection is present
        char *redirection_file_tokens[MAX_TOKENS];
        int num_redirection_files = get_tokens(reds[1], " ", redirection_file_tokens);
        if (num_redirection_files == 0) {
            // Multiple redirection files in an error
            write_error("No input file mentioned");
            return 0;
        }
        if (num_redirection_files != 1) {
            // Multiple redirection files in an error
            write_error("Multiple redirection files in an error");
            return 0;
        }
        strcpy(input, reds[0]);
        redirection_file = strdup(redirection_file_tokens[0]);
    } 
    else {
        // Multiple redirections is an error
        write_error("Multiple redirections is an error");
        return 0;
    }

    char *tokens[MAX_TOKENS];
    int num_tokens = get_tokens(input, " ", tokens);

    if (num_tokens == 0) {
        return 0;
    }
    if (strcmp(tokens[0], "cd") == 0 || strcmp(tokens[0], "exit") == 0 || strcmp(tokens[0], "path") == 0) {
        // Build in command
        if (strcmp(tokens[0], "exit") == 0) {
            if (num_tokens != 1) {
                write_error("exit returned error");
                return 0;
            }

            return -1;
        } 
        else if (strcmp(tokens[0], "cd") == 0) {
            //if cd has no arguments - return error
            if (num_tokens != 2) {
                write_error("cd returned error");
                return 0;
            }
            //if cd has more than 1 argument - return error
            int cdret = exec_chdir(tokens[1]);

            if (cdret == -1) {
                //error processing
                write_error("exec_chdir returned -1");
                return 0;
            }
        } 
        else if (strcmp(tokens[0], "path") == 0) {
            return exec_path(tokens, num_tokens);
        }
    } 
    else {
        // Not a built in command
        char *exec_path = strdup(tokens[0]);
        int executable_exist = get_path(exec_path);
        if (executable_exist == -1) {

            if(num_red_tokens == 2){//If a redirection operator was given as input
                int fd = open(redirection_file, O_TRUNC | O_RDWR | O_CREAT, S_IRWXU);
                dup2(fd, STDERR_FILENO);
                close(fd);
            }
            
            write_error(exec_path);            

            return 0;
        }


        int rc = fork();
        if (rc < 0) {
            write_error("fork returned -1");
            return 0;
        } 
        else if (rc == 0) {
            if (num_red_tokens == 2) {
                // TODO: For write_error STD ERR
                int fd = open(redirection_file, O_TRUNC | O_RDWR | O_CREAT, S_IRWXU);
                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);
                close(fd);
            }
            execv(exec_path, tokens); // runs command
        } 
        else {
            // parent goes down this path (main)
            wait(NULL);
        }
    }

    return 1;
}

/* exec_parallel_commands: Executes parallel commands seperated by &
 * 1. Calls strtok_r() and gets each command seperated by &
 * 2. Calls exec_single_command
 */
int exec_parallel_commands(char *input) {
    char *ref = input;
    char *command;
    int num_commands = 0;
    for (int i = 0; (command = strtok_r(ref, "&", &ref)); i++) {
        num_commands += 1;
        int rc = fork();
        if (rc < 0) {
            write_error("fork returned -1");
            return 0;
        } 
        else if (rc == 0) {
            exec_single_command(command);
            exit(0);
        }
    }
    //If there are no parallel commands
    if(num_commands == 0){
        write_error("No parallel command");
        return 0;
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
 * 1. Uses strtok_r and extracts tokens from a string sing given delimiter
 * 2. Returns List of tokens
 */
int get_tokens(char *str, char *delim, char *tokens[]) {
    char *ref = str;
    char *token;
    int num_tokens = 0;

    for (int i = 0; (token = strtok_r(ref, delim, &ref)); i++) {
        if (num_tokens >= MAX_TOKENS) {
            // TODO: Fix error messages
            // printf("Max tokens reached");
            break;
        }
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
 * Returns -1 of no path has the executable
 */
int get_path(char *command) {
    for (int i = 0; i < PATH_LENGTH; i++) {
        char *path = strdup(PATHS[i]);
        strcat(path, "/");
        strcat(path, command);
        if (access(path, X_OK) == 0) {
            strcpy(command, path);
            return i;
        }
    }
    return -1;
}

/* Error Processing */
void write_error(char error[]) {
    // Comment the printf to stop printing debug messages
    printf("%s\n", error);
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

/****************************** BUILT-IN COMMAND FUNCTIONS **********************************************/

/*
 * Sets the path given by the user.
 */
int exec_path(char *tokens[], int num_tokens) {
    PATH_LENGTH = 0;
    for (int i = 1; i < num_tokens; i++) {
        PATHS[PATH_LENGTH++] = tokens[i];
    }
    return 1;
}

/*
 * Changes directory to path specified by the user.
 * 1. Uses chdir() system call
 * 2. Returns 0 if directory was changed successfully.
 * 3. Returns 1 if directory was not changed successfully.
 */
int exec_chdir(const char *directory) {
    int cd_ret;
    cd_ret = chdir(directory);
    if (cd_ret == -1) {
        return -1;
    }
    return 0;
}