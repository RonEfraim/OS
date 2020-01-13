
/* ============================================================================
 * Name:       ex7a2 - Client
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    RPC connection
 * ============================================================================
 */


//===========================================================================//
#include <stdio.h>
#include "/usr/include/rpc/rpc.h"
#include "ex7.h"
//===========================================================================//
void connect_clnt(char *server);
void check_null(int *result, char *server);
//===========================================================================//
#define MAX_TEXT 20
CLIENT *cl;
//===========================================================================//

int main(int argc, char **argv)
{
	double *result_sum;
	int *result_dup;
	int *result_lc;
	char *server;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <host> <two ints in a st5ring>\n",argv[0]);
		exit(EXIT_FAILURE); }

	//connect to client
	server = argv[1];
	connect_clnt(server);

	int choose;
	while(1)
	{
		puts("Please enter your choice");
		scanf("%d", &choose);
		switch(choose)
		{
		case 1:
		{
			puts("Please enter a string");
			char str[MAX_TEXT];
			scanf("%s", str);
			//check if lowercase
			char *msg = &str[0];
			result_lc = is_lowercase_1(&msg,cl);
			check_null(result_lc,server);

			*result_lc == 1 ? printf("String is lowercase\n\n") :
			printf("String is not lowercase\n\n");
			break;
		}
		case 2:
		{
			puts("Please enter a rational number");
			double num;
			scanf("%lf", &num);
			//sums the digits
			result_sum = digit_sum_1(&num,cl);
			check_null((int*)result_sum,server);

			printf("Result : %d\n", (int)*result_sum);
			break;
		}
		case 3:
		{
			puts("Please enter two string devided by a space");
			char str1[MAX_TEXT];
			char str2[MAX_TEXT];

			//connect strings
			scanf("%s", str1);
			strcat(str1," ");
			scanf("%s", str2);
			strcat(str1,str2);
			char *msg = &str1[0];
			//check for same letters
			result_dup = check_dup_1(&msg,cl);
			check_null(result_dup,server);

			printf("Number of same letters is : %d\n", (int)*result_dup);
			break;
		}

		case 0:
		{
			puts("0 was entered, bye bye");
			exit(EXIT_SUCCESS);
		}

		default:
		{
			puts("Wrong input");
			exit(EXIT_FAILURE);
		}

		}
	}
	//remove client and exit
	clnt_destroy(cl);
	exit(EXIT_SUCCESS);
}

//===========================================================================//

void connect_clnt(char *server)
{
	cl = clnt_create(server, RPC_PROG, CURR_VER, "tcp");
	if (cl == NULL)
	{
		clnt_pcreateerror(server);
		exit(EXIT_FAILURE);
	}
}

//===========================================================================//

void check_null(int *result, char *server)
{
	if(result == NULL)
	{
		clnt_perror(cl,server);
		exit(EXIT_FAILURE);
	}
}
