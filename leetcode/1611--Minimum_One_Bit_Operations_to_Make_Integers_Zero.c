#include<stdio.h>
#include<stdlib.h>

#include<stdbool.h>

#define BIT(i) (0x1u<<(i))

// by operation of 0 to 4 bits operations, we could find that operations is recursive
// by the same idea of inclusion-exclusion principle, find a particular number could by many times of add and minus repeatly
int minimumOneBitOperations(int n)
{
  int ans = 0;
  signed char msb = -1;
  bool negation = false;
  for(int i=31; i>=0; i--)
  {
    if(BIT(i)&n)
    {
      if(msb<0)
      {
        msb=i;
        ans += BIT(msb)*(1-2*(int)negation);
      }
      if(msb>i)
        msb=-1;
    }
    else if(msb>i)
    {
      if(msb-1==i)
      {
        ans+=((BIT(msb)-1)*(1-2*(int)negation));
        negation^=0x1;
      }
      msb=-1;
    }      
  }
  return ans;
}
#if 0
0000
0001 1
0011 2
0010 3
0110 4
0111 5
0101 6
0100 7
1100 8
1101 9
1111 10
1110 11
1010 12
1011 13
1001 14
1000 15
#endif

int main(void)
{
  printf("%d\n", minimumOneBitOperations(6));
  return 0;
}
