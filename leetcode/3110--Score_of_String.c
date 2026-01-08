#include<stdio.h>
#include<stdlib.h>

int scoreOfString(char* s)
{
  int ans = 0;
  char last = 0;
  while(*s!=0)
  {
    if(last)
    {
      ans += abs(*s-last);
    }
    last = *s;
    s++;
  }
  return ans;
}

int main(void)
{
  
  return 0;
}
