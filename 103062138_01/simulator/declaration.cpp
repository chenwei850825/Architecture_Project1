#include"declaration.h"

declaration::declaration()
{
    ;
}
int declaration::PC = 0;
int declaration::reg[32] = {0};
vector< bitset<8> > declaration::dMemory = vector< bitset<8> >(1024);
vector< bitset<32> > declaration::iMemory = vector< bitset<32> >(256);
//char declaration::dMemory[1024] = {0};
unsigned declaration::halt = 0;
unsigned declaration::cycle = 0;

int declaration::writeToZero = 0;
int declaration::numberOverflow = 0;
int declaration::memoryOverflow = 0;
int declaration::dataMisalign = 0;
