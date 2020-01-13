
/* ============================================================================
 * Name:       ex4a1 - Server (gcd)
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    GCD and Primes servers
 * ============================================================================
 */

//GCD- server
//============================================================================//
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
//===========================================================================//
#define MAX_TEXT 100
#define RECEIVE 1
#define SEND 2
//===========================================================================//
int get_gcd(const int n1, const int n2);
void catch_sig_int(int);
int msgid;
//===========================================================================//
struct My_msg
{
	long _type;
	int _str[MAX_TEXT];
	int _result[MAX_TEXT];
};
//===========================================================================//

int main()
{
	signal(SIGINT, catch_sig_int);

	struct My_msg my_msg;

	//get key
	key_t key ;
	if((key = ftok(".", 'g')) == -1) {
		perror("ftok() failed");
		exit(EXIT_FAILURE);
	}

	//open msg q
	if((msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0600)) == -1)
	{
		perror("msgget() failed");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		//get msg
		my_msg._type = RECEIVE;
		if(msgrcv(msgid,&my_msg,sizeof(struct My_msg),my_msg._type,0) == -1)
		{
			perror("msgrcv() failed");
			exit(EXIT_FAILURE);
		}

		my_msg._result[0] = get_gcd(my_msg._str[0],my_msg._str[1]);
		my_msg._result[1] = -1;

		//send msg
		my_msg._type = SEND;
		if(msgsnd(msgid,&my_msg,sizeof(struct My_msg),0) == -1)
		{
			perror("msgsnd() failed");
			exit(EXIT_FAILURE);
		}
	}
}

//===========================================================================//
int get_gcd(const int n1, const int n2)
{
	//calculate GCD
	unsigned int i, gcd;

	for(i=1; i <= n1 && i <= n2; ++i)
	{
		if(n1%i==0 && n2%i==0)
			gcd = i;
	}

	return gcd;
}

//===========================================================================//
void catch_sig_int(int sig_num)
{
	//close msg q
	if (msgctl(msgid,IPC_RMID,NULL) == -1)
	{
		perror("msgctl() failed") ;
		exit(EXIT_FAILURE) ;
	}
}

