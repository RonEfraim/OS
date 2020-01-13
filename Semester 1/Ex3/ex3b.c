
/* ============================================================================
 * Name:       ex3b
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    Pipe Sorted Array
 * ============================================================================
 *
 * This program creates 5 children, each will sort a fifth of a 500 cells array
 * and the children will send the sorted parts of the array to dad threw a pipe
 *
 * Compile: gcc -Wall ex3b.c -0 ex3b
 * Run: ex3b
 * Input: none
 * Output: minimum and maximum numbers of the sorted array (first and last cell)
 * ============================================================================
 */

//============================================================================
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
//===========================================================================//
#define CHILDREN 5
#define RANGE 1001
#define CELLS 500
int pipe_descs[2];
pid_t status;
//===========================================================================//
void do_child(const unsigned i, int arr[]);
void quicksort(int arr[], int from, int to);
int partition(int arr[], int from, int to);
void swap(int *num1, int *num2);
void do_dad(int arr[]);
void fill_arr(int arr[]);
void merge(int arr[], int l, int m, int r);
void print_min_max(const int arr[]);
//===========================================================================//

int main()
{
	int arr[CELLS];
	unsigned int i;
	srand(17);

	fill_arr(arr);							//fill array with rands

	if(pipe(pipe_descs) == -1)				//open pipe
	{
		perror("cannot open pipe");
		exit(EXIT_FAILURE);
	}

	for(i=0;i<CHILDREN;i++)
	{
		pid_t status = fork();				//make child
		if(status < 0)
		{
			puts("canno't fork");
			exit(EXIT_FAILURE);
		}
		else if(status == 0)
		{
			do_child(i,arr);				//child func
		}
	}

	for(i=0;i<CHILDREN;i++)
		wait(NULL);							//release children

	do_dad(arr);

	close(pipe_descs[0]);					//close before ending program
	wait(NULL);

	return EXIT_SUCCESS;
}

//===========================================================================//
void fill_arr(int arr[])
{
	unsigned int i;
	for(i=0;i<CELLS;i++)
		arr[i] = (rand() % RANGE);			//fill arr with rands
}
//===========================================================================//
void do_child(const unsigned i, int arr[])
{
	close(pipe_descs[0]);					//close pipe inp
	unsigned int start = (i*100);
	unsigned int end = ((i+1)*100);
	quicksort(arr,start,end);				//sort with qs

	//write to pipe
	if((write(pipe_descs[1],&arr[start],sizeof(int)*100)) == -1)
	{
		perror("cannot write()");
		exit(EXIT_FAILURE);
	}

	close(pipe_descs[1]);					//close pipe outp
	exit(EXIT_SUCCESS);
}

//===========================================================================//
void quicksort(int arr[], int from, int to) {
    int pivot_place = partition(arr, from, to);

    if (from < pivot_place - 1)
        quicksort(arr, from, pivot_place - 1);

    if (to > pivot_place + 1)
        quicksort(arr, pivot_place + 1, to);
}
int partition(int arr[], int from, int to) {
    int pivot_place = from;
    int i;

    for (i = from + 1; i <= to; i++)
       if (arr[i] < arr[pivot_place]) {
         swap(&arr[i], &arr[pivot_place + 1]);
         swap(&arr[pivot_place + 1], &arr[pivot_place]);
         pivot_place++;
       }
    return pivot_place;
}
void swap(int *num1, int *num2) {
	int temp = *num1;
	*num1 = *num2;
	*num2 = temp;
}

//===========================================================================//
void do_dad(int arr[])
{
	int i;

	close(pipe_descs[1]);
	if((read(pipe_descs[0],arr,sizeof(int)*500)) == -1) //read from pipe
	{
		perror("cannot read()");
		exit(EXIT_FAILURE);
	}

	for(i=0;i<CHILDREN;i++)
	{
		unsigned int start = 0;
		unsigned int middle = ((i+1)*100)-1;
		unsigned int end = (middle+100);
		merge(arr,start,middle,end);			//sort with ms

	}

	close(pipe_descs[0]);						//close pipe inp
	print_min_max(arr);
}
//===========================================================================//
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

//===========================================================================//
void print_min_max(const int arr[])
{
	if(arr[0]<0 || arr[1]<0)
		printf("min num = %d\n", arr[3]);
	else
		printf("min num = %d\n", arr[0]);
	printf("max num = %d\n", arr[CELLS-1]);
}

