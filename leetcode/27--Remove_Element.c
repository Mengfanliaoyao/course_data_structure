#include <stdio.h>
#include <stdlib.h>


int removeElement(int* nums, int numsSize, int val)
{
  unsigned char actual_element_count = 0;
  for(unsigned char i = 0; i < numsSize; i++)
  {
    if(nums[i] == val) continue; // remove all elements == val
    if(actual_element_count!=i)
      nums[actual_element_count] = nums[i]; // update remaining elements
    actual_element_count++;
  }
  return actual_element_count;
}
