/*
This program should halt iff the goldbach conjecture is wrong.

C program which is very close to brainfuck
We have no functions*, no +, -, =, ==, !, %, *, /. <<, for, if, switch, ->, struct, ...
Only allow: ++, --, index "pointer" p and fixed indexes in one single array d, while()
 and one single enum (for variable positions inside d)
The idea is to reduce this so that it translates directly to brainfuck.

*The exception is the print(), and debug() function, that is hidden with macros and
 the calls are ignored on the conversation script.
*/


#ifndef TESTHALT //allow CLI-overwritting
  //set when we don't test even numbers but ever 3. number till 37
  //This way we can test if the code can halt
  #define TESTHALT    0
  #define PRINT       0 //Print the numbers we check and how many prime pairs we found
  #define PRINTASCII  0 //print as ascii characters (adds 32 before printing. Hard to read.
#endif


#ifndef GEN_SIMPLE
  #include <stdbool.h>
  #include <stdint.h>
  #include <stdio.h>
#endif


enum VariablePosition_T
{
  V_found,     //How many prime pairs did we found for V_s1+V_s2

  V_always0,
  V_always11,

  V_searching,
  //We use a optimized mod algorithm
  //This order should not be changed without changing the algorithm
  V_mod0,         //has 0 to indicate start position
  V_modPrime,     //the number we check to be prime or not
  V_modPrime1,    //moved from V_modDivisor1 durring modulo operation
  V_modDivisor,   //current divisor to check if V_modPrime is a multiple of
  V_modDivisor1,  //move from V_mod01 durring modulo operation
  V_mod01,        //0 to mark end position / to move 0 to 0
  V_mod02,        //0 to have a NOP-move (0 to 0)
  V_isPrime,       //Was the last test a prime number / did we already test s2

  V_always1,
  V_always12,

  V_2searching,
  V_2mod0,         //has 0 to indicate start position
  V_2modPrime,     //the number we check to be prime or not
  V_2modPrime1,    //moved from V_modDivisor1 durring modulo operation
  V_2modDivisor,   //current divisor to check if V_modPrime is a multiple of
  V_2modDivisor1,  //move from V_mod01 durring modulo operation
  V_2mod01,        //0 to mark end position / to move 0 to 0
  V_2mod02,        //0 to have a NOP-move (0 to 0)
  V_2isPrime,   //Was the last test a prime number / did we already test s2

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

#if GEN_SIMPLE
  #define MOVENEXT() \
    p++; p++; p++; p++; p++; p++; p++; p++; p++; p++; p++;
  #define MOVEPRE() \
    p--; p--; p--; p--; p--; p--; p--; p--; p--; p--; p--;

#else
  #define MOVENEXT() \
    d+=11;

  #define MOVEPRE() \
    d-=11;
#endif

#define DO8(t)   t; t; t; t; t; t; t; t;
#define DO32(t)  DO8(t) DO8(t) DO8(t) DO8(t)

#define ADD32(name) DO32(d[name]++)

#define SUB32(name) DO32(d[name]--)

#ifndef GEN_SIMPLE
size_t p=0; //Data "pointer", in brainfuck it can be manipulated with <>

int D[2000]={0};
int *d=D+50;

const char *varName(enum VariablePosition_T i)
  {
    switch(i)
      {
        case V_found:              return "V_found";
        case V_always0:            return "V_always0";
        case V_always11:           return "V_always11";
        case V_searching:          return "V_searching";
        case V_mod0:               return "V_mod0";
        case V_modPrime:           return "V_modPrime";
        case V_modPrime1:          return "V_modPrime1";
        case V_modDivisor:         return "V_modDivisor";
        case V_modDivisor1:        return "V_modDivisor1";
        case V_mod01:              return "V_mod01";
        case V_mod02:              return "V_mod02";
        case V_isPrime:            return "V_isPrime";
        case V_always1:            return "V_always1";
        case V_always12:           return "V_always12";
        case V_2searching:         return "V_2searching";
        case V_2mod0:              return "V_2mod0";
        case V_2modPrime:          return "V_2modPrime";
        case V_2modPrime1:         return "V_2modPrime1";
        case V_2modDivisor:        return "V_2modDivisor";
        case V_2modDivisor1:       return "V_2modDivisor1";
        case V_2mod01:             return "V_2mod01";
        case V_2mod02:             return "V_2mod02";
        case V_2isPrime:           return "V_2isPrime";
        default: break;
      }
    return "IMVALID";
  }

void debug(const char *m)
  {
    printf("debug %s: %lu (%s)\n",m,p,varName(p));
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
  d[V_always12]++;
  d[V_always1]++;

  #if !TESTHALT
    d[V_modPrime]++;
  #endif
  d[V_modPrime]++;

  d[V_always11]++;
  d[V_found]++;
  while( d[V_found] )
    {
      while( d[V_found] )
        { d[V_found]--; }

      //V_s1 has the current value, bit we start with V_s1=2, V_s2=N-2 and
      // check everything till we stop at V_s1=N-1, V_s2=1 (don't test this case)
      while( d[V_2modPrime] )
        {
           d[V_2modPrime]--;
           d[V_modPrime]++;
        }
      d[V_2modPrime]++;
      d[V_2modPrime]++;

      #if TESTHALT
        d[V_modPrime]++;
      #endif
      debug("before");

      d[V_modPrime]--;
      while( d[V_modPrime] ) //we decrease second sumand and increase first till second one is 1
        {
          //This way we test each possible pair s1+s2=N with 1<s2<N-2
          //Where N is the number we want to test
          //We could only test up to s1=s2, but this code is probably shorter?
          d[V_modPrime]++;

          while( d[V_always11] ) //test both summands for beeing prime
            {
              d[V_modDivisor]++; //we test if this is a divisor. start with 1+1=2
              d[V_searching]++;
              while( d[V_searching] ) //Do we still search for a divisor?
                {
                  d[V_modDivisor]++;  //Test all possible divisors >1 till we find one
                  //Modulo operation, calc V_modPrime%V_modDivisor
                  while( d[V_modPrime] ) //we calculate prime%divisor
                    {
                      d[V_modPrime1]++; //create a backup of V_modPrime
                      d[V_modDivisor]--;
                      d[V_modDivisor1]++;
                      p=V_modPrime;
                      p++;
                      p++;
                      //The following lines move back V_modDivisor1 to V_modDivisor iff
                      // V_modDivisor==0, move 0 to 0 otherwise (NOP).
                      while( d[p] )
                        { p++; }
                      p++;
                      while( d[p] )
                        {
                          d[p]--;
                          p--;
                          d[p]++;
                          p++;
                        }
                      //aligin pointer back to V_modPrime
                      p--; p--;
                      while( d[p] )
                        { p--; }
                      p++;
                      d[V_modPrime]--;
                    }
                  //We now have V_modPrime=0, V_modPrime1=<oldPrime>,
                  //V_modDivisor1=V_modPrime%V_modDivisor (of before the loop)

                  while( d[V_modPrime1] ) //restore V_modPrime to the old value
                    {
                      d[V_modPrime1]--;
                      d[V_modPrime]++;
                    }

                  d[V_searching]--;
                  while( d[V_modDivisor1] ) //0 when we found a divisor
                    {
                      d[V_searching]++;
                      while( d[V_modDivisor1] ) //restore divisor
                        {
                          d[V_modDivisor1]--;
                          d[V_modDivisor]++;
                        }
                    }
                }
              //here, V_modDivisor is the smallest divisor>1 of prime
              //test if c==prime, if this is true the smallest divisor is prime
              // and prime is actually a prime number
              while( d[V_modDivisor] )
                {
                  d[V_modPrime]--;
                  d[V_modPrime1]++;
                  d[V_modDivisor]--;
                }
              d[V_isPrime]++;
              while( d[V_modPrime] ) //true if V_modPrime is not a prime number
                {
                  d[V_isPrime]--;
                  while( d[V_modPrime] )
                    {
                       d[V_modPrime]--;
                       d[V_modPrime1]++;
                    }
                }
              while( d[V_modPrime1] ) //restore V_modPrime to the old value
                {
                  d[V_modPrime1]--;
                  d[V_modPrime]++;
                }
              MOVENEXT();
            }
          MOVEPRE();

          //found a pair of primes if V_isPrime is 1
          debug("neo");
          while( d[V_isPrime] )
            {
              d[V_isPrime]--;
              MOVEPRE();
              while( d[V_isPrime] )
                {
                  d[V_isPrime]--;
                  d[V_found]++;
                }
              MOVENEXT();
            }
          MOVEPRE();
          while( d[V_isPrime] )
            { d[V_isPrime]--; }
          d[V_2modPrime]++; //test next summand pair
          d[V_modPrime]--;

          d[V_modPrime]--;
        }
      d[V_modPrime]++;

      #if PRINT
        p=V_2modPrime;;
        d[V_2modPrime]++;
        #if PRINTASCII
          ADD32(V_2modPrime);
          print();
          SUB32(V_2modPrime);
        #else
          print();
        #endif
        d[V_2modPrime]--;
        p=V_found;
        #if PRINTASCII
          ADD32(V_found);
          print();
          SUB32(V_found);
        #else
          print();
        #endif
      #endif
    }
#ifndef GEN_SIMPLE
}
#endif



