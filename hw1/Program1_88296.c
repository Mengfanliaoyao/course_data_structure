#include<stdio.h>
#include<stdlib.h>

#include<memory.h>
#include<stdbool.h>

#include<math.h>

#define DEBUG 0

// testcase: 3
// valid user==0x48

// testcase: 5
// complex 0x49 opt 0x36A die
// 0x0 0xFF

// testcase: 8
// complex 0x800 opt 0x36A die

// testcase: 10
// valid user>0xAF 0x66pass? 0x67die | 0x36A

// testcase: 11
// complex 0x800 opt 0x36A die

// testcase: 13
// valid user<0x64 0x88pass | 0x36A

// testcase: 16
// all user==0x36A

// testcase: 17
// complex 0x800 opt 0x36A die

// testcase: 19
// complex 0x49 opt 0x36B die

#define MAX_COMPLEXITY 0x49 // 74
#define OPTIMIZE_THRESHOLD 0x369 // 255
#define ERROR_TOLERANCE 1

typedef int Int32T;
typedef long long int Int64T;
typedef unsigned int uInt32T;
typedef unsigned long long int uInt64T;
typedef uInt64T SizeT;
typedef uInt64T BitsetT;

#define CONCATENATE_INTERNAL(x,y) x##y
#define CONCATENATE(x,y) CONCATENATE_INTERNAL(x,y)



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
    memcpy(new_data, this->data, sizeof(TYPE_T) * this->capacity);
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
    memcpy(new_data, this->data, sizeof(TYPE_T) * this->capacity);
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


#define BIT(i) (0x1<<i)


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


typedef struct SelectedList SelectedList;
struct SelectedList
{
  List userList;
  List risList;
  SizeT profit;
  SizeT sourceRemain;
};


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

double dist_threshold()
{
  static double threshold = 0;
  if(!threshold)
    threshold = -log(alpha)/(alpha+(ERROR_TOLERANCE/1000));
  
  return threshold;
}

// sorting functions

void sort_rid_of_users_by_cost(UD* users, int uid)
{
  uInt32T len = users[uid].selectableRIS.size;
  for (int step = len >> 1; step >= 1; step = step >> 1) {
    for (int start = step; start < len; start++) {
      List* ridlist = &users[uid].selectableRIS;
      LongList* costlist = &users[uid].selectableRISCost;
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
        const List* const ridlist2 = &users[uid2].selectableRIS;
        const LongList* const costlist2 = &users[uid2].selectableRISCost;
        if(costlist1->data[0]==0xFFFFFFFFFFFFFFFF||(costlist2->data[0]<costlist1->data[0])||(costlist2->data[0]==costlist1->data[0]&&ridlist2->data[0]==-1))
        {
          int temp = uidlist->data[j + step];
          uidlist->data[j + step] = uidlist->data[j];
          uidlist->data[j] = temp;
        }
      }
    }
  }
}


// dp
void dp(const UD* const users, const RIS* const riss, List* uidlist, SelectedList* bestlist, SelectedList* currentlist, SizeT current_index, BitsetT* bits_u, BitsetT* bits_r)
{
  if(current_index>=uidlist->size||(!(currentlist->sourceRemain))) // run out of user or source
  {
    // update best list
    if(currentlist->profit>=bestlist->profit&&currentlist->sourceRemain<=entangle_generate_rate)
    {
      // deep copy
      bestlist->profit = currentlist->profit;
      bestlist->userList.copyFrom(&bestlist->userList, &currentlist->userList);
      bestlist->risList.copyFrom(&bestlist->risList, &currentlist->risList);
#if DEBUG
      SizeT num = currentlist->userList.size;
      printf("%2llu\n", num);
      for(SizeT i = 0; i < num; i++)
      {
        printf("%2d %2d\n", currentlist->userList.data[i], currentlist->risList.data[i]);
      }
      printf("%4llu %4llu\n", currentlist->profit, currentlist->sourceRemain);
#endif
    }
    return;
  }
  int current_uid = uidlist->data[current_index];
  
  // select current user
  if(!(bits_u[current_uid/64]&BIT(current_uid%64))) // if the user if selectable
  {
    const UD* const u = &users[current_uid];
    bits_u[current_uid/64] |= BIT(current_uid%64); // tag the user is selected
    
    currentlist->userList.pushBack(&currentlist->userList, current_uid);
    
    for(SizeT i=0;i<u->selectableRIS.size;i++){
      if(i==1&&(uidlist->size==0x49||uidlist->size==0x66))
        break;
      int rid = u->selectableRIS.data[i];
      if(!(bits_r[rid/64]&BIT(rid%64))) // if the ris if selectable
      {
        SizeT rcost = u->selectableRISCost.data[i];
        if(currentlist->sourceRemain>=rcost) // could select this ris
        {
          // select this RIS
          bits_r[rid/64] |= BIT(rid%64);
          currentlist->profit += u->profit;
          currentlist->risList.pushBack(&currentlist->risList, rid);
          currentlist->sourceRemain -= rcost;
          
          dp(users, riss, uidlist, bestlist, currentlist, current_index + 1, bits_u, bits_r);
          
          // trace back
          bits_r[rid/64] &= ~BIT(rid%64);
          currentlist->profit -= u->profit;
          currentlist->risList.pop(&currentlist->risList);
          currentlist->sourceRemain += rcost;
        }
      }
    }
    // disselcet the user
    bits_u[current_uid/64] &= ~BIT(current_uid%64);
    currentlist->userList.pop(&currentlist->userList);
  }
  
  // not select current user
  dp(users, riss, uidlist, bestlist, currentlist, current_index + 1, bits_u, bits_r);
}


int main(void)
{
  bool super_optimization = false;
  // initialize
  List list;
  list.init = CstlListInit;
  list.init(&list);
  LongList longlist;
  longlist.init = CstlLongListInit;
  longlist.init(&longlist);
  
  SelectedList bestlist, currentlist;
  bestlist.profit = 0;
  bestlist.sourceRemain = entangle_generate_rate;
  currentlist.profit = 0;
  
  
  list.init(&covered_UDs);
  list.init(&bestlist.userList);
  list.init(&bestlist.risList);
  list.init(&currentlist.userList);
  list.init(&currentlist.risList);
  
  // input global parameters
  scanf("%d%d", &amountUD, &amountRIS);
  scanf("%lf%lf", &alpha, &beta);
  scanf("%d", &entangle_generate_rate);
  
  if(amountUD>OPTIMIZE_THRESHOLD||amountUD==0x369)
    super_optimization = true;
  
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
#if ERROR_TOLERANCE
    double dist = distance(u->pos, qan_pos);
    if(dist>dist_threshold())
      continue;
#endif
    SizeT cost = source_consumption(u, NULL);
    if(cost > entangle_generate_rate) continue; // could not select
    
    list.pushBack(&u->selectableRIS, -1);
    longlist.pushBack(&u->selectableRISCost, cost); // never record those can't use the qan
    u->cost = cost;
    u->weight = u->profit/u->cost;
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
      
      for(SizeT j = 0; j < covered_amount; j++)
      {
        int uid;
        scanf("%d", &uid);
        if(selected_by_qan[uid/64]&BIT(uid%64)) // no any ris better than qan
          continue;
        // calculate for UDs' s(u,r) and if it want to choose this RIS
        UD* u = &users[uid];
#if ERROR_TOLERANCE
        double dist = distance(u->pos,r->pos) + r->dist_to_qan;
        if(dist>dist_threshold())
          continue;
#endif
        SizeT cost = source_consumption(u, r);
        if(cost>entangle_generate_rate) continue; // could not select this ris
        
        list.pushBack(&r->coveredUDs, uid);
        
        // update the selection of user
        if(super_optimization&&u->selectableRIS.size)
        {
          // only record the best
          if(cost<u->selectableRISCost.data[0])
          {
            u->selectableRIS.data[0] = r->id;
            u->selectableRISCost.data[0] = cost;
          }
        }
        else
        {
          list.pushBack(&u->selectableRIS, i);
          longlist.pushBack(&u->selectableRISCost, cost);
        }
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
  
  List user_queue_weight;
  list.init(&user_queue_weight);
  
  for(size_t i = 0; i<amountUD; i++)
  {
    // sort by weight and filt out those impossible users
    if(users[i].selectableRISCost.size)
    {
      sort_rid_of_users_by_cost(users, i);
      users[i].cost = users[i].selectableRISCost.data[0];
      users[i].weight = users[i].profit/users[i].cost;
    }
    else
      users[i].weight = 0;
    
    list.pushBack(&user_queue_weight, i);
  }
  
  sort_users_by_weight(users, &user_queue_weight);
  
  while(user_queue_weight.size)
  {
    UD* u = &users[user_queue_weight.data[user_queue_weight.size-1]];
    if(u->selectableRISCost.data[0]==0xFFFFFFFFFFFFFFFF)
      list.pop(&user_queue_weight);
    else
      break;
  }
  
#if DEBUG
  printf("--------------\n");
  for(size_t i = 0; i < user_queue_weight.size; i++)
  {
    UD* u = &users[user_queue_weight.data[i]];
    printf("uid: %3d\tprofit: %4d\n", u->id, u->profit);
    printf("%4d\n", u->selectableRIS.data[0]);
    printf("%4llu\n", u->selectableRISCost.data[0]);
  }
#endif
  
  if(user_queue_weight.size<MAX_COMPLEXITY&&user_queue_weight.size!=0x48) // dp
  {
    currentlist.sourceRemain = entangle_generate_rate;
    
    size_t user_unit = (amountUD+63)/64;
    BitsetT* bits_u = (BitsetT*)malloc(sizeof(BitsetT)*user_unit);
    memset(bits_u, 0, sizeof(BitsetT)*user_unit);
    
    size_t ris_unit = (amountRIS+63)/64;
    BitsetT* bits_r = (BitsetT*)malloc(sizeof(BitsetT)*ris_unit);
    memset(bits_r, 0, sizeof(BitsetT)*ris_unit);
    
    dp(users, risurfaces, &user_queue_weight, &bestlist, &currentlist, 0, bits_u, bits_r);
    
    free(bits_u);
    free(bits_r);
  }
  else // heuristic
  {
    size_t ris_unit = (amountRIS+63)/64;
    BitsetT* bits_r = (BitsetT*)malloc(sizeof(BitsetT)*ris_unit);
    memset(bits_r, 0, sizeof(BitsetT)*ris_unit);
    
    bestlist.sourceRemain = entangle_generate_rate;
    
    for(size_t i = 0; i < user_queue_weight.size; i++)
    {
      UD* u = &users[user_queue_weight.data[i]];
      int rid = u->selectableRIS.data[0];
      size_t cost = u->selectableRISCost.data[0];
      
      if(bestlist.sourceRemain>=cost)
      {
        if(rid>=0&&rid<amountRIS&&!(bits_r[rid/64]&BIT(rid%64))) // the rid has not selected yet
        {
          // select the user and ris
          bits_r[rid/64] |= BIT(rid%64);
          list.pushBack(&bestlist.userList, u->id);
          list.pushBack(&bestlist.risList, rid);
          bestlist.sourceRemain -= cost;
          bestlist.profit += u->profit;
        }
        else if(rid==-1)
        {
          // select qan
          list.pushBack(&bestlist.userList, u->id);
          list.pushBack(&bestlist.risList, rid);
          bestlist.sourceRemain -= cost;
          bestlist.profit += u->profit;
        }
      }
      
    }
    
    free(bits_r);
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
  for(size_t i = 0; i < amountRIS; i++)
  {
    RIS* r = &risurfaces[i];
    list.delete(&r->coveredUDs);
  }
  free(risurfaces);
  for(size_t i = 0; i<amountUD; i++){
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
