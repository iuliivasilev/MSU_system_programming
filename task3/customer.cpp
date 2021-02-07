#include <iostream>
#include <cstdio>
#include <cstring>
#include "customer.h"
using namespace std;
#define BUFFER_SIZE 128
void customer::Dell(){            //Clean basket, but returning product don't realize
        Item *buf = basket;
        while(buf!=NULL)
        {
            basket = basket->next;
            delete buf;
            buf = basket;
        }
        cout << "The basket is empty\n";
        return;
    };
void customer::add(char * id,int quan) // function for adding product to basket
    {
        Item *buf1 = new Item, * buf2 = basket;
        while ((buf2!=NULL)&&(strcmp(id,buf2->name))) buf2 = buf2->next;
        if (buf2!=NULL) {buf2->quantity += quan; return;}
        strcpy(buf1->name,id);
        buf1->quantity = quan;
        buf1->next = basket;
        basket = buf1;  
        return;
    }
void customer::print()const{      //print current basket status
       Item *buf = basket;
       cout << "BASKET\n";
       if (buf==NULL) cout << "The basket is empty\n";
       else cout <<"\t\tname"<<"\t\t"<<"quantity\n";
       while (buf!=NULL){
           cout <<buf-> name <<"\t\t\t"<< buf->quantity<< endl;
           buf=buf->next;
       }
       return;
    };
void customer::buy(char * id,int quan){ //finding product in shop,checking availability,adding to basket
        Item *buf = item;
        while ((buf!=NULL)&&(strcmp(id,buf->name))) buf = buf->next;
        if (buf==NULL) {cout << "Item do not find\n";return;};
        if (buf->quantity - quan > 0){
            cout << "Buy " << buf->name <<endl; 
            buf->quantity -= quan; 
            customer::add(buf->name,quan); //when we go to customer from admin we use another func -> ::
            return;
        };
        customer::add(buf->name,buf->quantity);
        cout << "Buy "<< buf->name <<":"<< buf->quantity<< endl;
        buf->quantity = 0;
        item = clear(item);
        return;
    };
void customer::twin(){         //twined product in basket
        Item *buf = basket;
        while (buf!=NULL) {buy(buf->name,buf->quantity);buf = buf->next;}
        cout <<"Success\n";
        return;
    };
