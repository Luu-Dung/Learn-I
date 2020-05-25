
#include <sys/types.h> /* for pid_t */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for fork */

int main()
{
  pid_t pid = getpid();
  //pid_t tid = gettid();
  printf("pid: %d, threadID: %d \n", pid,1);
  printf("pid: %d, threadID: %d \n", pid,1);
sleep(10);
  printf("pid: %d, threadID: %d \n", pid,1);
  printf("pid: %d, threadID: %d \n", pid,1);

    return 0;
}
