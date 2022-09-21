# Dash Requirements

* Dash Shell has two Modes:
	1. Interactive
		Prompt - "dash> "
		It allows the user to type commands directly.


	2. Batch
		prompt: > ./dash batch.txt
		Reads input from a batch file and executes commands from therein.
		
* Dash shell creates a new process for every command, except the built-in commands - cd, exit and path - which are implemented internally.

* Shell should support standard output redirection:
	ls -la /tmp > output
	
		- Here the standard output of ls program should be redirected to the file output. 
		- The standard error should also be rerouted to the file output.
		- If output file exists before running the program, overwrite it.
		- The exact format of redirection is a command (and possibly some arguments) followed by the redirection symbol followed by a filename.
		- Multiple redirection operators or multiple files to the right of the redirection sign are errors.
		- After extracting each command, we need to look for ">"

* User can launch parallel Commands - &	

* Print only one error message:
	
	char error_message[30] = "An error has occurred\n";
	
	write(STDERR_FILENO, error_message, strlen(error_message));


- parse the input
	- for batch mode, need to read file line by line

- Extract the tokens
	1. Check for parallel commands (&)
	

For every command	
- Extract the tokens with space delimiter
	- store in array
- check whether the command is a built-in command or not.
	- implement exit, cd(using chdir()) and path.
	- invoke the implementation

- if it is not a built-in command, 	
	- scan all the paths and check using access().
	- if it is present in the particular path
		- fork
		- In the child process
			
			-If user requested for redirection
				- close std out file desc
				- open file with name given by user (eg "output")
				- execv(path, arg)
			- if no redirection requested
				- execv(path, arg)
	- if not, it is an error

create child process (fork + exec)
    - parent waits for child to finish (wait())
When process is done prompt for next command

Repeat above till user types exit




