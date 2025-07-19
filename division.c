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

//set when we don't test even numbers but ever 3. number till 37
//This way we can test if the code can halt
#define TESTHALT 0

#ifndef GEN_SIMPLE
  #include <stdbool.h>
  #include <stdint.h>
  #include <stdio.h>
#endif

#define IFVAR(name) \
  name,             \
  name ## 1,        \
  name ## 2,        \
  name ## end,      \

#define IFVAR_R(name) \
  name ## 1,        \
  name,             \


enum VariablePosition_T
{
  V_N,
  V_found,
  V_s1,
  V_s2,
  IFVAR(V_bothPrime)
  IFVAR_R(V_testS2)
  V_prime,
  IFVAR(V_b)
  V_a,
  IFVAR(V_c)
  V_r,
  V_t,
  V_searching,
};

#define IF( name ) \
  d[name##1]=1;    \
  p=name;          \
  while(d[p])      \
    {

#define IF_END     \
      p++;         \
      p++;         \
    }              \
  p++;             \
  while(d[p])      \
    { p++; }       \
  p--;             \
  while(d[p])      \
    { p++; }       \

#define IFR( name )\
  d[name##1]=1;    \
  p=name;          \
  while(d[p])      \
    {

#define IFR_END    \
      p--;         \
      p--;         \
    }              \
  p--;             \
  while(d[p])      \
    { p--; }       \
  p++;             \
  while(d[p])      \
    { p--; }       \

#ifndef GEN_SIMPLE
int main(void)
{
  size_t p=0; //Data pointer, in brainfuck it could be manipulated with <>
  int d[100]={0};
#endif
  d[V_found]=1;
  d[V_N]=2;
  #if TESTHALT
    d[V_N]=1;
  #endif
  while(d[V_found])
    {
      d[V_N]++;
      d[V_N]++;
      #if TESTHALT
        d[V_N]++;
      #endif
      //Test if N is sumnof 2 primes
      d[V_s1]=2;
      d[V_s2]=d[V_N];
      d[V_s2]--;
      d[V_s2]--;
      d[V_found]=0;
      while(d[V_s2]-1) //we decrease second sumand and increase first till second one is 1
        {
          d[V_bothPrime]=0; //increased for every summand which is prime
          d[V_testS2]=2; //Which summand we test and how many loops (2).
          while(d[V_testS2]) //test both summands for beeing prime
            {
              d[V_testS2]--;
              d[V_prime]=d[V_s1]; //whicch number we test for beeing prime
              IFR(V_testS2)
                {
                  d[V_prime]=d[V_s2];
                }
              IFR_END
              d[V_r]=0;
              d[V_t]=0;
              d[V_c]=1; //we test if this is a divisor.
              d[V_a]=0;
              d[V_searching] = 1;
              while(d[V_searching]) //search for the smallest divisor >1
                {
                  d[V_c]++;
                  d[V_r]=1; //Running
                  d[V_t]=0;
                  d[V_b]=d[V_prime]; //copy of prime we can decrease.
                  d[V_a]=d[V_c];
                  while(d[V_r]) //we calculate prime%c or bb%a
                    {
                      d[V_t]=d[V_a];
                      while(d[V_t])
                        {
                          d[V_r]--;
                          d[V_t]--;
                          d[V_b]--;

                          IF(V_b)
                              d[V_r]++;
                          IF_END
                        }
                    }
                  d[V_searching]--;
                  IF(V_b) //set when c was not a divisor of prime (prime%c!=0)
                    { d[V_searching]++; }
                  IF_END
                }
              //test if c==prime, if this is true the smallest divisor is prime
              // and prime is actually a prime number
              while(d[V_prime])
                {
                  d[V_prime]--;
                  d[V_c]--;
                }
              d[V_bothPrime]++;
              IF( V_c ) //true if c!=prime => not a prime number
                  d[V_bothPrime]--;
              IF_END
            }

          d[V_found]++;

          //found a pair if bothPrime is 2 (both summands are prime)
          d[V_bothPrime]--;
          d[V_bothPrime]--;
          IF( V_bothPrime )
              d[V_found]--;
          IF_END
          d[V_s2]--; //test next summand pair
          d[V_s1]++;
        }
      printf("found N %i, %i\n",d[V_N],d[V_found]);
    }
}








