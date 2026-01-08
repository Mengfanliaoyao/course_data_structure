
#include<stdio.h>
#include<stdlib.h>

#include<memory.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

int gcd(int a, int b)
{
  if(a<b) return gcd(b,a);
  int r;
  do
  {
    r = a%b;
    if(r==1) return 1;
    a = b;
    b = r;
  }while(r>0);
  return a;
}

struct ListNode* insertGreatestCommonDivisors(struct ListNode* head)
{
  struct ListNode* now = head;
  struct ListNode* last = NULL;
  while(now)
  {
    if(last)
    {
      struct ListNode* new_node = (struct ListNode*)malloc(sizeof(struct ListNode));
      new_node->val = gcd(last->val, now->val);
      last->next = new_node;
      new_node->next = now;
    }
    last = now;
    now = now->next;
  }
  return head;
}

int main(void)
{
  return 0;
}
