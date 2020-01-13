
//===========================================================================//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//===========================================================================//
void minus(const int num1, const int num2);
int if_digit(char *str);
//===========================================================================//
int main(int argc, char **argv)
{
	//if bad file
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
		exit(EXIT_FAILURE) ;
	}

	//check if valid
	int i;
	for(i=1;i<3;i++)
		if_digit(argv[i]);

	int num1 = atoi(argv[1]);
	int num2 = atoi(argv[2]);

	//decrease
	minus(num1,num2);


	return EXIT_SUCCESS;
}

//===========================================================================//
int if_digit(char *str)
{
	int i;
	for(i=0;str[i]!='\0';i++)
	{
		if(!isdigit(str[i]))
		{
			if(str[i] == '-')
				continue;
			puts("error");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}
//===========================================================================//
void minus(const int num1, const int num2)
{
	printf("%d",(num1 - num2));
}
//===========================================================================//

