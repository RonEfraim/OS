
//===========================================================================//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//===========================================================================//
void sum(char *str);
void check(char *str);
//===========================================================================//
int main(int argc, char **argv)
{
	//if bad file
	/*
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
		exit(EXIT_FAILURE) ;
	}
	*/

	//sums the string
	sum(argv[1]);


	return EXIT_SUCCESS;
}

//===========================================================================//
void sum(char *str)
{
	int temp;
	int result =0;

	char *token = NULL;

	token =  strtok(str, " ");

	//break and summ the string
	while(token != NULL)
	{
		//check if valid
		check(token);
		temp = atoi(token);
		result += temp;

		token = strtok(NULL, " ");
	}

	printf("%d\n", result);
}

//===========================================================================//
void check(char *str)
{
	int i;
	for(i=0;str[i]!='\0';i++)
	{
		if(!isdigit(str[i]))
		{
			if(str[i] == '-' || str[i] == '\n')
				continue;
			puts("error");
			exit(EXIT_FAILURE);
		}
	}
}

