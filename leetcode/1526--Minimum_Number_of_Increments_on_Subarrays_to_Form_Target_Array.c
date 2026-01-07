#include<stdio.h>
#include<stdlib.h>

#include<stdbool.h>


int minNumberOperations(int* target, int targetSize) {
  int ans = 0;
  
  bool incresing = false;
  bool decresing = false;
  
  int last = target[0];
  int horizon = 0;
  int count = 0;
  for(int i = 1; i < targetSize; i++)
  {
    if((incresing||(!incresing&&!decresing))&&target[i]<last) // concerning the situation that equal for a while then drop
    {
      incresing = false;
      decresing = true;
      // update times need to operate
      ans+=(last - horizon);
      count = 0;
    }
    else if(decresing&&target[i]>last)
    {
      incresing = true;
      decresing = false;
      horizon = last;
    }
    else
    {
      if(target[i]<last)
      {
        if(i==1)
        {
          ans+=(last-horizon);
          count = 0;
        }
        incresing = false;
        decresing = true;
      }
      else if(target[i]>last)
      {
        incresing = true;
        decresing = false;
      }
    }
    last = target[i];
    count++;
  }
  if((incresing&&count)||(!incresing&&!decresing)) // need to concern situation that all equal or the last one is beggest of last segement
    ans+=(last-horizon);
  return ans;
}

int main(void)
{
  int a[] = {3,1,1,2};
  printf("%d\n", minNumberOperations(a, sizeof(a)/sizeof(int)));
  return 0;
}
