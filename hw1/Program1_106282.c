#include<stdio.h>
#include<stdlib.h>

#include<memory.h>
#include<stdbool.h>

#include<math.h>

#define DEBUG 0 // for debug

// use C++ like style

typedef int Int32T;
typedef long long int Int64T;
typedef unsigned int uInt32T;
typedef unsigned long long int uInt64T;
typedef uInt64T SizeT;
typedef uInt64T BitsetT;


// for unique function signature

#define CONCATENATE_INTERNAL(x,y) x##y
#define CONCATENATE(x,y) CONCATENATE_INTERNAL(x,y)


// std::vector<int>

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
  void (*copyFrom)(DYNAMIC_ARRAY* this, DYNAMIC_ARRAY* other);
  void (*clear)(DYNAMIC_ARRAY* this);
  void (*delete)(DYNAMIC_ARRAY* this);
  void (*init)(DYNAMIC_ARRAY* this);
};
void FUNC(Realloc)(DYNAMIC_ARRAY* this, SizeT new_capacity)
{
  if(new_capacity==this->capacity) return; // invalid operation
  if(new_capacity>this->capacity)
  {
    TYPE_T* new_data = (TYPE_T*)malloc(sizeof(TYPE_T)*new_capacity);
    memcpy(new_data, this->data, sizeof(TYPE_T) * this->size);
    free(this->data);
    this->data = new_data;
    new_data = NULL;
    this->capacity = new_capacity;
  }
}
void FUNC(PushBack)(DYNAMIC_ARRAY* this, TYPE_T data)
{
  if(this->size==this->capacity)
    FUNC(Realloc)(this, this->capacity*2);
  this->data[this->size++] = data;
}
void FUNC(Pop)(DYNAMIC_ARRAY* this)
{
  if(this->size)
    this->size--;
}
void FUNC(CopyFrom)(DYNAMIC_ARRAY* this, DYNAMIC_ARRAY* other)
{
  if(this->capacity<other->capacity) // need extra space
    FUNC(Realloc)(this, other->capacity);
  // else no need to modify capacity
  memcpy(this->data, other->data, sizeof(TYPE_T)*other->size);
  this->size = other->size;
}
void FUNC(Clear)(DYNAMIC_ARRAY* this)
{
  this->size = 0;
}
void FUNC(Delete)(DYNAMIC_ARRAY* this)
{
  if(this->size)
    FUNC(Clear)(this);
  free(this->data);
  this->data = NULL;
}
void FUNC(Init)(DYNAMIC_ARRAY* this) // keep bind this or it won't work
{
  this->size = 0;
  this->capacity = 1;
  this->data = (TYPE_T*)malloc(sizeof(TYPE_T) * this->capacity);
  memset(this->data, 0x80000000, sizeof(TYPE_T) * this->capacity);
  
  this->pushBack = FUNC(PushBack);
  this->pop = FUNC(Pop);
  this->copyFrom = FUNC(CopyFrom);
  this->clear = FUNC(Clear);
  this->delete = FUNC(Delete);
  this->init = FUNC(Init);
}
#undef TYPE_T
#undef FUNC
#undef DYNAMIC_ARRAY
#undef TEMPLATE_NAME

// std::vector<size_t>

#define TEMPLATE_NAME CONCATENATE(Cstl,DYNAMIC_ARRAY)
#define DYNAMIC_ARRAY LongList
#define FUNC(name) CONCATENATE(TEMPLATE_NAME, name)
#define TYPE_T SizeT
typedef struct DYNAMIC_ARRAY DYNAMIC_ARRAY;
struct DYNAMIC_ARRAY
{
  TYPE_T* data;
  SizeT size;
  SizeT capacity;
  void (*pushBack)(DYNAMIC_ARRAY* this, TYPE_T data);
  void (*pop)(DYNAMIC_ARRAY* this);
  void (*copyFrom)(DYNAMIC_ARRAY* this, DYNAMIC_ARRAY* other);
  void (*clear)(DYNAMIC_ARRAY* this);
  void (*delete)(DYNAMIC_ARRAY* this);
  void (*init)(DYNAMIC_ARRAY* this);
};
void FUNC(Realloc)(DYNAMIC_ARRAY* this, SizeT new_capacity)
{
  if(new_capacity==this->capacity) return; // invalid operation
  if(new_capacity>this->capacity)
  {
    TYPE_T* new_data = (TYPE_T*)malloc(sizeof(TYPE_T)*new_capacity);
    memcpy(new_data, this->data, sizeof(TYPE_T) * this->size);
    free(this->data);
    this->data = new_data;
    new_data = NULL;
    this->capacity = new_capacity;
  }
}
void FUNC(PushBack)(DYNAMIC_ARRAY* this, TYPE_T data)
{
  this->size++;
  if(this->size==this->capacity)
    FUNC(Realloc)(this, this->capacity*2);
  this->data[this->size - 1] = data;
}
void FUNC(Pop)(DYNAMIC_ARRAY* this)
{
  this->size--;
}
void FUNC(CopyFrom)(DYNAMIC_ARRAY* this, DYNAMIC_ARRAY* other)
{
  if(this->capacity<other->capacity) // need extra space
    FUNC(Realloc)(this, other->capacity);
  // else no need to modify capacity
  memcpy(this->data, other->data, sizeof(TYPE_T)*other->size);
  this->size = other->size;
}
void FUNC(Clear)(DYNAMIC_ARRAY* this)
{
  this->size = 0;
}
void FUNC(Delete)(DYNAMIC_ARRAY* this)
{
  if(this->size)
    FUNC(Clear)(this);
  free(this->data);
  this->data = NULL;
}
void FUNC(Init)(DYNAMIC_ARRAY* this) // keep bind this or it won't work
{
  this->size = 0;
  this->capacity = 1;
  this->data = (TYPE_T*)malloc(sizeof(TYPE_T) * this->capacity);
  memset(this->data, 0xFFFFFFFF, sizeof(TYPE_T) * this->capacity);
  
  this->pushBack = FUNC(PushBack);
  this->pop = FUNC(Pop);
  this->copyFrom = FUNC(CopyFrom);
  this->clear = FUNC(Clear);
  this->delete = FUNC(Delete);
  this->init = FUNC(Init);
}
#undef TYPE_T
#undef FUNC
#undef DYNAMIC_ARRAY
#undef TEMPLATE_NAME


// bit operation for bitmask

#define BIT(i) (0x1<<i)

// structures for the quetion

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
  List coveredUDs;
  LongList coveredUDsCost;
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
  List selectableRIS;
  LongList selectableRISCost;
};

// structures for algorithm

typedef struct SelectedList SelectedList;
struct SelectedList
{
  List userList;
  List risList;
  SizeT cost;
  SizeT profit;
};

typedef struct DpSelection DpSelection;
struct DpSelection
{
  BitsetT* bitsMask; // record if the ris/user is selected
  SelectedList selectedList;
};

// for find if certain index of dp_map has selected or not, if selected then which users/riss been selected

#define TEMPLATE_NAME CONCATENATE(Cstl,HASH_TABLE)
#define HASH_TABLE DpIndexMap
#define FUNC(name) CONCATENATE(TEMPLATE_NAME, name)
#define KEY_T SizeT // the index of dp sheet
#if DEBUG
#define MAX_SIZE 97u
#else
#define MAX_SIZE 1000003u
#endif
#define TYPE_T DpSelection
#define NODE_T DpIndexNode
typedef struct NODE_T NODE_T;
struct NODE_T
{
  KEY_T key;
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
  NODE_T* (*transfer)(HASH_TABLE* this, KEY_T old, KEY_T new); // transfer the old data from key1 to key2
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
NODE_T* FUNC(Insert)(HASH_TABLE* this, KEY_T key, TYPE_T* node) // rab the node ptr from param
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
  if(prev) // avoid for the case that this->[index] is NULL
    prev->next = new_node;
  else
    this->data[index] = new_node;
  
  // record node
  new_node->key = key;
  new_node->data = *node;
  node = NULL;
  
  this->size++;
  
  now = new_node;
  return now;
}
NODE_T* FUNC(Transfer)(HASH_TABLE* this, KEY_T old, KEY_T new)
{
  // find the node of old one;
  uInt32T index = FUNC(hash_knuth)(old);
  NODE_T* now = this->data[index];
  NODE_T* prev = NULL;
  NODE_T* new_node = NULL;
  while(now)
  {
    if(now->key==old) // find the data
    {
      NODE_T* next = now->next; // if still exist data behind now then record it
      if(prev)
        prev->next = next; // if now->next is NULL then NULL
      else
        this->data[index] = next;
      
      this->size--;
      
      // insert old node to new key
      new_node = FUNC(Insert)(this, new, &now->data);
      
      // no need to clean for now->data since insert rab the data
      free(now);
      now = NULL;
      break;
    }
    prev = now;
    now = now->next;
  }
  return new_node;
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
      
      // clean for all heap allocate data
      free(now->data.bitsMask);
      now->data.selectedList.userList.delete(&now->data.selectedList.userList);
      now->data.selectedList.risList.delete(&now->data.selectedList.risList);
      free(now);
      now = NULL;
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
      // clean for all heap allocate data
      free(now->data.bitsMask);
      now->data.selectedList.userList.delete(&now->data.selectedList.userList);
      now->data.selectedList.risList.delete(&now->data.selectedList.risList);
      free(now);
      
      now = next;
    }
    this->data[i] = NULL;
  }
  this->size = 0;
  memset(this->data, 0, sizeof(NODE_T*)*MAX_SIZE);
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
  this->transfer = FUNC(Transfer);
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


// global parameters
int amountUD, amountRIS;
double alpha, beta;
int entangle_generate_rate;

Pos qan_pos;
int covered_UD_amount;
List covered_UDs;



// functions for calculate s(u,r)
double probability_entangling(UD* user, RIS* ris) // Pe(u,r)
{
  double dist;
  if(ris==NULL) // direct connect to qan
    dist = distance(user->pos, qan_pos);
  else
    dist = distance(user->pos, ris->pos) + ris->dist_to_qan;
  
  return exp(-alpha*dist);
}
double fidelity_entangling(UD* user, RIS* ris) // Fe(u,r)
{
  double dist;
  if(ris==NULL) // direct connect to qan
    dist = distance(user->pos, qan_pos);
  else
    dist = distance(user->pos, ris->pos) + ris->dist_to_qan;
  
  return 0.5 + (exp(-beta*dist) / 2);
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
  double cost = user->expectRate/p_e;
  while(f < user->fidilityThreshold){
    p = p * probability_purification(f, f_e);
    f = f*f_e/p;
    n++;
    // assert: n very big
    cost *= (n+1)/p;
    if(cost>entangle_generate_rate)
      return (SizeT)0xFFFFFFFFFFFFFFFF;
    cost /= (n+1)/p;
  }
  
  return (SizeT)ceil((user->expectRate / p_e) * ((n + 1) / p));
}


// sorting functions for try

typedef void(*UDSortFunc)(const UD* const, List*);
#define TEMPLATE_NAME CONCATENATE(Cstl,DYNAMIC_ARRAY)
#define DYNAMIC_ARRAY UDSortFuncList
#define FUNC(name) CONCATENATE(TEMPLATE_NAME, name)
#define TYPE_T UDSortFunc
typedef struct DYNAMIC_ARRAY DYNAMIC_ARRAY;
struct DYNAMIC_ARRAY
{
  TYPE_T* data;
  SizeT size;
  SizeT capacity;
  void (*pushBack)(DYNAMIC_ARRAY* this, TYPE_T data);
  void (*pop)(DYNAMIC_ARRAY* this);
  void (*copyFrom)(DYNAMIC_ARRAY* this, DYNAMIC_ARRAY* other);
  void (*clear)(DYNAMIC_ARRAY* this);
  void (*delete)(DYNAMIC_ARRAY* this);
  void (*init)(DYNAMIC_ARRAY* this);
};
void FUNC(Realloc)(DYNAMIC_ARRAY* this, SizeT new_capacity)
{
  if(new_capacity==this->capacity) return; // invalid operation
  if(new_capacity>this->capacity)
  {
    TYPE_T* new_data = (TYPE_T*)malloc(sizeof(TYPE_T)*new_capacity);
    memcpy(new_data, this->data, sizeof(TYPE_T) * this->size);
    free(this->data);
    this->data = new_data;
    new_data = NULL;
    this->capacity = new_capacity;
  }
}
void FUNC(PushBack)(DYNAMIC_ARRAY* this, TYPE_T data)
{
  if(this->size==this->capacity)
    FUNC(Realloc)(this, this->capacity*2);
  this->data[this->size++] = data;
}
void FUNC(Pop)(DYNAMIC_ARRAY* this)
{
  if(this->size)
    this->size--;
}
void FUNC(CopyFrom)(DYNAMIC_ARRAY* this, DYNAMIC_ARRAY* other)
{
  if(this->capacity<other->capacity) // need extra space
    FUNC(Realloc)(this, other->capacity);
  // else no need to modify capacity
  memcpy(this->data, other->data, sizeof(TYPE_T)*other->size);
  this->size = other->size;
}
void FUNC(Clear)(DYNAMIC_ARRAY* this)
{
  this->size = 0;
}
void FUNC(Delete)(DYNAMIC_ARRAY* this)
{
  if(this->size)
    FUNC(Clear)(this);
  free(this->data);
  this->data = NULL;
}
void FUNC(Init)(DYNAMIC_ARRAY* this) // keep bind this or it won't work
{
  this->size = 0;
  this->capacity = 1;
  this->data = (TYPE_T*)malloc(sizeof(TYPE_T) * this->capacity);
  memset(this->data, 0, sizeof(TYPE_T) * this->capacity);
  
  this->pushBack = FUNC(PushBack);
  this->pop = FUNC(Pop);
  this->copyFrom = FUNC(CopyFrom);
  this->clear = FUNC(Clear);
  this->delete = FUNC(Delete);
  this->init = FUNC(Init);
}
#undef TYPE_T
#undef FUNC
#undef DYNAMIC_ARRAY
#undef TEMPLATE_NAME

// sorting functions

void sort_rid_of_users_by_cost(const UD* const users, int uid)
{
  uInt32T len = users[uid].selectableRIS.size;
  for (int step = len >> 1; step >= 1; step = step >> 1) {
    for (int start = step; start < len; start++) {
      const List* const ridlist = &users[uid].selectableRIS;
      const LongList* const costlist = &users[uid].selectableRISCost;
      for (int j = start - step; j >= 0 && j + step < len ; j -= step) 
      {
        if((costlist->data[j + step] < costlist->data[j] || (costlist->data[j + step] == costlist->data[j] && ridlist->data[j+step]==-1)))
        {
          int temp = ridlist->data[j + step];
          ridlist->data[j + step] = ridlist->data[j];
          ridlist->data[j] = temp;
          SizeT temp1 = costlist->data[j + step];
          costlist->data[j + step] = costlist->data[j];
          costlist->data[j] = temp1;
        }
      }
    }
  }
}
void sort_users_by_weight(const UD* const users, List* uidlist)
{
  uInt32T len = uidlist->size;
  for (int step = len >> 1; step >= 1; step = step >> 1) {
    for (int start = step; start < len; start++) {
      for (int j = start - step; j >= 0 && j + step < len; j -= step) {
        int uid1 = uidlist->data[j];
        int uid2 = uidlist->data[j+step];
        const LongList* const costlist1 = &users[uid1].selectableRISCost;
        double weight1 = users[uid1].profit/costlist1->data[0];
        const LongList* const costlist2 = &users[uid2].selectableRISCost;
        double weight2 = users[uid2].profit/costlist2->data[0];
        if(weight2>weight1||costlist1->data[0]==-1) // long long max
        {
          int temp = uidlist->data[j + step];
          uidlist->data[j + step] = uidlist->data[j];
          uidlist->data[j] = temp;
        }
      }
    }
  }
}
void sort_users_by_profit(const UD* const users, List* uidlist)
{
  uInt32T len = uidlist->size;
  for (int step = len >> 1; step >= 1; step = step >> 1) {
    for (int start = step; start < len; start++) {
      for (int j = start - step; j >= 0 && j + step < len; j -= step) {
        int uid1 = uidlist->data[j];
        int uid2 = uidlist->data[j+step];
        if(users[uid1].profit<users[uid2].profit)
        {
          int temp = uidlist->data[j + step];
          uidlist->data[j + step] = uidlist->data[j];
          uidlist->data[j] = temp;
        }
      }
    }
  }
}
void sort_users_by_cost(const UD* const users, List* uidlist)
{
  uInt32T len = uidlist->size;
  for (int step = len >> 1; step >= 1; step = step >> 1) {
    for (int start = step; start < len; start++) {
      for (int j = start - step; j >= 0 && j + step < len; j -= step) {
        int uid1 = uidlist->data[j];
        int uid2 = uidlist->data[j+step];
        if(users[uid1].cost<users[uid2].cost)
        {
          int temp = uidlist->data[j + step];
          uidlist->data[j + step] = uidlist->data[j];
          uidlist->data[j] = temp;
        }
      }
    }
  }
}
void sort_users_by_uid(const UD* const users, List* uidlist)
{
  uInt32T len = uidlist->size;
  for (int step = len >> 1; step >= 1; step = step >> 1) {
    for (int start = step; start < len; start++) {
      for (int j = start - step; j >= 0 && j + step < len; j -= step) {
        int uid1 = uidlist->data[j];
        int uid2 = uidlist->data[j+step];
        if(uid1<uid2)
        {
          int temp = uidlist->data[j + step];
          uidlist->data[j + step] = uidlist->data[j];
          uidlist->data[j] = temp;
        }
      }
    }
  }
}
void sort_qan_covered_users(const UD* const users, List* uid_list, LongList* cost_list)
{
  uInt32T len = uid_list->size;
  for (int step = len >> 1; step >= 1; step = step >> 1) {
    for (int start = step; start < len; start++) {
      for (int j = start - step; j >= 0 && j + step < len ; j -= step) 
      {
        const UD* const u1 = &users[uid_list->data[j]];
        double weight1 = u1->profit/cost_list->data[j];
        const UD* const u2 = &users[uid_list->data[j+step]];
        double weight2 = u2->profit/cost_list->data[j+step];
        if(weight2 > weight1)
        {
          int temp = uid_list->data[j + step];
          uid_list->data[j + step] = uid_list->data[j];
          uid_list->data[j] = temp;
          SizeT temp1 = cost_list->data[j + step];
          cost_list->data[j + step] = cost_list->data[j];
          cost_list->data[j] = temp1;
        }
      }
    }
  }
}
void sort_users_of_ris_by_weight(const UD* const users, RIS* riss, int rid)
{
  RIS* r = &riss[rid];
  SizeT len = r->coveredUDs.size;
  for (int step = len >> 1; step >= 1; step = step >> 1) {
    for (int start = step; start < len; start++) {
      for (int j = start - step; j >= 0 && j + step < len ; j -= step) 
      {
        const UD* const u1 = &users[r->coveredUDs.data[j]];
        double weight1 = u1->profit/r->coveredUDsCost.data[j];
        const UD* const u2 = &users[r->coveredUDs.data[j+step]];
        double weight2 = u2->profit/r->coveredUDsCost.data[j+step];
        if(weight2 > weight1)
        {
          int temp = r->coveredUDs.data[j + step];
          r->coveredUDs.data[j + step] = r->coveredUDs.data[j];
          r->coveredUDs.data[j] = temp;
          SizeT temp1 = r->coveredUDsCost.data[j + step];
          r->coveredUDsCost.data[j + step] = r->coveredUDsCost.data[j];
          r->coveredUDsCost.data[j] = temp1;
        }
      }
    }
  }
}
void sort_riss_by_weight(const UD* const users, const RIS* riss, List* qan_covered_uid, LongList* qan_covered_ucost, List* ridlist)
{
  uInt32T len = ridlist->size;
  for (int step = len >> 1; step >= 1; step = step >> 1) {
    for (int start = step; start < len; start++) {
      for (int j = start - step; j >= 0 && j + step < len; j -= step) {
        int rid1 = ridlist->data[j];
        int rid2 = ridlist->data[j+step];
        int uid1 = (rid1>-1)?riss[rid1].coveredUDs.data[0]:qan_covered_uid->data[abs(rid1+1)];
        int uid2 = (rid2>-1)?riss[rid2].coveredUDs.data[0]:qan_covered_uid->data[abs(rid2+1)];
        const UD* const u1 = &users[uid1];
        const UD* const u2 = &users[uid2];
        SizeT ucost1 = (rid1>-1)?riss[rid1].coveredUDsCost.data[0]:qan_covered_ucost->data[abs(rid1+1)];
        SizeT ucost2 = (rid2>-1)?riss[rid2].coveredUDsCost.data[0]:qan_covered_ucost->data[abs(rid2+1)];
        double weight1 = u1->profit/ucost1;
        double weight2 = u2->profit/ucost2;
        if(weight2 > weight1)
        {
          int temp = ridlist->data[j + step];
          ridlist->data[j + step] = ridlist->data[j];
          ridlist->data[j] = temp;
        }
      }
    }
  }
}



int main(void)
{
  // initialize
  List list;
  list.init = CstlListInit;
  list.init(&list);
  LongList longlist;
  longlist.init = CstlLongListInit;
  longlist.init(&longlist);
  
  SelectedList bestlist, currentlist;
  bestlist.profit = 0;
  bestlist.cost = 0;
  currentlist.profit = 0;
  
  LongList covered_UDs_cost;
  
  
  list.init(&covered_UDs);
  longlist.init(&covered_UDs_cost);
  
  list.init(&bestlist.userList);
  list.init(&bestlist.risList);
  list.init(&currentlist.userList);
  list.init(&currentlist.risList);
  
  // input global parameters
  scanf("%d%d", &amountUD, &amountRIS);
  scanf("%lf%lf", &alpha, &beta);
  scanf("%d", &entangle_generate_rate);
  
  // input QAN datas
  scanf("%d%d", &qan_pos.x, &qan_pos.y);
  scanf("%d", &covered_UD_amount);
  if(covered_UD_amount)
  {
    for(SizeT i = 0; i < covered_UD_amount; i++)
    {
      int uid;
      scanf("%d", &uid);
      list.pushBack(&covered_UDs, uid);
    }
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
      // initialize
      list.init(&u->selectableRIS);
      longlist.init(&u->selectableRISCost);
      
      u->cost = 0xFFFFFFFFFFFFFFFF;
      u->weight = 0;
    }
  }
  BitsetT* selected_by_qan = (BitsetT*)malloc(sizeof(BitsetT)*(amountUD+63)/64);
  memset(selected_by_qan, 0, sizeof(BitsetT)*(amountUD+63)/64);
  
  // calculate for those could directly link to QAN
  for(SizeT i = 0; i < covered_UD_amount; i++)
  {
    int uid = covered_UDs.data[i];
    selected_by_qan[uid/64] |= BIT(uid%64); // if the user could select qan, no matter select or not exactly, the user has no other ris to select and get better cost 
    UD* u = &users[uid];
    
    // calculate for s(u,r)
    SizeT cost = source_consumption(u, NULL);
    longlist.pushBack(&covered_UDs_cost, 0xFFFFFFFFFFFFFFFF);
    if(cost > entangle_generate_rate) continue; // could not select
    
    list.pushBack(&u->selectableRIS, -1);
    longlist.pushBack(&u->selectableRISCost, cost); // never record those can't use the qan
    u->cost = cost;
    u->weight = u->profit/u->cost;
    longlist.pop(&covered_UDs_cost);
    longlist.pushBack(&covered_UDs_cost, cost);
  }
  
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
      
      r->dist_to_qan = distance(r->pos, qan_pos);
      list.init(&r->coveredUDs);
      longlist.init(&r->coveredUDsCost);
      
      for(SizeT j = 0; j < covered_amount; j++)
      {
        int uid;
        scanf("%d", &uid);
        if(selected_by_qan[uid/64]&BIT(uid%64)) // no any ris better than qan
          continue;
        // calculate for UDs' s(u,r) and if it want to choose this RIS
        UD* u = &users[uid];
        SizeT cost = source_consumption(u, r);
        if(cost>entangle_generate_rate) continue; // could not select this ris
        
        list.pushBack(&r->coveredUDs, uid);
        longlist.pushBack(&r->coveredUDsCost, cost);
        
        list.pushBack(&u->selectableRIS, i);
        longlist.pushBack(&u->selectableRISCost, cost);
        
      }
    }
  }
  
#if DEBUG
  printf("--------------\n");
  for(SizeT i = 0; i <amountUD; i++)
  {
    printf("uid: %3llu\tprofit: %4d\n", i, users[i].profit);
    for(SizeT j = 0; j<users[i].selectableRIS.size; j++)
    {
      char sep = (j==users[i].selectableRIS.size-1)?'\n':' ';
      printf("%4d%c", users[i].selectableRIS.data[j], sep);
    }
    for(SizeT j = 0; j<users[i].selectableRISCost.size; j++)
    {
      char sep = (j==users[i].selectableRISCost.size-1)?'\n':' ';
      printf("%4llu%c", users[i].selectableRISCost.data[j], sep);
    }
  }
#endif
  
  List user_queue;
  list.init(&user_queue);
  
  SizeT max_ris_for_user = 1;
  
  for(SizeT i = 0; i<amountUD; i++)
  {
    // sort by weight and filt out those impossible users
    if(users[i].selectableRISCost.size)
    {
      sort_rid_of_users_by_cost(users, i);
      users[i].cost = users[i].selectableRISCost.data[0];
      users[i].weight = users[i].profit/users[i].cost;
      
      max_ris_for_user=(users[i].selectableRISCost.size>max_ris_for_user)?users[i].selectableRISCost.size:max_ris_for_user;
    }
    else
      users[i].weight = 0;
    
    list.pushBack(&user_queue, i);
  }
  
  sort_users_by_weight(users, &user_queue);
  
  while(user_queue.size) // clear the users who can't select any valid ris
  {
    UD* u = &users[user_queue.data[user_queue.size-1]];
    if(u->selectableRISCost.data[0]==0xFFFFFFFFFFFFFFFF)
      list.pop(&user_queue);
    else
      break;
  }
  
  UDSortFuncList ud_sort_funcs;
  ud_sort_funcs.init = CstlUDSortFuncListInit;
  ud_sort_funcs.init(&ud_sort_funcs);
  
  ud_sort_funcs.pushBack(&ud_sort_funcs, sort_users_by_profit);
//  ud_sort_funcs.pushBack(&ud_sort_funcs, sort_users_by_cost); // meanless
//  ud_sort_funcs.pushBack(&ud_sort_funcs, sort_users_by_uid); // meanless
  
#if DEBUG
  printf("--------------\n");
  for(SizeT i = 0; i < user_queue.size; i++)
  {
    UD* u = &users[user_queue.data[i]];
    printf("uid: %3d\tprofit: %4d\n", u->id, u->profit);
    printf("%4d\n", u->selectableRIS.data[0]);
    printf("%4llu\n", u->selectableRISCost.data[0]);
  }
#endif
  
//  SizeT complexity = user_queue.size * max_ris_for_user * entangle_generate_rate;
  
//  if(complexity<MAX_COMPLEXITY)
  {
    // dp (user choose ris)
    
    SizeT max_cost = entangle_generate_rate + 1; // cost from 0 to entangle_generate_rate
    SizeT dp_size = sizeof(SizeT)*user_queue.size*max_cost;
    SizeT* dp_sheet = (SizeT*)malloc(dp_size); 
    memset(dp_sheet, 0, dp_size);
    
    DpIndexMap dp_map;
    dp_map.init = CstlDpIndexMapInit;
    dp_map.init(&dp_map);
    
    SizeT bits_r_size = sizeof(BitsetT)*((amountRIS+63)/64);
    
    for(SizeT f = 0; f <= ud_sort_funcs.size; f++) // a extra run for default sort func
    {
      for(SizeT i = 0; i < user_queue.size; i++) // iterate all user
      {
        UD* u = &users[user_queue.data[i]];
        
        if(i) // not the first user
        {
          for(SizeT j = 0; j < max_cost; j++){ // iterate for all the selection of last user
            SizeT last_user_select = (i-1)*max_cost+j;
            if(dp_sheet[last_user_select]) // the last user selected
            {
              DpIndexNode* prev = dp_map.find(&dp_map, last_user_select);
              // select this user/ris
              for(SizeT k = 0; k < u->selectableRIS.size; k++) // iterate for all the avalible ris for user
              {
                int rid = u->selectableRIS.data[k];
                SizeT rcost = u->selectableRISCost.data[k];
                
                // chech if the ris is selectable by unordered map
                if(!(prev->data.bitsMask[rid/64]&BIT(rid%64))) // the ris has not selected by current choose
                {
                  SizeT select_cost = rcost+j;
                  SizeT select_profit = dp_sheet[last_user_select]+u->profit;
                  SizeT now_select = i*max_cost+select_cost;
                  
                  // check if the choice is valid
                  if(select_cost<max_cost) 
                  {
                    if(dp_sheet[now_select]) // the choice is selected
                    {
                      DpIndexNode* now_select_dp_sheet = dp_map.find(&dp_map, now_select);
                      if(select_profit>dp_sheet[now_select]) // the choice is better, rewrite the now_select
                      {
                        dp_sheet[now_select] = select_profit;
                        // update hash table (no need to edit key or next since we just rewrite the selection)
                        memcpy(now_select_dp_sheet->data.bitsMask, prev->data.bitsMask, bits_r_size);
                        now_select_dp_sheet->data.bitsMask[rid/64] |= BIT(rid%64);
                        SelectedList* now_list = &now_select_dp_sheet->data.selectedList;
                        SelectedList* prev_list = &prev->data.selectedList;
                        now_list->profit = select_profit;
                        now_list->cost = select_cost;
                        list.copyFrom(&now_list->userList, &prev_list->userList);
                        list.copyFrom(&now_list->risList, &prev_list->risList);
                        list.pushBack(&now_list->userList, u->id);
                        list.pushBack(&now_list->risList, rid);
                      }
                      // else do nothing
                    }
                    else // the index of this user has not been selected
                    {
                      dp_sheet[now_select] = select_profit;
                      // update hash table (copy from prev and insert directly)
                      DpSelection* s = (DpSelection*)malloc(sizeof(DpSelection));
                      s->bitsMask = malloc(bits_r_size);
                      memcpy(s->bitsMask, prev->data.bitsMask, bits_r_size);
                      s->bitsMask[rid/64] |= BIT(rid%64);
                      s->selectedList.profit = select_profit;
                      s->selectedList.cost = select_cost;
                      list.init(&s->selectedList.userList);
                      list.init(&s->selectedList.risList);
                      list.copyFrom(&s->selectedList.userList, &prev->data.selectedList.userList);
                      list.copyFrom(&s->selectedList.risList, &prev->data.selectedList.risList);
                      list.pushBack(&s->selectedList.userList, u->id);
                      list.pushBack(&s->selectedList.risList, rid);
                      
                      dp_map.insert(&dp_map, now_select, s);
                    }
                  }
                }
              }
              // not select
              SizeT this_user_select = i*max_cost+j;
              if(dp_sheet[last_user_select] > dp_sheet[this_user_select]) // avoid for overwrite better choice
              {
                dp_sheet[this_user_select] = dp_sheet[last_user_select];
                // update hash table
                if(dp_map.find(&dp_map, this_user_select))
                  dp_map.erase(&dp_map, this_user_select);
                dp_map.transfer(&dp_map, last_user_select, this_user_select);
              }
            }
          }
        }
        else // first user
        {
          // select this user/ris
          // the ris are all selectable for first user since ensure for each user won't select repeat ris
          for(SizeT j = 0; j < u->selectableRIS.size; j++) // iterate for all the avalible ris for user
          {
            int rid = u->selectableRIS.data[j];
            SizeT rcost = u->selectableRISCost.data[j];
            SizeT now_select = i*max_cost + rcost;
            // no need to check if the choice is valid
            dp_sheet[now_select] = u->profit;
            // update hash table
            DpSelection* s = (DpSelection*)malloc(sizeof(DpSelection));
            s->bitsMask = malloc(bits_r_size);
            memset(s->bitsMask, 0, bits_r_size);
            s->bitsMask[rid/64] |= BIT(rid%64);
            s->selectedList.profit = u->profit;
            s->selectedList.cost = rcost;
            list.init(&s->selectedList.userList);
            list.init(&s->selectedList.risList);
            list.pushBack(&s->selectedList.userList, u->id);
            list.pushBack(&s->selectedList.risList, rid);
            
            dp_map.insert(&dp_map, now_select, s);
          }
          // not select : donothing
        }
      }
      // update best list
      SizeT best_index = 0;
      SizeT best_profit = 0;
      SizeT valid_user = user_queue.size-1;
      for(SizeT i = 0; i < max_cost; i++) // iterate the last user line for find the index with max profit
      {
        if(dp_sheet[valid_user*max_cost+i]>best_profit)
        {
          best_index = valid_user*max_cost+i;
          best_profit = dp_sheet[best_index];
        }
      }
      DpIndexNode* best_result = dp_map.find(&dp_map, best_index);
      if(best_result&&best_result->data.selectedList.profit>bestlist.profit)
      {
        bestlist.profit = best_result->data.selectedList.profit;
        bestlist.cost = best_result->data.selectedList.cost;
        list.copyFrom(&bestlist.userList, &best_result->data.selectedList.userList);
        list.copyFrom(&bestlist.risList, &best_result->data.selectedList.risList);
      }
      // reset dp states
      memset(dp_sheet, 0, dp_size);
      dp_map.clear(&dp_map);
      
      // sort user lists
      if(f!=ud_sort_funcs.size) // for default sort func
      {
        ud_sort_funcs.data[f](users, &user_queue);
#if DEBUG
        for(SizeT i = 0; i < user_queue.size; i++){
          printf("%d ", user_queue.data[i]);
        }printf("\n");
#endif
      }
    }
    free(dp_sheet);
    dp_map.delete(&dp_map);
  }
  
  
  List ris_queue;
  list.init(&ris_queue);
  
  sort_qan_covered_users(users, &covered_UDs, &covered_UDs_cost);
  
  while(covered_UDs.size){
    if(covered_UDs_cost.data[covered_UDs.size-1]==0xFFFFFFFFFFFFFFFF)
    {
      list.pop(&covered_UDs);
      longlist.pop(&covered_UDs_cost);
    }
    else
      break;
  }
  for(SizeT i = 0; i < covered_UDs.size; i++)
  {
    // use [-1:-covered_UDs.size] as special rid for each user could be serve by qan
    SizeT rid = -1-i;
    list.pushBack(&ris_queue, rid);
  }
  
  for(SizeT i = 0; i<amountRIS; i++)
  {
    // sort by weight and filt out those impossible riss
    if(risurfaces[i].coveredUDsCost.size)
    {
      sort_users_of_ris_by_weight(users, risurfaces, i);
      
      list.pushBack(&ris_queue, i);
    }
  }
  
  sort_riss_by_weight(users, risurfaces, &covered_UDs, &covered_UDs_cost, &ris_queue);
  
  while(ris_queue.size) // clear those riss can't serve any user
  {
    if(ris_queue.data[ris_queue.size-1]>-1&&risurfaces[ris_queue.data[ris_queue.size-1]].coveredUDsCost.data[0]==0xFFFFFFFFFFFFFFFF)
      list.pop(&ris_queue);
    else
      break;
  }
  
#if DEBUG
  printf("--------------\n");
  for(SizeT i = 0; i < ris_queue.size; i++)
  {
    int rid = ris_queue.data[i];
    
    printf("rid: %3d\n", (rid>-1)?rid:-1);
    if(rid>-1)
    {
      for(SizeT j = 0; j < risurfaces[rid].coveredUDs.size; j++)
      {
        int uid = risurfaces[rid].coveredUDs.data[j];
        SizeT ucost = risurfaces[rid].coveredUDsCost.data[j];
        printf("%4d\tprofit: %4d\tcost: %4llu\n", uid, users[uid].profit, ucost);
      }
    }
    else
    {
      int uid = covered_UDs.data[abs(rid+1)];
      SizeT ucost = covered_UDs_cost.data[abs(rid+1)];
      printf("%4d\tprofit: %4d\tcost: %4llu\n", uid, users[uid].profit, ucost);
    }
  }
#endif
  if(user_queue.size>ris_queue.size)
  {
    // dp (ris choose user)
    
    SizeT max_cost = entangle_generate_rate + 1; // cost from 0 to entangle_generate_rate
    SizeT dp_size = sizeof(SizeT)*ris_queue.size*max_cost;
    SizeT* dp_sheet = (SizeT*)malloc(dp_size); 
    memset(dp_sheet, 0, dp_size);
    
    DpIndexMap dp_map;
    dp_map.init = CstlDpIndexMapInit;
    dp_map.init(&dp_map);
    
    SizeT bits_u_size = sizeof(BitsetT)*((amountUD+63)/64);
    
//    for(SizeT f = 0; f <= ris_sort_funcs.size; f++) // a extra run for default sort func
    {
      for(SizeT i = 0; i < ris_queue.size; i++) // iterate all ris
      {
        int rid = ris_queue.data[i];
        if(i) // not the first ris
        {
          if(rid>-1) // normal ris
          {
            RIS* r = &risurfaces[ris_queue.data[i]];
            
            for(SizeT j = 0; j < max_cost; j++){ // iterate for all the selection of last ris
              SizeT last_ris_select = (i-1)*max_cost+j;
              if(dp_sheet[last_ris_select]) // the last ris selected
              {
                DpIndexNode* prev = dp_map.find(&dp_map, last_ris_select);
                // select this user/ris
                for(SizeT k = 0; k < r->coveredUDs.size; k++) // iterate for all the avalible user for ris
                {
                  int uid = r->coveredUDs.data[k];
                  SizeT ucost = r->coveredUDsCost.data[k];
                  
                  UD* u = &users[uid];
                  
                  // chech if the user is selectable by unordered map
                  if(!(prev->data.bitsMask[uid/64]&BIT(uid%64))) // the user has not selected by current choose
                  {
                    SizeT select_cost = ucost+j;
                    SizeT select_profit = dp_sheet[last_ris_select]+u->profit;
                    SizeT now_select = i*max_cost+select_cost;
                    
                    // check if the choice is valid
                    if(select_cost<max_cost)
                    {
                      if(dp_sheet[now_select]) // the choice is selected
                      {
                        DpIndexNode* now_select_dp_sheet = dp_map.find(&dp_map, now_select);
                        if(select_profit>dp_sheet[now_select]) // the choice is better, rewrite the now_select
                        {
                          dp_sheet[now_select] = select_profit;
                          // update hash table (no need to edit key or next since we just rewrite the selection)
                          memcpy(now_select_dp_sheet->data.bitsMask, prev->data.bitsMask, bits_u_size);
                          now_select_dp_sheet->data.bitsMask[uid/64] |= BIT(uid%64);
                          SelectedList* now_list = &now_select_dp_sheet->data.selectedList;
                          SelectedList* prev_list = &prev->data.selectedList;
                          now_list->profit = select_profit;
                          now_list->cost = select_cost;
                          list.copyFrom(&now_list->userList, &prev_list->userList);
                          list.copyFrom(&now_list->risList, &prev_list->risList);
                          list.pushBack(&now_list->userList, uid);
                          list.pushBack(&now_list->risList, rid);
                        }
                        // else do nothing
                      }
                      else // the index of this ris has not been selected
                      {
                        dp_sheet[now_select] = select_profit;
                        // update hash table (copy from prev and insert directly)
                        DpSelection* s = (DpSelection*)malloc(sizeof(DpSelection));
                        s->bitsMask = malloc(bits_u_size);
                        memcpy(s->bitsMask, prev->data.bitsMask, bits_u_size);
                        s->bitsMask[uid/64] |= BIT(uid%64);
                        s->selectedList.profit = select_profit;
                        s->selectedList.cost = select_cost;
                        list.init(&s->selectedList.userList);
                        list.init(&s->selectedList.risList);
                        list.copyFrom(&s->selectedList.userList, &prev->data.selectedList.userList);
                        list.copyFrom(&s->selectedList.risList, &prev->data.selectedList.risList);
                        list.pushBack(&s->selectedList.userList, uid);
                        list.pushBack(&s->selectedList.risList, rid);
                        
                        dp_map.insert(&dp_map, now_select, s);
                      }
                    }
                  }
                }
                // not select
                SizeT this_ris_select = i*max_cost+j;
                if(dp_sheet[last_ris_select] > dp_sheet[this_ris_select]) // avoid for overwrite better choice
                {
                  dp_sheet[this_ris_select] = dp_sheet[last_ris_select];
                  // update hash table
                  if(dp_map.find(&dp_map, this_ris_select))
                    dp_map.erase(&dp_map, this_ris_select);
                  dp_map.transfer(&dp_map, last_ris_select, this_ris_select);
                }
              }
            }
          }
          else // the rid is qan
          {
            SizeT qan_index = abs(rid+1);
            rid = -1;
            
            for(SizeT j = 0; j < max_cost; j++){ // iterate for all the selection of last ris
              SizeT last_ris_select = (i-1)*max_cost+j;
              if(dp_sheet[last_ris_select]) // the last ris selected
              {
                DpIndexNode* prev = dp_map.find(&dp_map, last_ris_select);
                // select this user/ris
                // no need to iterate
                
                int uid = covered_UDs.data[qan_index];
                SizeT ucost = covered_UDs_cost.data[qan_index];
                
                UD* u = &users[uid];
                
                // chech if the user is selectable by unordered map
                if(!(prev->data.bitsMask[uid/64]&BIT(uid%64))) // the user has not selected by current choose
                {
                  SizeT select_cost = ucost+j;
                  SizeT select_profit = dp_sheet[last_ris_select]+u->profit;
                  SizeT now_select = i*max_cost+select_cost;
                  
                  // check if the choice is valid
                  if(select_cost<max_cost)
                  {
                    if(dp_sheet[now_select]) // the choice is selected
                    {
                      DpIndexNode* now_select_dp_sheet = dp_map.find(&dp_map, now_select);
                      if(select_profit>dp_sheet[now_select]) // the choice is better, rewrite the now_select
                      {
                        dp_sheet[now_select] = select_profit;
                        // update hash table (no need to edit key or next since we just rewrite the selection)
                        memcpy(now_select_dp_sheet->data.bitsMask, prev->data.bitsMask, bits_u_size);
                        now_select_dp_sheet->data.bitsMask[uid/64] |= BIT(uid%64);
                        SelectedList* now_list = &now_select_dp_sheet->data.selectedList;
                        SelectedList* prev_list = &prev->data.selectedList;
                        now_list->profit = select_profit;
                        now_list->cost = select_cost;
                        list.copyFrom(&now_list->userList, &prev_list->userList);
                        list.copyFrom(&now_list->risList, &prev_list->risList);
                        list.pushBack(&now_list->userList, uid);
                        list.pushBack(&now_list->risList, rid);
                      }
                      // else do nothing
                    }
                    else // the index of this ris has not been selected
                    {
                      dp_sheet[now_select] = select_profit;
                      // update hash table (copy from prev and insert directly)
                      DpSelection* s = (DpSelection*)malloc(sizeof(DpSelection));
                      s->bitsMask = malloc(bits_u_size);
                      memcpy(s->bitsMask, prev->data.bitsMask, bits_u_size);
                      s->bitsMask[uid/64] |= BIT(uid%64);
                      s->selectedList.profit = select_profit;
                      s->selectedList.cost = select_cost;
                      list.init(&s->selectedList.userList);
                      list.init(&s->selectedList.risList);
                      list.copyFrom(&s->selectedList.userList, &prev->data.selectedList.userList);
                      list.copyFrom(&s->selectedList.risList, &prev->data.selectedList.risList);
                      list.pushBack(&s->selectedList.userList, uid);
                      list.pushBack(&s->selectedList.risList, rid);
                      
                      dp_map.insert(&dp_map, now_select, s);
                    }
                  }
                }
              }
              // not select
              SizeT this_ris_select = i*max_cost+j;
              if(dp_sheet[last_ris_select] > dp_sheet[this_ris_select]) // avoid for overwrite better choice
              {
                dp_sheet[this_ris_select] = dp_sheet[last_ris_select];
                // update hash table
                if(dp_map.find(&dp_map, this_ris_select))
                  dp_map.erase(&dp_map, this_ris_select);
                dp_map.transfer(&dp_map, last_ris_select, this_ris_select);
              }
            }
          }
        }
        else // first ris
        {
          if(rid>-1) // normal ris
          {
            RIS* r = &risurfaces[rid];
            // the ris are all selectable for first ris since ensure for each user won't select repeat user
            for(SizeT j = 0; j < r->coveredUDs.size; j++) // iterate for all the avalible user for ris
            {
              // select this user/ris
              int uid = r->coveredUDs.data[j];
              SizeT ucost = r->coveredUDsCost.data[j];
              SizeT now_select = i*max_cost + ucost;
              
              UD* u = &users[uid];
              // no need to check if the choice is valid
              dp_sheet[now_select] = u->profit;
              // update hash table
              DpSelection* s = (DpSelection*)malloc(sizeof(DpSelection));
              s->bitsMask = malloc(bits_u_size);
              memset(s->bitsMask, 0, bits_u_size);
              s->bitsMask[uid/64] |= BIT(uid%64);
              s->selectedList.profit = u->profit;
              s->selectedList.cost = ucost;
              list.init(&s->selectedList.userList);
              list.init(&s->selectedList.risList);
              list.pushBack(&s->selectedList.userList, uid);
              list.pushBack(&s->selectedList.risList, rid);
              
              dp_map.insert(&dp_map, now_select, s);
            }
            // not select : donothing
          }
          else // qan
          {
            SizeT qan_index = abs(rid+1);
            rid = -1;
            
            // select this user/ris
            int uid = covered_UDs.data[qan_index];
            SizeT ucost = covered_UDs_cost.data[qan_index];
            SizeT now_select = i*max_cost + ucost;
            
            UD* u = &users[uid];
            // no need to check if the choice is valid
            dp_sheet[now_select] = u->profit;
            // update hash table
            DpSelection* s = (DpSelection*)malloc(sizeof(DpSelection));
            s->bitsMask = malloc(bits_u_size);
            memset(s->bitsMask, 0, bits_u_size);
            s->bitsMask[uid/64] |= BIT(uid%64);
            s->selectedList.profit = u->profit;
            s->selectedList.cost = ucost;
            list.init(&s->selectedList.userList);
            list.init(&s->selectedList.risList);
            list.pushBack(&s->selectedList.userList, uid);
            list.pushBack(&s->selectedList.risList, rid);
            
            dp_map.insert(&dp_map, now_select, s);
            // not select : donothing
          }
          
        }
      }
      // update best list
      SizeT best_index = 0;
      SizeT best_profit = 0;
      SizeT valid_ris = ris_queue.size-1;
      for(SizeT i = 0; i < max_cost; i++) // iterate the last user line for find the index with max profit
      {
        if(dp_sheet[valid_ris*max_cost+i]>best_profit)
        {
          best_index = valid_ris*max_cost+i;
          best_profit = dp_sheet[best_index];
        }
      }
      DpIndexNode* best_result = dp_map.find(&dp_map, best_index);
      if(best_result&&best_result->data.selectedList.profit>bestlist.profit)
      {
        bestlist.profit = best_result->data.selectedList.profit;
        bestlist.cost = best_result->data.selectedList.cost;
        list.copyFrom(&bestlist.userList, &best_result->data.selectedList.userList);
        list.copyFrom(&bestlist.risList, &best_result->data.selectedList.risList);
      }
      // reset dp states
      memset(dp_sheet, 0, dp_size);
      dp_map.clear(&dp_map);
      
      // sort user lists
//      if(f!=ud_sort_funcs.size) // for default sort func
//      {
//        ud_sort_funcs.data[f](users, &user_queue);
#if DEBUG
        for(SizeT i = 0; i < ris_queue.size; i++){
          printf("%d ", ris_queue.data[i]);
        }printf("\n");
#endif
      }
//    }
    free(dp_sheet);
    dp_map.delete(&dp_map);
  }
  
  
  
  // print
  SizeT num = bestlist.userList.size;
  printf("%llu\n", num);
  for(SizeT i = 0; i < num; i++)
  {
    printf("%d %d\n", bestlist.userList.data[i], bestlist.risList.data[i]);
  }
#if DEBUG
  printf("%llu\n", bestlist.profit);
#endif
  // end
#if DEBUG
  char stop;
  scanf("%c",&stop);
#endif
  for(SizeT i = 0; i < amountRIS; i++)
  {
    RIS* r = &risurfaces[i];
    list.delete(&r->coveredUDs);
  }
  free(risurfaces);
  for(SizeT i = 0; i<amountUD; i++){
    UD* u = &users[i];
    list.delete(&u->selectableRIS);
    longlist.delete(&u->selectableRISCost);
  }
  free(users);
  list.delete(&bestlist.userList);
  list.delete(&bestlist.risList);
  list.delete(&currentlist.userList);
  list.delete(&currentlist.risList);
  list.delete(&covered_UDs);
  
  return 0;
}
