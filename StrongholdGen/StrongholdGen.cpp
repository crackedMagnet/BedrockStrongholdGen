//
//  StrongholdGen.cpp
//  StrongholdGen
//
//  Created by Matthew Ferguson on 6/02/21.
//
#include <iostream>
#include "StrongholdGen.hpp"



StrongholdGen::StrongholdGen()
{
    
}

GenResult* StrongholdGen::generate(int seed, int chunkx, int chunkz)
{
        
    rand._setSeed(seed);
    int i1=rand.nextInt()|1;
    int i2=rand.nextInt()|1;
    rand._setSeed(i1*chunkx+i2*chunkz^seed);
    rand.nextInt(); //burn 1
    
    sh.reset();
    /*for(int i=0;i<2000;i++)
    {
        sh.rooms[i]=nullptr;
        sh.pendingRoomsIdx[i]=0;
    }*/
    int dirIdx=rand.nextInt(4);
    int startx=(chunkx<<4)+2;
    int startz=(chunkz<<4)+2;
    sh.startBox=new Box(startx,64,startz,startx+4,74,startz+4);
    //sh.startBox=new Box(Direction(0),1,2,3,startx,64,startz,startx+4,74,startz+4);
    
    Room *start=new Room(Direction(dirIdx),SpiralStairs,&rand,0,sh.startBox,false);
    sh.rooms[sh.roomPos]=start;
    sh.roomPos++;
    sh.forceNextPiece=FiveWay;
    sh.addExits(start, &rand);
    
    while(sh.pendingRoomsCount>0&&sh.portalRoom==nullptr)//&&sh.roomPos<100) //putting a 100 cap in for performance.
    {
        int selectPending=rand.nextInt(sh.pendingRoomsCount);
        Room *pendingRoom=sh.rooms[sh.pendingRoomsIdx[selectPending]];
        for(int i=selectPending;i<sh.pendingRoomsCount;i++)
        {
            sh.pendingRoomsIdx[i]=sh.pendingRoomsIdx[i+1];
        }
        sh.pendingRoomsCount--;
        sh.addExits(pendingRoom, &rand);
        
    }
    if(sh.portalRoom!=nullptr)
    {
        Box *prb=sh.portalRoom->roomBox;
        Direction prd=sh.portalRoom->direction;
        
        //Box *portalBox=new Box(prb->getX(prd, 4, 7),prb->getY(3),prb->getZ(prd, 4, 7),prb->getX(prd, 8, 12),prb->getY(3),prb->getZ(prd, 8, 12));
        int pminx=prb->getX(prd, 3, 12);
        int pminz=prb->getZ(prd, 3, 12);
        int pmaxx=prb->getX(prd, 7, 8);
        int pmaxz=prb->getZ(prd, 7, 8);
        int py=prb->getY(3);
        if(pminx>pmaxx)// i'm not sure if this check is actually required or not.
        {
            int tmp=pminx;
            pminx=pmaxx;
            pmaxx=tmp;
        }
        if(pminz>pmaxz)
        {
            int tmp=pminz;
            pminz=pmaxz;
            pmaxz=tmp;
        }
        
        Box *portalBox=new Box(pminx,py,pminz,pmaxx,py,pmaxz);
        //Box *portalBox=new Box(prb->getX(prd, 3, 12),prb->getY(3),prb->getZ(prd, 3, 12),prb->getX(prd, 7, 8),prb->getY(3),prb->getZ(prd, 7, 8));
        int minCX=portalBox->minX>>4;
        int maxCX=portalBox->maxX>>4;
        int minCZ=portalBox->minZ>>4;
        int maxCZ=portalBox->maxZ>>4;
        
        for(int cx=minCX;cx<=maxCX;cx++)
            for(int cz=minCZ;cz<=maxCZ;cz++)
            {
                int bx=cx*16;
                int bz=cz*16;
                Box *box=new Box(bx,0,bz,bx+15,128,bz+15);
                //std::string inChunkStr="false";
                //if(box->contains(portalBox->minX, portalBox->minZ)&&box->contains(portalBox->maxX, portalBox->maxZ)) inChunkStr="true";
                int count=0;
                for(int i=0;i<sh.roomPos;i++)
                {
                    
                    count+=sh.rooms[i]->getCount(box);
                    if(sh.rooms[i]->roomType==PortalRoom) break;
                    //count+=sh.rooms[i]->getCount(&box);
                    
                }
               
                if(count==1295||count==1339||count==1377||count==1513||count==2245||count==2500||count==2635)
                {
                    
                    
                    GenResult *result=new GenResult();
                    result->seed=seed;
                    result->cx=cx;
                    result->cz=cz;
                    result->count=count;
                    result->oneChunk=box->contains(portalBox->minX, portalBox->minZ)&&box->contains(portalBox->maxX, portalBox->maxZ);
                    if(!result->oneChunk) //has chunk overlap, so find out how much of the portal is in the chunk to filter out layouts that won't work.
                    {
                        int dx=0;
                        int dz=0;
                        if(portalBox->minX<box->maxX) dx=box->maxX-portalBox->minX;
                        else if(portalBox->minX>box->maxX) dx=portalBox->minX-box->maxX;
                        else dx=portalBox->maxX-portalBox->minX;
                        
                        if(portalBox->minZ<box->maxZ) dz=box->maxZ-portalBox->minZ;
                        else if(portalBox->minZ>box->maxZ) dz=portalBox->minZ-box->maxZ;
                        else dx=portalBox->maxZ-portalBox->minZ;
                        result->dx=dx;
                        result->dz=dz;
                        
                    }
                    delete portalBox;
                    delete box;
                    if(result->oneChunk||(result->dx>=2&&result->dz>=2))
                        return result;
                    else return nullptr;
                    //if(box->contains(portalBox->minX, portalBox->minZ)&&box->contains(portalBox->maxX, portalBox->maxZ))
                    //std::cout << std::to_string(seed) <<"\t" <<std::to_string(cx)  <<"\t"<<std::to_string(cz)<< "\t" <<std::to_string(count)<<" "<<inChunkStr<<"\n";
                }
                
                delete box;
            }
        delete portalBox;
        return nullptr;
        
        
        //4 3 8
               //7,3 12
    }
    else badStrongholdCount++;
    return nullptr;
    
    // BoundingBox var7 = BoundingBox.orientBox(var2, var3, var4, -4, -3, 0, 10, 9, 11, var5);
    //Room *fiveWay=sh.createRoom(FiveWay, rand, <#int x#>, <#int y#>, <#int z#>, <#Direction dir#>, <#int depth#>)
    //no roll for first 5 way
  
    
    
    
}
int cor=Corridor;
int pri=Prison;

Room::Room(Direction dir,RoomType rt, BedrockRandom *rand, int depth, Box *box, bool rollDoor)
{
    direction=dir;
    roomType=rt;
    roomBox=box;
    //std::cout<<std::to_string(rt)<<" ";
    //roomBox->outputToConsole();
    genDepth=depth;
    if(rollDoor&&rt!=PortalRoom) rand->_genRandInt32();
    switch (rt) {
        case FiveWay:
            fl=rand->nextBool();
            bl=rand->nextBool();
            fr=rand->nextBool();
            br=rand->nextInt(3)>0;
            break;
        case Library:
            isHigh=box->maxY-box->minY>5;
            break;
        case LargeRoom:
            isStoreRoom=rand->nextInt(5)==2;
            break;
        case Corridor:
            leftExit=rand->nextInt(2)==0;
            rightExit=rand->nextInt(2)==0;
            break;
        default:
            break;
            
    }
    
    
}

//Room::Room(Direction dir,RoomType rt, BedrockRandom *rand, int depth, Box *box)
//{
 //   Room(dir,rt,rand,depth,box,true);
//}

Box::Box(Direction dir, int x, int y, int z, int minX,int minY,int minZ,int maxX,int maxY,int maxZ)
{
    //int i=0;
    switch (dir) {
        case NORTH:
            this->minX=x+minX;
            this->minY=y+minY;
            this->minZ=z - maxZ + 1 + minZ;
            this->maxX=x + maxX - 1 + minX;
            this->maxY=y + maxY - 1 + minY;
            this->maxZ=z + minZ;
            
            //Box(x + minX, y + minY, z - maxZ + 1 + minZ, x + maxX - 1 + minX, y + maxY - 1 + minY, z + minZ);
            break;
        case SOUTH:
            this->minX=x+minX;
            this->minY=y+minY;
            this->minZ=z + minZ;
            this->maxX= x + maxX - 1 + minX;
            this->maxY=y + maxY - 1 + minY;
            this->maxZ=z + maxZ - 1 + minZ;
            //Box(x + minX, y + minY, z + minZ, x + maxX - 1 + minX, y + maxY - 1 + minY, z + maxZ - 1 + minZ);
            break;
        case WEST:
            this->minX=x - maxZ + 1 + minZ;
            this->minY=y+minY;
            this->minZ=z + minX;
            this->maxX=  x + minZ;
            this->maxY=y + maxY - 1 + minY;
            this->maxZ=z + maxX - 1 + minX;
           // Box(x - maxZ + 1 + minZ, y + minY, z + minX, x + minZ, y + maxY - 1 + minY, z + maxX - 1 + minX);
            break;
        case EAST:
            this->minX=x + minZ;
            this->minY=y + minY;
            this->minZ=z + minX;
            this->maxX=x + maxZ - 1 + minZ;
            this->maxY= y + maxY - 1 + minY;
            this->maxZ= z + maxX - 1 + minX;
            //Box(x + minZ, y + minY, z + minX, x + maxZ - 1 + minZ, y + maxY - 1 + minY, z + maxX - 1 + minX);
            break;
        default:
            Box(NORTH, 0,0,0,0,0,0,0,0,0);
            break;
    }
    
}

Box::Box(int minX,int minY,int minZ,int maxX,int maxY,int maxZ)
{
    this->minX=minX;
    this->minY=minY;
    this->minZ=minZ;
    this->maxX=maxX;
    this->maxY=maxY;
    this->maxZ=maxZ;
}
long createRoomCount=0;
Room* Stronghold::createRoom(RoomType rt, BedrockRandom *rand, int x, int y, int z, Direction dir, int depth)
{
    createRoomCount++;
    Box* box;
    switch (rt) {
        case ChestCorridor:
            box=new Box(dir,x,y,z,-1,-1,0,5,5,7);
            break;
        case FiveWay:
            box=new Box(dir,x,y,z,-4,-3,0,10,9,11);
            break;
        case LeftTurn:
        case RightTurn:
            box=new Box(dir,x,y,z,-1,-1,0,5,5,5);
            break;
        case Library:
            box=new Box(dir,x,y,z,-4,-1,0,14,11,15);
            if(hitsAnotherRoom(box))
            {
                delete box;
                box=new Box(dir,x,y,z,-4,-1,0,14,6,15);
            }
            break;
        case PortalRoom:
            box=new Box(dir,x,y,z,-4,-1,0,11,8,16);
            break;
        case Prison:
            box=new Box(dir,x,y,z,-1,-1,0,9,5,11);
            break;
        case LargeRoom:
            box=new Box(dir,x,y,z,-4,-1,0,11,7,11);
            break;
        case SpiralStairs:
            box=new Box(dir,x,y,z,-1,-7,0,5,11,5);
            break;
        case Corridor:
            box=new Box(dir,x,y,z,-1,-1,0,5,5,7);
            break;
        case StrightStairs:
            box=new Box(dir,x,y,z,-1,-7,0,5,11,8);
            break;
        default:
            box=new Box(NORTH, 0,0,0,0,0,0,0,0,0);
            break;
    }
    if(box->minY<=10||hitsAnotherRoom(box))
    {
        delete box;
        return nullptr;
    }
    //if(hitsAnotherRoom(box)) return nullptr;
    return new Room(dir,rt, rand, depth, box,true);
    
}

bool Stronghold::hitsAnotherRoom(Box* box)
{
    
    for(int i=0;i<roomPos;i++)
    {
        Room* room=rooms[i];
        if(room==nullptr) return false;
        if(room->roomBox->overlaps(box)) return true;
        
    }
    
    return false;
}

Room* Stronghold::selectRoom(BedrockRandom rand, int x, int y, int z, Direction dir, int depth) //testing passing by value here to do the clone
{
    if(this->forceNextPiece==FiveWay)
    {
        depth++;
        this->forceNextPiece=Corridor;
        Room *room=createRoom(FiveWay, &rand, x, y, z, dir, depth);
        this->addRoom(room);
        return room;
    }
    if(depth>50) return nullptr;
    
    int dx=x-this->startBox->minX;
    int dy=y-this->startBox->minY;
    if(dx>=112||dx<=-112||dy>=112||dy<=-112) return nullptr;
    
    depth++;
    
    //update weights?  na i'll do that on removal.
    int attempts=0;
    while(attempts<5)
    {
        attempts++;
        int skipped=0;
        int selection=rand.nextInt(this->totalWeight);
        //std::cout<<std::to_string(selection)<<"\n";
        for(int i=0;i<11;i++)
        {
            while(this->RoomWeights[i+skipped]==0&&i+skipped<11) skipped++;
            if(i+skipped>=11) break;
            selection-=this->RoomWeights[i+skipped];
            if(selection<0)
            {
                RoomType rt=RoomType(i+skipped);
                if (roomPos>2&&rt==lastRoom) break;
                if(this->minDepth[rt]>0 && depth<=minDepth[rt]) break;
                Room *room=createRoom(rt, &rand, x, y, z, dir, depth);
                if(room!=nullptr)
                {
                    this->placeCount[rt]++;
                    if(this->PlaceLimits[rt]>0&&this->placeCount[rt]>=this->PlaceLimits[rt])
                    {
                        //remove weighting
                        this->totalWeight-=this->RoomWeights[rt];
                        this->RoomWeights[rt]=0; //0 weights are skipped above so as if it doesn't exist in the array.
                    }
                    this->addRoom(room);
                    lastRoom=rt;
                    if(rt==PortalRoom) portalRoom=room;
                    return room;
                }
                
                    
            }
        }
    }
        
    Box *fillBox=this->fillBox(dir, x, y, z);
    if(fillBox!=nullptr && fillBox->minY>1)
    {
        Room *room=new Room(dir,Filler, &rand,depth, fillBox, false);
        this->addRoom(room);
        return room;
    }
    return nullptr;
}

Room* Stronghold::createRoomForward(Room* fromRoom, BedrockRandom *rand, int x, int y)
{
    Box *box=fromRoom->roomBox;
    switch (fromRoom->direction) {
        case NORTH:
            return selectRoom(*rand, box->minX+x, box->minY+y, box->minZ-1, fromRoom->direction, fromRoom->genDepth);
            break;
        case SOUTH:
            return selectRoom(*rand, box->minX+x, box->minY+y, box->maxZ+1, fromRoom->direction, fromRoom->genDepth);
            break;
        case WEST:
            return selectRoom(*rand, box->minX-1, box->minY+y, box->minZ+x, fromRoom->direction, fromRoom->genDepth);
            break;
        case EAST:
            return selectRoom(*rand, box->maxX+1, box->minY+y, box->minZ+x, fromRoom->direction, fromRoom->genDepth);
            break;
        default:
            break;
    }
    return nullptr;
    
}

Room* Stronghold::createRoomLeft(Room* fromRoom, BedrockRandom *rand, int x, int y)
{
    Box *box=fromRoom->roomBox;
    switch (fromRoom->direction) {
        case NORTH:
            return selectRoom(*rand, box->minX-1, box->minY+x, box->minZ+y, WEST, fromRoom->genDepth);
            break;
        case SOUTH:
            return selectRoom(*rand, box->minX-1, box->minY+x, box->minZ+y, WEST, fromRoom->genDepth);
            break;
        case WEST:
            return selectRoom(*rand, box->minX+y, box->minY+x, box->minZ-1, NORTH, fromRoom->genDepth);
            break;
        case EAST:
            return selectRoom(*rand, box->minX+y, box->minY+x, box->minZ-1, NORTH, fromRoom->genDepth);
            break;
        default:
            break;
    }
    return nullptr;
}

Room* Stronghold::createRoomRight(Room* fromRoom, BedrockRandom *rand, int x, int y)
{
    Box *box=fromRoom->roomBox;
    switch (fromRoom->direction) {
        case NORTH:
            return selectRoom(*rand, box->maxX+1, box->minY+x, box->minZ+y, EAST, fromRoom->genDepth);
            break;
        case SOUTH:
            return selectRoom(*rand, box->maxX+1, box->minY+x, box->minZ+y, EAST, fromRoom->genDepth);
            break;
        case WEST:
            return selectRoom(*rand, box->minX+y, box->minY+x, box->maxZ+1, SOUTH, fromRoom->genDepth);
            break;
        case EAST:
            return selectRoom(*rand, box->minX+y, box->minY+x, box->maxZ+1, SOUTH, fromRoom->genDepth);
            break;
        default:
            break;
    }
    return nullptr;
}




long overlapsCallCount=0;
bool Box::overlaps(Box* box2)
{
    overlapsCallCount++;
    bool output=!(maxX<box2->minX||minX>box2->maxX||maxY<box2->minY||minY>box2->maxY||maxZ<box2->minZ||minZ>box2->maxZ);
    
    return output;
    /*if(maxX<box2->minX) return false;
    if(minX>box2->maxX) return false;
    if(maxY<box2->minY) return false;
    if(minY>box2->maxY) return false;
    if(maxZ<box2->minZ) return false;
    if(minZ>box2->maxZ) return false;
    
    return true;*/
}
/*bool Box::overlaps2(Box box2)
{
    
    if(maxX<box2.minX) return false;
    if(minX>box2.maxX) return false;
    if(maxY<box2.minY) return false;
    if(minY>box2.maxY) return false;
    if(maxZ<box2.minZ) return false;
    if(minZ>box2.maxZ) return false;
    
    return true;
}*/

void Stronghold::addExits(Room *room,BedrockRandom *rand)
{
    Direction dir=room->direction;
    int lowY=3;
    int highY=5;
    switch (room->roomType) {
        case FiveWay:
            
            if(dir==NORTH||dir==WEST)
            {
                lowY=5;
                highY=3;
            }
            createRoomForward(room, rand, 5, 1);
            if(room->fl) createRoomLeft(room, rand, lowY, 1);
            if(room->bl) createRoomLeft(room, rand, highY, 7);
            if(room->fr) createRoomRight(room, rand, lowY, 1);
            if(room->br) createRoomRight(room, rand, highY, 7);
            break;
        case Library:
            return;
            break;
        case PortalRoom:
            return;
            break;
        case Filler:
            return;
            break;
        case SpiralStairs:
        case ChestCorridor:
        case Prison:
        case StrightStairs:
            createRoomForward(room, rand, 1, 1);
            break;
        case LeftTurn:
            if(dir==SOUTH||dir==WEST)
                createRoomRight(room, rand, 1, 1);
            else createRoomLeft(room, rand, 1, 1);
            break;
        case RightTurn:
            if(dir==SOUTH||dir==WEST)
                createRoomLeft(room, rand, 1, 1);
            else createRoomRight(room, rand, 1, 1);
            break;
        case LargeRoom:
            createRoomForward(room, rand, 4, 1);
            createRoomLeft(room, rand, 1, 4);
            createRoomRight(room, rand, 1, 4);
            break;
        case Corridor:
            createRoomForward(room, rand, 1, 1);
            if(room->leftExit)
                createRoomLeft(room, rand, 1, 2);
            if(room->rightExit)
                createRoomRight(room, rand, 1, 2);
            break;
    
        default:
            break;
    }
}

int Room::getCount(Box *box)
{
    if(this->roomBox->overlaps(box))
    {
        int count=0;
        switch (this->roomType) {
            case ChestCorridor:
                if(box->contains(roomBox->getX(direction, 3, 3),roomBox->getZ(direction, 3, 3)))
                    count++;
                return 130+count;
                break;
            case FiveWay:
                return 595;
                break;
            case LeftTurn:
            case RightTurn:
                return 98;
                break;
            case Library:
                //count=1156;
                
                //count=1191;
                if(box->contains(roomBox->getX(direction, 3, 5),roomBox->getZ(direction, 3, 5)))
                    count++;
                if(this->isHigh)
                {
                    if(box->contains(roomBox->getX(direction, 12, 1),roomBox->getZ(direction, 12, 1)))
                        count++;
                    //count+=234;
                    return count+1426;
                    //count+=269;
                }
                
                //tall should be 1426 without chests
                return count+1156;
                break;
            case PortalRoom:
                return 760;
                break;
            case LargeRoom:
                if(isStoreRoom && box->contains(roomBox->getX(direction, 3, 8),roomBox->getZ(direction, 3, 8))) return 443;
                return 442;
                break;
            case Corridor:
                return 134;
                break;
            case SpiralStairs:
                return 194;
                break;
            case StrightStairs:
                return 278;
                break;
            case Prison:
                return 318;
                break;
            default:
                break;
        }
    }
    return 0;
}


int Box::getX(Direction dir,int x, int z)
{
    if(dir==WEST) return maxX-z;
    if(dir==EAST) return minX+z;
    return minX+x;
}

int Box::getY(int y)
{
    return minY;
    
}
int Box::getZ(Direction dir,int x, int z)
{
    if(dir==NORTH) return maxZ-z;
    if(dir==SOUTH) return minZ+z;
    return minZ+x;
}

bool Box::contains(int x, int z)
{
    return x>=minX&&x<=maxX&&z>=minZ&&z<=maxZ;
}
/*
Box::Box()
{
    
}*/

void Box::outputToConsole()
{
    std::cout<<" minx="<<std::to_string(this->minX)<<" miny="<<std::to_string(this->minY)<<" minz="<<std::to_string(this->minZ)<<" maxx="<<std::to_string(this->maxX)<<" maxy="<<std::to_string(this->maxY)<<" maxz="<<std::to_string(this->maxZ)<<"\n";
}

void Stronghold::reset()
{
    RoomWeights[0]=40;
    RoomWeights[1]=5;
    RoomWeights[2]=20;
    RoomWeights[3]=20;
    RoomWeights[4]=10;
    RoomWeights[5]=5;
    RoomWeights[6]=5;
    RoomWeights[7]=5;
    RoomWeights[8]=5;
    RoomWeights[9]=10;
    RoomWeights[10]=10;
    
    for(int i=0;i<11;i++)
    {
        placeCount[i]=0;
    }

    for(int i=0;i<roomPos;i++)
    {
        delete rooms[i];
    }
    roomPos=0;
    totalWeight=135;
    pendingRoomsCount=0;
    forceNextPiece=Corridor;
    portalRoom=nullptr;
    
    
}
    Box* Stronghold::fillBox(Direction dir, int x, int y, int z)
    {
        Box *initBox=new Box(dir,x,y,z,-1,-1,0,5,5,4);
        int intersectPos=-1;
        for(int i=0;i<roomPos;i++)
        {
            if(rooms[i]->roomBox->overlaps(initBox))
            {
                intersectPos=i;
                break;
            }
        }
        if(intersectPos==-1)
        {
            delete initBox;
            return nullptr;
        }
        if(rooms[intersectPos]->roomBox->minY==initBox->minY)
        {
            for(int i=3;i>=1;i--)
            {
                delete initBox;
                initBox=new Box(dir,x,y,z,-1,-1,0,5,5,i-1);
                if(!rooms[intersectPos]->roomBox->overlaps(initBox))
                {
                    delete initBox;
                    return new Box(dir,x,y,z,-1,-1,0,5,5,i);
                }
            }
        }
        delete initBox;
        return nullptr;
    }

;
