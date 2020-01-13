
/* ============================================================================
 * Name:       ex8a
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    Sort With Threads
 * ============================================================================
 * This program creates 5 processes.
 * each process will allocate an array with 10 cells and insert random values in
 * range of 1-1000 and will sort the array with bubble sort and make it global
 * with pthread_setspecific() function.
 * the main process will get all of the 5 sorted arrays and will merge them
 * into 1 sorted array and will print it.
 *
 * Compile: gcc -pthread -o ex8a ex8a.c
 * Run: ex8a
 * Input: none
 * Output: all of the numbers rolled sorted
 * ============================================================================
 */

//===========================================================================//
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
//===========================================================================//
#define PROCESSES 5
#define RANGE 1001
#define ARR_SIZE 10
//===========================================================================//
void* my_func();
void init_key();
void bbl_sort();
void merge(int arr[], int left_index, int mid_index, int right_index);
void print_arr(const int stat_arr[]);
void swap(int *xp, int *yp);
//===========================================================================//
int *arr;
pthread_key_t key;
pthread_once_t threads_init = PTHREAD_ONCE_INIT;
//===========================================================================//
int main()
{
	pthread_t thread_data[PROCESSES];
	int status,*ret,j;
	srand(17);

	unsigned int i;
	for(i=0;i<PROCESSES;i++) //run from 0 or 1????????????????????????
	{
		//create process
		if((status = pthread_create(&thread_data[i],NULL,my_func,NULL)) != 0)
		{
			perror("pthread_create() failed");
			exit(EXIT_FAILURE);
		}
	}

	//get sorted arrays from threads
	int stat_arr[PROCESSES * ARR_SIZE];
	unsigned int index = 0;
	for(i=0;i<PROCESSES;i++)
	{
		pthread_join(thread_data[i],(void**)&ret);
		for(j=0;j<ARR_SIZE;j++)
		{
			//stat_arr[index] = *(ret + j);
			stat_arr[index] = ret[j];
			index++;
		}

		free(arr);	//free alloc
	}

	unsigned int inc = 0;
	for (i=0;i<PROCESSES-1;i++)
	{
		merge(stat_arr,0,ARR_SIZE-1+inc,ARR_SIZE-1+ARR_SIZE+inc);
		inc+=ARR_SIZE;
	}

	//print result
	print_arr(stat_arr);



	return EXIT_SUCCESS;
}

//===========================================================================//
void* my_func()
{
	int rc;
	//alloc arr
	int *arr = (int*) malloc (ARR_SIZE * sizeof(int));
	if(arr == NULL) {
		perror("Malloc failed()");
		exit(EXIT_FAILURE); }

	//insert rand vals
	unsigned int i;
	for(i=0;i<ARR_SIZE;i++)
		arr[i] = rand() % RANGE;

	//get key
	pthread_once(&threads_init, init_key);

	//make arr global
	if((rc = pthread_setspecific(key,arr))) {
		fputs("Pthread_setspecific() failed", stderr);
		exit(EXIT_FAILURE); }

	bbl_sort();


	return arr;


	pthread_exit(NULL); //NEED????????????????
}
//===========================================================================//
void init_key()
{
	//get key
	int rc;
	rc = pthread_key_create(&key,NULL);
	if(rc)
	{
		fputs("Pthread_key_create() failed", stderr);
		exit(EXIT_FAILURE);
	}
}
//===========================================================================//
void bbl_sort()
{
	//get the global arr
	int *arr = (int*) pthread_getspecific(key);

	//sort with bubble sort
	int i, j;
	for (i = 0; i < ARR_SIZE-1; i++)
		for (j = 0; j < ARR_SIZE-i-1; j++)
			if (arr[j] > arr[j+1])
				swap(&arr[j], &arr[j+1]);
}
//===========================================================================//
void swap(int *xp, int *yp)
{
	//swap for bubble sort
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}
//===========================================================================//
void print_arr(const int stat_arr[])
{
	//print sorted arr
	unsigned int i;
	for(i=0;i<PROCESSES * ARR_SIZE;i++)
		printf("%d ", stat_arr[i]);
	puts("");
}
//===========================================================================//
void merge(int arr[], int left_index, int mid_index, int right_index)
{
	//merge the sorted arrs
	int i;
	int j;
	int k;
	int n1 = mid_index - left_index + 1;
	int n2 = right_index - mid_index;

	int left_arr[n1];
	int right_arr[n2];

	for (i = 0; i < n1; i++)
		left_arr[i] = arr[left_index + i];
	for (j = 0; j < n2; j++)
		right_arr[j] = arr[mid_index + 1 + j];

	i = 0;
	j = 0;
	k = left_index;
	while (i < n1 && j < n2) {
		if (left_arr[i] <= right_arr[j]) {
			arr[k] = left_arr[i];
			i++;
		} else {
			arr[k] = right_arr[j];
			j++;
		}
		k++;
	}

	while (i < n1) {
		arr[k] = left_arr[i];
		i++;
		k++;
	}

	while (j < n2) {
		arr[k] = right_arr[j];
		j++;
		k++;
	}
}
