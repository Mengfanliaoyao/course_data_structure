int maxSumDivThree(int* nums, int numsSize) {
  int ans = 0;
  int least1[] = {2147483647, 2147483647};
  int least2[] = {2147483647, 2147483647};
  for(int i=0; i<numsSize; i++)
  {
    if(!((nums[i]+1)%3))   // 3k+1
    {
      if(nums[i]<least1[0])
      {
        least1[1] = least1[0];
        least1[0] = nums[i];
      }
      else if(nums[i]<least1[1])
      {
        least1[1] = nums[i];
      }
    }
    else if(!((nums[i]+2)%3))   // 3k+2
    {
      if(nums[i]<least2[0])
      {
        least2[1] = least2[0];
        least2[0] = nums[i];
      }
      else if(nums[i]<least2[1])
      {
        least2[1] = nums[i];
      }
    }       
    ans += nums[i];
  }
  if(!((ans+1)%3))   // 3k+1
  {
    if(least2[1]!=2147483647)
    {
      int temp = least2[0] + least2[1];
      if(temp<least1[0])
        ans-=temp;
      else
        ans-=least1[0];
    }
    else
      ans-=least1[0];
  }
  else if(!((ans+2)%3))   // 3k+2
  {
    if(least1[1]!=2147483647)
    {
      int temp = least1[0] + least1[1];
      if(temp<least2[0])
        ans-=temp;
      else
        ans-=least2[0];
    }
    else
      ans-=least2[0];
  }
  
  return ans;
}

int main(void)
{
  return 0;
}
