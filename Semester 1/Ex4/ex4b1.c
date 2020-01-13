
/* ============================================================================
 * Name:       ex4b1 - Server (gcd)
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    GCD and Primes servers
 * ============================================================================
 */

//GCD- server
//============================================================================//
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
//===========================================================================//
int get_gcd(const int n1, const int n2);
void catch_sig_int(int);
//===========================================================================//

int main()
{
	signal(SIGINT, catch_sig_int);

	int num1, num2, num3;

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

	int status = fscanf(fdr, "%d %d", &num1, &num2);

	while(status != EOF)
	{
		num3 = get_gcd(num1,num2);
		fprintf(fdw, "%d ", num3);
		fflush(fdw);
		sleep(1);
		status = fscanf(fdr, "%d %d ", &num1, &num2);
	}

	fclose(fdw);
	fclose(fdr);

	return EXIT_SUCCESS;
}

//===========================================================================//
int get_gcd(const int n1, const int n2)
{
	//calculate GCD
	unsigned int i, gcd;

	for(i=1; i <= n1 && i <= n2; ++i)
	{
		if(n1%i==0 && n2%i==0)
			gcd = i;
	}

	return gcd;
}

//===========================================================================//
void catch_sig_int(int sig_num)
{
	//close fifo
	system("rm my_fifo");
	puts("fifo removed");
	exit(EXIT_SUCCESS);
}
