# 5348-project-1
Two Modes:
1. Interactive
Prompt - "dash> "
Type a command (ls)


2. Batch
prompt> ./dash batch.txt


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




