/*
C program that is a playing ground for algorithm in brainfuck
It is close to brainfuck, f() only uses ++ and --, no
 functions, pointers, for, >, <, ==, ... (only !=0 is allowed),
 switch case, else, if, = (except to set pointer p) ...
Allowed is ++, --, while( n ), int and setting p when position is
known is allowed.
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

//We need some helper variables when we want to emulate a if
//Use this when we make a variable that can be used like
// if( variable )
//We need <variable> 1 0 0 in the array.
//in the end of the if we go to the first 0
//after that, the change from 1 to 0 is used to align
//the pointer again to a known position
#define IFVAR(name) \
  name,             \
  name ## 1,        \
  name ## 2,        \
  name ## end,      \

//if we use 2 variables next to each other, that both need if
//use this for the second. it reduces needed variables
//We use <variableA> 1 0 0 1 <variableB> in the array.
#define IFVAR_R(name) \
  name ## 1,        \
  name,             \


enum VariablePosition_T
{
  V_N,
  V_found,
  V_temp1,
  V_s1,
  V_s2,
  V_bothPrime, //increased for every summand which is prime
  IFVAR(V_b)
  IFVAR_R(V_testS2)
  V_c,
  V_prime,
  V_r,
  V_t,
  V_searching,
};

#define ADDEQUAL( to, from, temp ) \
  while( d[from] )                 \
    {                              \
      d[from]--;                   \
      d[temp]++;                   \
      d[to]++;                     \
    }                              \
  while( d[temp] )                 \
    {                              \
      d[temp]--;                   \
      d[from]++;                   \
    }                              \

//To simulate if with only while. use it only for IFVAR variables
#define IF( name ) \
  d[name##1]++;    \
  p=name;          \
  while( d[p] )    \
    {

//Last 2 -- is to set p back to name
#define IF_END(name)\
      p=name;      \
      p++;         \
      p++;         \
    }              \
  p++;             \
  while( d[p] )    \
    { p++; }       \
  p--;             \
  while( d[p] )    \
    { p++; }       \
  p--;             \
  p--;             \

//To simulate if with only while. use it only for IFVAR_R variables
#define IFR( name )\
  d[name##1]++;    \
  p=name;          \
  while( d[p] )    \
    {

#define IFR_END(name)\
      p=name;      \
      p--;         \
      p--;         \
    }              \
  p--;             \
  while( d[p] )    \
    { p--; }       \
  p++;             \
  while( d[p] )    \
    { p--; }       \
  p++;             \
  p++;             \

#ifndef GEN_SIMPLE
int main(void)
{
  size_t p=0; //Data pointer, in brainfuck it could be manipulated with <>
  int d[100]={0};
#endif
  d[V_found]++;
  d[V_N]++;
  d[V_N]++;
  #if TESTHALT
    d[V_N]--;
  #endif
  while( d[V_found] )
    {
      while( d[V_found] ) { d[V_found]--; }
      d[V_N]++;
      d[V_N]++;
      #if TESTHALT
        d[V_N]++;
      #endif
      //Test if N is sumnof 2 primes
      while( d[V_s1] ) { d[V_s1]--; }
      d[V_s1]++;
      d[V_s1]++;
      ADDEQUAL( V_s2, V_N, V_temp1 );
      d[V_s2]--;
      d[V_s2]--;
      d[V_s2]--;
      while( d[V_s2] ) //we decrease second sumand and increase first till second one is 1
        {
          d[V_s2]++;
          d[V_testS2]++; //Which summand we test and how many loops (2).
          d[V_testS2]++;
          while( d[V_testS2] ) //test both summands for beeing prime
            {
              d[V_testS2]--;
              ADDEQUAL( V_prime, V_s1, V_b2 );
              IFR(V_testS2)
                  while( d[V_prime] ) { d[V_prime]--; }
                  ADDEQUAL( V_prime, V_s2, V_b2 );
              IFR_END(V_testS2)
              d[V_c]++; //we test if this is a divisor. start with 1+1=2
              d[V_searching]++;
              while( d[V_searching] ) //search for the smallest divisor >1
                {
                  d[V_c]++;
                  d[V_r]++; //Running
                  ADDEQUAL( V_b, V_prime, V_b2 );
                  while( d[V_r] ) //we calculate prime%c or bb%a
                    {
                      ADDEQUAL( V_t, V_c, V_b2 );
                      while( d[V_t] )
                        {
                          d[V_r]--;
                          d[V_t]--;
                          d[V_b]--;

                          IF( V_b )
                              d[V_r]++;
                          IF_END( V_b )
                        }
                    }
                  d[V_searching]--;
                  IF(V_b) //set when c was not a divisor of prime (prime%c!=0)
                    d[V_searching]++;
                  IF_END( V_b )
                  while( d[V_b] )  { d[V_b]++; }
                }
              //test if c==prime, if this is true the smallest divisor is prime
              // and prime is actually a prime number
              while( d[V_prime] )
                {
                  d[V_prime]--;
                  d[V_c]--;
                }
              d[V_bothPrime]++;
              while( d[V_c] ) //true if c!=prime => not a prime number
                {
                  d[V_bothPrime]--;
                  while( d[V_c] )  { d[V_c]++; }
                }
            }


          //found a pair if bothPrime is 2 (both summands are prime)
          d[V_bothPrime]--;
          d[V_bothPrime]--;
          while( d[V_bothPrime] )
            {
              d[V_found]--;
              while( d[V_bothPrime] )  { d[V_bothPrime]++; }
            }
          d[V_found]++;
          d[V_s1]++;
          d[V_s2]--; //test next summand pair
          d[V_s2]--;
        }
      printf("found TN %i, %i\n",d[V_N],d[V_found]);
    }
#ifndef GEN_SIMPLE
}
#endif








