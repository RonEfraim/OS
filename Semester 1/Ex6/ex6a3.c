
/* ============================================================================
 * Name:       ex6a3 - Client
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    GCD and Primes servers
 * ============================================================================
 */


//CLIENT
//===========================================================================//
#include <stdbool.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <sys/socket.h>
//===========================================================================//
#define SIZE 3
#define MAX_LEN 100
//===========================================================================//
void client(int argc, char *argv[]);
//===========================================================================//
int main(int argc, char *argv[])
{
	client(argc,argv);


	return EXIT_SUCCESS;
}

//===========================================================================//
void client(int argc, char *argv[])
{

	if (argc < 5) {
		fprintf(stderr, "Usage: %s <host name>\n", argv[0]) ;
		exit(EXIT_FAILURE); }

	int rc;
	int my_socket;
	int gcd_arr[SIZE];
	int p_arr[MAX_LEN];
	bool quit = false;
	struct addrinfo con_kind, *addr_info_res;

	short choose;
	while(!quit)
	{
		//reset struct with 0
		memset(&con_kind,0,sizeof(con_kind));
		con_kind.ai_family = AF_UNSPEC;
		con_kind.ai_socktype = SOCK_STREAM;
		con_kind.ai_flags = AI_PASSIVE;

		//get key from user
		scanf("%d", &choose);
		switch(choose)
		{
		case 0:
		{
			if((rc = getaddrinfo(argv[1], argv[2], &con_kind,
					&addr_info_res) != 0))
			{
				fprintf(stderr, "getaddrinfo() failed %s\n", gai_strerror(rc));
				exit(EXIT_FAILURE);
			}

			if((my_socket = socket(addr_info_res->ai_family,
					addr_info_res->ai_socktype,
					addr_info_res->ai_protocol)) < 0)
			{
				perror("socket() failed");
				exit(EXIT_FAILURE);
			}

			if((rc = connect(my_socket, addr_info_res->ai_addr,
					addr_info_res->ai_addrlen)) > 0)
			{
				perror("connect() failed");
				exit(EXIT_FAILURE);
			}

			scanf("%d %d", &gcd_arr[1], &gcd_arr[2]);

			//send 2 numbers for gcd
			if((write(my_socket, &gcd_arr, sizeof(gcd_arr))) == -1) {
				perror("write() failed");
				exit(EXIT_FAILURE); }

			//get answer
			if((rc = read(my_socket, &gcd_arr[0], sizeof(int))) == -1) {
				perror("read() failed");
				exit(EXIT_FAILURE); }

			printf("%d \n", gcd_arr[0]);

			break;
		}

		case 1:
		{
			if((rc = getaddrinfo(argv[3], argv[4],
					&con_kind, &addr_info_res) != 0))
			{
				fprintf(stderr, "getaddrinfo() failed %s\n", gai_strerror(rc));
				exit(EXIT_FAILURE);
			}

			if((my_socket = socket(addr_info_res->ai_family,
					addr_info_res->ai_socktype,
					addr_info_res->ai_protocol)) < 0)
			{
				perror("socket() failed");
				exit(EXIT_FAILURE);
			}

			if((rc = connect(my_socket, addr_info_res->ai_addr,
					addr_info_res->ai_addrlen)) > 0)
			{
				perror("connect() failed");
				exit(EXIT_FAILURE);
			}

			//get number for primes
			int num ;
			scanf("%d", &num);
			if((write(my_socket, &num, sizeof(int))) == -1)
			{
				perror("write() failed");
				exit(EXIT_FAILURE);
			}

			//get answer from server
			if((rc = read(my_socket, p_arr, sizeof(p_arr))) == -1)
			{
				perror("read() failed");
				exit(EXIT_FAILURE);
			}

			//prints answer
			unsigned int i;
			for(i = 0 ; p_arr[i] != -1 ; i++)
				printf("%d ", p_arr[i]);
			puts("");

			break;
		}

		case 2:
		{
			//free list and end program
			quit = true;
			puts("user entered 2 to finish");
			break;
		}

		default:
		{
			//user enters wrong key
			perror("wrong key");
			exit(EXIT_FAILURE);
		}

		//close socket and free list
		close(my_socket);
		freeaddrinfo(addr_info_res);
		}
	}
}
