
/* ============================================================================
 * Name:       ex7a1 - Server
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    RPC connection
 * ============================================================================
 */


//===========================================================================//

#include <ctype.h>
#include <stdio.h>
#include "/usr/include/rpc/rpc.h"
#include "ex7.h"
#define MAX_TEXT 20

//===========================================================================//

int *is_lowercase_1_svc(char **msg, struct svc_req *foo)
{
	unsigned int i = 0;
	static int result = 1;
	result = 1;

	while(msg[0][i] != '\0')
	{
		//check if lowercase
		if(!islower(msg[0][i]))
		{
			result = 0;
			break;
		}
		i++;
	}

	return(&result);
}

//===========================================================================//

double *digit_sum_1_svc(double *x, struct svc_req *foo)
{
	static double result = 0;
	result = 0;

	//mul the num till its whole
	while(*x != (int)*x)
		(*x) *= 10;

	int num = (int)*x;
	//sums digits
	while (num != 0)
	{
		int remainder = (num % 10);
		result += remainder;
		num /= 10;
	}

	return (&result);
}

//===========================================================================//

int *check_dup_1_svc(char **msg, struct svc_req *foo)
{
	static int result = 0;
	result = 0;
	unsigned int i = 0, j = 0;
	char str1[MAX_TEXT];
	char str2[MAX_TEXT];

	//insert part 1 to str1
	while(msg[0][i] != ' ')
	{
		str1[i] = msg[0][i];
		i++;
	}
	str1[i] = '\n';
	i++;

	//insert part 2 to str2
	while(msg[0][i] != '\0')
	{
		str2[j] = msg[0][i];
		i++;
		j++;
	}
	str2[j] = '\n';

	//check for same letters
	for(i=0;str1[i] != '\n';i++)
		for(j=0;str2[j] != '\n';j++)
			if(str1[i] == str2[j])
			{
				result++;
				break;
			}

	return(&result);
}

//===========================================================================//

