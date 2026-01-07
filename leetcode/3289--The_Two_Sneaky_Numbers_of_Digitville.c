#include<stdio.h>
#include<stdlib.h>

#include<memory.h>


typedef unsigned long long SizeT;

#define BIT(i) (0x1<<i)
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* getSneakyNumbers(int* nums, int numsSize, int* returnSize) {
  char bitsize = numsSize/64+1;
  SizeT* bitset = (SizeT*)malloc(sizeof(SizeT)*bitsize);
  memset(bitset, 0, sizeof(SizeT)*bitsize);
  
  int* ans = (int*)malloc(sizeof(int)*2);
  char offset = 0;
  
  for(int i = 0; i < numsSize; i++)
  {
    if(bitset[nums[i]/64]&BIT(nums[i]%64)) // find repeat number
    {
      ans[offset++] = nums[i];
      if(offset == 2)
        break;
    }
    else
      bitset[nums[i]/64] |= BIT(nums[i]%64);
//    printf("%llx\n", bitset[0]);
  }
  *returnSize = offset;
  return ans;
}

int main(void)
{
  int a[] = {7,1,5,4,3,4,6,0,9,5,8,2};
  int rsize;
  int* ans = getSneakyNumbers(a, sizeof(a)/sizeof(int), &rsize);
  for(int i = 0; i < rsize; i++)
  {
    printf("%d ", ans[i]);
  }
  return 0;
}
