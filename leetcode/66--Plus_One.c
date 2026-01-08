#include<stdio.h>
#include<stdlib.h>

#include<memory.h>

#include<stdbool.h>

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* plusOne(int* digits, int digitsSize, int* returnSize) {
  
  digits[digitsSize - 1]++; // num + 1
  
  bool carry = false;
  for(int i = digitsSize - 1; i >= 0; i--)
  {
    if(carry)
    {
      digits[i] += 1;
      carry = false;
    }
    // check if current digit need carry
    carry = (digits[i]/10);
    digits[i]%=10;
  }
  
  // check if need extra memory for digits since carry
  *returnSize = digitsSize + carry;
  
  int* ans = (int*)malloc(sizeof(int) * *returnSize);
  if(carry) ans[0] = 1;
  memcpy(ans + carry, digits, sizeof(int) * digitsSize);
  
  return ans;
}


int main(void)
{
  
  return 0;
}
