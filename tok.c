#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * This program counts the number of sentences in a given string and
 * the number of tokens each in sentence.
 * Terminates when user enters BYE
 */

int
main(int argc, char *argv[])
{

	// define the required variables
	char* buffer;
	size_t bufsize = 32;
    size_t inputlen;

	buffer = (char *)malloc(bufsize * sizeof(char));

	char* token;
	char* sentence;
	char* bye = strdup("bye\n");
	
	while (1) {
		
		printf("Enter the string: ");
		// use getline() to get the input string
		inputlen = getline(&buffer, &bufsize, stdin);
		char* ref = buffer;
		printf("%zu", inputlen);

		if(strcmp(buffer, bye) == 0){
			//printf("strcmp(buffer, bye");
			break;
		}

		//printf("%d",strcmp(buffer, bye));



		// The input string consists of several sentences separated by '.'
		// Each sentence consists of several tokens separated by ' ' (space).
		// Using strtok_r() find the number of sentences and the number of tokens
		// in each sentence. HINT: man strtok and look at the sample program at the end.
		while((sentence = strtok_r(ref, ".?", &ref))){
			printf("Sentence: %s\n", sentence);

			char* ref_t = sentence;
			while((token = strtok_r(ref_t, " ", &ref_t))){

				printf("Token:\t");
				printf("%s\n",token);
			}
		}
		// Print the results.
		// If the first token is BYE, break from the while loop (check strcmp/strncmp)

	}

    exit(EXIT_SUCCESS);
}

