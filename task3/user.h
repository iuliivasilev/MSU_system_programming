#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;
#define BUFFER_SIZE 128
#ifndef USERINC
#define USERINC
class user{
    public:
    struct Item{
        char name[BUFFER_SIZE]; //name of product
        int quantity;  
        Item * next;
    } * item,* basket; // item - store for shop,basket - store for customer.
    char secbuffer[BUFFER_SIZE];
    int quan;
    Item * clear(Item * it){ // delete the product with 0 - quantity
        if (it==NULL) return it;
        if (it->quantity == 0){
            Item *buf = it;
            it = clear(it->next);
            delete buf;
            return it;
        }
        it->next = clear(it->next);
        return it;
    }
    user(){ item = NULL;basket = NULL ;};//construct define
    virtual ~user(){                             //destructor: delete basket+shop
        Item * buf = item;
        while (buf!=NULL)
        {
            item=item->next;
            delete buf;
            buf = item;
        }
        buf = basket;
        while (buf!=NULL)
        {
            item=item->next;
            delete buf;
            buf = item;
        }
    };
    virtual void print()const=0; //clean virtual functions -> class user - abstract
    virtual void Dell()=0; 
    virtual void twin()=0;
    virtual void add(char * id,int quan)=0;
};
#endif
