/** Compilation: gcc -o memreader memreader.c -lrt -lpthread **/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include "shmem.h"

void report_and_exit(const char* msg) {
    perror(msg);
    exit(-1);
}

int main() {
    int fd = shm_open(BackingFile, O_RDWR, AccessPerms);

    /* empty to begin */
    if (fd < 0) report_and_exit("Can't get file descriptor...");

    /* get a pointer to memory */
    caddr_t memptr = mmap(  NULL,           /* let system pick where to put segment */
                            ByteSize,       /* how many bytes */
                            PROT_READ | PROT_WRITE, /* access protections */
                            MAP_SHARED, /* mapping visible to other processes */
                            fd, /* file descriptor */
                            0); /* offset: start at 1st byte */

    if ((caddr_t) -1 == memptr) report_and_exit("Can't access segment...");

    // Create semaphore to synchronize shared memory
    // ----> sem_open() creates a new POSIX "named semaphore" or opens an existing semaphore.
    sem_t *semptr = sem_open( SemaphoreName,  // Name of semaphore
                             O_CREAT,       // create the semaphore
                             AccessPerms,   // protection perms
                             0);            // initial value

    if (semptr == NULL) report_and_exit("Can't sem_open...");

    /* use semaphore as a mutex (lock) by waiting for writer to increment it */
    if (!sem_wait(semptr)){ /* wait until semaphore != 0 */
        int i;
        for (i=0; i < strlen(MemContents); i++)
            write(STDOUT_FILENO, memptr + i, 1); // Write one byte to standard output at a time
        sem_post(semptr);
    }

    // clean up
    munmap(memptr, ByteSize);   //Deallocate any mapping for the region starting at ADDR and extending LEN
    close(fd);
    sem_close(semptr);
    unlink(BackingFile);
    return 0;
}