#ifndef DECLARATION_H
#define DECLARATION_H


#include<bitset>
#include<vector>
using namespace std;


class declaration {

    declaration();
public:
  static int PC, reg[32];
  static vector<bitset<32> > iMemory;
  //static char dMemory[1024];
  static vector<bitset<8> > dMemory;
  static unsigned halt, cycle;
  static int writeToZero, numberOverflow, memoryOverflow, dataMisalign;

};



#endif // DECLARATION_H

