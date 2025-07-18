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



int main(void)
{

  size_t p=0; //Data pointer, in brainfuck it could be manipulated with <>

  int found=1;
  int N=2;
  while(found)
    {
      N++;
      N++;
      //Test if N is sumnof 2 primes
      int s1=2;
      int s2=N-2;
      found=0;
      while(s2-1) //we decrease second sumand and increase first till second one is 1
        {
          int bothPrime=0; //increased for every summand which is prime
          int testS2=2; //Which summand we test and how many loops (2).
          while(testS2) //test both summands for beeing prime
            {
              testS2--;
              int prime=s1; //whicch number we test for beeing prime
              if(testS2)
                {
                  prime=s2;
                }
              int r=0;
              int t=0;
              int c=1; //we test if this is a divisor.
              int bb[]={0,1,1,0}; //placeholder, we later need it to implement if in brainfuck
              int a=0;
              int searching = 1;
              while(searching) //search for the smallest divisor >1
                {
                  c++;
                  r=1; //Running
                  t=0;
                  bb[0]=prime; //copy of prime we can decrease.
                  a=c;
                  while(r) //we calculate prime%c or bb%a
                    {
                      t=a;
                      while(t)
                        {
                          r--;
                          t--;
                          bb[0]--;

                          //simulate if(bb[0]) with while
                          p=0;
                          while(bb[p])
                            {
                              r++;
                              p++;
                              p++;
                            }
                          p++;
                          while(bb[p])
                            { p++; }
                          printf("p %zu\n",p);
                        }
                    }
                  searching--;
                  if(bb[0]) //set when c was not a divisor of prime (prime%c!=0)
                    { searching++; }
                }
              //test if c==prime, if this is true the smallest divisor is prime
              // and prime is actually a prime number
              while(prime)
                {
                  prime--;
                  c--;
                }
              bothPrime++;
              if( c ) //true if c!=prime => not a prime number
                {
                  bothPrime--;
                }
            }

          found++;

          //found a pair if bothPrime is 2 (both summands are prime)
          bothPrime--;
          bothPrime--;
          if( bothPrime )
            {
              found--;
            }
          s2--; //test next summand pair
          s1++;
        }
      printf("found N %i, %i\n",N,found);
    }
}








