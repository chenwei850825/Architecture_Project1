#ifndef EXECUTION_H
#define EXECUTION_H

#include<iostream>
#include<bitset>
#include<fstream>
#include"declaration.h"
#include"constants.h"
using namespace std;

class execution
{


    bitset<32>instruction;
    bitset<6>opcode;
    bitset<5>rs;
    bitset<5>rt;
    bitset<5>rd;
    bitset<5>shamt;
    bitset<6>funct;
    bitset<16>immediate;
    bitset<26>address;
    int overflow;

    fstream test;


public:
    execution();
    void fetch( int PC );
    void decode_execute();
    void R_type();
    void I_type();
    void J_type();
    void terminal_type();



};

#endif // INSTRUCTION_H

