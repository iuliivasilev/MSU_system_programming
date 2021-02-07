#include <iostream>
#include <cstdio>
#include <cstring>
#include "polynomial.h"
using namespace std;
#define BUFFER_SIZE 128
void treatmentpol(Polynomial &P) //procesing of reading polynomial
{
    char buffer[BUFFER_SIZE];
    while(1){
        try{
            cout << "input polynom:\n";
            fgets(buffer,BUFFER_SIZE-1,stdin);
            Polynomial Z(buffer); //recognition string
            P = Z;
            return;            
        }catch(Except ex){ //processing exeption
             cout << ex.msg;
             switch(ex.type){
                case 1: cout << ":error in polynomial.cpp\n";
                        break;
                case 2: cout << ":error in main.cpp\n";
                        break;
             }
        }
    }
}
int
main()
{
    try{
    int a,b;
    char buffer[BUFFER_SIZE],secbuffer[BUFFER_SIZE];
    cout << "You can use follow command: add,mult,dec,integr,deff,defineIntegr,value\n";
    Polynomial P; // current value of polynom
    treatmentpol(P);//processing polynom on errors
    for(;;)
    {
        cout << "input command:\n";
        fgets(buffer,BUFFER_SIZE-1,stdin);
        if ((!strcmp(buffer,"add\n"))||(!strcmp(buffer,"mult\n"))
           ||(!strcmp(buffer,"dec\n"))){
            Polynomial Q;
            treatmentpol(Q);//processing polynom on errors
            if (!strcmp(buffer,"add\n")) { P = P+Q;P.printpol();}
            if (!strcmp(buffer,"mult\n")){ P = P*Q;P.printpol();}
            if (!strcmp(buffer,"dec\n")) { P = P-Q;P.printpol();}
        }
        if ((!strcmp(buffer,"integr\n"))||(!strcmp(buffer,"deff\n"))
           ||(!strcmp(buffer,"defineIntegr\n"))||(!strcmp(buffer,"value\n"))){
            if (!strcmp(buffer,"integr\n")) {P = P.Integr();P.printpol();}
            if (!strcmp(buffer,"deff\n")) {P = P.Def();P.printpol();}
            if (!strcmp(buffer,"defineIntegr\n")){
                cout << "Input side of integr(two value) in format: a < b\n";
                cin >> a >> b;
                if (a>b) throw Except(2,"Input error(integr)\n");//processing side of integral
                cout << P.DIntegr(a,b) << " < - value\n";
            }
            if (!strcmp(buffer,"value\n")){
                cout << "Input value x of polynomial: ";
                cin >> a;
                cout << P(a) << "< - value";
            }
        }
        if (!strcmp(buffer,"print\n")) P.printpol();
        if (!strcmp(buffer,"exit\n")) break; //ending work
    }
    }catch(const std::bad_alloc &e){   
            e.what();
            return 1;
    }catch(Except ex){
             cout << ex.msg;
             switch(ex.type){
                case 1: cout << ":error in polynomial.cpp\n";
                case 2: cout << ":error in main.cpp\n";
        }
    }
    return 0;
}
