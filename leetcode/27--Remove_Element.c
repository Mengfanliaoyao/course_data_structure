#include <stdio.h>
#include <stdlib.h>


int removeElement(int* nums, int numsSize, int val)
{
  int actual_element_count = 0;
  for(int i = 0; i < numsSize; i++)
  {
    if(nums[i] == val) continue;
    if(actual_element_count!=i)
      nums[actual_element_count] = nums[i];
    actual_element_count++;
  }
  return actual_element_count;
}
