#include<stdio.h>
#include<stdlib.h>

#include<memory.h>

typedef signed char sByte;

// walls   : -1
// want    : 0
// guards  : 1
// guarded : >1

int countUnguarded(int m, int n, int** guards, int guardsSize, int* guardsColSize, int** walls, int wallsSize, int* wallsColSize) {
  const static sByte directions[] = {1,-1,1,-1}; // 0: right 1: left 2: up 3: down
  
  int map_size = (m+2)*(n+2)*sizeof(sByte);
  sByte* map = (sByte*)malloc(map_size);
  memset(map, -1, map_size); // bound
  for(int i = 0; i < m; i++)
    memset(map + (n+2) * (i + 1) + 1, 0, n * sizeof(sByte));
  
  for(int i = 0; i < wallsSize; i++)
  {
    int coord = (n + 2)*(walls[i][0] + 1) + (walls[i][1] + 1);
    map[coord] = -1;
  }
  
  int* q = malloc(sizeof(int)*guardsSize);
  
  for(int i = 0; i < guardsSize; i++)
  {
    int coord = (n + 2) * (guards[i][0] + 1) + (guards[i][1] + 1);
    q[i] = coord;
    map[coord] = 1;
  }

#if DEBUG
  for(int i = 0; i < map_size; i++)
    printf("%2d%c", (int)map[i], !((i+1)%(n+2))?'\n':' ');
#endif
  
  int ans = m * n - wallsSize - guardsSize;
  for(int i = 0; i < guardsSize; i++) // iterate all guard
  {
    int coord = q[i];
    for(sByte dir = 0; dir < 4; dir++)
    {
      int crd = coord;
      do
      {
        if(!map[crd])
        {
          ans--;
          map[crd] = 2;
        }
        crd=(dir>1)?(crd+directions[dir]*(n+2)):(crd+directions[dir]);
      }
      while((map[crd]!=1)&&(map[crd]!=-1));
    }
  }

#if DEBUG
  printf("========================\n");
  for(int i = 0; i < map_size; i++)
    printf("%2d%c", (int)map[i], !((i+1)%(n+2))?'\n':' ');
#endif
  
  return ans;
}

int main(void)
{
  int m = 4, n = 6;
  int guards_size = 3;
  int gs[] = {0,0,1,1,2,3};
  int** guards = malloc(sizeof(int)*guards_size);
  for(int i = 0; i < guards_size; i++)
  {
    guards[i] = malloc(sizeof(int)*2);
    guards[i][0] = gs[i*2];
    guards[i][1] = gs[i*2+1];
  }
  
  int walls_size = 3;
  int ws[] = {0,1,2,2,1,4};
  int** walls = malloc(sizeof(int)*guards_size);
  for(int i = 0; i < walls_size; i++)
  {
    walls[i] = malloc(sizeof(int)*2);
    walls[i][0] = ws[i*2];
    walls[i][1] = ws[i*2+1];
  }
  
  int p;
  int ans = countUnguarded(m,n,guards,guards_size,&p,walls,walls_size,&p);
  printf("%d\n", ans);
  
  return 0;
}
