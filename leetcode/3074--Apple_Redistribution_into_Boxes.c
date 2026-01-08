#include<stdio.h>
#include<stdlib.h>

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

typedef sByteT (*Cmp)(const void* a, const void* b); // more true the key get, will have lower index
typedef void (*Swap)(void* a, void* b);

#define INF 1.0/0.0

#endif

#ifdef CSTL_TYPE
#ifndef CSTL_ALGORITHM
#define CSTL_ALGORITHM

#define MAX(a, b) (a>b)?a:b
#define MIN(a, b) (a<b)?a:b
void shell_sort(void* arr, int len, SizeT data_size, Cmp cmp, Swap swap) {
  for (int step = len >> 1; step >= 1; step >>= 1) {
    for (int start = step; start < len; start++) {
      for(int j = start - step; j >= 0 && cmp((ByteT*)arr + (j + step) * data_size, 
        (ByteT*)arr + j * data_size) > 0; j -= step) {
        swap((ByteT*)arr + (j + step) * data_size, (ByteT*)arr + j * data_size);
      }
    }
  }
}
#endif
#endif

sByteT cmp_i(const void* a, const void* b)
{
  int va = *(int*)a, vb = *(int*)b;
  
  return va - vb; // the bigger value has less index 
}
void swap_i(void* a, void* b)
{
  int temp = *(int*)a;
  *(int*)a = *(int*)b;
  *(int*)b = temp;
}

int minimumBoxes(int* apple, int appleSize, int* capacity, int capacitySize) {
  // count all apples
  int apple_amount = 0;
  for(int i = 0; i < appleSize; i++)
    apple_amount += apple[i];
  
  shell_sort(capacity, capacitySize, sizeof(int), cmp_i, swap_i); // sort boxes by greater capacity more proir
  for(int i = 0; i < capacitySize; i++)
  {
    printf("%d ", capacity[i]);
    apple_amount -= capacity[i];
    if(apple_amount <= 0)
      return i + 1;
  }
  return -1;
}
