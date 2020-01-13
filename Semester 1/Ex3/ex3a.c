
/* ============================================================================
 * Name:       ex3a
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    Pipe Towers Of Hanoi
 * ============================================================================
 *
 * This program creates 5 processes, each will do the towers of Hanoi with
 * 5,10,15,20,25 rings and will send the information threw the standard output
 * with a redirection from the pipe output and receive the information threw
 * the standard input.
 * the program will calculate the time took each kind of child to do the
 * algorithm and print the min max and average run time for each kind of child.
 *
 * Compile: gcc -Wall ex3a.c -0 ex3a
 * Run: ex3a
 * Input: none
 * Output: minimum, maximum and average run time of the towers of Hanoi job for
 * each number of rings required.
 * ============================================================================
 */

//===========================================================================//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <wait.h>
#include <time.h>
//===========================================================================//
#define SIZE 5
int pipe_descs[2];
pid_t status;
//===========================================================================//
void do_child(int rings);
void do_dad();
void hanoi(int rings, char a, char b, char c);
//===========================================================================//
int main()
{

	int i, j;
	int arr[SIZE] = {5,10,15,20,25};

	if(pipe(pipe_descs) == -1)
	{
		perror("cannot open pipe");
		exit(EXIT_FAILURE);
	}

	for(i=0; i < SIZE; i++)
	{
		for(j=0;j<SIZE;j++)
		{
			pid_t status = fork();
			if(status < 0)
			{
				fputs("canno't fork", stderr);
				exit(EXIT_FAILURE);
			}

			if(status == 0)
			{
				do_child(arr[j]);
				return EXIT_SUCCESS;
			}
		}
	}

	for(i=0;i<SIZE*SIZE;i++)
		wait(NULL);

	do_dad();

	return EXIT_SUCCESS;
}

//===========================================================================//
void do_child(int rings)
{
	close(pipe_descs[0]);
	time_t start = time(NULL);
	hanoi(rings, 'a', 'b' ,'c');
	time_t t = (time(NULL) - start);
	int x = t;

	dup2(pipe_descs[1],STDOUT_FILENO);
	printf("%d %d ", rings, x);


	return;
}

//===========================================================================//
void hanoi(int rings, char a, char b, char c)
{
	//stop condition
	if(rings == 1)
		return;

	else
	{
		hanoi(rings - 1, a, b, c);
		hanoi(rings - 1, b, c, a);
	}
}

//===========================================================================//
void do_dad()
{
	close(pipe_descs[1]);
	int rings, time, i;
	int min5=INT_MAX, min10=INT_MAX, min15=INT_MAX,
			min20=INT_MAX, min25=INT_MAX;
	int max5=0, max10=0, max15=0,max20=0, max25=0;
	int sum5=0, sum10=0, sum15=0, sum20=0, sum25=0;
	dup2(pipe_descs[0],STDIN_FILENO);


	for(i=0;i<SIZE*SIZE;i++)
	{
		scanf("%d %d", &rings, &time);
		switch(rings)
		{
		case 5:
			sum5+=time;
			if(time <= min5)
				min5 = time;
			if(time >= max5)
				max5 = time;
			break;
		case 10:
			sum10+=time;
			if(time <= min10)
				min10 = time;
			if(time >= max10)
				max10 = time;
			break;
		case 15:
			sum15+=time;
			if(time <= min15)
				min15 = time;
			if(time >= max15)
				max15 = time;
			break;
		case 20:
			sum20+=time;
			if(time <= min20)
				min20 = time;
			if(time >= max20)
				max20 = time;
			break;
		case 25:
			sum25+=time;
			if(time <= min25)
				min25 = time;
			if(time >= max25)
				max25 = time;
			break;
		}
	}

	int rings_arr[] = {5,10,15,20,25};
	int sum_arr[] = {sum5,sum10,sum15,sum20,sum25};
	int min_arr[] = {min5,min10,min15,min20,min25};
	int max_arr[] = {max5,max10,max15,max20,max25};

	for(i=0;i<SIZE;i++)
	{
		printf("min:%d max:%d avr:%d ",
				min_arr[i],max_arr[i],(sum_arr[i]/SIZE));
		puts("");
	}

}

