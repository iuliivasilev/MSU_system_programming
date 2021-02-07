#include <iostream>
#include <cstdio>
#include <cstring>
#include "user.h"
using namespace std;
#define BUFFER_SIZE 128
#ifndef DEVELINC
#define DEVELINC
class developer: public virtual user{ //support for the shop
    public:
    void print()const;       //print status shop
    developer():user(){;}; //construct default
    void add(char * id,int quan); //adding product to shop
    void twin(); // as well as for customer, only for shop
    void Dell(); //-||-
    virtual ~developer(){
        Dell();
    };
};
#endif
