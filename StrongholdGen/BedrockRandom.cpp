//
//  BedrockRandom.cpp
//  StrongholdGen
//
//  Created by Matthew Ferguson on 6/02/21.
//

#include "BedrockRandom.hpp"


BedrockRandom::BedrockRandom(int seed) {
           valid = true;
           _setSeed(seed);
    };
BedrockRandom::BedrockRandom() {
}
void BedrockRandom::_initGenRandFast(int initialValue) {
           mt[0] = initialValue;
           for (mtiFast = 1; mtiFast <= M; mtiFast++) {
               mt[mtiFast] = 1812433253
                       * ((mt[mtiFast - 1] >> 30) ^ mt[mtiFast - 1]) // note this used to be >>> for unsigned shift.
                       + mtiFast;
           }
           mti = N;
       }
void BedrockRandom::_setSeed(int seed) {
           seed = seed;
           mti = N + 1; // uninitialized
           haveNextNextGaussian = false;
           nextNextGaussian = 0;
           _initGenRandFast(seed);
   };
void BedrockRandom::_initGenRand(int initialValue) {
           mt[0] = initialValue;
           for (mti = 1; mti < N; mti++) {
               mt[mti] = 1812433253
                       * ((mt[mti - 1] >> 30) ^ mt[mti - 1])
                       + mti;
           }
           mtiFast = N;
};
int BedrockRandom::nextInt()
{
   return _genRandInt32()>>1;
}

float BedrockRandom::nextFloat()
{
   /*unsigned int i=_genRandInt32();
   double f=i;//_genRandInt32();
   f*=TWO_POW_M32;
   return f;*/
   double f=_genRandInt32();
   return f*TWO_POW_M32;
};

int BedrockRandom::nextInt(int bound) {
       //if(log) System.out.println("nextInt(int bound)");
       if (bound > 0)
           return (int) (((unsigned long)(_genRandInt32())) %  bound);
       else
           return 0;
   }
int BedrockRandom::nextInt(int a, int b) {
       if (a < b)
           return a + nextInt(b - a);
       else
           return a;
   }
unsigned int BedrockRandom::_genRandInt32() {
       if (mti == N) {
           mti = 0;
       } else if (mti > N) {
           _initGenRand(5489);
           mti = 0;
       }

       if (mti >= N - M) {
           if (mti >= N - 1) {
               mt[N - 1] = MAG_01[mt[0] & 1]
               ^ (((mt[0] & LOWER_MASK) | (mt[N - 1] & UPPER_MASK)) >> 1)
                       ^ mt[M - 1];
           } else {
               mt[mti] = MAG_01[mt[mti + 1] & 1]
               ^ (((mt[mti + 1] & LOWER_MASK) | (mt[mti] & UPPER_MASK)) >> 1)
                       ^ mt[mti - (N - M)];
           }
       } else {
           mt[mti] = MAG_01[mt[mti + 1] & 1]
           ^ (((mt[mti + 1] & LOWER_MASK) | (mt[mti] & UPPER_MASK)) >> 1)
                   ^ mt[mti + M];

           if (mtiFast < N) {
               mt[mtiFast] = 1812433253
                       * ((mt[mtiFast - 1] >> 30) ^ mt[mtiFast - 1])
                       + mtiFast;
               mtiFast++;
           }
       }

       unsigned int ret = mt[mti++];
   ret = (((ret ^ (ret >> 11)) << 7) & 0x9d2c5680) ^ ret ^ (ret >> 11);
   ret = ((ret << 15) & 0xefc60000) ^ ret ^ ((((ret << 15) & 0xefc60000) ^ ret) >> 18);
   return ret;
       //return ret>>1;
};
//};
bool BedrockRandom::nextBool()
{
    return (_genRandInt32() & 0x8000000) != 0;
}
