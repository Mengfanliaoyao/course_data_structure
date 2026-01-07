#include<stdio.h>
#include<stdlib.h>

#include<stdbool.h>

bool kLengthApart(int* nums, int numsSize, int k) {
  int last = -1;
  for(int i = 0; i < numsSize; i++)
  {
    if(nums[i])
    {
      if((last>=0)&&(i-last-1<k))
        return false;
      last = i;
    }
  }
  return true;
}

int main(void)
{
  return 0;
}
