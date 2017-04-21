// File: TwoPipesTwoChildren.cpp
// Author: Ryan Austin Glaser
// Date: 03/30/2017
// Purpose: CS3376
// Description: Program to execute "ls -ltr | grep 3376 | wc -l" by dividing the three commands amongst two children and one parent process.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main (int argc, char **argv)
{
int status; 
char *cat_args[] = {"ls", "-ltr", NULL};
char *grep_args[] = {"grep", "3376", NULL};
char *wc_args[] = {"wc", "-l", NULL};

pid_t FirstChild, SecondChild;

int Pipe_1[2], Pipe_2[2]; // create two pipes one for the ls to grep and the second for the grep to wc commands

pipe(Pipe_1);
pipe(Pipe_2);

// First child

if ((FirstChild = fork()) == -1)
	{
	perror("Error creating the first child");
	exit(1);
	}
if (FirstChild > 0) //are we in parent? if yes fork second child
	{
	if ((SecondChild = fork()) == -1)
		{
		perror("Error creating the second child");
		exit(1);
		}
	}
if (FirstChild == 0) // first child stuff
	{
	// redirect stdout to Pipe_1 write end
	dup2(Pipe_1[1], 1);
	// close the pipes
	close(Pipe_1[0]);
	close(Pipe_1[1]);
	execvp(*cat_args, cat_args);
	exit (0);
	}
	else  if (SecondChild == 0) // second child stuff
		{
		//redirect stdin to Pipe_1 read end
		dup2(Pipe_1[0],0);
		// close the pipes
		close(Pipe_1[1]);
		close(Pipe_1[0]);
		close(Pipe_2[0]);
		//redirect stdout to Pipe_2 write end
		dup2(Pipe_2[1],1);
		close(Pipe_2[2]);
		execvp(*grep_args, grep_args);
		}
		else // Parent
			{
			//redirect stdin to Pipe_2 read end
			dup2(Pipe_2[0],0);
			close(Pipe_1[1]);
			close(Pipe_1[0]);
			close(Pipe_2[1]);
			close(Pipe_2[0]);
			execvp(*wc_args, wc_args);
			}
return (0);
}

