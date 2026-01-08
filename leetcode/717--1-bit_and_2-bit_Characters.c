#include<stdio.h>
#include<stdlib.h>

#include<stdbool.h>

bool isOneBitCharacter(int* bits, int bitsSize) {
  bool flag = false;
  for(int i = 0; i < bitsSize; i++)
  {
    if(flag)
      flag = false;
    else if(bits[i]) // if reach any 1, since 2 bits characters all begin with 1, ignore the next bit
      flag = true;
    else if (i==bitsSize - 1) // last bit is one bit character
      return true;
  }
  return false; // last bit been ignored, is 2 bits character
}


int main(void)
{
  return 0;
}
