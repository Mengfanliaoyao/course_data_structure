#include <stdio.h>
#include <stdlib.h>

int smallestRepunitDivByK(int k) 
{
  // find 1....1 % k == 0
  
  // by remainder thm. a = b*q+r -> s*a = s*b*q + (s*r)%q
  
  int i = 1, r = 0;
  while(k%2&&i<100000)
  {
    r = ((r*10) + 1)%k; // update remainder
    if(!r) // remainder == 0, find first num divisible by k
      return i;
    i++;
  }
  
  return -1;
}

int main(void)
{
  return 0;
}
