#ifndef BUILTINCMD_H
#define BUILTINCMD_H

#define MAX_PATHS 100

int exec_path(char *tokens[], int num_tokens);
int exec_chdir(const char* directory);

#endif