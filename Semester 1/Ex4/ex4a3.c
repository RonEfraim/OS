
/* ============================================================================
 * Name:       ex4a3 - Client
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    GCD and Primes servers
 * ============================================================================
 */

//client
//============================================================================
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
//===========================================================================//
#define SEND 1
#define RECEIVE 2
#define MAX_TEXT 100
//===========================================================================//
int msqid;
int qid;
//===========================================================================//
struct My_msg
{
	long _type;
	int _str[MAX_TEXT];
	int _result[MAX_TEXT];
};
//===========================================================================//
void send_msg(const key_t key, struct My_msg *my_msg);
void receive_msg(const key_t key, struct My_msg *my_msg);
//===========================================================================//

int main(int argc, char *argv[])
{
	struct My_msg my_msg;
	key_t key;
	//if GCD
	if(strcmp(argv[1], "0") == 0)
	{
		//change type
		my_msg._type = SEND;

		//get key for server 1
		if((key = ftok(".", 'g')) == -1) {
			perror("cannot ftok()");
			exit(EXIT_FAILURE); }

		my_msg._str[0]=atoi(argv[2]);
		my_msg._str[1]=atoi(argv[3]);
	}

	//if primes
	else if(strcmp(argv[1], "1") == 0)
	{
		//change type
		my_msg._type = SEND;//run

		//get key for server 1
		if((key = ftok(".", 'd')) == -1) {
			perror("cannot ftok()");
			exit(EXIT_FAILURE); }

		my_msg._str[0]=atoi(argv[2]);
		//my_msg._str[0] = argv[2];
	}

	else
		return EXIT_FAILURE;


	send_msg(key,&my_msg);
	receive_msg(key,&my_msg);

	return EXIT_SUCCESS;
}

//===========================================================================//
void send_msg(const key_t key, struct My_msg *my_msg)
{
	//connect to msg q
	if((msqid = msgget(key,0)) == -1) {
		perror("msggetsend_msg() failed");
		exit(EXIT_FAILURE);	}

	//send to msg q
	if(msgsnd(msqid,my_msg,sizeof(struct My_msg),0) == -1) {
		perror("cannot msgsnd()");
		exit(EXIT_FAILURE); }
}

//===========================================================================//
void receive_msg(const key_t key, struct My_msg *my_msg)
{
	(*my_msg)._type = RECEIVE;
	if((qid = msgget(key,0)) == -1) {
		perror("cannot msgget()");
		exit(EXIT_FAILURE); }

	int flag = 0;
	if((flag = msgrcv(qid,my_msg,sizeof(struct My_msg),RECEIVE,0)) == -1){
		perror("cannot msgrcv()");
		exit(EXIT_FAILURE); }

	int i = 0;
	while((*my_msg)._result[i] != -1)
	{
		printf("%d ", (*my_msg)._result[i]);
		i++;
	}
}
