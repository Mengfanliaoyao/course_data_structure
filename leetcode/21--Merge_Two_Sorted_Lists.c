#include<stdio.h>
#include<stdlib.h>



struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2)
{
  struct ListNode* head = list1?(list2?((list1->val<list2->val)?list1:list2):list1):list2;
  struct ListNode* li1 = (list1&&head==list1)?list1->next:list1;
  struct ListNode* li2 = (list2&&head==list2)?list2->next:list2;
  
  struct ListNode* now = head;
  
  while(li1&&li2) // merge till any list become empty
  {
    char flag = (li1->val<li2->val);
//    printf("%d %p %p\n", flag, li1, li2);
    now->next = flag?li1:li2;
    li1 = flag?li1->next:li1;
    li2 = flag?li2:li2->next;
    now = now->next;
  }
  if(!li1&&now)
    now->next = li2;
  else if(!li2&&now)
    now->next = li1;
  
  return head;
}

int main(void)
{
  int data1[] = {1,2,3};
  struct ListNode* list1 = malloc(sizeof(data1)/sizeof(int)*sizeof(struct ListNode));
  for(int i = 0; i < sizeof(data1)/sizeof(int); i++)
  {
    (list1+i)->val = data1[i];
    (list1+i)->next = (i==sizeof(data1)/sizeof(int)-1)?NULL:(list1+i+1);
  }
  int data2[] = {1,3,4};
  struct ListNode* list2 = malloc(sizeof(data2)/sizeof(int)*sizeof(struct ListNode));
  for(int i = 0; i < sizeof(data2)/sizeof(int); i++)
  {
    (list2+i)->val = data2[i];
    (list2+i)->next = (i==sizeof(data2)/sizeof(int)-1)?NULL:(list2+i+1);
  }
  struct ListNode* result = mergeTwoLists(list1, list2);
  struct ListNode* iter = result;
  while(iter)
  {
    printf("%d ", iter->val);
    iter = iter->next;
  }
}
