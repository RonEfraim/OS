

/* ============================================================================
 * Name:       ex9b
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    Threads Semaphore
 * ============================================================================
 * This program creates 3 semaphores, 2 children and 1 shared memory.
 * the parent starts the bingo game:
 * each child will create an array sized 10 and insert random values.
 * the parent also rolled a random value and send it to the children through
 * the shared memory.
 * when a child finishes covering the numbers he kills his brother and prints
 * his numbers and sends a signal to the parent and exits.
 * when the parent gets the signal he closes the semaphore and the shared
 * memory.
 *
 * possible race conditions:
 * 1) parents add 2 numbers and child #1 gets number #1 and child #2 gets number
 * 2 and so on until one wins (good condition).
 * 2) parent adds 2 items and child #1 get number #1 and child #1 tries to get
 * another number -> child #1 will continue to wait -> child #2 will starve
 * until a contact switch hopefully happens.
 *
 * Compile: gcc ex9b.c -o thread -ex9b
 * Run: ./ex9b
 * Input: none
 * Output: all of the numbers rolled sorted
 * ============================================================================
 */

//-------------------------------------------

#include <sys/types.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <sys/msg.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

//-------------------------------------------

#define NUM_OF_CHILD 2
#define MAX_RAND 100

//-------------------------------------------

int shm_id;
sem_t *sem;

//-------------------------------------------

void do_finish(int arr[], int child_kind);
int find_num(int num, int arr[]);
void catch_child1(int signum);
void catch_child2(int signum);
void catch_term(int signum);
void print_arr(int arr[]);
void do_random(int arr[]);
void close_sem();
void open_sem();
void do_child();

//-------------------------------------------

int main()
{
	signal(SIGUSR1, catch_child1);
	signal(SIGUSR2, catch_child2);
	signal(SIGTERM,catch_term);

	int* father_ptr;
	key_t key;
	pid_t pid;
	int i;

	srand(17);

	//open semaphore
	open_sem();
	//get key
	if ((key = ftok("./ex9b.c", 'b')) == -1) {
		perror("ftok failed");
		exit(EXIT_FAILURE); }

	//maked shared memory
	if ((shm_id = shmget(key, 16, IPC_CREAT | IPC_EXCL | 0600)) == -1) {
		perror("failed to get memory\n");
		exit(EXIT_FAILURE); }

	//gets pointer to shared memory
	if ((father_ptr = (int*)shmat(shm_id, NULL, 0)) < 0) {
		perror("failed to attach memory");
		exit(EXIT_FAILURE); }


	//make children
	for (i=0; i < NUM_OF_CHILD; i++)
	{
		pid = fork();
		if (pid == 0)
		{
			do_child(i);
			i++;
		}
		father_ptr[i] = pid;
	}

	//random func
	do_random(father_ptr);


	return EXIT_SUCCESS;
}

//--------------------------------------------------------

void do_random(int arr[])
{
	arr[2] = arr[3] = 0;

	while (1)
	{
		int num;
		if(arr[2]==0 && arr[3]==0)
		{

			num = (rand()% MAX_RAND) + 1;
			arr[2]=num;
			arr[3]=num;
		}
	}
}

//-------------------------------------------------------

void do_child(child_kind)
{
	int arr[10];
	int i,j;
	int *child_ptr;
	bool check_arr[10];
	int counter=0;

	for(i=0; i<10; i++)
		arr[i]= (rand()% MAX_RAND) + 1;

	//zero
	memset(&check_arr, 0, 10 * sizeof(bool));

	//get pointer to shm
	if ((child_ptr = (int*)shmat(shm_id, NULL, 0)) < 0) {
		perror("filed to attach memory");
		exit(EXIT_FAILURE); }


	while(1)
	{
		int num;
		num = child_ptr[child_kind+2];
		if(num != 0)
		{
			j = find_num(num, arr);

			if(j != -1 && check_arr[j] != true)
			{
				check_arr[j] = true;
				counter++;
			}
			if(counter == 10)
				do_finish(arr, child_kind);
		}

		child_ptr[child_kind+2] = 0;
	}
}

//------------------------------------------------------------

int find_num(int num, int arr[])
{
	int i;
	for(i=0; i<10; i++)
		if(arr[i] == num)
			return i;

	return -1;
}

//---------------------------------------------------------------

void do_finish(int arr[], int child_kind)
{
	sem_wait(sem);
	//-----------critical-----------//
	if(child_kind == 0) {
		kill(getppid(), SIGUSR1);
		print_arr(arr); }

	if(child_kind == 1) {
		kill(getppid(), SIGUSR2);
		print_arr(arr); }
	sem_post(sem);
	close_sem();

	exit(EXIT_SUCCESS);
}

//-----------------------------------------------------------------

void print_arr(int arr[])
{
	int i;
	for(i=0; i<10; i++)
		printf("%d ", arr[i]);
	puts("");
}

//-------------------------------------------------------------------

void catch_child1(int signum)
{
	int* catch_ptr;
	if ((catch_ptr = (int*)shmat(shm_id, NULL, 0)) < 0)
	{
		perror("failed to attach memory");
		exit(EXIT_FAILURE);
	}
	//kill and remove shm
	kill((pid_t)(catch_ptr[1]), SIGKILL);
	shmctl(shm_id, IPC_RMID, NULL);

	wait(NULL);
	wait(NULL);

	exit(EXIT_SUCCESS);
}

//--------------------------------------------------------------------

void catch_child2(int signum)
{
	int* catch_ptr;
	if ((catch_ptr = (int*)shmat(shm_id, NULL, 0)) < 0)
	{
		perror("failed to attach memory");
		exit(EXIT_FAILURE);
	}
	//kill and remove
	kill((pid_t)catch_ptr[0], SIGKILL);

	if((shmctl(shm_id, IPC_RMID, NULL)) == -1)
	{
		perror("shmctl failed\n");
		exit(EXIT_FAILURE);
	}

	wait(NULL);
	wait(NULL);

	exit(EXIT_SUCCESS);
}

//.......................................................................

void catch_term(int signum)
{
	//catch signal
	if((shmctl(shm_id, IPC_RMID, NULL)) == -1)
	{
		perror("shmctl failed\n");
		exit(EXIT_FAILURE);
	}
}

//.......................................................................

void open_sem()
{
	//open semaphore
	sem = sem_open("/my_mutex20", O_CREAT, 0644, 1);
	if(sem == SEM_FAILED)
	{
		perror("sem_open() failed");
		exit(EXIT_FAILURE);
	}
}

//.......................................................................

void close_sem()
{
	//close semaphore
	sem_close(sem);
	sem_unlink("/my_mutex20");
}

