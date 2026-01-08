
#include<stdio.h>
#include<stdlib.h>

int maxDistinct(char* s)
{
  char ans = 0;
  char alpha[26] = {0};
  while(*s!=0)
  {
    if(!alpha[(*s)-'a'])
    {
      ans++;
      alpha[(*s)-'a'] = 1;
    }
    s++;
  }
  return ans;
}
