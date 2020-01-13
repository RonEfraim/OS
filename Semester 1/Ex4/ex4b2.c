
/* ============================================================================
 * Name:       ex4b2 - Server (primes)
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    GCD and Primes servers
 * ============================================================================
 */

//PRIMES - server
//============================================================================
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
//===========================================================================//
#define MAX_NUMS 100
//===========================================================================//
void get_primes(int p_arr[], int num);
void catch_sig_int(int);
//===========================================================================//

int main()
{
	signal(SIGINT, catch_sig_int);
	int p_arr[MAX_NUMS];
	//make fifo
	if(mkfifo("my_fifo1", S_IFIFO | 0644) == -1) {
		perror("mkfifor() failed");
		exit(EXIT_FAILURE); }
	if(mkfifo("my_fifo2", S_IFIFO | 0644) == -1) {
		perror("mkfifow() failed");
		exit(EXIT_FAILURE); }

	//open for read
	FILE *fdr = NULL;
	fdr = fopen("my_fifo1", "r");
	FILE *fdw = NULL;
	fdw = fopen("my_fifo2", "w");

	if(!fdr) {
		perror("fopen()r failed");
		exit(EXIT_FAILURE); }
	if(!fdw) {
		perror("fopen()w failed");
		exit(EXIT_FAILURE); }

	int num1;

	int status = fscanf(fdr, "%d", &num1);
	int i;

	while(status != EOF)
	{
		get_primes(p_arr,num1);
		for(i =0; p_arr[i]!=-1; i++)
		{
			fprintf(fdw, "%d ", p_arr[i]);
			fflush(fdw);
		}

		status = fscanf(fdr, "%d", &num1);
	}

	fclose(fdw);
	fclose(fdr);

	return EXIT_SUCCESS;
}

//===========================================================================//
void get_primes(int p_arr[], int num)
{
	int i, j = 0;
	for (i=2; i<=num; i++)
	{
		while (num%i == 0)
		{
			p_arr[j] = i;
			j++;
			num/=i;
		}
	}
	p_arr[j] = -1;
}

//===========================================================================//
void catch_sig_int(int sig_num)
{
	//close fifo
	system("rm my_fifo");
	puts("fifo removed");
	exit(EXIT_SUCCESS);
}
