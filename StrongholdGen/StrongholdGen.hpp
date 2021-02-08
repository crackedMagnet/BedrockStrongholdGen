//
//  StrongholdGen.hpp
//  StrongholdGen
//
//  Created by Matthew Ferguson on 6/02/21.
//

#ifndef StrongholdGen_hpp
#define StrongholdGen_hpp

#include <stdio.h>
#include "BedrockRandom.hpp"

enum Direction:int{SOUTH,WEST,NORTH,EAST};
struct GenResult
{
    int seed;
    int cx;
    int cz;
    int count;
    bool oneChunk;
    int dx;
    int dz;
};
class Box
{
public:
    Box(int minX,int minY,int minZ,int maxX,int maxY,int maxZ);
    Box(Direction dir, int x, int y, int z, int minX,int minY,int minZ,int maxX,int maxY,int maxZ);
    //Box();
    int getX(Direction dir,int x, int z);
    int getY(int y);
    int getZ(Direction dir,int x, int z);
    bool contains(int x, int z);
    
    bool overlaps(Box* box2);
//    bool overlaps2(Box box2);
    void outputToConsole();
    int minX;
    int maxX;
    int minY;
    int maxY;
    int minZ;
    int maxZ;
};

struct BlockPos
{
    int x;
    int y;
    int z;
};






enum RoomType{Corridor,Prison,LeftTurn,RightTurn,LargeRoom,StrightStairs,SpiralStairs,FiveWay,ChestCorridor,Library,PortalRoom,Filler};

class Room
{
public:
    //Room(Direction dir,RoomType rt, BedrockRandom *rand, int depth, Box *box);
    Room(Direction dir,RoomType rt, BedrockRandom *rand,int depth, Box *box, bool rollDoor);
    ~Room()
    {
        delete roomBox;
    }
    
    int getCount(Box *box);
    RoomType roomType;
    Direction direction;
    Box *roomBox=nullptr;
    int genDepth=0;
    //5 way exits
    bool fl; //front left
    bool fr; //front right
    bool bl; //back left
    bool br; //back right
    
    //Corridor exits
    bool leftExit;
    bool rightExit;
    
    //library height
    bool isHigh;
    
    //large room type, we only care about the storeroom as that has a chest.
    bool isStoreRoom;
    
    
    
    
};
class Stronghold
{
private:
    int RoomWeights[11]={40,5,20,20,10,5,5,5,5,10,10};
    int PlaceLimits[11]={0,5,0,0,6,5,5,4,4,2,1};
    int minDepth[11]={0,0,0,0,0,0,0,0,0,4,5};
    RoomType lastRoom;
public:
    long overlapCallCount=0;
    int placeCount[11]={0,0,0,0,0,0,0,0,0,0};
    Room *rooms[20000]; //just a guess at the required number.  will sort that out later.
    int roomPos=0;
    int totalWeight=135;
    int pendingRoomsIdx[2000];
    int pendingRoomsCount=0;
    RoomType forceNextPiece=Corridor;
    Room *portalRoom=nullptr;
    void addRoom(Room *room)
    {
        rooms[roomPos]=room;
        pendingRoomsIdx[pendingRoomsCount]=roomPos;
        pendingRoomsCount++;
        roomPos++;
        
    }
    void reset();
    Box *startBox=nullptr;
    void addExits(Room *room,BedrockRandom *rand);
    Room* selectRoom(BedrockRandom rand, int x, int y, int z, Direction dir, int depth);
    Room* createRoomForward(Room* fromRoom, BedrockRandom *rand, int x, int y);
    Room* createRoomLeft(Room* fromRoom, BedrockRandom *rand, int x, int y);
    Room* createRoomRight(Room* fromRoom, BedrockRandom *rand, int x, int y);
    Room* createRoom(RoomType rt, BedrockRandom *rand, int x, int y, int z, Direction dir, int depth);
   
    Box* fillBox(Direction dir, int x, int y, int z);
    bool hitsAnotherRoom(Box* box);
    
};

class StrongholdGen
{
public:
    StrongholdGen();
    GenResult* generate(int seed, int chunkx, int chunkz);
    long badStrongholdCount=0;
private:
    BedrockRandom rand;
    Stronghold sh;
    
};


#endif /* StrongholdGen_hpp */
