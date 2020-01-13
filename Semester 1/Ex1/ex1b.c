
/* ============================================================================
 * Name:       ex1b
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    Calculator
 * ============================================================================
 *
 * This program creates 5 children, and the user enters one of the four
 * commands: plus- to add 2 nums, minus- to decrease 2 nums, sum- to add each
 * number of the string, max- to return the max num from the string.
 * program will convert the string to ints and do the action requested or type
 * exit to quit.
 *
 * Compile: gc -Wall ex1b.c -0 ex1b
 * Run: ex1b
 * Input: action name (plus, minus, sum, max) or exit, and string required for
 * that action.
 * Output: the result of the calculating action.
 * ============================================================================
 */

//===========================================================================//
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <wait.h>
#include <string.h>
//===========================================================================//
const unsigned int MAX_SIZE = 100;
//===========================================================================//
void do_child(const char* choice);
int f_plus();
int f_minus();
int f_sum();
int f_max();
//===========================================================================//
int main()
{
	char *choice;

	//get users choise of action
	scanf("%s", choice);

	while(choice != "exit")
	{
		//make child
		pid_t status = fork();

		if(status < 0)
		{
			puts("canno't fork");
			exit(EXIT_FAILURE);
		}

		//if child
		else if(status == 0)
		{
			do_child(choice);
			exit(EXIT_SUCCESS);
		}
		else
			wait(NULL);
		
		scanf("%s", choice);
	}

	return EXIT_SUCCESS;
}

//===========================================================================//
void do_child(const char* choice)
{
	if(strcmp(choice,"plus") == 0 )
		f_plus();

	else if(strcmp(choice , "minus") == 0)
		f_minus();

	else if(strcmp(choice , "sum") == 0)
		f_sum();

	else if(strcmp(choice , "max") == 0)
		f_max();

		return;
}

//===========================================================================//
int f_plus()
{
	char str1[MAX_SIZE],
		 str2[MAX_SIZE];

	scanf("%s %s", str1, str2);

	//check if file can be opened and send input to file
	if(execlp("./plus", "plus", str1, str2, NULL) != 0)
	{
		perror("error");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}
//===========================================================================//
int f_minus()
{
	char str1[MAX_SIZE],
		 str2[MAX_SIZE];

	scanf("%s %s", str1, str2);

	//check if file can be opened and send input to file
	if(execlp("./minus", "minus", str1, str2, NULL) != 0)
	{
		puts("error");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

//===========================================================================//
int f_sum()
{
	char str[MAX_SIZE];

	getchar();
	fgets(str, MAX_SIZE-1, stdin);
	//getchar();

	//scanf("%s", str);


	//check if file can be opened and send input to file
	if(execlp("./sum", "sum", str, NULL) != 0)
	{
		puts("error");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

//===========================================================================//
int f_max()
{
	char str[MAX_SIZE];
	char *token = NULL,
		 *args[MAX_SIZE];

	int i;

	//make first word file name
	getchar();
	fgets(str, MAX_SIZE, stdin);
	token =  strtok(str, " ");
	args[0] = "./max";

	//break down words
	for(i=1 ; token != NULL; i++)
	{
		args[i] = token;
		token = strtok(NULL, " ");
	}
	//make null terminated
	args[i]= NULL;

	//check if file can be opened and send input to file
	if(execvp(args[0], args) != 0)
	{
		perror("execvp() failed");
		exit(EXIT_FAILURE);
	}


	return EXIT_SUCCESS;
}
