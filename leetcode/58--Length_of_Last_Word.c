
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

int lengthOfLastWord(char* s)
{
  int len = strlen(s) - 1; // not include '\0'
  for(int i = len; i >= 0; i--)
  {
    if(s[i]==' ')
    {
      if(i==len)
        len--;
      else
        return len - i;
    }
  }
  return len + 1;
}

int main(void)
{
  printf("%d\n", lengthOfLastWord(" a"));
  return 0;
}
