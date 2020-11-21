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

    // Open DATA file
    if ((fd = open(FileName, O_RDONLY)) < 0) // fd =-1 -> action error
        report_and_exit("open failed...");

    // Get current lock value
    fcntl(fd, F_GETLK, &lock);

    if (lock.l_type != F_UNLCK) // Try to Lock file fd (to acess data file)
        report_and_exit("file is NOT unlock...");

    lock.l_type = F_RDLCK;
    if (fcntl(fd, F_SETLK, &lock) < 0)
        report_and_exit("fail to set READONLY lock to data file...");

    // Read the bytes one at a time
    int buf; // buffer for reading
    while (read(fd, &buf, 1) > 0) // 0 signals EOF
        write(STDOUT_FILENO, &buf, 1); //write one byte to standard output

    // After do something with fd, Now we release lock
    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) < 0)
        report_and_exit("Fail to unlock...");

    close(fd);
    return 0;

}