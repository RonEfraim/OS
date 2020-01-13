

/* ============================================================================
 * Name:       ex6a1 - Server (gcd)
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    GCD and Primes servers
 * ============================================================================
 */

//SERVER - GCD
//===========================================================================//
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
#define COSTUMERS 5
struct addrinfo *addr_info_res;
//===========================================================================//
int get_gcd(int n1, int n2) ;
void catch_sig_int(int sig_num);
void server(int argc, char *argv[]);
//===========================================================================//
int main(int argc, char *argv[])
{
	server(argc,argv);


	return EXIT_SUCCESS;
}

//===========================================================================//
void server(int argc, char * argv[])
{

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <host name>\n", argv[0]) ;
		exit(EXIT_FAILURE); }

	signal(SIGINT, catch_sig_int) ;

	struct addrinfo con_kind;

	int serving_socket;
	int main_socket;
	int arr[SIZE];
	fd_set c_rfd;
	fd_set rfd;
	int rc;
	int fd;

	//reset struct with 0
	memset(&con_kind,0,sizeof(con_kind));
	con_kind.ai_family = AF_UNSPEC;
	con_kind.ai_socktype = SOCK_STREAM;
	con_kind.ai_flags = AI_PASSIVE;

	if((rc = getaddrinfo(NULL, argv[1], &con_kind, &addr_info_res)) != 0) {
		fprintf(stderr, "getaddrinfo() failed %s\n", gai_strerror(rc));
		exit(EXIT_FAILURE); }

	if((main_socket = socket(addr_info_res -> ai_family,
			addr_info_res -> ai_socktype, addr_info_res -> ai_protocol)) < 0) {
		perror("socket() failed");
		exit(EXIT_FAILURE); }

	rc = bind(main_socket, addr_info_res-> ai_addr, addr_info_res-> ai_addrlen);
	if(rc) {
		perror("bind() failed");
		exit(EXIT_FAILURE); }
	rc = listen(main_socket, COSTUMERS);

	if(rc) {
		perror("listen() failed");
		exit(EXIT_FAILURE); }

	FD_ZERO(&rfd);
	FD_SET(main_socket, &rfd);


	while (1)
	{
		c_rfd = rfd;
		rc = select(getdtablesize(), &c_rfd, NULL, NULL, NULL);
		if (FD_ISSET(main_socket, &c_rfd))
		{
			serving_socket = accept(main_socket,NULL, NULL);

			if (serving_socket >= 0) {
				FD_SET(serving_socket, &rfd); }
		}

		//run on fd array
		for(fd=main_socket+1;fd<getdtablesize();fd++)
		{
			if(FD_ISSET(fd,&c_rfd))
			{
				rc = read(fd, &arr, sizeof(arr));
				if(rc == 0) {
					close(fd);
					FD_CLR(fd, &rfd);
				}
				else if(rc > 0)
				{
					arr[0] = get_gcd(arr[1], arr[2]);
					int gcd = arr[0];

					//send answer to client
					if((write(fd, &gcd, sizeof(int))) == -1)
					{
						perror("write() failed");
						exit(EXIT_FAILURE);
					}
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
int get_gcd(int n1, int n2)
{
	int temp;
	while (n2 != 0)
	{
		temp = n1 % n2;

		n1 = n2;
		n2 = temp;
	}
	return n1;
}

//===========================================================================//
void catch_sig_int(int sig_num)
{
	//free list and quit
	freeaddrinfo(addr_info_res);
	puts("\n list freed \n");
	exit(EXIT_SUCCESS);
}


