#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define FileName "data.dat"
#define DataString "Now is the winter of our discontent\nMade glorious summer by this sun of York\n"


void report_and_exit(const char* msg){
    perror(msg);
    exit(-1);
}

int main(){
    struct flock lock;
    lock.l_type = F_WRLCK;      // read/write (exclusive versus shared) lock.
    lock.l_whence = SEEK_SET;   // base for seek offsets
    lock.l_start = 0;           // 1st byte in file
    lock.l_len = 0;             // 0 here mean 'until EOF'
    lock.l_pid = getpid();      // get pid of producer process


    int fd; // File desriptor

    // Create(open if file exist)
    if ((fd = open(FileName, O_RDWR|O_CREAT, 0666)) < 0) // fd =-1 -> action error
        report_and_exit("open failed...");

    if (fcntl(fd, F_SETLK, &lock) < 0) // Try to Lock file fd (to acess data file)
        report_and_exit("fcntl fail to get lock...");
    else {
        write(fd, DataString, strlen(DataString)); // Write data to file ( also call transfer)
        fprintf(stderr, "Process %d has written to data file...\n", lock.l_pid);
    }

    // After do something with fd, Now we release lock
    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) < 0)
        report_and_exit("Fail to unlock...");

    close(fd);
    return 0;

}