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
  V_found,     //How many prime pairs did we found for V_s1+V_modS2
  V_isPrime,   //Was the last test a prime number
  V_testSummand, //if there is a summand left to test or did we test s1 and s2
  V_s1,        //summand 2, s1+V_modS2=N, N is the number we test

  //We use a optimized mod algorithm
  //This order should not be changed without changing the algorithm
  V_mod0,         //has 0 to indicate start position
  V_modS2,        //the number we check to be prime or not, and a summand
  V_modS2_,       //moved from V_modS2 durring modulo operation
  V_modDivisor,   //current divisor to check if V_modS2 is a multiple of
  V_modDivisor1,  //move from V_modDivisor durring modulo operation
  V_mod01,        //0 to mark end position / to move 0 to 0
  V_mod02,        //0 to have a NOP-move (0 to 0)

  V_searching, //we still search for a divisor


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

#define DO8(t)   t; t; t; t; t; t; t; t;
#define DO32(t)  DO8(t) DO8(t) DO8(t) DO8(t)

#define ADD32(name) DO32(d[name]++)

#define SUB32(name) DO32(d[name]--)

#ifndef GEN_SIMPLE
size_t p=0; //Data "pointer", in brainfuck it can be manipulated with <>
int d[100]={0};

const char *varName(enum VariablePosition_T i)
  {
    switch(i)
      {
        case V_found:            return "V_found:      ";
        case V_testSummand:      return "V_testSummand:";
        case V_s1:               return "V_s1:         ";
        case V_isPrime:          return "V_isPrime:    ";

        case V_mod0:             return "V_mod0        ";
        case V_modS2:            return "V_modS2       ";
        case V_modS2_:           return "V_modS2_      ";
        case V_modDivisor:       return "V_modDivisor  ";
        case V_modDivisor1:      return "V_modDivisor1 ";
        case V_mod01:            return "V_mod01       ";
        case V_mod02:            return "V_mod02       ";

        case V_searching:        return "V_searching:  ";
        default:                 break;
      }
    return "IMVALID";
  }

void debug(const char *m)
  {
    fprintf(stderr,"debug %s:\n",m);
    for(int i=0;i<V_COUNT;i++)
      {
        fprintf(stderr,"%s: %i\n",varName(i),d[i]);
      }
  }

void print(void)
  {
    printf("%i\n",d[p]);
    fflush(stdout);
  }

int main(void)
{
#endif
  #if !TESTHALT
    d[V_s1]++;
  #endif
  d[V_found]++;
  while( d[V_found] )
    {
      while( d[V_found] )
        { d[V_found]--; }
      //V_s1 has the current value, bit we start with V_s1=2, V_s2=N-2 and
      // check everything till we stop at V_s1=N-1, V_s2=1 (don't test this case)
      while( d[V_s1] )
        {
           d[V_s1]--;
           d[V_modS2]++;
        }
      d[V_s1]++;


      d[V_s1]++;
      #if TESTHALT
        d[V_modS2]++;
      #endif
      while( d[V_modS2] ) //we decrease second sumand and increase first till second one is 1
        {
          //This way we test each possible pair s1+s2=N with 1<s2<N-2
          //Where N is the number we want to test
          //We could only test up to s1=s2, but this code is probably shorter?
          d[V_modS2]++;
          d[V_testSummand]++; //loop counter to test s2 then s1
          d[V_testSummand]++;
          while( d[V_testSummand] ) //test both summands for beeing prime
            {
              d[V_testSummand]--;
              d[V_modDivisor]++; //we test if this is a divisor. start with 1+1=2
              d[V_searching]++;
              while( d[V_searching] ) //Do we still search for a divisor?
                {
                  d[V_modDivisor]++;  //Test all possible divisors >1 till we find one
                  //Modulo operation, calc V_modS2%V_modDivisor
                  while( d[V_modS2] ) //we calculate s2%divisor
                    {
                      d[V_modS2_]++; //create a backup of V_modS2
                      d[V_modDivisor]--;
                      d[V_modDivisor1]++;
                      p=V_modS2;
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
                      //aligin pointer back to V_modS2
                      p--; p--;
                      while( d[p] )
                        { p--; }
                      p++;
                      d[V_modS2]--;
                    }
                  //We now have V_modS2=0, V_modS2_=<oldPrime>,
                  //V_modDivisor1=V_modS2%V_modDivisor (of before the loop)

                  while( d[V_modS2_] ) //restore V_modS2 to the old value
                    {
                      d[V_modS2_]--;
                      d[V_modS2]++;
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
                  d[V_modS2]--;
                  d[V_modDivisor]--;
                  d[V_mod0]++; //keep a copy of V_modS2
                }
              d[V_isPrime]++;
              while( d[V_modS2] ) //true if V_modS2 is not a prime number
                {
                  d[V_isPrime]--;
                  while( d[V_modS2] )
                    {
                       d[V_modS2]--;
                       d[V_mod0]++; //keep a copynof V_modS2
                    }
                }
              //swap data between s1 and V_modS2 (now in V_mod0)
              while( d[V_s1] )
                {
                   d[V_s1]--;
                   d[V_modS2]++;
                }
              while( d[V_mod0] )
                {
                   d[V_mod0]--;
                   d[V_s1]++;
                }
            }

          //found a pair of primes if V_isPrime is 2
          d[V_found]++;
          d[V_isPrime]--;
          d[V_isPrime]--;
          while( d[V_isPrime] )
            {
              d[V_found]--;
              while( d[V_isPrime] )
                { d[V_isPrime]++; }
            }
          d[V_s1]++; //test next summand pair
          d[V_modS2]--;
          d[V_modS2]--;
        }
      debug("end");
      #if PRINT
        d[V_s1]++;
        p=V_s1;
        #if PRINTASCII
          ADD32(V_modS2);
          print();
          SUB32(V_modS2);
        #else
          print();
        #endif
        d[V_s1]--;
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



