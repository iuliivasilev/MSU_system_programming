#include <iostream>
#include <cstdio>
#include <cstring>
#include "user.h"
using namespace std;
#define BUFFER_SIZE 128
#ifndef CUSTINC
#define CUSTINC
class customer: public virtual user{ // Usual user,which planing to buy product by shop
    public:
    void add(char * id,int quan);// function for adding product to basket
    customer():user(){};
    void Dell();//Clean basket, but returning product don't realize
    void print()const;     //print current basket status
    void buy(char * id,int quan); //finding product in shop,checking availability,adding to basket
    void twin();//twined product in basket
    virtual ~customer(){ //destructor
        Dell();
    };
};
#endif

