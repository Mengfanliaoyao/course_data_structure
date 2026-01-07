#include<stdio.h>
#include<stdlib.h>

#include<stdbool.h>

#include<memory.h>


int gcd(int a, int b)
{
  if(a<b) return gcd(b,a);
  int r;
  do
  {
    r = a%b;
    if(r==1) return 1;
    a = b;
    b = r;
  }while(r>0);
  return a;
}

int minOperations(int* nums, int numsSize) {
  int ans = 0;
  int* gcds = malloc(sizeof(int)*(numsSize-1));
  bool end = false;
  for(int i = 0; i < numsSize; i++)
  {
    if(nums[i]==1)
      end = true;
    else
      ans++;
    if(i!=numsSize-1)
      gcds[i] = gcd(nums[i], nums[i+1]);
  }
  if(end)
  {
    free(gcds);
    return ans;
  }
  
  int round = 0;
  bool find = false;
  while(!end)
  {
    end = true;
    for(int i = 0; i < numsSize - round - 1; i++)
    {
      end = false;
      if(gcds[i]==1)
      {
        end = true;
        find = true;
        break;
      }
      if(i!=numsSize - round - 2)
        gcds[i] = gcd(gcds[i], gcds[i+1]);
    }
    round++;
  }
  
  if(find)
    ans--;
  else if(gcds[0]==1)
    find = true;
  
  free(gcds);
  return (find)?(ans+round):(ans-1-round);
}


int main(void)
{
  return 0;
}
