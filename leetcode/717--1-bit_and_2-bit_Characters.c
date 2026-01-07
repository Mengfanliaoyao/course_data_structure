#include<stdio.h>
#include<stdlib.h>

#include<stdbool.h>

bool isOneBitCharacter(int* bits, int bitsSize) {
  bool flag = false;
  for(int i = 0; i < bitsSize; i++)
  {
    if(flag)
      flag = false;
    else if(bits[i])
      flag = true;
    else if (i==bitsSize - 1)
      return true;
  }
  return false;
}


int main(void)
{
  return 0;
}
