#include<stdio.h>
#include<stdlib.h>

#include<memory.h>

typedef unsigned long long uInt64T;
typedef uInt64T SizeT;

// the quetion is similar to ask there are buckets with amount value

//        first blank (the least index)
// -  -        |  - -    --  -     -   -    -
// - -----  -- v ----- -------   --------  --
// ------------ ----------------------------- <- we don't care about those buckets greater since the gap exists
// |----------------------------------------| <- the least count
// |            completely full             |
// |----------------------------------------|
// 0                                      value

// thus, find value * least count + least index is where the blank is

int findSmallestInteger(int* nums, int numsSize, int value)
{
  SizeT* value_counts = (SizeT*)malloc(sizeof(SizeT)*value);
  memset(value_counts, 0, sizeof(SizeT)*value);
  
  // iterate whole nums array
  for(int i = 0; i < numsSize; i++)
  {
    int a = nums[i]%value;
    if(a<0) a += value;
    value_counts[a]++;
  }
  int least_index = -1;
  SizeT least_count = 0xFFFFFFFFFFFFFFFF;
  // iterate whole value_counts
  for(int i = 0; i < value; i++)
  {
    if(!value_counts[i]) return i;
    if(value_counts[i]<least_count)
    {
      least_index = i;
      least_count = value_counts[i];
    }
  }
  
  return least_index + least_count * value;
}

int main(void)
{
  int nums[] = {3,0,3,2,4,2,1,1,0,4};
  printf("%d", findSmallestInteger(nums, sizeof(nums)/sizeof(int), 5));
  
  return 0;
}
