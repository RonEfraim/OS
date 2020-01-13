
/* ============================================================================
 * Name:       ex2a
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    Signals
 * ============================================================================
 *
 * This program creates a child and runs 10 times in a loop. the signals that
 * are set are- SIGUSR1, SIGTERM.
 * in the loop-
 * sleep for a random number of seconds (0-3)
 * sends to the other SIGUSR1 signal
 * counts the signals sent
 * in SIGUSR1-
 * prints "process <pid> got signal SIGUSR1"
 * if signals received are bigger or equal to signals sent times 2, prints
 * "process <pid> surrender <signals received> <signals sent>"
 * sends SIGTERM signal
 * in SIGTERM-
 * prints "process <pid> win <signals received> <signals sent>"
 *
 * Compile: gcc -Wall ex2a.c -0 ex2a
 * Run: ex2a
 * Input: none
 * Output: (a few) possible race conditions:
 * 1) father will roll 0 10 times and will not sleep and the child will not get
 * the processor and father will win.
 * 2) one process surrender and before doing the kill function give the
 * processor and send another signal and the other process will win
 *
 * ============================================================================
 */

//============================================================================
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <time.h>
//===========================================================================//
const unsigned int MIN_SIG_SENT = 2;
const unsigned int SECONDS = 4;
const unsigned int SIZE = 10;
unsigned int sig_sent = 0;
unsigned int sig_get = 0;
pid_t send_to;
pid_t status;
//===========================================================================//
void catch_sig_term(int);
void catch_sig_usr(int);
//===========================================================================//
int main()
{
	unsigned int i;

	srand (time(NULL));
	signal (SIGUSR1, catch_sig_usr);
	signal (SIGTERM, catch_sig_term);


	pid_t status = fork();						//make a child

	if(status < 0)								//if fork failed
	{
		puts("canno't fork");
		exit(EXIT_FAILURE);
	}

	if(status == 0)								//if child save fathers pid
		send_to = getppid();

	else										//if father save sons pid
		send_to = status;


	for(i=0;i<SIZE;i++)
	{
		unsigned int time_rand = (rand() % SECONDS);
		sleep(time_rand);
		sig_sent++;
		kill(send_to, SIGUSR1);
	}

	wait(NULL);									//free child (if needed)


	return EXIT_SUCCESS;
}

//===========================================================================//
void catch_sig_term(int sig_num)
{
	signal (SIGTERM, catch_sig_term);			//reload signal

	printf("process %d win %d %d \n", getpid(),sig_sent,sig_get);
	exit(EXIT_SUCCESS);
}

//===========================================================================//
void catch_sig_usr(int sig_num)
{
	signal (SIGUSR1, catch_sig_usr);			//reload signal

	printf("process %d got signal SIGUSR1 \n", getpid());
	sig_get++;

	if(sig_sent >= MIN_SIG_SENT && sig_get >= (sig_sent * 2))
	{
		printf("process %d surrender %d %d \n", getpid(),sig_sent,sig_get);
		kill(send_to, SIGTERM);
		exit(EXIT_SUCCESS);
	}
}

