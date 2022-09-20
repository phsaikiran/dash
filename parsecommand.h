#ifndef PARSECOMMAND_H
#define PARSECOMMAND_H

#define MAX_TOKENS 20
#define MAX_TOKEN_LENGTH 100

int get_tokens(char *str, char *delim, char *tokens[]);

int read_command(char *input);

int get_path(char *command);

#endif //PARSECOMMAND_H
