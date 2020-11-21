/*
 * Linux IPC via pipe()
 *
 * By Lei Yang <leiyang@fastmail.com>
 * March 20, 2017
 *
 * Expected output: Message from the parent: Hello from the the parent!
 *
 * Released under WTFPL 2.0
 */


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int fid[2] = {0};
    int result = pipe(fid);
    if (result != 0)
    {
        printf("Failed to create pipe 0.0\n");
    }

    pid_t childid = fork();
    if (childid == -1)
    {
        printf("Failed to fork 0.0\n");
    }

    /* divergent */
    if (childid == 0) // in child, fork() return 0 value.
    {
        /* child */
        printf("In Child\n");
        close(fid[0]); /* child closes the write end */
        char buffer[50];
        read(fid[1], buffer, sizeof(buffer));
        printf("Message from the parent: %s", buffer);
        exit(0);
    }
    else            // in parent, fork() return a value khac 0.
    {
        /* parent */
        printf("In Parent\n");
        close(fid[1]); /* parent closes the read end */
        char towrite[] = "Hello from the the parent!\n";
        write(fid[0], towrite, strlen(towrite) + 1);
        exit(0);
    }
    return 0;
}