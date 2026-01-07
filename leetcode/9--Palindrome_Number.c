#include<stdio.h>
#include<stdlib.h>

#include<stdbool.h>

bool isPalindrome(int x) {
  if(x < 0) return false;
  
  char s[10] = {0};
  int digit_num = 0;
  int temp = x;
  while(temp)
  {
    s[digit_num++] = temp%10;
    temp/=10;
  }
  
  int l = 0;
  int r = digit_num - 1;
  
  while(l<r){
    if(s[l]!=s[r])
      return false;
    
    l++;
    r--;
  }
  
  return true;
}


int main(void)
{
  printf("%d\n", isPalindrome(131));
  
  return 0;
}
