#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

int a =2;

void test_call();
void test_call()
{
  a ++;
  printf("Call from main to out_size: %d\n",a);
}
