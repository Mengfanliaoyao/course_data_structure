
 struct TreeNode {
     int val;
     struct TreeNode *left;
     struct TreeNode *right;
 };

#include<stdio.h>
#include<stdlib.h>

#include<memory.h>

#define MAX(a,b) ((a>b)?a:b)

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
int maxDepthRecursive(struct TreeNode* root)
{
  if(!root) return 0;
//  printf("%d ", root->val);
  return MAX(maxDepthRecursive(root->left), maxDepthRecursive(root->right)) + 1;
}

int maxDepth(struct TreeNode* root)
{
  if(!root) return 0;
  
  struct TreeNode** queue = (struct TreeNode**)malloc(10000 * sizeof(struct TreeNode*));
  int front = 0, rear = 0;
  queue[rear++] = root;
  
  int depth = 0;
  while(front < rear) // !queue.empty()
  {
    // iterate for all nodes of same level
    int current_level_count = rear - front;
    for(int i = 0; i < current_level_count; i++)
    {
      struct TreeNode* node = queue[front++];
      
      // if the node have children, push them into queue
      if(node->left) queue[rear++] = node->left;
      if(node->right) queue[rear++] = node->right;
    }
    // current level complete, level count + 1
    depth++;
  }
  free(queue);
  
  return depth;
}

int main(void)
{
  struct TreeNode* root = NULL;
  maxDepth(root);
  return 0;
}
