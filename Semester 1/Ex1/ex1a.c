
/* ============================================================================
 * Name:       ex1a
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    Towers Of Hanoi
 * ============================================================================
 *
 * This program creates 5 children, that each does the Towers Of Hanoi
 * algorithm. child number i=0 does the algorithm with 10 rings, child num i=1
 * does the algorithm with 20 rings, i=2 with 25, i=3 with 30 and i=4 with 35
 * rings.
 * the program will pring the time took each child to get the job done.
 *
 * Compile: gc -Wall ex1a.c -0 ex1a
 * Run: ex1a
 * Input: none
 * Output: the time too each child to do the Hanoi job.
 * ============================================================================
 */

//===========================================================================//
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <wait.h>
//===========================================================================//
const unsigned int SIZE = 5; //num of children
//===========================================================================//
void do_child(FILE* output, unsigned int rings);
void hanoi(FILE* output, unsigned int rings, char a, char b, char c);
//===========================================================================//
int main(int argc, char **argv)
{
	FILE* output;
	output = fopen("/dev/null", "w");

	unsigned int i;

	for(i=0; i < SIZE; i++)
	{
		//make a kil and put his id into status
		pid_t status = fork();

		//if fork failed
		if(status < 0)
		{
			fputs("canno't fork", stderr);
			exit(EXIT_FAILURE);
		}

		//if is child, send him to do Hanoi with
		//the num of rings required
		 if(status == 0)
		{
			switch(i)
			{
			case 0:
				do_child(output, 10);
				break;
			case 1:
				do_child(output, 20);
				break;
			case 2:
				do_child(output, 25);
				break;
			case 3: 
				do_child(output, 30);
				break;
			case 4:
				do_child(output, 35);
				break;
			default:
				return EXIT_SUCCESS;
			}

		}

		//if parent
		 else
		{
			return EXIT_SUCCESS;
		}
	}

	//for(i = 0; i < SIZE; i++)
		//son_id = wait(&status);

	return EXIT_SUCCESS;
}

//===========================================================================//
void do_child(FILE* output, unsigned int rings)
{
	printf("%d = ",rings); //num of rings
	time_t start = time(NULL); //start time count
	hanoi(output, rings, 'a', 'b' ,'c'); //towers of hanoi func
	time_t x = (time(NULL) - start); //x is the time took to do job
	printf("%ld\n", x); //print time
	return;
}

//===========================================================================//
void hanoi(FILE* output, unsigned int rings, char a, char b, char c)
{
	//stop condition
	if(rings == 1)
	{
		fprintf(output, "move ring of size 1 from %c to %c", a, c);
		return;
	}

	else
	{
		hanoi(output, rings - 1, a, b, c);
		fprintf(output, "move ring of size %d from %c to %c", rings, a, c);
		hanoi(output, rings - 1, b, c, a);
	}
}

