#include<stdio.h>

#include<stdbool.h>

int maxOperations(char* s) {
  int ans = 0;
  bool blank = false;
  int count1 = 0;
  while(*s)
  {
    if(*s=='1')
    {
      if(blank)
      {
        blank = false;
        ans+=count1;
      }
      count1++;
    }
    else blank = true;
    s++;
  }
  if(blank) ans+=count1;
  return ans;
}

int main(void)
{
  
  return 0;
}
