/** Compilation: gcc -o memwriter memwriter.c -lrt -lpthread **/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include "shmem.h"

void report_and_exit(const char *msg){
    perror(msg);
    exit(-1);
}

int main(){

    // get a fd for backingfile ( that the system coordinates with the shared memory)
    int fd = shm_open( BackingFile,      // name from shmem.h
                       O_RDWR | O_CREAT, // open with read/write, creat if need
                       AccessPerms);    // access permission is 0644

    if (fd < 0) report_and_exit("Fail to open shared mem segment...");

    // Allocates ByteSize bytes ("future" shared memory)
    ftruncate(fd, ByteSize); // get the bytes

    // create a pointer to memory as "shared"
    caddr_t memptr = mmap(NULL,                     // let system pick whare to put segment
                          ByteSize,                 // how many bytes
                          PROT_READ | PROT_WRITE,   // access protection
                          MAP_SHARED,               // indicates that the allocated memory is shareable among processes
                          fd,                       // file descpritor
                          0);                       // offset: start at 1st byte

    if ((__caddr_t)-1 == memptr) report_and_exit("Can't get segment...");

    fprintf(stderr, "shared mem address: %p [0...%d]\n",memptr, ByteSize - 1);
    fprintf(stderr, "backing file:       /dev/shm%s\n", BackingFile);

    // Create semaphore to synchronize shared memory
    // ----> sem_open() creates a new POSIX "named semaphore" or opens an existing semaphore. 
    sem_t *semptr = sem_open( SemaphoreName,  // Name of semaphore
                             O_CREAT,       // create the semaphore
                             AccessPerms,   // protection perms
                             0);            // initial value

    if (semptr == NULL) report_and_exit("Can't sem_open...");

    // Copy some data to share mem
    strcpy(memptr, MemContents);

    // in create the semaphore so that memreader can read
    if (sem_post(semptr) < 0) report_and_exit("Can't sem_post...");

    sleep(12);  // give reader a chance before clean shared memory (reader should read data in this time)

    // clean up
    munmap(memptr, ByteSize); // unmap to storage
    close(fd);
    sem_close(semptr);
    shm_unlink(BackingFile); // unlink from the backing file
    return 0;

}
