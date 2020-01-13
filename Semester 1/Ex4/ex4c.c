
/* ============================================================================
 * Name:       ex4c
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    Father To Child Message Queue
 * ============================================================================
 *
 * This program opens a msg q and make 2 children. each child defnes array with
 * 10 cells and fill it with random ints. the dad rolls ints from 0 to 100 in
 * an endless loop and and send to each child, once a child got all the ints
 * from dad and marked them all in his array it kills the other child and send
 * the dad a message and prints the numbers.
 *
 * Compile: gcc -Wall ex4c.c -0 ex4c
 * Run: ex4c
 * Input: none
 * Output: the array of the child that finished first
 * ============================================================================
 */

//============================================================================//
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
//===========================================================================//
#define CHILDREN 2
#define RANGE 100
#define CELLS 10
#define CHILD1 1
#define CHILD2 2
#define DONE 3
#define DAD 4
//===========================================================================//
struct My_msg
{
	long _type;
	long _done;
	int _num;
};
//===========================================================================//
void do_child(int arr[], const int child_num);
void fill_arr(int arr[]);
void send_msg();
int search(int arr[], const int val);
void bubbleSort(int arr[], int n);
void catch_sig_kill(int sig_num);
void print_arr(const int arr[]);
void swap(int *xp, int *yp);
void do_dad();
void free_q();
int msgid;
int counter = 0;
pid_t child_id[CHILDREN];
struct My_msg my_msg;
int arr1[CELLS];
int arr2[CELLS];
int msgid;
//===========================================================================//

int main()
{
	signal(SIGKILL, catch_sig_kill);
	srand(time(NULL));

	//get key
	key_t key;
	if((key = ftok(".", 'y')) == -1) {
		perror("ftok() failed");
		exit(EXIT_FAILURE);
	}

	//open msg q
	if((msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0600)) == -1)
	{
		perror("msgget() failed");
		exit(EXIT_FAILURE);
	}

	int i;
	for(i=1;i<CHILDREN+1;i++)
	{
		pid_t status = fork();
		child_id[i] = status;
		if(status < 0)
		{
			puts("fork() failed");
			exit(EXIT_FAILURE);
		}

		else if(status == 0)
		{
			if(i == 1)
				do_child(arr1,i);
			else
				do_child(arr2,i);

			return EXIT_SUCCESS;
		}
	}

	do_dad();

	//release msg q
	free_q();

	return EXIT_SUCCESS;
}

//===========================================================================//
void do_child(int arr[], const int child_num)
{
	fill_arr(arr);
	//bubbleSort(arr, CELLS);

	while(counter != CELLS)
	{
		//get msg
		my_msg._type = child_num;
		if(msgrcv(msgid,&my_msg,sizeof(struct My_msg),my_msg._type,0) == -1)
		{
			perror("msgrcv() failed");
			exit(EXIT_FAILURE);
		}

		if(search(arr,my_msg._num) == 0)
			counter++;
	}

	//kill other child
	//kill(child_id[0]+child_id[1]-getpid(), SIGKILL);
	kill(child_id[0]+child_id[1]-getpid(), SIGKILL);

	my_msg._type = DAD;
	my_msg._done = DONE;
	my_msg._num  = child_num;

	//send msg to dad
	if(msgsnd(msgid,&my_msg,sizeof(struct My_msg),0) == -1)
	{
		perror("msgsnd() failed");
		exit(EXIT_FAILURE);
	}
}

//===========================================================================//
void fill_arr(int arr[])
{
	unsigned int i;
	for(i=0;i<CELLS;i++)
		arr[i] = (rand() % RANGE);
}

//===========================================================================//
void bubbleSort(int arr[], int n)
{
	int i, j;
	for (i = 0; i < n-1; i++)
		for (j = 0; j < n-i-1; j++)
			if (arr[j] > arr[j+1])
				swap(&arr[j], &arr[j+1]);
}

void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

//===========================================================================//
int search(int arr[], const int val)
{
	//search value
	unsigned int i;
	for(i=0;i<CELLS;i++)
	{
		if(arr[i] == -1)
			continue;

		if(arr[i] == val)
		{
			arr[i] = -1;
			return 0;
		}
	}

	return -1;
}

//===========================================================================//
void do_dad()
{
	while(1)
	{
		my_msg._num = rand() % RANGE;

		//send msg
		my_msg._type = CHILD1;
		send_msg();

		my_msg._type = CHILD2;
		send_msg();
	}

	free_q();
}

//===========================================================================//
void send_msg()
{
	//send msg to child
	if(msgsnd(msgid,&my_msg,sizeof(struct My_msg),0) == -1)
	{
		perror("msgsnd() failed");
		exit(EXIT_FAILURE);
	}
}

//===========================================================================//
void print_arr(const int arr[])
{
	unsigned int i;
	for(i=0;i<CELLS;i++)
		printf("%d", arr[i]);
}

//===========================================================================//
void free_q()
{
	//close msg q
	if (msgctl(msgid,IPC_RMID,NULL) == -1)
	{
		perror("msgctl() failed") ;
		exit(EXIT_FAILURE) ;
	}
}

//===========================================================================//
void catch_sig_user(int sig_num)
{
	my_msg._type = DAD;
	if(msgrcv(msgid,&my_msg,sizeof(struct My_msg),my_msg._type,0) == -1)
	{
		perror("msgrcv() failed");
		exit(EXIT_FAILURE);
	}

	if(my_msg._done == DONE)
		my_msg._num == CHILD1 ? print_arr(arr1):print_arr(arr2);

	else
		perror("NOT GOOD!");
}
