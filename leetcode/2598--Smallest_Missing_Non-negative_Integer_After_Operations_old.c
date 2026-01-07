#include<stdio.h>
#include<stdlib.h>

#include<memory.h>

typedef unsigned long long uInt64T;
typedef uInt64T SizeT;

#define BIT(i) (0x1ull<<i)

// macro concatenate for unique function signature of template

#define CONCATENATE_INTERNAL(x,y) x##y
#define CONCATENATE(x,y) CONCATENATE_INTERNAL(x,y)

// std::vector<size_t>
#define TEMPLATE_NAME CONCATENATE(cstl,DYNAMIC_ARRAY)
#define DYNAMIC_ARRAY LongList
#define FUNC(name) CONCATENATE(TEMPLATE_NAME, name)
#define TYPE_T SizeT
typedef struct DYNAMIC_ARRAY DYNAMIC_ARRAY;
struct DYNAMIC_ARRAY
{
  TYPE_T* data;
  SizeT size;
  SizeT capacity;
  void (*resize)(DYNAMIC_ARRAY* this, SizeT size); // distribute a specific size with initial value 0
  void (*pushBack)(DYNAMIC_ARRAY* this, TYPE_T data);
  void (*pop)(DYNAMIC_ARRAY* this);
  void (*copyFrom)(DYNAMIC_ARRAY* this, DYNAMIC_ARRAY* other);
  void (*clear)(DYNAMIC_ARRAY* this);
  void (*delete)(DYNAMIC_ARRAY* this);
  void (*init)(DYNAMIC_ARRAY* this);
  void (*initBySize)(DYNAMIC_ARRAY* this, SizeT size);
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
void FUNC(Resize)(DYNAMIC_ARRAY* this, SizeT size)
{
  if(size<=this->capacity) return; // invalid operation
  
  TYPE_T* new_data = (TYPE_T*)malloc(sizeof(TYPE_T)*size);
  memcpy(new_data, this->data, sizeof(TYPE_T) * this->size);
  memset(new_data + this->size, 0, (size - this->size)*sizeof(TYPE_T));
  free(this->data);
  this->data = new_data;
  new_data = NULL;
  this->capacity = size;
  this->size = size;
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
void FUNC(CopyFrom)(DYNAMIC_ARRAY* this, DYNAMIC_ARRAY* other) // deep copy
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
  
  this->resize = FUNC(Resize);
  this->pushBack = FUNC(PushBack);
  this->pop = FUNC(Pop);
  this->copyFrom = FUNC(CopyFrom);
  this->clear = FUNC(Clear);
  this->delete = FUNC(Delete);
  this->init = FUNC(Init);
}
void FUNC(InitBySize)(DYNAMIC_ARRAY* this, SizeT size) // keep bind this or it won't work
{
  this->size = size;
  this->capacity = size;
  this->data = (TYPE_T*)malloc(sizeof(TYPE_T) * this->capacity);
  memset(this->data, 0, sizeof(TYPE_T) * this->capacity);
  
  this->resize = FUNC(Resize);
  this->pushBack = FUNC(PushBack);
  this->pop = FUNC(Pop);
  this->copyFrom = FUNC(CopyFrom);
  this->clear = FUNC(Clear);
  this->delete = FUNC(Delete);
  this->init = FUNC(Init);
  this->initBySize = FUNC(InitBySize);
}
#undef TYPE_T
#undef FUNC
#undef DYNAMIC_ARRAY
#undef TEMPLATE_NAME

int findSmallestInteger(int* nums, int numsSize, int value)
{
  LongList bitset;
  bitset.initBySize = cstlLongListInitBySize;
  SizeT bit_size = value/64 + 1;
  bitset.initBySize(&bitset, bit_size);
  
  // iterate whole nums array
  for(int i = 0; i < numsSize; i++)
  {
    int a = nums[i]%value;
    if(a<0) a += value;
    SizeT offset = 0;
    while(bitset.data[(a+offset)/64]&BIT((a+offset)%64)) // find new blank to fill
    {
      offset+=value;
      if((a+offset)/64>=bitset.size)
        bitset.resize(&bitset, bitset.size*2);
    }
    printf("tagged: %llu\n", (a+offset));
    bitset.data[(a+offset)/64] |= BIT((a+offset)%64);
  }
  // iterate whole bitset
  for(SizeT i = 0; i < bitset.size*64; i++)
  {
    if(!(bitset.data[i/64]&BIT(i%64))) // there is no element to fill the blank
      return i;
  }
  
  return -1;
}

int main(void)
{
  int nums[] = {3,0,3,2,4,2,1,1,0,4};
  printf("%d", findSmallestInteger(nums, sizeof(nums)/sizeof(int), 5));
  
  return 0;
}
