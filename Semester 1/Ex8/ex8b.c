
/* ============================================================================
 * Name:       ex8b
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    Find Vals In Matrix
 * ============================================================================
 * This program fills matrix in random values and creates 4 threads.
 * each process will run in infinite loop and will roll random value in the
 * range of 1-1000. the process will look for the value he rolled in the
 * fathers matrix and when he finds he will put 0 instead of the value. if all
 * the row will be filled with 0's the process will print it's number and will
 * kill all the other processes.
 *
 * Compile: gcc -pthread -o ex8b ex8b.c
 * Run: ex8b
 * Input: none
 * Output: the number of the process that found all the values in his row first
 * ============================================================================
 */

//===========================================================================//
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
//===========================================================================//
#define N 20
#define NUM_OF_THREADS 4
#define RANGE 1000
#define ARR_SIZE 10
//===========================================================================//
void get_data(int bingo[][N]);
void* my_func(void *args);
void kill_all();
//===========================================================================//
int arr[NUM_OF_THREADS];
int bingo[NUM_OF_THREADS][N];
pthread_t thread_data[NUM_OF_THREADS];
pthread_once_t threads_init = PTHREAD_ONCE_INIT;
//===========================================================================//
int main()
{
	int status;
	srand(17);

	//insert rand vals
	get_data(bingo);

	unsigned int i;
	for(i=0;i<NUM_OF_THREADS;i++)
	{
		//create process
		arr[i] = i;
		if((status = pthread_create(&thread_data[i],NULL,my_func,&bingo[i][0])) != 0)
		{
			fputs("pthread_create() failed", stderr);
			exit(EXIT_FAILURE);
		}
	}

	pthread_exit(NULL);
}

//===========================================================================//
void* my_func(void *args)
{
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

	int *row = (int*) args;
	unsigned int i;
	bool zeroes = true;

	while(1)
	{
		int roll = (rand() % RANGE)+1;
		for(i=0;i<N;i++)	//check if num is in mat
		{
			if(row[i] == roll)
			{
				row[i] = 0;
				for(i=0;i<N;i++)	//check if arr has all zeroes
				{
					if(row[i] != 0)
					{
						zeroes = false;
						break;
					}
				}
				if(zeroes)	//if row is all zeroes
				{
					//the child the finished kill the rest
					pthread_once(&threads_init, kill_all);
					pthread_exit(NULL);
				}
			}
			zeroes = true;
		}
	}
}
//===========================================================================//
void kill_all()
{
	int i, me;
	for(i=0;i<NUM_OF_THREADS;i++)
	{
		if(thread_data[i] == pthread_self())
		{
			me = i;
			continue;
		}
		pthread_cancel(thread_data[i]);		//kill rest of children
	}
	printf("%d \n", me);	//print child num
}
//===========================================================================//
void get_data(int bingo[][N])
{
	//insert rand vals
	unsigned int i,j;
	for(i=0;i<NUM_OF_THREADS;i++)
		for(j=0;j<N;j++)
			bingo[i][j] = (rand() % RANGE)+1;
}
