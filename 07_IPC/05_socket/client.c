// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080

/* performs the classic 3-step:

    socket(…): get a file descriptor for the socket connection
	  |
    get or create the address of the host
	  |
    connect(…): connect to the server
	  |
	read(..) or send(..) data
*/

int main(int argc, char const *argv[])
{
	/* declare variables */
	int sockfd = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from client";
	char buffer[1024] = {0};


	 /* fd for the socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}


	/* get or create the address of the host */
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}


	/* connect to the server: configure server's address 1st */
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}


	/* Write some stuff and read the echoes. */
	send(sockfd , hello , strlen(hello) , 0 );
	printf("Message sent to server.\n");
	valread = read( sockfd , buffer, 1024);
	printf("%s\n",buffer );


	/* close the connection (Optional)*/
	puts("Client done, about to exit...");
  	close(sockfd); 
	return 0;
}

