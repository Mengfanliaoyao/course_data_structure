#include<stdio.h>
#include<stdlib.h>

#include<stdbool.h>
#include<string.h>


int minCost(char* colors, int* neededTime, int neededTimeSize) {
  int len = strlen(colors);
  
  int start = 0, end = 0;
  char last = 0;
  
  int ans = 0;
  bool repeated = false;
  
  for(int i = 0; i <= len; i++)
  {
    char color = (i==len)?0:colors[i];
    
    if(repeated&&color!=last)
    {
      // for the selected range, let a max cost remain is equal to pick all the others smaller   
      int max_time = 0xFFFFFFFF;
      int sum = 0;
      for(int j = start; j < end; j++)
      {
        sum += neededTime[j];
        max_time = (neededTime[j]>max_time)?neededTime[j]:max_time;
      }
      
      sum -= max_time;
      ans += sum;
      
      // maintain the range of repetation colors consecutive
      repeated = false;
      start = end+1;
    }
    else if(!repeated&&color==last)
    {
      // maintain the range of repetation colors consecutive
      start = end - 1;
      repeated = true;
    }
    
    last = color;
    end++;
  }
  return ans;
}

int main(void)
{
  char s[] = "cddcdcae";
  int t[] = {4,8,8,4,4,5,4,2};
  minCost(s,t,sizeof(t)/sizeof(int));
}
