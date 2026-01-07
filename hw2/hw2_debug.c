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

#define INF 1.0/0.0

#define MAX(a, b) (a>b)?a:b
#define MIN(a, b) (a<b)?a:b

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
#undef FUNC
#undef SIZE_T
#undef DYNAMIC_ARRAY
#undef TEMPLATE_NAME

#endif


// ==================== cstlHeap =========================(std::priorty_queue)
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
BinaryTree* build_balance_binary_tree(const Path* path);
void delete_binary_tree(BinaryTree* tree);
int selectable_binary_tree(BinaryTree* tree, int* time_map);
void print_binary_tree(BinaryTree* tree, int base_time);

#define CHECK 1
#if CHECK
#define MAXN 10000

int adj[MAXN][MAXN];
int visited[MAXN];
int parent[MAXN];
int dist[MAXN];
int dist2[MAXN][MAXN];
int disabled_repeater[MAXN] = {0};

int hop[MAXN];
int visited2[MAXN];

void find_path(int start)
{
  
  for (int i = 0; i < node_count; i++)
  {
    hop[i] = INT_MAX;
    visited[i] = 0;
    parent[i] = -1;
  }
  
  int queue[MAXN];
  int front = 0, rear = 0;
  
  hop[start] = 0;
  visited[start] = 1;
  queue[rear++] = start;
  
  while (front < rear)
  {
    int u = queue[front++];
    
    for (int v = 0; v < node_count; v++)
    {
      if (adj[u][v] > 0 && !visited[v] && !disabled_repeater[v])
      {
        visited[v] = 1;
        hop[v] = hop[u] + 1;
        queue[rear++] = v;
      }
    }
  }
  
  for (int i = 0; i < node_count; i++)
  {
    dist[i] = INT_MAX;
    visited2[i] = 0;
    parent[i] = -1;
  }
  dist[start] = 0;
  
  for (int iter = 0; iter < node_count; iter++)
  {
    int u = -1, min_d = INT_MAX;
    for (int i = 0; i < node_count; i++)
    {
      if (!visited2[i] && hop[i] != INT_MAX && dist[i] < min_d)
      {
        min_d = dist[i];
        u = i;
      }
    }
    if (u == -1)
    {
      break;
    }
    visited2[u] = 1;
    
    for (int v = 0; v < node_count; v++)
    {
      
      if (adj[u][v] > 0 && !visited2[v] && hop[v] == hop[u] + 1 && !disabled_repeater[v])
      {
        if (dist[u] + adj[u][v] < dist[v])
        {
          dist[v] = dist[u] + adj[u][v];
          parent[v] = u;
        }
      }
    }
  }
}

int real_path[MAXN][MAXN];
int real_length[MAXN];

void reverse(int end, int ID)
{
  int path[MAXN], len = 0;
  for (int cur = end; cur != -1; cur = parent[cur])
  {
    path[len++] = cur;
  }
  int j = 0;
  for (int i = len - 1; i >= 0; i--)
  {
    real_path[ID][j++] = path[i];
  }
  real_length[ID] = j;
}
#endif

#define DEBUG 0


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
#if CHECK
  for(int i = 0; i < node_count; i++)
    for(int j = 0; j < node_count; j++)
      adj[i][j] = 0;
#endif
  links = (Link*)malloc(link_count * sizeof(Link)); 
  for(int i = 0; i < link_count; i++)
  {
    scanf("%d%d%d%d", &i, &links[i].end1, &links[i].end2, &links[i].distance);
#if CHECK
    adj[links[i].end1][links[i].end2] = links[i].distance;
    adj[links[i].end2][links[i].end1] = links[i].distance;
#endif
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
#if CHECK
    find_path(tasks[i].source);
    reverse(tasks[i].destination, i);
    PathList* pl = malloc(sizeof(PathList));
    pl->paths = malloc(sizeof(Path*));
    pl->count = 1;
    pl->capacity = 1;
    pl->paths[0] = malloc(sizeof(Path));
    pl->paths[0]->nodeList = cstlListCreate();
    pl->paths[0]->edgeList = cstlListCreate();
    int last_node_id = 0;
    for(int j = 0; j < real_length[i]; j++)
    {
      pl->paths[0]->nodeList->pushBack(pl->paths[0]->nodeList, real_path[i][j]);
      if(j)
      {
        int node_id = real_path[i][j];
        for(int k = 0; k < nodes[node_id].links->count; k++)
        {
          int eid = nodes[node_id].links->data[k];
          if(last_node_id == links[eid].end1 && node_id == links[eid].end2)
          {
            pl->paths[0]->edgeList->pushBack(pl->paths[0]->edgeList, eid);
          }
        }
      }
      last_node_id = real_path[i][j];
    }
    tasks[i].paths = pl;
#else
    tasks[i].paths = find_all_shortest_path(tasks[i].source, tasks[i].destination);
#endif
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
    if(tasks[i].paths->count)
      task_queue->pushBack(task_queue, i);
    
  }
  
  int* memory_map = malloc((time_limit + 1) * node_count * sizeof(int));
  for(SizeT i = 0; i < node_count; i++)
  {
    for(SizeT j = 0; j < time_limit + 1; j++)
    {
      memory_map[i * (time_limit + 1) + j] = nodes[i].memory;
    }
  }
  
  // sort tasks by less path -> less distance
  List* select_task = cstlListCreate();
  List* select_base_time = cstlListCreate();
  for(SizeT i = 0; i < task_queue->count; i++)
  {
    tasks[i].tree = build_balance_binary_tree(tasks[i].paths->paths[0]);
#if DEBUG
    print_binary_tree(tasks[i].tree, i);
#endif
    int base_time = selectable_binary_tree(tasks[i].tree, memory_map);
    if(base_time) // find
    {
      select_task->pushBack(select_task, i);
      select_base_time->pushBack(select_base_time, base_time);
    }
  }
  
  printf("%d\n", (int)select_task->count);
  for(SizeT i = 0; i < select_task->count; i++)
  {
    int task_id = select_task->data[i];
    printf("%d ", task_id);
    List* node_list = tasks[task_id].paths->paths[0]->nodeList;
    for(SizeT j = 0; j < node_list->count; j++)
      printf("%d%c", node_list->data[j], (j == node_list->count - 1)?'\n':' ');
    print_binary_tree(tasks[i].tree, select_base_time->data[i]);
  }
  
  return 0;
}


// ================== algorithms ======================

sByteT pq_cmp(const void* a, const void* b)
{
  const HeapNode* node1 = a;
  const HeapNode* node2 = b;
  if(fabs(node1->distance - node2->distance) < 1e-9) return 0; // equal
  
  return (node1->distance < node2->distance)?1:-1; // min heap
}

  int shortest_path_len(int start, int goal)
  {
    if (start == goal) return 0;
    
    int* queue = malloc(node_count * sizeof(int));
    bool* visited = calloc(node_count, sizeof(bool));
    int head = 0, rear = 0;
    
    queue[rear] = start;
    rear = (rear+1)%node_count;
    
    visited[start] = true;
    int dist = 0;  // current_layer
    
    while (head < rear) {
      int layer_size = rear - head;
      
      for (int i = 0; i < layer_size; i++) {
        int u = queue[head];
        head = (head+1)%node_count;
        for (int j = 0; j < nodes[u].links->count; j++) {
          Link* edge = &links[nodes[u].links->data[j]];
          int v = (u == edge->end1) ? edge->end2 : edge->end1;
          
#if DEBUG
          printf("node%d to node%d dist:%d\n", u, v, dist + 1);
#endif
          
          if (v == goal) {
            free(queue);
            free(visited);
            return dist + 1;
          }
          
          if (!visited[v]) {
            visited[v] = true;
            queue[rear] = v;
            rear = (rear+1)%node_count;
          }
        }
      }
      dist++;  // current layer complete
    }
    
    free(queue);
    free(visited);
    return -1;  // find no path
  }
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
PathList* find_all_shortest_path(int start, int goal)
{
  PathList* result = malloc(sizeof(PathList));
  result->capacity = 0;
  result->count = 0;
  
  int min_len = 10000000;// shortest_path_len(start, goal);
  
  if(min_len==-1) return result; // empty pathlist
  
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
  all_path->paths[0] = start_path;
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
    
    if(processed[u]) continue;
    processed[u] = true;
    
    if(u == goal) break;
#if DEBUG
    printf("processing: node%d\n", u);
#endif
    if(path_len[u] > min_len) continue;
    
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
        double new_dist = distance[u] + edge->distance;// - log(sucess_prob) - 2 * log(nodes[v].memory); // shorter path, through nodes with more memory better
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
  SizeT goal_path_count = path_cache[goal]->count;
  if(goal_path_count > 0)
  {
    result->capacity = goal_path_count; 
    result->paths = malloc(result->capacity * sizeof(Path*));
    result->count = 0;
    
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
BinaryTree* build_balance_binary_tree(const Path* path)
{
  const List* edgelist = path->edgeList;
  BinaryTreeNode** tree_nodes = malloc(edgelist->count * sizeof(BinaryTreeNode*));
  for(SizeT i = 0; i < edgelist->count; i++)
    tree_nodes[i] = malloc(sizeof(BinaryTreeNode));
  for(SizeT i = 0; i < edgelist->count; i++)
  {
    int edge_id = edgelist->data[i];
    Link* edge = &links[edge_id];
    tree_nodes[i]->end1 = edge->end1;
    tree_nodes[i]->end2 = edge->end2;
    tree_nodes[i]->height = 1;
    tree_nodes[i]->left = NULL;
    tree_nodes[i]->right = NULL;
    tree_nodes[i]->through_node = -1;
  }
  int rear = edgelist->count - 1;
  while(rear)
  {
    int front = 0, new_rear = 0;
    while(front < rear){
      tree_nodes[new_rear++] = union_binary_tree(tree_nodes[front], tree_nodes[front + 1]);
      front += 2;
    }
    if(front==rear) // left 1 node
      tree_nodes[new_rear++] = tree_nodes[rear];
    rear = new_rear - 1; // actual element count
  }
  BinaryTree* tree = malloc(sizeof(BinaryTree));
  // verify ?
  tree->root = tree_nodes[0];
  tree->height = (int)ceil(log2(edgelist->count)) + 1;
  tree_nodes[0]->height = tree->height;
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
void select_binary_tree_node(BinaryTreeNode* root, int* time_map, int base_time)
{
  time_map[root->end1 * (time_limit + 1) + (root->height + base_time)] -= 1;
  time_map[root->end2 * (time_limit + 1) + (root->height + base_time)] -= 1;
  if(root->through_node!=-1)
    time_map[root->through_node * (time_limit + 1) + (root->height + base_time)] -= 2;
    
  if(root->left)
    select_binary_tree_node(root->left, time_map, base_time);
  if(root->right)
    select_binary_tree_node(root->left, time_map, base_time);
}
bool selectable_binary_tree_node(BinaryTreeNode* root, int* time_map, int base_time)
{
  bool selectable = 1;
  selectable &= (time_map[root->end1 * (time_limit + 1) + (root->height + base_time)] > 1);
#if DEBUG
  printf("node: %d, time: %lld, %s\n", root->end1, (root->height + base_time), (selectable)?"selectable":"error");
#endif
  selectable &= (time_map[root->end2 * (time_limit + 1) + (root->height + base_time)] > 1);
#if DEBUG
  printf("node: %d, time: %lld, %s\n", root->end2, (root->height + base_time), (selectable)?"selectable":"error");
#endif
  if(root->through_node!=-1)
  {
    selectable &= (time_map[root->through_node * (time_limit + 1) + (root->height + base_time)] > 2);
#if DEBUG
    printf("node: %d, time: %lld, %s\n", root->through_node, (root->height + base_time), (selectable)?"selectable":"error");
#endif
    selectable &= (time_map[root->through_node * (time_limit + 1) + (root->height - 1 + base_time - 2)] > 2); // also check for child layer
#if DEBUG
    printf("node: %d, time: %lld, %s\n", root->through_node, (root->height - 1 + base_time), (selectable)?"selectable":"error");
#endif
  }
  if(!selectable) return false;
  
  if(root->left)
    selectable &= selectable_binary_tree_node(root->left, time_map, base_time);
  if(root->right)
    selectable &= selectable_binary_tree_node(root->left, time_map, base_time);
  
  return selectable;
}
int selectable_binary_tree(BinaryTree* tree, int* time_map)
{
  ByteT* occupancy_count = calloc(node_count, sizeof(ByteT));
  ByteT* leaf_remain_count = calloc(node_count, sizeof(ByteT));
  
  // level order traversal
  BinaryTreeNode** queue = malloc((1<<tree->height) * 2 * sizeof(BinaryTreeNode*)); // for h-height binary tree, there are mostly 2^(h+1) nodes
  for(SizeT i = 1; i < time_limit - tree->height + 1; i++) // h-height binary tree need at least h time slots, e.g. timeslot 4 could serve 4-height tree
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
      
      BinaryTreeNode* t = queue[head++];
      current_level_count--;
      occupancy_count[t->end1]++;
      occupancy_count[t->end2]++;
      if(t->through_node!=-1)
        occupancy_count[t->through_node]+=2;
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
  return 0;
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
  else
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
