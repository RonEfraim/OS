
/* ============================================================================
 * Name:       ex6a2 - Server (primes)
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    GCD and Primes servers
 * ============================================================================
 */

//SERVER - PRIMES
//===========================================================================//
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
//===========================================================================//
#define MAX_TEXT 100
#define COSTUMERS 5
//===========================================================================//
void get_primes(int num, int *arr);
void server(int argc, char *argv[]);
void catch_sig_int(int sig_num);
struct addrinfo *addr_info_res;
//===========================================================================//
int main(int argc, char *argv[])
{
	server(argc,argv);


	return EXIT_SUCCESS;
}

//===========================================================================//
void server(int argc, char *argv[])
{

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <host name>\n", argv[0]) ;
		exit(EXIT_FAILURE); }

	signal(SIGINT, catch_sig_int);

	int fd;
	fd_set rfd;
	fd_set c_rfd;
	int arr[MAX_TEXT];
	int main_socket;
	int serving_socket;
	struct addrinfo con_kind;

	//reset struct with 0
	memset(&con_kind,0,sizeof(con_kind));
	con_kind.ai_family = AF_UNSPEC;
	con_kind.ai_socktype = SOCK_STREAM;
	con_kind.ai_flags = AI_PASSIVE;

	int rc;
	if((rc = getaddrinfo(NULL, argv[1], &con_kind, &addr_info_res)) != 0) {
		fprintf(stderr, "getaddrinfo() failed %s\n", gai_strerror(rc));
		exit(EXIT_FAILURE); }

	if((main_socket = socket(addr_info_res -> ai_family,
			addr_info_res -> ai_socktype, addr_info_res -> ai_protocol)) < 0)
	{
		perror("socket() failed");
		exit(EXIT_FAILURE) ;
	}

	rc = bind(main_socket, addr_info_res-> ai_addr, addr_info_res-> ai_addrlen);
	if (rc) {
		perror("bind() failed");
		exit(EXIT_FAILURE); }

	rc = listen(main_socket, COSTUMERS);
	if (rc) {
		perror("listen() failed");
		exit(EXIT_FAILURE); }

	//zero fd
	FD_ZERO(&rfd);
	//set main socket in fd
	FD_SET(main_socket, &rfd);



	while (1)
	{
		c_rfd = rfd;
		rc = select(getdtablesize(), &c_rfd, NULL, NULL, NULL);
		if (FD_ISSET(main_socket, &c_rfd)) {
			serving_socket = accept(main_socket,NULL, NULL);

			if (serving_socket >= 0) {
				FD_SET(serving_socket, &rfd);
			}
		}

		int num;
		//run on fd array
		for(fd=main_socket+1;fd<getdtablesize();fd++)
		{
			if (FD_ISSET(fd, &c_rfd))
			{
				rc = read(fd, &num, sizeof(int));
				if (rc == 0) {
					close(fd);
					FD_CLR(fd, &rfd);
				}
				else if(rc > 0)
				{
					//primes function
					get_primes(num,arr);

					//send answer to client
					if((write(fd, &arr, sizeof(arr))) == -1) {
						perror("write() failed");
						exit(EXIT_FAILURE); }
				}
				else
				{
					//if can't read
					perror("read() failed");
					exit(EXIT_FAILURE);
				}
			}
		}
	}
}

//===========================================================================//
void get_primes(int num, int *arr)
{
	int i,j = 0;

	while(num%2 == 0)
	{
		arr[j] = 2;
		j++ ;
		num = num/2;
	}

	for(i=3;i<=num/2;i=i+2)
	{
		while(num%i == 0)
		{
			arr[j] = i;
			j++;
			num /= i;
		}
	}

	if (num > 2) {
		arr[j] = num;
		j++; }

	arr[j]=-1;
}

//===========================================================================//
void catch_sig_int(int sig_num)
{
	//free list and quit
	freeaddrinfo(addr_info_res);
	puts("\n list freed \n");
	exit(EXIT_SUCCESS);
}

