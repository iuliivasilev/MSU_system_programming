#include <iostream>
#include <cstdio>
#include <cstring>
#include "admin.h"
using namespace std;
#define BUFFER_SIZE 128
void admin::print()const{ //status of order + shop status
        developer::print();
        customer::print();
        cout <<"Your gift:" << gift << endl;
        return;
    };
void admin::Dell(){//clean all
        developer::Dell();
        customer::Dell();
        return;
    }; 
void admin::twin(){gift *=2;return;};
void admin::add(char * id,int quan){ // adding a gift for this order
        customer::add(id,quan+gift);
        cout << (gift ? "You use yourself gift\n": "You have not gift\n");
        gift = 0;
        return;
    };
void admin::use(){  // order registration
        if (basket==NULL) {cout << "No!Basket is empty!\n";return;}
        cout << "Order issued and you have +1 gift\n";
        gift++;
        customer::Dell();
        return;
    };
