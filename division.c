/*
C program that detects divisbility.
It is close to brainfuck, f() only uses ++ and --, no
 functions, pointers, for, >, <, ==, ... (only !=0 is allowed),
 switch case, else...
The idea is to have a easier experiment before using brainfuck
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

void f(int a,int b);
int main(int argc, char **argv)
  {
    int a,b;
    (void)argc;
    (void)argv;
    for(a=1;a<20;a++)
      for(b=1;b<20;b++)
      {
        f(a,b);
        printf("%d %d\n",a,b);
      }
    return 0;
  }


void f(int a,int b)
{
  int r,t;
  
  r=1; //Running
  t=0;
  int bb[]={b,1,0}; //placeholder, we later need it to implement if in brainfuck
  while(r)
  {
    t=a;
    while(t)
    {
      r--;
      t--;
      bb[0]--;
      if(bb[0])
        { r++; }
    }
  }
  printf("%d\n",bb[0]);
}




