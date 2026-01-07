#include<stdio.h>
#include<stdlib.h>

#include<memory.h>

#include<limits.h>

#define MAX(a,b) (a>b)?a:b
#define MIN(a,b) (a<b)?a:b

// O(n)
long long maxSubarraySum(int* nums, int numsSize, int k) {
  
  long long prefix = 0;
  long long *min_prefix = (long long*)malloc(k*sizeof(long long));
  for(int i = 0; i < k; i++)
    min_prefix[i] = LLONG_MAX; // long long max
  
  long long ans = LLONG_MIN;
  for(int i=0;i<numsSize;i++){
    prefix += nums[i];
    
    int mod = (i+1)%k;
    if(!mod) // length of subarray % k ==0
      ans = MAX(ans, prefix);
    if(min_prefix[mod]!=LLONG_MAX)
      ans = MAX(ans, prefix-min_prefix[mod]);
    
    min_prefix[mod] = MIN(min_prefix[mod], prefix);
  }
  
  return ans;
}

// O(n^2/k)
#if 0
long long maxSubarraySum(int* nums, int numsSize, int k) {
  
  long long *prefix_sum = (long long*)calloc(numsSize, sizeof(long long));
  long long ans = 0x8000000000000000ll;
  for(int i=0;i<numsSize;i++){
    prefix_sum[i] = (i)?(prefix_sum[i-1]+nums[i]):(nums[i]);
    if(!((i+1)%k))
      ans = MAX(ans, prefix_sum[i]);
  }
  for(int i=0;i<numsSize;i++){
    for(int j=i%k;j<i;j+=k){
      ans = MAX(ans, prefix_sum[i]-prefix_sum[j]);
    }
  }
  return ans;
}
#endif

int main(void)
{
  return 0;
}
