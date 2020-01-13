
/* ============================================================================
 * Name:       ex9a
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    Threads Semaphore
 * ============================================================================
 * This program will allocated an array size 13 to shared memory.
 * the array will contain 2 child id's in the first 2 cells and random prime
 * numbers in cells 2-12 and primes counter in the 13'th cell.
 * the program will make 2 children.
 * each child will roll in an infinite loop and roll random numbers in range of
 * 2-1000 using srand(17), if the number rolled is prime it will insert it to
 * shared memory array, and whoever child will finish filling the array first
 * will kill his brother, print the array and free the shared memory and
 * semaphore.
 *
 * possible race conditions:
 * 1) first child done filling the array before the other child has done
 * anything.
 * 2) first child done filling the array before the other child wrote on the
 * shared memory, so the other child tries to attach to the shared memory to
 * the shared memory after it was already deleted by the first child - error
 * 3) both children will insert primes to the array so the array will be mixed
 *
 * Compile: gcc ex9a.c -o thread -ex9a
 * Run: ./ex9a
 * Input: none
 * Output: array of random primes
 * ============================================================================
 */


//===========================================================================//
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
//===========================================================================//
#define SHM_SIZE 13
#define CHILDREN 2
#define COUNTER 12
#define RANGE 998
#define DONE 10
//===========================================================================//
void make_children(key_t key, int *shm_ptr);
void do_child(int *shm_ptr, sem_t *sem);
void free_memory(int *shm_ptr);
void check_shmat(int *shm_ptr);
void kill_bro(int sig_num);
key_t make_key(key_t key);
int alloc_shm(key_t key);
bool is_prime(int num);
void print_result();
void close_sem();
void open_sem();
//===========================================================================//
int shm_id;
sem_t *sem;
//===========================================================================//
int main()
{
	srand(17);
	signal(SIGTERM, kill_bro);

	open_sem(); //open semaphore
	key_t key = make_key(key); //get key
	shm_id = alloc_shm(key); //allocate shared memory
	int *shm_ptr = (int*) shmat(shm_id, NULL, 0);
	check_shmat(shm_ptr); //check if shmat failed
	make_children(key,shm_ptr); //make 2 children
	wait(NULL); //wait for child


	return EXIT_SUCCESS;
}

//===========================================================================//
key_t make_key(key_t key)
{
	if((key = ftok("./ex9a.c", 'a')) == -1)
	{
		perror("ftok() failed");
		exit(EXIT_FAILURE);
	}
	return key;
}
//===========================================================================//
int alloc_shm(key_t key)
{
	if((shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0600)) == -1)
	{
		perror("shmget() failed");
		exit(EXIT_FAILURE);
	}
	return shm_id;
}
//===========================================================================//
void make_children(key_t key, int *shm_ptr)
{
	shm_ptr[COUNTER] = 0;
	unsigned int i;
	for(i=0;i<CHILDREN;i++)
	{
		shm_ptr[i] = fork();

		if(shm_ptr[i] < 0)
		{
			perror("fork() failed");
			exit(EXIT_FAILURE);
		}
		else if(shm_ptr[i] == 0)
		{
			shm_ptr[i] = getpid();
			do_child(shm_ptr,sem);
		}
		else
			continue;
	}
}

//===========================================================================//
void do_child(int *shm_ptr, sem_t *sem)
{
	while(1)
	{
		int roll = rand() % RANGE + 2;
		if(is_prime(roll))
		{
			//--------------critical section-------------------
			sem_wait(sem);
			if(getpid() == shm_ptr[0])
				sleep(1);
			shm_ptr[shm_ptr[COUNTER]+2] = roll;
			shm_ptr[COUNTER]++;
			if(shm_ptr[COUNTER] == DONE)
			{
				print_result();
				getpid() == shm_ptr[1] ?
						kill(shm_ptr[0], SIGTERM) : kill(shm_ptr[1], SIGTERM);
			}
			sem_post(sem);
			sleep(1);
		}
	}
}

//===========================================================================//
bool is_prime(int num)
{
	if (num <= 1) return 0;
	if (num % 2 == 0 && num > 2) return 0;
	unsigned int i;
	for(i = 3; i < num / 2; i+= 2)
	{
		if (num % i == 0)
			return false;
	}
	return true;
}

//===========================================================================//
void open_sem()
{
	sem = sem_open("/my_mutex20", O_CREAT, 0644, 1);
	if(sem == SEM_FAILED)
	{
		perror("sem_open() failed");
		exit(EXIT_FAILURE);
	}
}

//===========================================================================//
void check_shmat(int *shm_ptr)
{
	if(!shm_ptr)
	{
		perror("shmget() failed");
		exit(EXIT_FAILURE);
	}
}

//===========================================================================//
void close_sem()
{
	sem_close(sem);
	sem_unlink("/my_mutex20");
}

//===========================================================================//
void kill_bro(int sig_num)
{
	close_sem(sem);
	int *shm_ptr = (int*) shmat(shm_id, NULL, 0);
	check_shmat(shm_ptr);
	kill(shm_ptr[0], SIGTERM);
	kill(shm_ptr[1], SIGTERM);
	free_memory(shm_ptr);
	exit(EXIT_SUCCESS);
}

//===========================================================================//
void print_result()
{
	int *shm_ptr = (int*) shmat(shm_id, NULL, 0);
	check_shmat(shm_ptr);

	unsigned int i;
	for(i=2;i<SHM_SIZE-1;i++)
		printf("%d ", shm_ptr[i]);
	puts("");
}

//===========================================================================//
void free_memory(int *shm_ptr)
{
	shmdt(shm_ptr);
	if (shmctl(shm_id,IPC_RMID, NULL) == -1)
	{
		perror("shmctl failed");
		exit(EXIT_FAILURE);
	}
}
