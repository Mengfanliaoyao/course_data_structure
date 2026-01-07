#include<stdio.h>
#include<stdlib.h>

#include<memory.h>
#include<stdbool.h>

#include<math.h>

typedef int Int32T;
typedef long long int int64_t;
typedef unsigned int uInt32T;
typedef unsigned long long int uint64_t;
typedef uint64_t SizeT;



typedef struct Position Pos;
struct Position
{
  int x, y;
};

double distance(Pos p1, Pos p2)
{
  int x = abs(p1.x - p2.x);
  int y = abs(p1.y - p2.y);
  return sqrt(x * x + y * y);
}

typedef struct RIS RIS;
struct RIS
{
  int id;
  Pos pos;
  int* coveredUDs;
  double dist_to_qan; // distance to QAN
};

typedef struct UserDevice UD;
struct UserDevice
{
  // user provide
  int id;
  Pos pos;
  int profit;
  int expectRate;
  double fidilityThreshold;
  // for selection (only choose best)
  SizeT cost; // s(u,r)
  double weight; // profit/cost
  bool isSelectQAN;
  RIS* selectedRIS; // only one could be chosen
};

#define CONCATENATE_INTERNAL(x,y) x##y
#define CONCATENATE(x,y) CONCATENATE_INTERNAL(x,y)


#define TEMPLATE_NAME CONCATENATE(Cstl,HASH_TABLE)
#define HASH_TABLE UDMap
#define FUNC(name) CONCATENATE(TEMPLATE_NAME, name)
#define KEY_T uInt32T
#define MAX_SIZE 1000003u
#define TYPE_T UD
#define NODE_T UDNode
typedef struct NODE_T NODE_T;
struct NODE_T
{
  TYPE_T data;
  NODE_T* next;
};

typedef struct HASH_TABLE HASH_TABLE;
struct HASH_TABLE // hashtable
{
  NODE_T** data;
  SizeT size;
  
  NODE_T* (*find)(HASH_TABLE* this, KEY_T key);
  NODE_T* (*insert)(HASH_TABLE* this, KEY_T key, TYPE_T* node);
  void (*erase)(HASH_TABLE* this, KEY_T key);
  void (*clear)(HASH_TABLE* this);
  void (*delete)(HASH_TABLE* this); // keep call this after the use of unordered_map end
  void (*init)(HASH_TABLE* this); // keep call this or the unordered_map won't work
};
// internal functions
uInt32T FUNC(hash_knuth)(KEY_T key) { return (key * 2654435761u) % MAX_SIZE; }
NODE_T* FUNC(Find)(HASH_TABLE* this, KEY_T key)
{
  uInt32T index = FUNC(hash_knuth)(key);
  NODE_T* now = this->data[index];
  if(!now) // find no element
    return NULL;
  
  while(now)
  {
    if(now->data.id==key)
      return now;
    now = now->next;
  }
  return NULL;
}
NODE_T* FUNC(Insert)(HASH_TABLE* this, KEY_T key, TYPE_T* node)
{
  uInt32T index = FUNC(hash_knuth)(key);
  NODE_T* now = this->data[index];
  NODE_T* prev = NULL;
  while(now) // add node behind all nodes of the index
  {
    if(now->data.id==key)
      return NULL; // already have data, insert failed
    prev = now;
    now = now->next;
  }
  // now is always NULL
  
  NODE_T* new_node = (NODE_T*)malloc(sizeof(NODE_T));
  new_node->next = NULL;
  now = new_node;
  if(prev) // avoid for the case that this->[index] is NULL
    prev->next = now;
  else
    this->data[index] = now;
  
  now->data.id = node->id;
  now->data.pos.x = node->pos.x;
  now->data.pos.y = node->pos.y;
  now->data.profit = node->profit;
  now->data.expectRate = node->expectRate;
  now->data.fidilityThreshold = node->fidilityThreshold;
  now->data.cost = node->cost;
  now->data.weight = node->weight;
  now->data.isSelectQAN = node->isSelectQAN;
  now->data.selectedRIS = node->selectedRIS;
  
  this->size++;
  
  return now;
}
void FUNC(Erase)(HASH_TABLE* this, KEY_T key)
{
  uInt32T index = FUNC(hash_knuth)(key);
  NODE_T* now = this->data[index];
  NODE_T* prev = NULL;
  while(now)
  {
    if(now->data.id==key) // find the data
    {
      NODE_T* next = now->next; // if still exist data behind now then record it
      if(prev)
        prev->next = next; // if now->next is NULL then NULL
      else
        this->data[index] = next;
      
      this->size--;
      
      free(now);
      return;
    }
    prev = now;
    now = now->next;
  }
  // find no data, erase failed
}
void FUNC(Clear)(HASH_TABLE* this)
{
  for(SizeT i = 0; i < MAX_SIZE; i++)
  {
    NODE_T* now = this->data[i];
    NODE_T* next = NULL;
    while(now)
    {
      next = now->next;
      free(now);
      now = next;
    }
    this->data[i] = NULL;
  }
  this->size = 0;
}
void FUNC(Delete)(HASH_TABLE* this)
{
  if(this->size)
    FUNC(Clear)(this);
  free(this->data);
}
void FUNC(Init)(HASH_TABLE* this) // keep bind this or it won't work
{
  this->find = FUNC(Find);
  this->insert = FUNC(Insert);
  this->erase = FUNC(Erase);
  this->clear = FUNC(Clear);
  this->delete = FUNC(Delete);
  
  this->size = 0;
  this->data = (NODE_T**)malloc(sizeof(NODE_T*)*MAX_SIZE);
  
  memset(this->data, 0, sizeof(NODE_T*)*MAX_SIZE);
}
#undef NODE_T
#undef TYPE_T
#undef MAX_SIZE
#undef KEY_T
#undef FUNC
#undef HASH_TABLE
#undef TEMPLATE_NAME

#define TEMPLATE_NAME CONCATENATE(Cstl,HASH_TABLE)
#define HASH_TABLE IndexMap
#define FUNC(name) CONCATENATE(TEMPLATE_NAME, name)
#define KEY_T Int32T
#define MAX_SIZE 1000003u
#define TYPE_T SizeT
#define NODE_T IndexNode
typedef struct NODE_T NODE_T;
struct NODE_T
{
  KEY_T key;
  TYPE_T value;
  NODE_T* next;
};

typedef struct HASH_TABLE HASH_TABLE;
struct HASH_TABLE // hashtable
{
  NODE_T** data;
  SizeT size;
  
  NODE_T* (*find)(HASH_TABLE* this, KEY_T key);
  NODE_T* (*insert)(HASH_TABLE* this, KEY_T key, TYPE_T* node);
  void (*erase)(HASH_TABLE* this, KEY_T key);
  void (*clear)(HASH_TABLE* this);
  void (*delete)(HASH_TABLE* this); // keep call this after the use of unordered_map end
  void (*init)(HASH_TABLE* this); // keep call this or the unordered_map won't work
};
// internal functions
uInt32T FUNC(hash_knuth)(KEY_T key) { return (key * 2654435761u) % MAX_SIZE; }
NODE_T* FUNC(Find)(HASH_TABLE* this, KEY_T key)
{
  uInt32T index = FUNC(hash_knuth)(key);
  NODE_T* now = this->data[index];
  if(!now) // find no element
    return NULL;
  
  while(now)
  {
    if(now->key==key)
      return now;
    now = now->next;
  }
  return NULL;
}
NODE_T* FUNC(Insert)(HASH_TABLE* this, KEY_T key, TYPE_T* node)
{
  uInt32T index = FUNC(hash_knuth)(key);
  NODE_T* now = this->data[index];
  NODE_T* prev = NULL;
  while(now) // add node behind all nodes of the index
  {
    if(now->key==key)
      return NULL; // already have data, insert failed
    prev = now;
    now = now->next;
  }
  // now is always NULL
  
  NODE_T* new_node = (NODE_T*)malloc(sizeof(NODE_T));
  new_node->next = NULL;
  now = new_node;
  if(prev) // avoid for the case that this->[index] is NULL
    prev->next = now;
  else
    this->data[index] = now;
  
  now->key = key;
  
  this->size++;
  
  return now;
}
void FUNC(Erase)(HASH_TABLE* this, KEY_T key)
{
  uInt32T index = FUNC(hash_knuth)(key);
  NODE_T* now = this->data[index];
  NODE_T* prev = NULL;
  while(now)
  {
    if(now->key==key) // find the data
    {
      NODE_T* next = now->next; // if still exist data behind now then record it
      if(prev)
        prev->next = next; // if now->next is NULL then NULL
      else
        this->data[index] = next;
      
      this->size--;
      
      free(now);
      return;
    }
    prev = now;
    now = now->next;
  }
  // find no data, erase failed
}
void FUNC(Clear)(HASH_TABLE* this)
{
  for(SizeT i = 0; i < MAX_SIZE; i++)
  {
    NODE_T* now = this->data[i];
    NODE_T* next = NULL;
    while(now)
    {
      next = now->next;
      free(now);
      now = next;
    }
    this->data[i] = NULL;
  }
  this->size = 0;
}
void FUNC(Delete)(HASH_TABLE* this)
{
  if(this->size)
    FUNC(Clear)(this);
  free(this->data);
}
void FUNC(Init)(HASH_TABLE* this) // keep bind this or it won't work
{
  this->find = FUNC(Find);
  this->insert = FUNC(Insert);
  this->erase = FUNC(Erase);
  this->clear = FUNC(Clear);
  this->delete = FUNC(Delete);
  
  this->size = 0;
  this->data = (NODE_T**)malloc(sizeof(NODE_T*)*MAX_SIZE);
  
  memset(this->data, 0, sizeof(NODE_T*)*MAX_SIZE);
}
#undef NODE_T
#undef TYPE_T
#undef MAX_SIZE
#undef KEY_T
#undef FUNC
#undef HASH_TABLE
#undef TEMPLATE_NAME

#define TEMPLATE_NAME CONCATENATE(Cstl,DYNAMIC_ARRAY)
#define DYNAMIC_ARRAY List
#define FUNC(name) CONCATENATE(TEMPLATE_NAME, name)
#define TYPE_T int
typedef struct DYNAMIC_ARRAY DYNAMIC_ARRAY;
struct DYNAMIC_ARRAY
{
  TYPE_T* data;
  SizeT size;
  SizeT capacity;
  void (*pushBack)(DYNAMIC_ARRAY* this, TYPE_T data);
  void (*pop)(DYNAMIC_ARRAY* this);
  void (*clear)(DYNAMIC_ARRAY* this);
  void (*delete)(DYNAMIC_ARRAY* this);
  void (*init)(DYNAMIC_ARRAY* this);
};
TYPE_T* FUNC(Realloc)(DYNAMIC_ARRAY* this, SizeT new_capacity)
{
  
}
void FUNC(PushBack)(DYNAMIC_ARRAY* this, TYPE_T data)
{
  
}
void FUNC(Pop)(DYNAMIC_ARRAY* this)
{
  
}
void FUNC(Clear)(DYNAMIC_ARRAY* this)
{
  
}
void FUNC(Delete)(DYNAMIC_ARRAY* this)
{
  
}
void FUNC(Init)(DYNAMIC_ARRAY* this) // keep bind this or it won't work
{
  this->size = 0;
  this->capacity = 2;
  this->data = (TYPE_T*)malloc(sizeof(TYPE_T) * this->capacity);
  
  this->pushBack = FUNC(PushBack);
  this->pop = FUNC(Pop);
  this->clear = FUNC(Clear);
  this->delete = FUNC(Delete);
}
#undef TYPE_T
#undef FUNC
#undef DYNAMIC_ARRAY
#undef TEMPLATE_NAME

typedef struct SelectedList SelectedList;
struct SelectedList
{
  List userList;
  List risList;
  SizeT profit;
};


// global parameters
int amountUD, amountRIS;
double alpha, beta;
int entangle_generate_rate;

Pos qan_pos;
int covered_UD_amount;
int* covered_UDs = NULL;



// functions for calculate s(u,r)
double probability_entangling(UD* user, RIS* ris) // Pe(u,r)
{
  static double e = 0;
  e = exp(-alpha);

  double dist;
  if(ris==NULL) // direct connect to qan
    dist = distance(user->pos, qan_pos);
  else
    dist = distance(user->pos, ris->pos) + ris->dist_to_qan;
  
  return e*exp(dist);
}
double fidelity_entangling(UD* user, RIS* ris) // Fe(u,r)
{
  static double e = 0;
  e = exp(-beta) / 2;
  
  double dist;
  if(ris==NULL) // direct connect to qan
    dist = distance(user->pos, qan_pos);
  else
    dist = distance(user->pos, ris->pos) + ris->dist_to_qan;
  
  return 0.5 + e * (exp(dist) / 2);
}
double probability_purification(double f1, double f2) // Pp(F1, F2)
{
  return 2*f1*f2-f1-f2+1;
}
double fidelity_purification(double f1, double f2) // Fp(F1, F2)
{
  return f1*f2/probability_purification(f1, f2);
}
SizeT source_consumption(UD* user, RIS* ris) // s(u,r)
{
  const double f_e = fidelity_entangling(user, ris);
  const double p_e = probability_entangling(user, ris);
  SizeT n = 0;
  // calculate n(u,r) and pP(u,r,n) at the same time
  double f = f_e;
  double p = 1;
  while(f >= user->fidilityThreshold){
    p = p * probability_purification(f, f_e);
    f = f*f_e/p;
    n++;
    // assert: n very big
  }
  
  return (SizeT)ceil((user->expectRate / p_e) * ((n + 1) / p));
}


// sorting functions
void sort_by_weight(UDMap* uidmap, List* uidlist, IndexMap index_map, SizeT len)
{
  
}
void sort_by_cost(UDMap* uidmap, List* uidlist, IndexMap index_map, SizeT len)
{
  
}


int main(void)
{
  // initialize
  UDMap uid_map;
  uid_map.init = CstlUDMapInit;
  uid_map.init(&uid_map); // keep call it!
  
  IndexMap user_index_map;
  user_index_map.init = CstlIndexMapInit;
  user_index_map.init(&user_index_map);
  
  List user_queue_weight, user_queue_cost;
  user_queue_weight.init = CstlListInit;
  user_queue_cost.init = CstlListInit;
  user_queue_weight.init(&user_queue_weight);
  user_queue_cost.init(&user_queue_cost);
  
  SelectedList current_list, best_list;
  current_list.userList.init = CstlListInit;
  current_list.risList.init = CstlListInit;
  best_list.userList.init = CstlListInit;
  best_list.risList.init = CstlListInit;
  current_list.userList.init(&current_list.userList);
  current_list.risList.init(&current_list.risList);
  best_list.userList.init(&best_list.userList);
  best_list.risList.init(&best_list.risList);
  
  // input global parameters
  scanf("%d%d", &amountUD, &amountRIS);
  scanf("%lf%lf", &alpha, &beta);
  scanf("%d", &entangle_generate_rate);
  
  // input QAN datas
  scanf("%d%d", &qan_pos.x, &qan_pos.y);
  scanf("%d", &covered_UD_amount);
  if(covered_UD_amount)
  {
    covered_UDs = malloc(covered_UD_amount*sizeof(int));
    for(SizeT i = 0; i < covered_UD_amount; i++)
      scanf("%d", &covered_UDs[i]);
  }
  
  // input for UDs
  UD* users = NULL;
  if(amountUD)
  {
    users = malloc(amountUD * sizeof(UD));
    for(SizeT i = 0; i < amountUD; i++)
    {
      UD* u = &users[i];
      scanf("%d%d%d%d%d%lf", &(u->id), &(u->pos.x), &(u->pos.y), &(u->profit), &(u->expectRate), &(u->fidilityThreshold));
      // unordered_map
      uid_map.insert(&uid_map, u->id, u);
    }
    free(users); // since every operation after are on UDMap, this could free
  }
  users = NULL;
  // calculate for those could directly link to QAN
  for(SizeT i = 0; i < covered_UD_amount; i++)
  {
    UDNode* n = uid_map.find(&uid_map, covered_UDs[i]);
    UD* u = NULL;
    if(n) // find
    {
      u = &(n->data);
      // calculate for s(u,r)
      SizeT cost = source_consumption(u, NULL);
      if(u->cost > entangle_generate_rate) continue; // could not select
      
      u->cost = cost;
      u->weight = u->profit/cost;
      u->isSelectQAN = true;
    }
  }
  // sort by weight and filt out those impossible users
  
  // input for RISs
  RIS* risurfaces = NULL;
  if(amountRIS)
  {
    risurfaces = malloc(amountRIS * sizeof(RIS));
    for(SizeT i = 0; i < amountRIS; i++)
    {
      RIS* r = &risurfaces[i];
      int covered_amount;
      scanf("%d%d%d%d", &(r->id), &(r->pos.x), &(r->pos.y), &covered_amount);
      r->coveredUDs = malloc(covered_amount*sizeof(UD*));
      r->dist_to_qan = distance(r->pos, qan_pos);
      for(SizeT j = 0; j < covered_amount; j++)
      {
        int uid;
        scanf("%d", &uid);
        r->coveredUDs[j] = uid;
        UDNode* n = uid_map.find(&uid_map, uid);
        if(n) // find
        {
          // calculate for UDs' s(u,r) and if it want to choose this RIS
          UD* u = &n->data;
          SizeT cost = source_consumption(u, r);
          if(cost>entangle_generate_rate) continue; // could not select this ris
          
          bool better = (cost < u->cost)?true:false;
          if(better) // user only select best qan/ris for self 
          {
            // update the selection of user
            u->cost = cost;
            u->weight = u->profit/cost;
            u->isSelectQAN = false;
            u->selectedRIS = r;
          }
        }
      }
      // sort by weight and filt out those impossible users
      
    }
  }
  
  // select
  if(amountUD*amountRIS<1000000) // dp
  {
    
  }
  else // heuristic
  {
    
  }
  
  // print
  SizeT num = best_list.userList.size;
  printf("%llu\n", num);
  for(SizeT i = 0; i < num; i++)
  {
    printf("%d %d\n", best_list.userList.data[i], best_list.risList.data[i]);
  }
  
  // end
  uid_map.delete(&uid_map); // avoid for memory leak
  user_index_map.delete(&user_index_map);
  user_queue_weight.delete(&user_queue_weight);
  user_queue_cost.delete(&user_queue_cost);
  current_list.userList.delete(&current_list.userList);
  current_list.risList.delete(&current_list.risList);
  best_list.userList.delete(&best_list.userList);
  best_list.risList.delete(&best_list.risList);
  free(risurfaces);
  risurfaces = NULL;
  
  return 0;
}
