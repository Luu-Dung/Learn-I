// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
#define Max 3


/* performs the classic four-step (to ready itself for client requests and then to accept individual requests.)

    socket(…): get a file descriptor for the socket connection
	  |
    bind(…): bind the socket to an address on the server's host
	  |
    listen(…): listen for client requests
	  |
    accept(…): accept a particular client request
	  |
	read(..) or send(..) data
*/


int main(int argc, char const *argv[])
{
	/* declare variables */
	int server_fd, client_fd, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char *hello = "Hello from server";

	/* create fd for the socket */
	server_fd = socket(AF_INET,      /* versus AF_LOCAL */
                    SOCK_STREAM,  /* reliable, bidirectional */
                    0);           /* system picks protocol (TCP) */
	if (server_fd == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	// if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
	// 											&opt, sizeof(opt)))
	// {
	// 	perror("setsockopt");
	// 	exit(EXIT_FAILURE);
	// }



	/* server's local address */
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );



	/* Bind (attaching) server's local address ìn memory */
	if (bind(server_fd, (struct sockaddr *)&address,
								sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}



	/* listen for clients, up to Max is set to 3)*/
	if (listen(server_fd, Max) < 0) 
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}



	/* acept client request */
	if ((client_fd = accept(server_fd, (struct sockaddr *)&address,
					(socklen_t*)&addrlen))<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}



	/* Write some stuff and read the echoes. */
	valread = read( client_fd , buffer, 1024);
	printf("%s\n",buffer );
	send(client_fd , hello , strlen(hello) , 0 );
	printf("Message sent to client.\n");


	/* break the connection (Optional)*/
	puts("Server done, about to exit...");
  	close(client_fd); 
	return 0;
}

