#include<stdlib.h>
#include<stdio.h>


int countOperations(int num1, int num2) {
  
  int ans = 0;
  // make sure num1 always > num2
  if(num1<num2)
  {
    int temp = num1;
    num1 = num2;
    num2 = temp;
  }
  
  if(!num1||!num2) return 0;
  
  
  while(1)
  {
    if(!num1||!num2) break;
    if(!(num1%num2)) 
    {
      ans+=num1/num2;
      break;
    };
    if((num1-num2)<num2)
    {
      int temp = num1-num2;
      num1 = ((temp)<num2)?num2:temp;
      num2 = (temp<num2)?temp:num2;
    }
    else
    {
      num1 = num1-num2;
    }
    
    ans++;
  }
  return ans;
}

int main(void)
{
  printf("%d\n", countOperations(2,3));
}
