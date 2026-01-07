#include<stdio.h>
#include<stdlib.h>

typedef long long int Int64T;
typedef long long unsigned int uInt64T;
typedef uInt64T SizeT;

int maximumEnergy(int* energy, int energySize, int k) {
  Int64T* prephix_sum = (Int64T*)malloc(sizeof(Int64T)*energySize);
  int q = energySize/k;
  int r = energySize%k;
  // 0 1 2 ... n-1
  //   k   k   r
  //
  Int64T max_sum = 0x8000000000000000; // INT64_MIN
  for(int i = 0; i < k; i++)
  {
    int offset = (i>=r)?((q-1)*k):(q*k);
    Int64T sum = 0;
    for(int j = offset + i; j>=0; j -= k)
    {
      sum+=*(energy+j);
      *(prephix_sum+j) = sum;
      max_sum = (sum>max_sum)?sum:max_sum;
    }
  }
  return max_sum;
}


int main(void)
{
  
  return 0;
}
