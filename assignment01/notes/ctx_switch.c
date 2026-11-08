// 编译运行：gcc -O2 -pthread -o ctx_switch ctx_switch.c && ./ctx_switch
// 两个线程绑定在同一个 CPU 上，用两根管道互相传 1 字节（ping-pong）。
// 每次传递都迫使内核从一个线程切换到另一个线程。
// 输出：每次单程传递的平均耗时 = 一次上下文切换 + 一次 write + 一次 read 系统调用。
// 对照组：同一线程对自己的管道 write 再 read，不发生线程切换，只有系统调用开销。
#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#define ROUNDS 200000
static int p1[2], p2[2];
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
static void pin(int cpu){cpu_set_t s;CPU_ZERO(&s);CPU_SET(cpu,&s);pthread_setaffinity_np(pthread_self(),sizeof s,&s);}
static void *peer(void *a){(void)a;pin(0);char c;for(int i=0;i<ROUNDS;i++){read(p1[0],&c,1);write(p2[1],&c,1);}return 0;}
int main(void){
  pipe(p1);pipe(p2);pin(0);char c='x';
  int q[2];pipe(q);double t0=now();
  for(int i=0;i<ROUNDS;i++){write(q[1],&c,1);read(q[0],&c,1);}
  double base=(now()-t0)/ROUNDS;
  pthread_t th;pthread_create(&th,0,peer,0);
  t0=now();
  for(int i=0;i<ROUNDS;i++){write(p1[1],&c,1);read(p2[0],&c,1);}
  double t=(now()-t0)/(2.0*ROUNDS);
  pthread_join(th,0);
  printf("同线程 write+read（无切换）: %.2f us\n",base*1e6);
  printf("跨线程单程传递（含 1 次切换）: %.2f us\n",t*1e6);
  printf("差值（约等于一次切换的开销）: %.2f us\n",(t-base)*1e6);
}
