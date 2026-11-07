// 依赖链实验：K 条互相独立的 double 加法链，总加法次数 N 固定，打印每次加法的平均耗时。
// 编译运行：gcc -O2 -fno-tree-vectorize -o ilp_chains ilp_chains.c && ./ilp_chains
// -fno-tree-vectorize：禁止编译器把多条链合并成 SIMD 指令，保证每次加法是一条标量 addsd。
// 不加 -ffast-math：编译器不得重排浮点加法，K=1 的链保持串行。
#include <stdio.h>
#include <time.h>
#define N 480000000L
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
static void run1(double x){
  double s0=0;
  double t0=now();
  for(long i=0;i<N/1;i++){ s0+=x; }
  double t=now()-t0;
  printf("K=%2d  per add = %.3f ns  checksum=%.1f\n",1,t/N*1e9,s0);
}
static void run2(double x){
  double s0=0, s1=1;
  double t0=now();
  for(long i=0;i<N/2;i++){ s0+=x; s1+=x; }
  double t=now()-t0;
  printf("K=%2d  per add = %.3f ns  checksum=%.1f\n",2,t/N*1e9,s0+s1);
}
static void run4(double x){
  double s0=0, s1=1, s2=2, s3=3;
  double t0=now();
  for(long i=0;i<N/4;i++){ s0+=x; s1+=x; s2+=x; s3+=x; }
  double t=now()-t0;
  printf("K=%2d  per add = %.3f ns  checksum=%.1f\n",4,t/N*1e9,s0+s1+s2+s3);
}
static void run8(double x){
  double s0=0, s1=1, s2=2, s3=3, s4=4, s5=5, s6=6, s7=7;
  double t0=now();
  for(long i=0;i<N/8;i++){ s0+=x; s1+=x; s2+=x; s3+=x; s4+=x; s5+=x; s6+=x; s7+=x; }
  double t=now()-t0;
  printf("K=%2d  per add = %.3f ns  checksum=%.1f\n",8,t/N*1e9,s0+s1+s2+s3+s4+s5+s6+s7);
}
static void run12(double x){
  double s0=0, s1=1, s2=2, s3=3, s4=4, s5=5, s6=6, s7=7, s8=8, s9=9, s10=10, s11=11;
  double t0=now();
  for(long i=0;i<N/12;i++){ s0+=x; s1+=x; s2+=x; s3+=x; s4+=x; s5+=x; s6+=x; s7+=x; s8+=x; s9+=x; s10+=x; s11+=x; }
  double t=now()-t0;
  printf("K=%2d  per add = %.3f ns  checksum=%.1f\n",12,t/N*1e9,s0+s1+s2+s3+s4+s5+s6+s7+s8+s9+s10+s11);
}
static void run16(double x){
  double s0=0, s1=1, s2=2, s3=3, s4=4, s5=5, s6=6, s7=7, s8=8, s9=9, s10=10, s11=11, s12=12, s13=13, s14=14, s15=15;
  double t0=now();
  for(long i=0;i<N/16;i++){ s0+=x; s1+=x; s2+=x; s3+=x; s4+=x; s5+=x; s6+=x; s7+=x; s8+=x; s9+=x; s10+=x; s11+=x; s12+=x; s13+=x; s14+=x; s15+=x; }
  double t=now()-t0;
  printf("K=%2d  per add = %.3f ns  checksum=%.1f\n",16,t/N*1e9,s0+s1+s2+s3+s4+s5+s6+s7+s8+s9+s10+s11+s12+s13+s14+s15);
}
int main(int c,char**a){ double x=1e-9*c; run1(x); run2(x); run4(x); run8(x); run12(x); run16(x); return 0; }
