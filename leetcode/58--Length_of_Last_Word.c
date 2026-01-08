
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

int lengthOfLastWord(char* s)
{
  int len = strlen(s) - 1; // not include '\0'
  for(int i = len; i >= 0; i--)
  {
    if(s[i]==' ') // ' ' seperates words
    {
      if(i==len) // delete ' ' after last word
        len--;
      else
        return len - i; // find ' ' before last word
    }
  }
  return len + 1; // find till start
}

int main(void)
{
  printf("%d\n", lengthOfLastWord(" a"));
  return 0;
}
