#ifndef PARSECOMMAND_H
#define PARSECOMMAND_H

#define MAX_TOKENS 20
#define MAX_PARALLEL_COMMANDS 10
#define MAX_TOKEN_LENGTH 100

int get_tokens(char *str, char *delim, char *tokens[]);

int exec_single_command(char *input);

int exec_parallel_commands(char *input);

int get_path(char *command);

#endif //PARSECOMMAND_H
