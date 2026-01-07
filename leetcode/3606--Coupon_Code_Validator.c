#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <memory.h>

#ifndef CSTL_TYPE
#define CSTL_TYPE

typedef signed char sByteT;
typedef char ByteT;
typedef sByteT Int8T;
typedef ByteT uInt8T;

typedef short int Int16T;
typedef unsigned short int uInt16T;

typedef int Int32T;
typedef unsigned int uInt32T;

typedef long long int Int64T;
typedef unsigned long long int uInt64T;

typedef uInt64T SizeT;
typedef uInt64T BitsetT;

typedef sByteT (*Cmp)(const void* a, const void* b); // more true the key get, will have lower index
typedef void (*Swap)(void* a, void* b);

#define INF 1.0/0.0

#endif

#ifdef CSTL_TYPE
#ifndef CSTL_ALGORITHM
#define CSTL_ALGORITHM

#define MAX(a, b) (a>b)?a:b
#define MIN(a, b) (a<b)?a:b
void shell_sort(void* arr, int len, SizeT data_size, Cmp cmp, Swap swap) {
  for (int step = len >> 1; step >= 1; step >>= 1) {
    for (int start = step; start < len; start++) {
      for(int j = start - step; j >= 0 && cmp((char*)arr + (j + step) * data_size, 
        (char*)arr + j * data_size) < 0; j -= step) {
        swap((char*)arr + (j + step) * data_size, (char*)arr + j * data_size);
      }
    }
  }
}
#endif
#endif

typedef struct Coupon Coupon;
struct Coupon
{
  char** code;
  int codeSize;
  char** businessLine;
  int businessLineSize;
};

Coupon* g_coupon = NULL;

sByteT coupon_cmp(const void* a, const void* b)
{
  int a_id = *(int*)a;
  int b_id = *(int*)b;

  char* line_a = g_coupon->businessLine[a_id];
  char* line_b = g_coupon->businessLine[b_id];
  sByteT not_equal = strcmp(line_a, line_b);
  if(not_equal)
    return not_equal;

  char* code_a = g_coupon->code[a_id];
  char* code_b = g_coupon->code[b_id];

  return strcmp(code_a ,code_b);
}
void coupon_swap(void* a, void* b)
{
  int temp = *(int*)a;
  *(int*)a = *(int*)b;
  *(int*)b = temp;
}

char** validateCoupons(char** code, int codeSize, char** businessLine, int businessLineSize, bool* isActive, int isActiveSize, int* returnSize)
{
  int max_size = MAX(MAX(codeSize, businessLineSize), isActiveSize);
  int* q = (int*)malloc(max_size*sizeof(int));
  int q_count = 0;
  // filt out valid ids
  for(int i = 0; i < max_size; i++)
  {
    int code_len = strlen(code[i]);
    if(!code_len) continue;
    bool valid = true;
    for(int j = 0; j < code_len; j++)
    {
      valid = (isdigit(code[i][j])||isalpha(code[i][j])||code[i][j]=='_'); // condition 1
      if(!valid) break;
    }
    if(valid&&isActive[i]) // condition 3
    {
      // condition 2
      if(!strcmp(businessLine[i], "electronics"))
        q[q_count++] = i;
      else if(!strcmp(businessLine[i], "grocery"))
        q[q_count++] = i;
      else if(!strcmp(businessLine[i], "pharmacy"))
        q[q_count++] = i;
      else if(!strcmp(businessLine[i], "restaurant"))
        q[q_count++] = i;
    }
  }
  *returnSize = q_count;

  // use struct for multisort
  g_coupon = malloc(sizeof(Coupon));
  g_coupon->code = code;
  g_coupon->codeSize = codeSize;
  g_coupon->businessLine = businessLine;
  g_coupon->businessLineSize = businessLineSize;

  shell_sort(q, *returnSize, sizeof(int), coupon_cmp, coupon_swap);

  // copy the actual codes as result
  char** result = malloc(sizeof(char*)*(*returnSize));
  for(int i = 0; i < *returnSize; i++)
  {
    result[i] = malloc(sizeof(char)*(strlen(code[q[i]])+1)); // extra space for '\0'
    strcpy(result[i], code[q[i]]);
  }

  return result;
}


