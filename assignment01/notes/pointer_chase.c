// 编译运行：gcc -O2 -o pointer_chase pointer_chase.c && ./pointer_chase
// 注意：数组按 4 KB 页随机访问，测得的时间包含 TLB miss 后的页表遍历。
// pointer chasing：数组每个元素存下一个要读的下标，顺序是随机排列的一个环。
// 每次读取都依赖上一次的结果，所以测到的是单次访存的延迟。
// 数组 512 MB，远大于 cache，绝大多数读取都要访问 DRAM。
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N (64L*1024*1024)   // 64M 个 size_t = 512 MB
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
int main(void){
  size_t *a=malloc(N*sizeof *a), *perm=malloc(N*sizeof *perm);
  for(size_t i=0;i<N;i++) perm[i]=i;
  srand(1); for(size_t i=N-1;i>0;i--){size_t j=((size_t)rand()<<16^rand())%(i+1);size_t t=perm[i];perm[i]=perm[j];perm[j]=t;}
  for(size_t i=0;i<N;i++) a[perm[i]]=perm[(i+1)%N];
  size_t p=perm[0]; const long STEPS=20000000;
  double t0=now(); for(long i=0;i<STEPS;i++) p=a[p]; double t=now()-t0;
  printf("每次依赖读取平均 %.1f ns (p=%zu)\n",t/STEPS*1e9,p);
}
