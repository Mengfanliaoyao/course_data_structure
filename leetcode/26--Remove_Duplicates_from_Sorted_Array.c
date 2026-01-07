#include<stdio.h>
#include<stdlib.h>

int removeDuplicates(int* nums, int numsSize)
{
  unsigned char ans = 0;
  char last = -127;
  int unique_element_ptr = 0;
  for(int i = 0; i < numsSize; i++)
  {
    if(last!=nums[i])
    {
      ans++;
      last = nums[i];
      nums[unique_element_ptr++] = last;
    }
  }
  return ans;
}
