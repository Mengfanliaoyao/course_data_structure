#include<stdio.h>
#include<stdlib.h>

#include<stdbool.h>
#include<memory.h>

#include<math.h>

#include<limits.h>


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
        (char*)arr + j * data_size) > 0; j -= step) {
        swap((char*)arr + (j + step) * data_size, (char*)arr + j * data_size);
      }
    }
  }
}
#endif
#endif

#ifndef MACRO_CONCATENATE
#define MACRO_CONCATENATE
// macro concatenate for unique function signature of template

#define CONCATENATE_INTERNAL(x,y) x##y
#define CONCATENATE(x,y) CONCATENATE_INTERNAL(x,y)

#endif

// ==================== cstlList =========================(std::vector<int>)
#ifndef CSTL_LIST
#define CSTL_LIST List

#define TEMPLATE_NAME CONCATENATE(cstl,DYNAMIC_ARRAY)
#define FUNC(name) CONCATENATE(TEMPLATE_NAME, name)
#define DYNAMIC_ARRAY List
#define SIZE_T SizeT
#define TYPE_T int
typedef struct DYNAMIC_ARRAY DYNAMIC_ARRAY;
struct DYNAMIC_ARRAY
{
  TYPE_T* data;
  SIZE_T count;
  SIZE_T capacity;
  
  void (*init)(DYNAMIC_ARRAY* this);                           // keep bind this or it won't work
  void (*pushBack)(DYNAMIC_ARRAY* this, TYPE_T data);
  void (*pop)(DYNAMIC_ARRAY* this);
  void (*sort)(DYNAMIC_ARRAY* this, SIZE_T len);               // not stable
  void (*copyFrom)(DYNAMIC_ARRAY* this, DYNAMIC_ARRAY* other); // deep copy
  void (*clear)(DYNAMIC_ARRAY* this);                          // only disrecord data but not release space
  void (*delete)(DYNAMIC_ARRAY* this);                         // keep call this to avoid memory leak
};
DYNAMIC_ARRAY* FUNC(Create)();
#undef TYPE_T
#undef SIZE_T
#undef DYNAMIC_ARRAY
#undef FUNC
#undef TEMPLATE_NAME

#endif


// ==================== cstlHeap =========================(std::priorty_queue)
#ifndef CSTL_HEAP
#define CSTL_HEAP

#define TEMPLATE_NAME CONCATENATE(Cstl,HEAP)
#define HEAP Heap
#define FUNC(name) CONCATENATE(TEMPLATE_NAME, name)
#define NODE_T HeapNode
#define SIZE_T SizeT

typedef struct HeapNode HeapNode;
struct HeapNode
{
  int nodeId;
  double distance;
};

typedef struct HEAP HEAP;
struct HEAP
{
  NODE_T** data;
  SIZE_T count;
  SIZE_T capacity;
  Cmp cmp;
  void (*push)(HEAP* this, NODE_T* node);
  NODE_T* (*pop)(HEAP* this);
  bool (*isEmpty)(HEAP* this);
  void (*delete)(HEAP* this);
  void (*init)(HEAP* this, Cmp cmp);
};
HEAP* cstlHeapCreate(Cmp cmp);

#undef SIZE_T
#undef NODE_T
#undef FUNC
#undef HEAP
#undef TEMPLATE_NAME

#endif

// =============== specific struct ====================

typedef struct Node Node;
struct Node
{
  int id;
  int memory;
  List* links;
};

typedef struct Link Link;
struct Link
{
  int id;
  int end1, end2;
  int distance;
};

typedef struct Path Path;
struct Path
{
  List* nodeList;
  List* edgeList;
};
typedef struct PathList PathList;
struct PathList
{
  Path** paths;
  double final_distance; // less is better
  SizeT count;
  SizeT capacity;
};

typedef struct BinaryTreeNode BinaryTreeNode;
struct BinaryTreeNode
{
  SizeT height;
  int end1, end2;
  int through_node;
  BinaryTreeNode* left, *right;
};
typedef struct BinaryTree BinaryTree;
struct BinaryTree
{
  SizeT height;
  BinaryTreeNode* root;
};

typedef struct Task Task;
struct Task
{
  int id;
  int source, destination;
  PathList* paths;
  BinaryTree* tree;
};

// =============== global parameters ===================

SizeT node_count, link_count;
double alpha, beta, sucess_prob;
SizeT time_limit, task_count;

Node* nodes = NULL;
Link* links = NULL;
Task* tasks = NULL;

// ================== algorithms ======================

PathList* find_all_shortest_path(int start, int goal);
BinaryTree* build_baseline_binary_tree(const Path* path);
BinaryTree* build_balance_binary_tree(const Path* path, bool left_skew);
void delete_binary_tree(BinaryTree* tree);
int selectable_binary_tree(BinaryTree* tree, int* time_map);
void print_binary_tree(BinaryTree* tree, int base_time);

SizeT get_count(int task_id) {
  return task_id < task_count && tasks[task_id].paths && tasks[task_id].paths->count > 0 
  ? tasks[task_id].paths->count : (0ull - 1);
}
double get_distance(int task_id) {
  return task_id < task_count && tasks[task_id].paths && tasks[task_id].paths->count > 0 
  ? tasks[task_id].paths->final_distance : INF;
}
SizeT get_len(int task_id) {
  return task_id < task_count && tasks[task_id].paths && tasks[task_id].paths->count > 0 
  ? tasks[task_id].paths->paths[0]->edgeList->count : (0ull - 1);
}
sByteT task_cmp(const void* a, const void* b)
{
  int task_a = *(int*)a;
  int task_b = *(int*)b;
  SizeT count_a = get_count(task_a);
  SizeT count_b = get_count(task_b);
  if(count_a!=count_b) return (count_a<count_b)?1:-1; // min count
  SizeT len_a = get_len(task_a);
  SizeT len_b = get_len(task_b);
  if(len_a!=len_b) return (len_a<len_b)?1:-1; // min hop
  double dist_a = get_distance(task_a);
  double dist_b = get_distance(task_b);
  if(fabs(dist_a-dist_b)>1e-9) return (dist_a<dist_b)?1:-1; // min distance
  
  return 0; // the less distance the more prior the task 
}
void task_swap(void* a, void* b)
{
  int temp = *(int*)a;
  *(int*)a = *(int*)b;
  *(int*)b = temp;
}


#define DEBUG 0
#define BASELINE 0

int main(void)
{
  // input
  scanf("%llu%llu", &node_count, &link_count);
  scanf("%lf%lf%lf", &alpha, &beta, &sucess_prob);
  scanf("%llu%llu", &time_limit, &task_count);
  
  nodes = (Node*)malloc(node_count * sizeof(Node));
  for(int i = 0; i < node_count; i++)
  {
    nodes[i].links = cstlListCreate();
    scanf("%d%d", &nodes[i].id, &nodes[i].memory);
  }
  links = (Link*)malloc(link_count * sizeof(Link)); 
  for(int i = 0; i < link_count; i++)
  {
    scanf("%d%d%d%d", &i, &links[i].end1, &links[i].end2, &links[i].distance);
    // use adjacency lists
    int node_id = links[i].end1;
    nodes[node_id].links->pushBack(nodes[node_id].links, i);
    node_id = links[i].end2;
    nodes[node_id].links->pushBack(nodes[node_id].links, i);
  }

  List* task_queue = cstlListCreate();
  tasks = (Task*)malloc(task_count * sizeof(Task));
  for(int i = 0; i < task_count; i++)
  {
    scanf("%d%d%d", &tasks[i].id, &tasks[i].source, &tasks[i].destination);
    tasks[i].paths = find_all_shortest_path(tasks[i].source, tasks[i].destination);

#if DEBUG
    for(SizeT j = 0; j < tasks[i].paths->count; j++)
    {
      Path* p = tasks[i].paths->paths[j];
      printf("path%d: \n\t", (int)j);
      for(SizeT k = 0; k < p->edgeList->count; k++)
      {
        printf("%d%c", p->edgeList->data[k], (k==p->edgeList->count-1)?'\n':' ');
      }
    }
#endif
    if(tasks[i].paths->count&&tasks[i].paths->paths[0]->edgeList->count) // valid path exist
      task_queue->pushBack(task_queue, i);
    
  }
  // time-> x 1 2 3 ... time_limit
  // node
  //  |
  //  v
  //  0
  //  1
  //  .
  //  .
  //  .
  
  int* memory_map = malloc((time_limit + 1) * node_count * sizeof(int)); // for select tree
  for(SizeT i = 0; i < node_count; i++)
  {
    for(SizeT j = 0; j < time_limit + 1; j++)
    {
      memory_map[i * (time_limit + 1) + j] = nodes[i].memory;
    }
  }
  
  // sort tasks by less path -> less distance
  shell_sort(task_queue->data, task_queue->count, sizeof(int), task_cmp, task_swap);
#if DEBUG
  for(int i=0;i<task_queue->count;i++){
    printf("%d%c", task_queue->data[i], (i==task_queue->count-1)?'\n':' ');
  }
#endif
  List* select_task = cstlListCreate();
  List* select_base_time = cstlListCreate();
  for(SizeT i = 0; i < task_queue->count; i++)
  {
    int task_id = task_queue->data[i];
    int base_time;
#if BASELINE
    tasks[task_id].tree = build_baseline_binary_tree(tasks[task_id].paths->paths[0]);
#else
#endif
    for(SizeT j = 0; j < tasks[task_id].paths->count; j++)
    {
      Path* path = tasks[task_id].paths->paths[j];
      
      // search for right_skew balance tree
      tasks[task_id].tree = build_balance_binary_tree(path, 0);
      base_time = selectable_binary_tree(tasks[task_id].tree, memory_map);
      if(base_time >= 0)
      {
        select_task->pushBack(select_task, task_id);
        select_base_time->pushBack(select_base_time, base_time);
        break;
      }
      else
        delete_binary_tree(tasks[task_id].tree);
      #if 0
      // search for left_skew balance tree
      tasks[task_id].tree = build_balance_binary_tree(path, 1);
      base_time = selectable_binary_tree(tasks[task_id].tree, memory_map);
      if(base_time >= 0)
      {
        select_task->pushBack(select_task, task_id);
        select_base_time->pushBack(select_base_time, base_time);
        break;
      }
      else
        delete_binary_tree(tasks[task_id].tree);
#endif
    }
    
    
#if DEBUG
    print_binary_tree(tasks[task_id].tree, i);
#endif
#if BASELINE
    int base_time = time_limit - tasks[task_id].tree->height;
    if(base_time >= 0) // find
    {
      bool selectable = 1;
      for(SizeT j = 0; selectable && j < tasks[task_id].paths->paths[0]->nodeList->count; j++)
      {
        List* nodelist = tasks[task_id].paths->paths[0]->nodeList;
        if(nodes[nodelist->data[j]].memory < ((j==0||j==nodelist->count-1)?1:2))
          selectable = false;
      }
      if(selectable)
      {
        for(SizeT j = 0; selectable && j < tasks[task_id].paths->paths[0]->nodeList->count; j++)
        {
          List* nodelist = tasks[task_id].paths->paths[0]->nodeList;
          nodes[nodelist->data[j]].memory -= ((j==0||j==nodelist->count-1)?1:2);
        }
        select_task->pushBack(select_task, task_id);
        select_base_time->pushBack(select_base_time, base_time);
      }
    }
#endif
  }
  
  // print
  printf("%d\n", (int)select_task->count);
  for(SizeT i = 0; i < select_task->count; i++)
  {
    int task_id = select_task->data[i];
    printf("%d ", task_id);
    List* node_list = tasks[task_id].paths->paths[0]->nodeList;
    for(SizeT j = 0; j < node_list->count; j++)
      printf("%d%c", node_list->data[j], (j == node_list->count - 1)?'\n':' ');
    print_binary_tree(tasks[task_id].tree, select_base_time->data[i]);
  }
  
  return 0;
}


// ================== algorithms ======================

sByteT pq_cmp(const void* a, const void* b) // for heap
{
  const HeapNode* node1 = a;
  const HeapNode* node2 = b;
  if(fabs(node1->distance - node2->distance) < 1e-9) return 0; // equal
  
  return (node1->distance < node2->distance)?1:-1; // min heap
}
int shortest_path_len(int start, int goal)
{
  if(start == goal) return 0;
  
  if(start < 0 || start >= node_count || goal < 0 || goal >= node_count)
    return -1;
  
  int* queue = malloc(node_count * 2 * sizeof(int));
  bool* visited = calloc(node_count, sizeof(bool));
  int* distance = calloc(node_count, sizeof(int));
  
  int head = 0, rear = 0;
  queue[rear++] = start;
  visited[start] = true;
  distance[start] = 0;
  
  while(head < rear && rear < node_count * 2) {
    int u = queue[head++];
    
    for(int i = 0; i < nodes[u].links->count; i++) {
      int edge_id = nodes[u].links->data[i];
      if(edge_id < 0 || edge_id >= link_count) continue;
      
      Link* edge = &links[edge_id];
      int v = (u == edge->end1) ? edge->end2 : edge->end1;
      
      if(v < 0 || v >= node_count) continue;
      
      if(v == goal) {
        free(queue); free(visited); free(distance);
        return distance[u] + 1;
      }
      
      if(!visited[v] && nodes[v].memory > 1) {
        if(rear < node_count * 2) {
          queue[rear++] = v;
          distance[v] = distance[u] + 1;
        }
      }
      visited[v] = true;
    }
  }
  
  free(queue); free(visited); free(distance);
  return -1;  // 未找到路徑
}
// for reduce code
Path* copy_and_extend_path(Path* src, int new_node, int new_edge) {
  Path* new_path = malloc(sizeof(Path));
  new_path->nodeList = cstlListCreate();
  new_path->edgeList = cstlListCreate();
  new_path->nodeList->copyFrom(new_path->nodeList, src->nodeList);
  new_path->edgeList->copyFrom(new_path->edgeList, src->edgeList);
  new_path->nodeList->pushBack(new_path->nodeList, new_node);
  new_path->edgeList->pushBack(new_path->edgeList, new_edge);
  return new_path;
}
PathList* find_all_shortest_path(int start, int goal) // dijasktra
{
  PathList* result = malloc(sizeof(PathList));
  result->capacity = 0;
  result->count = 0;
    
  int min_len = 1000000000;//shortest_path_len(start, goal);
#if DEBUG
  printf("min len: %d\n", min_len);
#endif
  
  if(min_len==-1) return result; // empty pathlist
  
  // initialize
  Heap* pq = cstlHeapCreate(pq_cmp);
  
  double* distance = malloc(node_count * sizeof(double));
  for(int i = 0; i < node_count; i++)
    distance[i] = INF;
  distance[start] = 0;
  
  PathList* all_path = malloc(sizeof(PathList)); // actual paths
  all_path->capacity = 1;
  all_path->count = 0;
  all_path->paths = malloc(all_path->capacity * sizeof(Path*));
  Path* start_path = malloc(sizeof(Path));
  start_path->edgeList = cstlListCreate();
  start_path->nodeList = cstlListCreate();
  start_path->nodeList->pushBack(start_path->nodeList, start);
  
  List** path_cache = malloc(node_count * sizeof(List*)); // path_cache[i] record path ids of best pathes to node i
  for(SizeT i = 0; i <node_count; i++)
    path_cache[i] = cstlListCreate();
  path_cache[start]->pushBack(path_cache[start], all_path->count);
  all_path->paths[0] = start_path; // push start
  all_path->count++;
  
  int* path_len = calloc(node_count ,sizeof(int));
  bool* processed = calloc(node_count, sizeof(bool));
  
  HeapNode* start_node = malloc(sizeof(HeapNode));
  start_node->nodeId = start;
  start_node->distance = 0;
  pq->push(pq, start_node);
  
  while(!(pq->isEmpty(pq))) // dijasktra
  {
    HeapNode* node = pq->pop(pq);
    int u = node->nodeId;
    free(node);
    
    if(processed[u]) continue; // avoid repeat visit
    processed[u] = true;
    
    if(u == goal) break; // there are no other better path haven't visited
    
//    if(path_len[u] > min_len) continue;
#if DEBUG
    printf("processing: node%d\n", u);
#endif
    for(int i = 0; i < nodes[u].links->count; i++)
    {
      int e = nodes[u].links->data[i];
      Link* edge = &links[e];
      int v = (edge->end1 == u)?edge->end2:edge->end1;
#if DEBUG
      printf("find path%d, direct to: node%d with memory %d\n", e, v, nodes[v].memory);
#endif
      if(nodes[v].memory > 1 || v==goal) // else can't do any swap  
      {
        double new_dist = distance[u] + edge->distance + 128 * log(nodes[v].memory);// - log(sucess_prob); // shorter path, through nodes with more memory better
        int new_len = path_len[u] + 1;
#if DEBUG  
        printf("\tif select: path_len: %d, total_distance: %3lf\n", new_len, new_dist);
#endif
        if(new_dist < distance[v] || (fabs(new_dist - distance[v]) < 1e-9 && new_len < path_len[v])) // better dist or better path_len
        {
#if DEBUG
          printf("select node%d!\n", v);
#endif
          distance[v] = new_dist;
          path_len[v] = new_len;
          // update paths
          path_cache[v]->clear(path_cache[v]);
          // copy and push all paths u have into all path
          for(SizeT j = 0; j < path_cache[u]->count; j++)
          {
            int path_id = path_cache[u]->data[j];
            if(path_id >= all_path->count) continue;
            Path* new_path = copy_and_extend_path(all_path->paths[path_id], v, e);
            
            if(all_path->count==all_path->capacity)
            {
              all_path->capacity <<= 1;
              all_path->paths = realloc(all_path->paths, all_path->capacity * sizeof(Path*));
            }
            all_path->paths[all_path->count] = new_path;
            path_cache[v]->pushBack(path_cache[v], all_path->count);
            all_path->count++;
          }
          // push current node into heap
          HeapNode* next_node = malloc(sizeof(HeapNode));
          next_node->nodeId = v;
          next_node->distance = new_dist;
          pq->push(pq, next_node);
        }
        else if((fabs(new_dist - distance[v]) < 1e-9 && new_len == path_len[v])) // same dist and same path_len
        {
#if DEBUG
          printf("node%d seems good?\n", v);
#endif
          // copy and push all paths u have into all path
          for(SizeT i = 0; i < path_cache[u]->count; i++)
          {
            int path_id = path_cache[u]->data[i];
            Path* new_path = copy_and_extend_path(all_path->paths[path_id], v, e);
            
            if(all_path->count==all_path->capacity)
            {
              all_path->capacity <<= 1;
              all_path->paths = realloc(all_path->paths, all_path->capacity * sizeof(Path*));
            }
            all_path->paths[all_path->count] = new_path;
            path_cache[v]->pushBack(path_cache[v], all_path->count);
            all_path->count++;
          }
        }
      }
    }
  }
  // check for result
  SizeT goal_path_count = path_cache[goal]->count;
  if(goal_path_count > 0)
  {
    result->capacity = goal_path_count; 
    result->paths = malloc(result->capacity * sizeof(Path*));
    result->count = 0;
    
    // copy all paths could lead to goal
    for(SizeT i = 0; i < goal_path_count; i++)
    {
      int path_id = path_cache[goal]->data[i];
      if(path_id < all_path->count)
      {
        Path* new_path = malloc(sizeof(Path));
        new_path->nodeList = cstlListCreate();
        new_path->edgeList = cstlListCreate();
        
        new_path->nodeList->copyFrom(new_path->nodeList, all_path->paths[path_id]->nodeList);
        new_path->edgeList->copyFrom(new_path->edgeList, all_path->paths[path_id]->edgeList);
        
        result->paths[i] = new_path;
        result->count++;
#if DEBUG
        printf("path%lld: ", i);
        for(SizeT j = 0; j < new_path->nodeList->count; j++)
          printf("%d%c", new_path->nodeList->data[j], (j==new_path->nodeList->count - 1)?'\n':' ');
#endif
      }
    }
  }
  result->final_distance = distance[goal];
  
#if DEBUG
  printf("final_distance: %3lf\n", result->final_distance);
#endif
  // release all resource
  free(distance);
  for(SizeT i = 0; i < all_path->count; i++)
  {
    all_path->paths[i]->edgeList->delete(all_path->paths[i]->edgeList);
    all_path->paths[i]->nodeList->delete(all_path->paths[i]->nodeList);
    free(all_path->paths[i]);
  }
  free(all_path->paths);
  free(all_path);
  for(SizeT i = 0; i < node_count; i++)
    path_cache[i]->delete(path_cache[i]);
  free(path_cache);
  free(path_len);
  free(processed);
  pq->delete(pq);
  
  return result;
}
void height_check_binary_tree(BinaryTreeNode* root, SizeT root_height)
{
  root->height = root_height;
  if(root->left)
    height_check_binary_tree(root->left, root_height-1);
  if(root->right)
    height_check_binary_tree(root->right, root_height-1);
}
BinaryTreeNode* union_binary_tree(BinaryTreeNode* left, BinaryTreeNode* right)
{
  BinaryTreeNode* parent = malloc(sizeof(BinaryTreeNode));
  parent->height = MAX(left->height, right->height) + 1;
  parent->left = left;
  parent->right = right;
  
  if(left->end1==right->end1||left->end1==right->end2)
  {
    parent->through_node = left->end1;
    parent->end1 = left->end2;
    parent->end2 = (parent->through_node==right->end1)?(right->end2):(right->end1);
  }
  else
  {
    parent->end1 = left->end1;
    parent->through_node = left->end2;
    parent->end2 = (parent->through_node==right->end1)?(right->end2):(right->end1);
  }
  
  return parent;
}
BinaryTree* build_baseline_binary_tree(const Path* path)
{
  const List* nodelist = path->nodeList;
  int leaf_count = nodelist->count - 1;
  
  BinaryTreeNode** tree_nodes = malloc(leaf_count * sizeof(BinaryTreeNode*));
  
  for(SizeT i = 0; i < leaf_count; i++)
  {
    tree_nodes[i] = malloc(sizeof(BinaryTreeNode));
    tree_nodes[i]->end1 = nodelist->data[i];
    tree_nodes[i]->end2 = nodelist->data[i + 1];
    tree_nodes[i]->height = 1;
    tree_nodes[i]->left = NULL;
    tree_nodes[i]->right = NULL;
    tree_nodes[i]->through_node = -1;
  }
  
  /*
                                                  tree_root
  node0 node1 ... -> x parent1 node2 -> ... x parent(leaf_count-1)
  */
  for(SizeT i = 0; i < leaf_count - 1; i++)
  {
    tree_nodes[i+1] = union_binary_tree(tree_nodes[i], tree_nodes[i+1]);
  }
  
  BinaryTree* tree = malloc(sizeof(BinaryTree));
  tree->root = tree_nodes[leaf_count - 1];
  tree->height = leaf_count;
  
  height_check_binary_tree(tree->root, tree->height);
  free(tree_nodes);
  return tree;
}
BinaryTree* build_balance_binary_tree(const Path* path, bool left_skew)
{
  const List* nodelist = path->nodeList;
  int leaf_count = nodelist->count - 1;
  BinaryTreeNode** tree_nodes = malloc(leaf_count * sizeof(BinaryTreeNode*));
  
  for(SizeT i = 0; i < leaf_count; i++)
  {
    tree_nodes[i] = malloc(sizeof(BinaryTreeNode));
    tree_nodes[i]->end1 = nodelist->data[i];
    tree_nodes[i]->end2 = nodelist->data[i+1];
    tree_nodes[i]->height = 1;
    tree_nodes[i]->left = NULL;
    tree_nodes[i]->right = NULL;
    tree_nodes[i]->through_node = -1;
  }
  int rear = leaf_count - 1; // actual element index
  while(rear)
  {
    int front = (left_skew)?(rear%2):0; // if left 1 node in left skew, means 1 to rear is 2k 
    int new_rear = 0;
    while (front < rear) 
    {
      tree_nodes[new_rear++] = union_binary_tree(tree_nodes[front], tree_nodes[front + 1]);
      front += 2;
    }
    if (!left_skew && front == rear) // left 1 node
    {
      tree_nodes[new_rear++] = tree_nodes[front];
    } // left skew do nothing since remaining is at index 0 already
    rear = new_rear - 1;
  }
  
  BinaryTree* tree = malloc(sizeof(BinaryTree));
  tree->root = tree_nodes[0];
  tree->height = (int)ceil(log2(leaf_count)) + 1;
  height_check_binary_tree(tree->root, tree->height);
  free(tree_nodes);
  
  return tree;
}
void delete_binary_tree_node(BinaryTreeNode* root)
{
  if(root->left)
    delete_binary_tree_node(root->left);
  if(root->right)
    delete_binary_tree_node(root->right);
  free(root);
}
void delete_binary_tree(BinaryTree* tree)
{
  delete_binary_tree_node(tree->root);
  free(tree);
}
#if DEBUG
void print_map(int* map)
{
  printf("================\n");
  for(SizeT i = 0; i < node_count; i++)
  {
    for(SizeT j = 0; j < time_limit + 1; j++)
      printf("%d%c", map[i * (time_limit + 1) + j], (j==time_limit)?'\n':' ');
  }
  printf("================\n");
}
#endif
SizeT count_tree_node(BinaryTreeNode* root)
{
  if(!root) return 0; // nothing
  return 1 + count_tree_node(root->left) + count_tree_node(root->right);
}
int selectable_binary_tree(BinaryTree* tree, int* time_map)
{
  ByteT* occupancy_count = calloc(node_count, sizeof(ByteT));
  ByteT* leaf_remain_count = calloc(node_count, sizeof(ByteT));
  
  SizeT tree_node_count = count_tree_node(tree->root) + 1;
  // level order traversal
  BinaryTreeNode** queue = malloc(tree_node_count * sizeof(BinaryTreeNode*));
  for(SizeT i = time_limit - tree->height ; i > 0; i--) // h-height binary tree need at least h time slots, e.g. timeslot 4 could serve 4-height tree
  {
    memset(occupancy_count, 0, node_count * sizeof(ByteT));
    memset(leaf_remain_count, 0, node_count * sizeof(ByteT));
    
    int head = 0, rear = 0;
    int level = 1, current_level_count = 1, next_level_count = 0;
    queue[rear++] = tree->root;
    bool selectable = true;
    while(head<rear)
    {
      if(!current_level_count) // enter next level
      {
        // check for this level
        for(SizeT j = 0; j < node_count; j++)
        {
          if(time_map[j * (time_limit + 1) + (i + tree->height - level + 1)] < occupancy_count[j])
          {
            selectable = false;
            break;
          }
          if(level == tree->height) // need extra check for entangling
          {
            if(time_map[j * (time_limit + 1) + (i + tree->height - level)] < leaf_remain_count[j])
            {
              selectable = false;
              break;
            }
          }
        }
        // update level state
        memcpy(occupancy_count, leaf_remain_count, node_count * sizeof(ByteT));
        memset(leaf_remain_count, 0, node_count * sizeof(ByteT));
        current_level_count = next_level_count;
        next_level_count = 0;
        level++;
      }
      if(!selectable) break;
      
      BinaryTreeNode* t = queue[head++]; // dequeue
      current_level_count--;
      occupancy_count[t->end1]++;
      occupancy_count[t->end2]++;
      if(t->through_node!=-1)
        occupancy_count[t->through_node]+=2;
      // enqueue
      if(t->left)
      {
        next_level_count++;
        queue[rear++] = t->left;
      }
      if(t->right)
      {
        next_level_count++;
        queue[rear++] = t->right;
      }
      if(!t->left&&!t->right) // leaf
      {
        leaf_remain_count[t->end1]++;
        leaf_remain_count[t->end2]++;
      }
    }
    for(SizeT j = 0; j < node_count; j++) // check for leaf
    {
      if(time_map[j * (time_limit + 1) + (i + tree->height - level + 1)] < occupancy_count[j])
      {
        selectable = false;
        break;
      }
      if(level == tree->height) // need extra check for entangling
      {
        if(time_map[j * (time_limit + 1) + (i + tree->height - level)] < leaf_remain_count[j])
        {
          selectable = false;
          break;
        }
      }
    }
    if(selectable)
    {
      memset(occupancy_count, 0, node_count * sizeof(ByteT));
      memset(leaf_remain_count, 0, node_count * sizeof(ByteT));
      
      head = 0, rear = 1;
      level = 1, current_level_count = 1, next_level_count = 0;
      while(head<rear)
      {
        if(!current_level_count) // enter next level
        {
          // check for this level
          for(SizeT j = 0; j < node_count; j++)
            time_map[j * (time_limit + 1) + (i + tree->height - level + 1)] -= occupancy_count[j];
          
#if DEBUG
            printf("time: %d\n", (int)(i + tree->height - level + 1));
            print_map(time_map);
#endif
          // update level state
          memcpy(occupancy_count, leaf_remain_count, node_count * sizeof(ByteT));
          memset(leaf_remain_count, 0, node_count * sizeof(ByteT));
          current_level_count = next_level_count;
          level++;
          next_level_count = 0;
        }
        BinaryTreeNode* t = queue[head++];
        current_level_count--;
        occupancy_count[t->end1]++;
        occupancy_count[t->end2]++;
        if(t->through_node!=-1)
          occupancy_count[t->through_node]+=2;
        if(t->left)
        {
          next_level_count++;
          rear++;
        }
        if(t->right)
        {
          next_level_count++;
          rear++;
        }
        if(!t->left&&!t->right) // leaf
        {
          leaf_remain_count[t->end1]++;
          leaf_remain_count[t->end2]++;
        }
      }
      for(SizeT j = 0; j < node_count; j++) // check for leaf nodes
      {
        time_map[j * (time_limit + 1) + (i + tree->height - level + 1)] -= occupancy_count[j];
        time_map[j * (time_limit + 1) + (i + tree->height - level)] -= leaf_remain_count[j];
      }
#if DEBUG
      print_map(time_map);
#endif
      return i;
    }
  }
  return -1;
}
void print_binary_tree_node(BinaryTreeNode* root, int base_time)
{
  if(root->left)
    print_binary_tree_node(root->left, base_time);
  if(root->right)
    print_binary_tree_node(root->right, base_time);
#if DEBUG
  printf("node height: %d\n", (int)root->height);
#endif
  if(root->through_node!=-1)
    printf("%d %d %d %d\n", root->end1, root->end2, root->through_node, (int)root->height + base_time);
  else // leaf
    printf("%d %d %d\n", root->end1, root->end2, (int)root->height + base_time);
}
void print_binary_tree(BinaryTree* tree, int base_time)
{
#if DEBUG
  printf("tree: %p\n", tree);
#endif
  print_binary_tree_node(tree->root, base_time);
}

// ==================== cstlList =========================(std::vector<int>)
#ifdef CSTL_LIST
#ifndef CSTL_LIST_INTERNAL
#define CSTL_LIST_INTERNAL
#define TEMPLATE_NAME CONCATENATE(cstl,DYNAMIC_ARRAY)
#define DYNAMIC_ARRAY List
#define FUNC(name) CONCATENATE(TEMPLATE_NAME, name)
#define SIZE_T SizeT
#define TYPE_T int

void FUNC(Realloc)(DYNAMIC_ARRAY* this, SIZE_T new_capacity);
void FUNC(Init)(DYNAMIC_ARRAY* this); // keep bind this or it won't work
void FUNC(PushBack)(DYNAMIC_ARRAY* this, TYPE_T data);
void FUNC(Pop)(DYNAMIC_ARRAY* this);
void FUNC(Sort)(DYNAMIC_ARRAY* this, SIZE_T len);
void FUNC(CopyFrom)(DYNAMIC_ARRAY* this, DYNAMIC_ARRAY* other); // deep copy
void FUNC(Clear)(DYNAMIC_ARRAY* this);
void FUNC(Delete)(DYNAMIC_ARRAY* this);

DYNAMIC_ARRAY* FUNC(Create)()
{
  DYNAMIC_ARRAY* list = (DYNAMIC_ARRAY*)malloc(sizeof(DYNAMIC_ARRAY));
  list->init = FUNC(Init);
  list->pushBack = FUNC(PushBack);
  list->pop = FUNC(Pop);
  list->sort = FUNC(Sort);
  list->copyFrom = FUNC(CopyFrom);
  list->clear = FUNC(Clear);
  list->delete = FUNC(Delete);
  
  list->count = 0;
  list->capacity = 0;
  list->data = NULL;
  
  return list;
}
void FUNC(Realloc)(DYNAMIC_ARRAY* this, SIZE_T new_capacity)
{
  this->data = realloc(this->data, sizeof(TYPE_T) * new_capacity);
  this->capacity = new_capacity;
}
void FUNC(Init)(DYNAMIC_ARRAY* this) // keep bind this or it won't work
{
  this->count = 0;
  this->capacity = 0;
  this->data = NULL;
  
  this->init = FUNC(Init);
  this->pushBack = FUNC(PushBack);
  this->pop = FUNC(Pop);
  this->sort = FUNC(Sort);
  this->copyFrom = FUNC(CopyFrom);
  this->clear = FUNC(Clear);
  this->delete = FUNC(Delete);
}
void FUNC(PushBack)(DYNAMIC_ARRAY* this, TYPE_T data)
{
  
  if((this->count+1)>=this->capacity)
  {
    this->capacity = (!this->capacity)?1:(this->capacity<<1);
    FUNC(Realloc)(this, this->capacity);
  }
  
  this->data[this->count++] = data;
}
void FUNC(Pop)(DYNAMIC_ARRAY* this)
{
  if(this->count)
    this->count--;
}
void FUNC(Sort)(DYNAMIC_ARRAY* this, SizeT len)
{
  for (int step = len >> 1; step >= 1; step = step >> 1) {
    for (int start = step; start < len; start++) {
      for (int j = start - step; j >= 0 && this->data[j + step] < this->data[j]; j -= step) {
        TYPE_T temp = this->data[j];
        this->data[j] = this->data[j+step];
        this->data[j+step] = temp;
      }
    }
  }
}
void FUNC(CopyFrom)(DYNAMIC_ARRAY* this, DYNAMIC_ARRAY* other) // deep copy
{
  if(this->capacity<other->count) // need extra space
    FUNC(Realloc)(this, other->count);
  // else no need to modify capacity
  memcpy(this->data, other->data, sizeof(TYPE_T)*other->count);
  this->count = other->count;
}
void FUNC(Clear)(DYNAMIC_ARRAY* this)
{
  this->count = 0;
}
void FUNC(Delete)(DYNAMIC_ARRAY* this)
{
  if(this->count)
    FUNC(Clear)(this);
  free(this->data);
  this->data = NULL;
}
#undef TYPE_T
#undef FUNC
#undef SIZE_T
#undef DYNAMIC_ARRAY
#undef TEMPLATE_NAME

#endif
#endif


// ==================== cstlHeap =========================(std::priorty_queue)
#define TEMPLATE_NAME CONCATENATE(Cstl,HEAP)
#define HEAP Heap
#define FUNC(name) CONCATENATE(TEMPLATE_NAME, name)
#define NODE_T HeapNode
#define SIZE_T SizeT

void FUNC(Swap)(HEAP* this, SIZE_T index1, SIZE_T index2)
{
  NODE_T* temp = this->data[index1];
  this->data[index1] = this->data[index2];
  this->data[index2] = temp;
}
void FUNC(HeapifyUp)(HEAP* this, SIZE_T index)
{
  while(index>1){
    SIZE_T parent = index>>1;
    if(!parent) break;
    
    NODE_T** t = this->data;
    if(this->cmp(t[parent], t[index]) < 0) // child win
    {
      FUNC(Swap)(this, parent, index);
      index = parent;
    }
    else break;
  }
}
void FUNC(HeapifyDown)(HEAP* this, SIZE_T index)
{
  while(1){
    SIZE_T left = (index<<1);
    SIZE_T right = left+1;
    SIZE_T winner = index;
    
    NODE_T** t = this->data;
    if(left<=this->count&&this->cmp(t[left],t[winner]) > 0) // left win
      winner = left;
    if(right<=this->count&&this->cmp(t[right],t[winner]) > 0) // right win
      winner = right;
    
    if(winner!=index)
    {
      FUNC(Swap)(this, index, winner);
      index = winner;
    }
    else
      break;
  }
}

void FUNC(Push)(HEAP* this, NODE_T* node)
{
  this->count++;
  if(this->count>=this->capacity) // run out of space
  {
    this->capacity=(this->capacity==0)?(4):(this->capacity<<1);
    this->data = realloc(this->data, this->capacity * sizeof(NODE_T*));
  }
  // insert node at the end
  this->data[this->count] = node;
  // heapify up
  FUNC(HeapifyUp)(this, this->count);  
}
NODE_T* FUNC(Pop)(HEAP* this)
{
  if(!this->capacity) abort();
  // return top element
  NODE_T* top = this->data[1];
  // move last element to top
  this->data[1] = this->data[this->count--]; // replace top by the end of heap and remove the end of heap
  // heapify down
  FUNC(HeapifyDown)(this, 1);
  
  return top;
}
bool FUNC(IsEmpty)(HEAP* this) {return (this->count==0);}
void FUNC(Delete)(HEAP* this) // need call init if want reuse
{
  this->count = 0;
  this->capacity = 0;
  free(this->data);
}
void FUNC(Init)(HEAP* this, Cmp cmp)
{
	this->data = NULL;
	this->count = 0;
	this->capacity = 0;
  this->cmp = cmp;
	this->push = FUNC(Push);
	this->pop = FUNC(Pop);
	this->isEmpty = FUNC(IsEmpty);
	this->delete = FUNC(Delete);
	this->init = FUNC(Init);
}

HEAP* cstlHeapCreate(Cmp cmp)
{
  HEAP* heap = malloc(sizeof(HEAP));
  heap->data = NULL;
  heap->count = 0;
  heap->capacity = 0;
  heap->cmp = cmp;
  heap->push = FUNC(Push);
  heap->pop = FUNC(Pop);
  heap->isEmpty = FUNC(IsEmpty);
  heap->delete = FUNC(Delete);
  heap->init = FUNC(Init);
  return heap;
}
#undef SIZE_T
#undef NODE_T
#undef FUNC
#undef HEAP
#undef TEMPLATE_NAME
