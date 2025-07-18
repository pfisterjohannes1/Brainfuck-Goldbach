/*
C program that detects divisbility.
It is close to brainfuck, f() only uses ++ and --, no
 functions, pointers, for, >, <, ==, ... (only !=0 is allowed),
 switch case, else...
The idea is to have a easier experiment before using brainfuck

f should output the smallest divisor or prime >1, if it is equal to prime it is actually prime.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>



void f(int prime)
{
  //Test if prime is prime

  int r=0;
  int t=0;
  int c=1; //possible diviser we Currently test for
  int bb[]={0,1,0}; //placeholder, we later need it to implement if in brainfuck
  int a=0;
  int searching = 1;
  while(searching)
  {
    c++;
    r=1; //Running
    t=0;
    bb[0]=prime;
    a=c;
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
//    printf("%d\n",bb[0]);
    searching--;
    if(bb[0])
      { searching++; }
  }
  //while(prime)
    {
    }
  printf("%d %d\n",prime,c);
  
}





int main(int argc, char **argv)
  {
    int a,b;
    (void)argc;
    (void)argv;
    //for(a=1;a<20;a++)
      for(b=2;b<200;b++)
      {
        f(b);
     //   printf("%d %d\n",a,b);
      }
    return 0;
  }



