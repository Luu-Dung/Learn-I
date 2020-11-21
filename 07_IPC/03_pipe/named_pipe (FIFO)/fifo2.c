// C program to implement one side of FIFO
// This side reads first, then reads
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int fc;

//** Create NAMED PIPE
	// FIFO file path
	char * myfifo = "/tmp/myfifo";

	// Creating the named file(FIFO)
	// mkfifo(<pathname>,<permission>)
	mkfifo(myfifo, 0666);



//** DATA Buffer
	char send_buf[80], rec_buf[80];



	while (1)
	{
//** RECEIVE DATA
		// First open in read only and read
		fc = open(myfifo,O_RDONLY);
		read(fc, rec_buf, 80);

		// Print the read string and close
		printf("User1: %s\n", rec_buf);
		close(fc);




//** SEND DATA
		// Now open in write mode and write
		// string taken from user.
		fc = open(myfifo,O_WRONLY);
		fgets(send_buf, 80, stdin);
		write(fc, send_buf, strlen(send_buf)+1);
		close(fc);
	}
	return 0;
}
