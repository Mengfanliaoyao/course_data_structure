#include<stdio.h>
#include<stdlib.h>
#include<corecrt.h>

#include<string.h>


int romanToInt(char* s) {
  int ans = 0;
  for(size_t i = 0; i<strlen(s); i++)
  {
    char c = *(s+i);
    switch (c) {
      case 'I':
        // check if the next digit is 'V' or 'X'
        if(i+1<strlen(s))
        {
          char next = *(s+i+1);
          if(next=='V'||next=='X')
          {
            ans+=(next=='V')?4:9; // if IV or IX, means 4 and 9 but not 1+5 or 1+10
            i++; // pass next digit
            break;
          }
        }
        ans+=1;
        break;
      case 'V':
        ans+=5;
        break;
      case 'X':
        // check if the next digit is 'L' or 'C'
        if(i+1<strlen(s))
        {
          char next = *(s+i+1);
          if(next=='L'||next=='C')
          {
            ans+=(next=='L')?40:90; // if XL or XC, means 40 and 90 but not 10+50 or 10+100
            i++; // pass next digit
            break;
          }
        }
        ans+=10;
        break;
      case 'L':
        ans+=50;
        break;
      case 'C':
        // check if the next digit is 'D' or 'M'
        if(i+1<strlen(s))
        {
          char next = *(s+i+1);
          if(next=='D'||next=='M')
          {
            ans+=(next=='D')?400:900; // if CD or CM, means 400 and 900 but not 100+500 or 100+1000
            i++; // pass next digit
            break;
          }
        }
        ans+=100;
        break;
      case 'D':
        ans+=500;
        break;
      case 'M':
        ans+=1000;
        break;
      default:
        break;
    }
  }
  return ans;
}


int main(void)
{
  printf("%d\n", romanToInt("MMMCDXLIV"));
  
  return 0;
}
