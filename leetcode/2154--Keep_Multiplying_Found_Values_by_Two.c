#include<stdio.h>
#include<stdlib.h>

#include<memory.h>

#ifndef CSTL_TYPE
#define CSTL_TYPE

typedef signed char sByteT;
typedef char ByteT;
typedef sByteT Int8T;
typedef ByteT uInt8T;

typedef short int Int16T;
typedef unsigned short int uInt16T;

typedef int Int32T;
typedef unsigned int uInt32T;

typedef long long int Int64T;
typedef unsigned long long int uInt64T;

typedef uInt64T SizeT;
typedef uInt64T BitsetT;

typedef sByteT (*cmp)(const void* a, const void* b); // more true the key get, will have lower index

#endif

#define BIT(i) (0x1ull<<i)

int findFinalValue(int* nums, int numsSize, int original) {
  uInt16T set = 0;
  for(int i = 0; i < numsSize; i++)
  {
    if(!(nums[i]%original))
    {
      uInt16T rate = nums[i]/original;
      if(!(rate&(rate-1))) // rate is power of 2
      {
//        printf("%d ", nums[i]);
        set |= rate;
      }
    }
  }
//  printf("%0b\n", set);
  for(ByteT i = 0; i < 16; i++)
  {
    if(!(set&BIT(i)))
    {
      return (original<<(i));
    }
  }
  return -1;
}


int main(void)
{
  int nums[] = {5,3,6,1,12,15};
  int origin = 3;
  printf("%d\n", findFinalValue(nums, sizeof(nums)/sizeof(int), origin));
}
