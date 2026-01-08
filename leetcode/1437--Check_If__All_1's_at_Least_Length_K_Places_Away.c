#include<stdio.h>
#include<stdlib.h>

#include<stdbool.h>

bool kLengthApart(int* nums, int numsSize, int k) {
  // to find at least k 0s seperates 1s, all need is check consecutive 1s if distance of last 1 are greater than k 
  int last = -1;
  for(int i = 0; i < numsSize; i++)
  {
    if(nums[i])
    {
      if((last>=0)&&(i-last-1<k)) // last 1 and this has distance less than k
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
