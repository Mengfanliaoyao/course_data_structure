#include<stdio.h>
#include<stdlib.h>

#include<stdbool.h>
#include<string.h>

bool hasSameDigits(char* s) {
  int n = strlen(s);
  while(n>2)
  {
    int num = s[0] - '0';
    int last = num;
    for(int i = 1; i < n; i++)
    {
      num += (s[i] - '0');
      s[i - 1] = '0' + num % 10;
      num -= last;
      last = s[i] - '0';
    }
    n--;
  }
  if(s[0]==s[1]) return true;
  return false;
}
