#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


extern int a;
int main(int argc, char **argv)
{
  test_call();
  a++;
  test_call();
	
  printf("%d\n",a);

  return 0;
}
