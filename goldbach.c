/*
C program that is a playing ground for algorithm in brainfuck
It is close to brainfuck, f() only uses ++ and --, no
 functions, pointers, for, >, <, ==, ... (only !=0 is allowed),
 switch case, else, if, = (except to set pointer p) ...
Allowed is ++, --, while( n ), int and setting p when position is
known is allowed.
printf() for debugging is ok.
The idea is to have a easier experiment before using brainfuck
*/

//set when we don't test even numbers but ever 3. number till 37
//This way we can test if the code can halt
#define TESTHALT 0
#define PRINT    0

#ifndef GEN_SIMPLE
  #include <stdbool.h>
  #include <stdint.h>
  #include <stdio.h>
#endif

//We need some helper variables when we want to emulate a if
//Use this when we make a variable that can be used like
// if( variable )
//We need <variable> 0 <non-0> in the array.
//in the end of the if we go to the 0
//after that, the change to 0 is used to align
//the pointer again to a known position
#define IFVAR(name) \
  name,             \
  name ## 0,        \



enum VariablePosition_T
{
  V_N,         //number we test for beeing sum of 2 primes
  V_found,     //How many prime pairs did we found for V_N
  V_testSummand, //if there is a summand left to test or did we test s1 and s2
  V_s2,        //summand 1, s1+s2=N
  V_s1,        //summand 2, s1+s2=N
  V_isPrime,   //Was the last test a prime number / did we already test s2
  V_prime,     //copy of s1 or s2 we count down for modulo operation used while tesing if prime
  IFVAR(V_b)   //copy of V_prime that we can count down for modulo operation
  //variable after this must be non-0 in the IF_END macro
  V_searching, //we still search for a divisor
  V_c,         //current divisor to test
  V_r,         //do we run modulo operation
  V_t,         //to substract V_b-V_c

  V_COUNT,
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
  p=name;          \
  while( d[p] )    \
    {

//Last 2 -- is to set p back to name
#define IF_END(name)\
      p=name;      \
      p++;         \
    }              \
  p++;             \
  while( d[p] )    \
    { p--; }       \
  p--;             \


#ifndef GEN_SIMPLE
size_t p=0; //Data pointer, in brainfuck it could be manipulated with <>
int d[100]={0};

const char *varName(enum VariablePosition_T i)
  {
    switch(i)
      {
        case V_N:                return "V_N:          ";
        case V_found:            return "V_found:      ";
        case V_s1:               return "V_s1:         ";
        case V_s2:               return "V_s2:         ";
        case V_isPrime:          return "V_isPrime:    ";
        case V_testSummand:      return "V_testSummand:";
        case V_b:                return "V_b:          ";
        case V_b0:               return "V_b0:         ";
        case V_c:                return "V_c:          ";
        case V_prime:            return "V_prime:      ";
        case V_r:                return "V_r:          ";
        case V_t:                return "V_t:          ";
        case V_searching:        return "V_searching:  ";
        default: break;
      }
    return "IMVALID";
  }

void debug(const char *m)
  {
    printf("debug %s:\n",m);
    for(int i=0;i<V_COUNT;i++)
      {
        printf("%s: %i\n",varName(i),d[i]);
      }
  }

void print(void)
  {
    printf("%i\n",d[p]);
  }

int main(void)
{
#endif
  d[V_N]++;
  #if !TESTHALT
    d[V_N]++;
  #endif
  d[V_found]++;
  while( d[V_found] )
    {
      while( d[V_found] ) { d[V_found]--; }
      //Test if N is sumnof 2 primes
      while( d[V_s1] ) { d[V_s1]--; }
      d[V_s1]++;
      d[V_s1]++;
      d[V_N]++;
      d[V_N]++;
      #if TESTHALT
        d[V_N]++;
      #endif
      #if PRINT
        p=V_N;
        print();
      #endif
      ADDEQUAL( V_s2, V_N, V_found );
      d[V_s2]--;
      d[V_s2]--;
      d[V_s2]--;
      while( d[V_s2] ) //we decrease second sumand and increase first till second one is 1
        {
          //This way we test each possible pair s1+s2=N with 1<s2<N-2
          //We could only test up to s1=s2, but this code is probably shorter?
          d[V_s2]++;
          d[V_testSummand]++; //loop counter to test s2 then s1
          d[V_testSummand]++;
          while( d[V_testSummand] ) //test both summands for beeing prime
            {
              d[V_testSummand]--;
              ADDEQUAL( V_prime, V_s2, V_b0 );
              while( d[V_isPrime] ) //we already tested s1 and it was prime, testns2 now
                { // if s1 was not prime, we test it s1 again (NOP)
                  d[V_isPrime]--;
                  while( d[V_prime] ) { d[V_prime]--; }
                  ADDEQUAL( V_prime, V_s1, V_isPrime );
                }
              d[V_c]++; //we test if this is a divisor. start with 1+1=2
              d[V_searching]++;
              while( d[V_searching] ) //Do we still search for a divisor?
                {
                  d[V_c]++;
                  d[V_r]++; //Running modulo operation
                  ADDEQUAL( V_b, V_prime, V_b0 );
                  while( d[V_r] ) //we calculate prime%c or b%a
                    {
                      d[V_r]--;
                      ADDEQUAL( V_t, V_c, V_r );
                      d[V_r]++;
                      while( d[V_t] )
                        {
                          d[V_t]--;
                          d[V_r]--;
                          d[V_b]--;
                          IF( V_b )
                              d[V_r]++;
                          IF_END( V_b )
                        }
                    }
                  d[V_searching]--;
                  while( d[V_b] ) //set when c was not a divisor of prime (prime%c!=0)
                    {
                      d[V_searching]++;
                      while( d[V_b] )  { d[V_b]++; }
                    }
                }
              //here, c is the smallest divisor>1 of prime
              //test if c==prime, if this is true the smallest divisor is prime
              // and prime is actually a prime number
              while( d[V_prime] )
                {
                  d[V_prime]--;
                  d[V_c]--;
                }
              d[V_isPrime]++;
              while( d[V_c] ) //true if c!=prime => not a prime number
                {
                  d[V_isPrime]--;
                  while( d[V_c] )  { d[V_c]++; }
                }
            }

          //found a pair of primes if V_isPrime is 1
          while( d[V_isPrime] )
            {
              d[V_found]++;
              d[V_isPrime]--;
            }
          d[V_s1]++;
          d[V_s2]--; //test next summand pair
          d[V_s2]--; //end at s2==1
        }
      #if PRINT
        p=V_found;
        print();
      #endif
    }
#ifndef GEN_SIMPLE
}
#endif



