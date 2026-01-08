#include<stdio.h>
#include<stdlib.h>

int removeDuplicates(int* nums, int numsSize)
{
  unsigned char ans = 0;
  char last = -127; // nums[i] on interval [-100,100]
  int unique_element_ptr = 0;
  for(int i = 0; i < numsSize; i++)
  {
    if(last!=nums[i]) // ignore same elements
    {
      ans++;
      last = nums[i];
      nums[unique_element_ptr++] = last; // update unique elements
    }
  }
  return ans;
}
