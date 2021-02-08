//
//  main.cpp
//  StrongholdGen
//
//  Created by Matthew Ferguson on 6/02/21.
//

#include <iostream>
#include "StrongholdGen.hpp"
#include <fstream>




int main(int argc, const char * argv[]) {
    int startSeed=std::stoi(argv[1]);
    //decseedfind(780100000, 3000);
    int seedJump=std::stoi(argv[2]);
    //std::string outfileprefix=argv[3];
    std::ofstream OutFileInsideChunk("/Users/matthewferguson/Projects/Minecraft XCode/Output/StrongholdGen"+std::to_string(startSeed)+" - One Chunk.txt");
    std::ofstream OutFileSplitChunk("/Users/matthewferguson/Projects/Minecraft XCode/Output/StrongholdGen"+std::to_string(startSeed)+"- Split Chunk.txt");
    // insert code here...
    //std::cout << "Hello, World!\n";
    StrongholdGen shg;
    //shg.generate(0, -136, -676);
    auto begin = std::chrono::high_resolution_clock::now();
    int count=0;
    int seed=startSeed;
    bool go=true;
    while(go)
    {
        GenResult *result=shg.generate(seed, 0, 0);
        if(result!=nullptr)
        {
            if(!(result->cx==0&&result->cz==0)) //filter out anything where the x and z chunk offset is 0 as the gen seed and the dec seed would have to be the same.
            {
                if(result->oneChunk)
                {
                    OutFileInsideChunk<<std::to_string(result->seed)<<"\t"<<std::to_string(result->cx)<<"\t"<<std::to_string(result->cz)<<"\t"<<std::to_string(result->count)<<"\n";
                    OutFileInsideChunk.flush();
                }
                else
                {
                    OutFileSplitChunk<<std::to_string(result->seed)<<"\t"<<std::to_string(result->cx)<<"\t"<<std::to_string(result->cz)<<"\t"<<std::to_string(result->count)<<"\t"<<std::to_string(result->dx)<<"\t"<<std::to_string(result->dz)<<"\n";
                    OutFileSplitChunk.flush();
                }
            }
            delete result;
        }
        seed+=seedJump;
        
        count++;
        if(count>=100000)
        {
            auto end = std::chrono::high_resolution_clock::now();
                auto dur = end - begin;
                auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
            float rate=((float)(count))/(float)ms;
            rate*=1000.0f;
            //lastReport=newTime;
            begin=end;
            count=0;
            std::cout<< std::to_string(seed) <<" - "<<std::to_string(ms)<<" ms - "<<std::to_string(rate)<<"/sec FakeStrongholdCount="<<std::to_string(shg.badStrongholdCount)<<"\n";
        }
    }
    
    OutFileInsideChunk.close();
    OutFileSplitChunk.close();
    return 0;
}
