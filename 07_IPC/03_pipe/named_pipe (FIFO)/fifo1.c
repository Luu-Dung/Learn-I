// C program to implement one side of FIFO
// This side writes first, then reads
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int fd;
//** Create NAMED PIPE

	// FIFO file path
	char * myfifo = "/tmp/myfifo";

	// Creating the named file(FIFO)
	// mkfifo(<pathname>, <permission>)
	mkfifo(myfifo, 0666); /* A FIFO special file is entered into the filesystem by calling mkfifo() in C */




//** DATA Buffer
	char send_buf[80], rec_buf[80];

	while (1)
	{
//** SEND DATA
		// Open FIFO for write only
		fd = open(myfifo, O_WRONLY);

		// Take an input arr2ing from user.
		// 80 is maximum length
		fgets(send_buf, 80, stdin);

		// Write the input arr2ing on FIFO
		// and close it
		write(fd, send_buf, strlen(send_buf)+1);
		close(fd);




//** RECEIVE DATA
		// Open FIFO for Read only
		fd = open(myfifo, O_RDONLY);

		// Read from FIFO
		read(fd, rec_buf, sizeof(rec_buf));

		// Print the read message
		printf("User2: %s\n", rec_buf);
		close(fd);
	}
	return 0;
}
