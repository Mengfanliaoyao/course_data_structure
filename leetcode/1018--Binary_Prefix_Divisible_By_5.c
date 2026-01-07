#include<stdio.h>
#include<stdlib.h>

#include<stdbool.h>
#include<memory.h>

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

bool* prefixesDivBy5(int* nums, int numsSize, int* returnSize) {
  // for each iteration, the new remainder is current_remainder * 2 + nums[i]
  // which equivalent to left shift nums[0:i-1] by one and append nums[i] at the end
  // by remainder thm. a = b*q+r -> s*a = s*b*q + (s*r)%q
  
  bool* results = malloc(sizeof(bool)*numsSize);
  *returnSize = numsSize;
  
  int remainder = 0;
  for(int i = 0; i < numsSize; i++)
  {
    remainder = ((remainder<<1) + nums[i])%5;
    results[i] = (remainder==0);
  }
  return results;
}

int main(void)
{
  return 0;
}
