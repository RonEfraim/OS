
/* ============================================================================
 * Name:       ex4a2 - Server (primes)
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    GCD and Primes servers
 * ============================================================================
 */

//PRIMES - server
//============================================================================
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include <math.h>
//===========================================================================//
#define MAX_TEXT 100
#define RECEIVE 1
#define SEND 2
int msgid;
//===========================================================================//
struct My_msg
{
	long _type;
	int _str[MAX_TEXT];
	int _result[MAX_TEXT];
};
//===========================================================================//
void get_primes(struct My_msg* my_msg, int num);
void catch_sig_int(int);
//===========================================================================//


int main()
{
	signal(SIGINT, catch_sig_int);

	struct My_msg my_msg;

	//get key
	key_t key ;
	if((key = ftok(".", 'd')) == -1) {
		perror("cannot ftok()");
		exit(EXIT_FAILURE);
	}

	//open new msg line
	if((msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0600)) == -1)
	{
		perror("cannot create message line");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		//get msg
		my_msg._type = RECEIVE;

		if(msgrcv(msgid,&my_msg,sizeof(struct My_msg),my_msg._type,0) == -1)
		{
			perror("cannot receive message");
			exit(EXIT_FAILURE);
		}

		get_primes(&my_msg,my_msg._str[0]);

		my_msg._type = SEND;
		if(msgsnd(msgid,&my_msg,sizeof(struct My_msg),0) == -1)
		{
			perror("cannot msgsnd()");
			exit(EXIT_FAILURE);
		}

	}

	unsigned int i;
	while(my_msg._str[i] != -1)
		printf("%d ", my_msg._str[i]);

	puts(" ");


	return EXIT_SUCCESS;
}

//===========================================================================//
void get_primes(struct My_msg* my_msg, int num)
{
	int i, j = 0;
	for (i=2; i<=num; i++)
	{
		while (num%i == 0)
		{
			(*my_msg)._result[j] = i;
			j++;
			num/=i;
		}
	}
	(*my_msg)._result[j] = -1;
}

//===========================================================================//
void catch_sig_int(int sig_num)
{
	//close msg q
	if(msgctl(msgid,IPC_RMID,NULL) == -1)
	{
		perror("msgctl() failed") ;
		exit(EXIT_FAILURE) ;
	}
}
