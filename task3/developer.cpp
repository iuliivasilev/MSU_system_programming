#include <iostream>
#include <cstdio>
#include <cstring>
#include "developer.h"
using namespace std;
#define BUFFER_SIZE 128
void developer::print()const{       //print status shop
       Item *buf = item;
       cout << "SHOP\n";
       if (buf==NULL) cout << "The shop is empty\n";
       else cout <<"\t\tname"<<"\t\t"<<"quantity\n";
       while (buf!=NULL){
           cout <<buf-> name <<"\t\t\t"<< buf->quantity<< endl;
           buf=buf->next;
       }
       return;
    };
void developer::add(char * id,int quan) //adding product to shop
    {
        Item *buf = new Item, * buf2 = item;
        while ((buf2!=NULL)&&(strcmp(id,buf2->name))) buf2 = buf2->next;
        if (buf2!=NULL) {buf2->quantity += quan; return;}
        strcpy(buf->name,id);
        buf->quantity = quan;
        buf->next = item;
        item = buf;  
        return;
    }
void developer::twin(){ // as well as for customer, only for shop
        Item *buf = item;
        while(buf!=NULL) {buf->quantity *=2; buf = buf->next;}
        cout <<"Success\n";
        return;
    };
void developer::Dell(){ //-||-
        Item *buf = item;
        while(buf!=NULL)
        {
            item = item->next;
            delete buf; 
            buf = item;
        }
        cout << "The shop is empty\n";
        return;
    };
