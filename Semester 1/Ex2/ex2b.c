
/* ============================================================================
 * Name:       ex2b
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    Signals
 * ============================================================================
 *
 * This program creates a child and runs in an infinite loop.
 * in the loop-
 * draws a random number (0-6)
 * if number = 0- prints "process <pid> ends" and quits.
 * if number is in range 1-3- waits 5 seconds to get a signal.
 * if a signal came- prints "process <pid> had a partner" and continues.
 * else- prints "process <pid> was left alone, and quits" and quits.
 * if he got 10 signals- prints "process <pid> surrender" and quits.
 *
 * Compile: gcc -Wall ex2b.c -0 ex2b
 * Run: ex2b
 * Input: none
 * Output: possible race conditions:
 * 1) father will get to pause and son release him and same for when the son
 * gets to pause and father release him, and both will get 10 signals and
 * both surrender.
 * 2) one process will surrender and the other and rolls 0-4 and go to sigusr1
 * and will print that the process has a partner
 *
 * ============================================================================
 */

//============================================================================
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <wait.h>
//===========================================================================//
const unsigned int SIG_RANGE = 10;
const unsigned int SECONDS = 5;
const unsigned int RANGE = 7;
unsigned int sig_count = 0;
pid_t send_to;
//===========================================================================//
void catch_sig_alarm(int);
void catch_sig_usr(int);
void main_func();
//===========================================================================//
int main()
{
	signal (SIGALRM, catch_sig_alarm);
	signal (SIGUSR1, catch_sig_usr);

	main_func();

	return EXIT_SUCCESS;
}

//===========================================================================//
void main_func()
{
	pid_t status = fork();				//make a child

	if(status < 0)						//if fork failed
	{
		puts("canno't fork");
		exit(EXIT_FAILURE);
	}

	if(status == 0)						//if child
		send_to = getppid();

	else								//if father
		send_to = status;


	while(1)							//endless loop
	{
		unsigned int time_rand = (rand() % RANGE); //A
		if(time_rand == 0) //B
		{
			printf("process %d ends\n", getpid());
			break;
		}

		else if(time_rand > 0 && time_rand < 4) //C
		{
			kill(send_to, SIGUSR1);

			alarm(SECONDS); //D
			pause();
			alarm(0);
		}

		else if(sig_count >= SIG_RANGE) //F
		{
			printf("process %d surrender\n", getpid());
			break;
		}
	}

	wait(NULL);							//free child

	return;
}

//===========================================================================//
void catch_sig_usr(int sig_num)
{
	sig_count++;
	signal (SIGUSR1, catch_sig_usr);	//reload signal
	printf("process %d had a partner\n", getpid());
}

//===========================================================================//
void catch_sig_alarm(int sig_num)
{
	printf("process %d was left alone, and quits\n", getppid()); //G
	exit(EXIT_SUCCESS);
}

