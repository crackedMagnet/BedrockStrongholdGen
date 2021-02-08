//
//  BedrockRandom.hpp
//  StrongholdGen
//
//  Created by Matthew Ferguson on 6/02/21.
//

#ifndef BedrockRandom_hpp
#define BedrockRandom_hpp

#include <stdio.h>
class BedrockRandom
{
private: static const int N = 624;
private: static const int M = 397;
private: static const int MATRIX_A = 0x9908b0df;
private: static const int UPPER_MASK = 0x80000000;
private: static const int LOWER_MASK = 0x7fffffff;
private: constexpr static const int MAG_01[2] = {0, MATRIX_A};
private: constexpr static const double TWO_POW_M32 = 1.0 / (1L << 32);
private: int seed; // (DWORD*) this + 0
private: unsigned int mt[N];// = new int[624]; // (DWORD*) this + 1
private: int mti; // (DWORD*) this + 625
private: bool haveNextNextGaussian; // (DWORD*) this + 626
private: float nextNextGaussian; // (float*) this + 627
private: int mtiFast; // (DWORD*) this + 628

private: bool valid = false; // Hackfix for setSeed being called too early in the superconstructor
public:
    BedrockRandom(int seed);
    BedrockRandom();
    //BedrockRandom(BedrockRandom *toClone);
    void _setSeed(int seed);
    unsigned int _genRandInt32();
    void _initGenRandFast(int initialValue);
    void _initGenRand(int initialValue);
    int nextInt();
    float nextFloat();
    int nextInt(int a, int b);
    int nextInt(int bound);
    bool nextBool();
};

#endif /* BedrockRandom_hpp */
