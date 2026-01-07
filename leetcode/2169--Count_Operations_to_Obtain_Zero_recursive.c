#include<stdlib.h>
#include<stdio.h>


int countOperations(int num1, int num2) {
  // make sure num1 always > num2
  if(num1<num2) return countOperations(num2, num1);
  
  if(!num1||!num2) return 0;
  
  if(!(num1%num2)) return num1/num2;
  
  return countOperations(num2, num1-num2) + 1;
}

int main(void)
{
  printf("%d\n", countOperations(2,3));
}
