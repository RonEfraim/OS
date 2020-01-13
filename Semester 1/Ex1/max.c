//===========================================================================//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//===========================================================================//
int max(int arr[], int temp);
void check(char *str);
//===========================================================================//
int main(int argc, char **argv)
{
	int i;
	int arr[argc-1];

	for(i=0;i<argc-1;i++)
	{
		//check if valid
		check(argv[i+1]);
		//insert word into array
		arr[i] = atoi (argv[i+1]);
	}

	//find max
	int maximum = max(arr, argc);
	printf("%d", maximum);


	return EXIT_SUCCESS;
}

//===========================================================================//
int max(int arr[], int temp)
{
	//set max
	int i, t_max=arr[0];
	for(i=0;i<temp-2;i++)
	{
		if(arr[i+1] > t_max)
			t_max = arr[i+1];
	}

	return t_max;
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
			puts("MAX error");
			exit(EXIT_FAILURE);
		}
	}
}

