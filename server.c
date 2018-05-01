/*
 * 	An implementation of Server Socket for Client-Server communications
 *
 *	Program Name    : server.c
 * 	Description	: Implementation of server socket
 *  	Date		: 01-05-2018
 * 	Author		: SIDHDHARTH YADAV
 *
 */

#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<sys/time.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	int result;

	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	fd_set readfds, testfds;

	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(9734);

	server_len = sizeof(server_address);

	bind(server_sockfd,(struct sockaddr *)&server_address,server_len);
	listen(server_sockfd, 5);

	FD_SET(server_sockfd, &readfds);

	while(1) 
	{
		char c;
		int fd;
		testfds = readfds;
		
		printf("Server is waiting for Client\n");

		result = select(FD_SETSIZE,&testfds,(fd_set *)0,(fd_set *)0,(struct timeval *)0);

		if(result < 1)
		{
			perror("Server failed initializing");
			exit(1);
		}
		
		for(fd = 0; fd < FD_SETSIZE; fd++) 
		{
			if(FD_ISSET(fd,&testfds))
			{
				if(fd == server_sockfd)
				{
					client_len = sizeof(client_address);
					client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address,&client_len);
					FD_SET(client_sockfd, &readfds);
					printf("Adding client on fd %d\n", client_sockfd);
				}
				else
				{
					read(fd, &c, 1);
					printf("Serving client on fd %d\n", fd);
					c++;
					sleep(1);
					write(fd, &c, 1);
				}
			}
		}
	}
}
