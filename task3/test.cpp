#include <iostream>
#include <cstdio>
#include <cstring>
#include "user.h"
#include "customer.h"
#include "developer.h"
#include "admin.h"
using namespace std;
#define BUFFER_SIZE 128
int treatmentinp() //processing error in cin
{
    int quan;
    while(1){
        cout << "input quantity\n";
        cin >> quan;
        if (cin.fail()) // if the previous extraction was failed
        {
            cin.clear(); // return cin to 'normal' mode
            cin.ignore(1000,'\n'); // delete the values of the previous input from the input buffer
        }
        else break;
    }
    cin.ignore(1000,'\n');
    return quan;
}
int main(){
 try{
    char buffer[BUFFER_SIZE],secbuffer[BUFFER_SIZE];
    admin A;
    developer * D = &A;
    customer *C = &A;
    int quan=0;
    for(;;){
        cout << "input status:\n";
        cin.getline(buffer,BUFFER_SIZE);
        if (!strcmp(buffer,"exit")) break;
        //interface for customer
        if (!strcmp(buffer,"customer")) 
            for(;;){
               cout << "<< customer >>\n input command:\n";
               cin.getline(secbuffer,BUFFER_SIZE);
               if (!strcmp(secbuffer,"exit")) break;
               if (!strcmp(secbuffer,"print")) C->customer::print();
               if (!strcmp(secbuffer,"buy"))
               {
                   cout << "input name\n";
                   cin.getline(secbuffer,BUFFER_SIZE);
                   cout << "input quantity\n";
                   quan = treatmentinp();
                   C->customer::buy(secbuffer,quan);
               }
               if (!strcmp(secbuffer,"delete")) C->customer::Dell();
               if (!strcmp(secbuffer,"twin")) C->customer::twin();
               if (!strcmp(secbuffer,"help")) 
                   cout << "You can use func: exit,print,delete,buy,twin,help\n";
        }
        //end interface for customer
        //interface for developer
        if (!strcmp(buffer,"developer")) 
            for(;;){
               cout << "<< developer >>\n input command:\n";
               cin.getline(secbuffer,BUFFER_SIZE);
               if (!strcmp(secbuffer,"exit")) break;
               if (!strcmp(secbuffer,"print")) D->developer::print();
               if (!strcmp(secbuffer,"add"))
               {
                   cout << "input name\n";
                   cin.getline(secbuffer,BUFFER_SIZE);
                   quan = treatmentinp();
                   D->developer::add(secbuffer,quan);
               }
               if (!strcmp(secbuffer,"delete")) D->developer::Dell();
               if (!strcmp(secbuffer,"twin")) D->developer::twin();
               if (!strcmp(secbuffer,"help")) 
                   cout << "You can use func: exit,print,add,delete,twin,help\n";
        }
        //end interface for developer
        //interface for admin
        if (!strcmp(buffer,"admin"))
            for(;;){
                cout << "<< admin >>\n input command:\n";
                cin.getline(secbuffer,BUFFER_SIZE);
                if (!strcmp(secbuffer,"exit")) break;
                if (!strcmp(secbuffer,"print")) A.print();
                if (!strcmp(secbuffer,"delete")) A.Dell();
                if (!strcmp(secbuffer,"twin")) A.twin();
                if (!strcmp(secbuffer,"use")) A.use();
                if (!strcmp(secbuffer,"add")){strcpy(secbuffer,"gift"); A.add(secbuffer,0);};
                if (!strcmp(secbuffer,"help")) 
                   cout << "You can use func: exit,print,delete,twin,add,use,help\n";
            }
         //end interface for admin
    }
    }catch(const std::bad_alloc &e){   
        e.what();
    }catch(...){cout << "!!unknown error!!\n";};
    cout << "END OF WORK!";
    return 0;
}
