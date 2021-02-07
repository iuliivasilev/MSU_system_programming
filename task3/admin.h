#include <iostream>
#include <cstdio>
#include <cstring>
#include "customer.h"
#include "developer.h"
using namespace std;
#define BUFFER_SIZE 128
#ifndef ADMININC
#define ADMININC
class admin: public developer,public customer{ // manager the ordering, control shop 
    public:
    int gift; //gift for customer for order
    admin():developer(),customer(){ gift = 0;};
    void print()const; //status of order + shop status
    void Dell();//clean all
    void twin();
    void add(char * id,int quan); // adding a gift for this order
    void use();  // order registration
};
#endif
