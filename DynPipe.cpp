// file: dynpipe.cpp
// author: Ryan Austin Glaser
// Date: 04/04/2017
// purpose: cs3376
// description:
//Program to make turn ou:r TwoPipesThreeChildren into a dynamic pipe
//making it to where the user can enter in up to 5 commands to have piped 
//and executed
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <string.h>
using namespace std;

void CommandParse(char *string, char **array); //function to do the work of strtok and tokenize the user input from the command line
int SizeOfArray (char *string);

int main(int argc, char **argv)
{
//decrement argc to exclude the program name
argc--;
//increment the argv array so that it is on the first command
argv++;
int status;
int NumOfArgs = argc;
pid_t Children[NumOfArgs - 1];
int Pipes[2*(NumOfArgs-1)];
int count;
if (argc < 2) // making sure we 
	{
	perror("not enough arguments we can't pipe anything");
	exit(1);
	}
//for loop to create all the pipes we need which should be num of commands arugements minus 1
for (int i = 0; i < NumOfArgs - 1; i++)
	{
	pipe(Pipes + i*2);
	}
// for  loop to fork children for as many arugments as the user typed to command line minus 1 because the parent will do the last command
for(count = 0; count < NumOfArgs - 1; count++)
	{
	if ((Children[count] = fork()) == -1)
		{
		perror("error forking a child process");
		exit(1);
		}
	if (Children[count] == 0) // we are inside child
		{
		char *Array[SizeOfArray(argv[count])]; // create an array to put the parsed argv into
		CommandParse(argv[count], Array); // parse and tokenize the argv array
		for(int j = 0; j < NumOfArgs - 1; j++) // close pipes
			{
			if(j == count)
				continue;
			else if (count != 0 && j == count - 1)
				continue;
			else
				{
				close(Pipes[j*2]);
				close(Pipes[j*2 +1]);
				}
			}
		 // open pipes for reading		
		if (count != 0)
			dup2(Pipes[(count-1)*2], 0);
			
		dup2(Pipes[count*2 +1], 1); // open pipes for writing
		close(Pipes[count*2]); //close pipe used for reading
		close(Pipes[count*2 +1]); // close the pipe used for writing
		if(count != 0)
			{
			close(Pipes[(count - 1)*2]);//close previous pipe used for reading
			close(Pipes[(count - 1)*2 +1]);//close previous pipe used for writing
			}
		// now we can execute the commands given to whatever child we are currently on
		execvp(*Array, Array); // execute the parsed Array command
		exit(0); //exit out of that child process
		}
	// we are now in the parent so we will increment the count and as long as the count is still less than the NumOfArgs and if it is less than we will
	// be creating another child to have execute some more commands
	}
char *Array[SizeOfArray(argv[count])]; //create another array and parse it for the parent to execute the last command
CommandParse(argv[count], Array);
//close the pipes except for the last one that is going to send the last command to parent
for (int k = 0; k < NumOfArgs - 2; k++) 
	{
	close(Pipes[k*2]); //closing read ends
	close(Pipes[k*2 + 1]); //closing write ends
	}

// open read end of pipe for parent to execute command
dup2(Pipes[(count-1)*2], 0);
// close the last pipe
close(Pipes[(count-1)*2]);
close(Pipes[(count-1)*2 +1]);
//finally execute the last command
execvp(*Array, Array);
return (0);
}

void CommandParse(char *string, char **array)
{
char *string_2 = string;
char *Token = strtok(string_2, " ");
int i = 0;
array[i++] = Token;
while (Token != NULL)
	{
	Token = strtok(NULL, " ");
	array[i++] = Token;
	}
}

int SizeOfArray(char *string)
{
int Spaces = 0;
int i;
for (i = 0; string[i] != '\0'; i++)
	{
	if (string[i] == ' ')
		{
		Spaces++;
		}
	}
	Spaces += 2;
	return Spaces;
}
