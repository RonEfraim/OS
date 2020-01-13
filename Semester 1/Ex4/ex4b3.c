
/* ============================================================================
 * Name:       ex4b3 - Client
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    GCD and Primes servers
 * ============================================================================
 */

//===========================================================================//
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
//===========================================================================//
#define MAX_TEXT 100
//===========================================================================//

int main(int argc, char *argv[])
{
	int num1, num2;

	//open to write
	FILE *fdw = NULL;
	fdw = fopen("my_fifo1", "w");
	FILE *fdr = NULL;
	fdr = fopen("my_fifo2", "r");

	//if fopen failed
	if(!fdw) {
		perror("fopen()w failed");
		exit(EXIT_FAILURE); }

	if(!fdr) {
		perror("fopen()r failed");
		exit(EXIT_FAILURE); }

	//if GCD
	if(strcmp(argv[1], "0") == 0)
	{
		num1 = atoi(argv[2]);
		num2 = atoi(argv[3]);
		fprintf(fdw, "%d %d ", num1,num2);
	}

	//if primes
	else if(strcmp(argv[1], "1") == 0)
	{
		num1 = atoi(argv[2]);
		fprintf(fdw, "%d ", num1);
	}

	else
		return EXIT_FAILURE;

	fflush(fdw);

	int status = fscanf(fdr, "%d", &num1);

	while(status != EOF)
	{
		printf("%d\n", num1);
		status = fscanf(fdr, "%d", &num1);
	}


	fclose(fdw);
	fclose(fdr);

	return EXIT_SUCCESS;
}
