#ifndef RESULT_H
#define RESULT_H

#include<iostream>
#include<iomanip>
#include<fstream>
#include"declaration.h"
using namespace std;
class result
{
public:
    result();
    void output(fstream &ostream);
    void errorDetect(fstream &ostream);
};

#endif
