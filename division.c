/*
C program that is a playing ground for algorithm in brainfuck
It is close to brainfuck, f() only uses ++ and --, no
 functions, pointers, for, >, <, ==, ... (only !=0 is allowed),
 switch case, else...
Allowed is ++, --, while(n), if, =, int is allowed.
printf() for debugging is ok.
The idea is to have a easier experiment before using brainfuck

f tests if a integer >3 is the sum of 2 primes
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>



void f(int N)
{
  //Test if N is sumnof 2 primes

  int s1=2;
  int s2=N-2;
  int found=0;
  while(s2-1)
    {
      int bothPrime=0;
      int testS2=2;
      while(testS2)
        {
          testS2--;
          int prime=s1;
          if(testS2)
            {
              prime=s2;
            }
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
              searching--;
              if(bb[0])
                { searching++; }
            }
          while(prime)
            {
              prime--;
              c--;
            }
          bothPrime++;
          if( c )
            {
              bothPrime--;
            }
        }

      bothPrime--;
      bothPrime--;
      found++;
      if( bothPrime )
        {
          found--;
        }
      s2--;
      s1++;
    }
  //found has the number of how many pairs we found that are both prime
  printf("found %i %i\n",found,N);
}





int main(int argc, char **argv)
  {
    int a,b;
    (void)argc;
    (void)argv;
    for(b=4;b<200;b++)
      {
        f(b);
      }
    return 0;
  }



