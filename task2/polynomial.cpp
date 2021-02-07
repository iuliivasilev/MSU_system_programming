#include <iostream>
#include <cstdio>
#include <cstring>
#include "polynomial.h"
using namespace std;
enum condition {waiting,cfread,xfind,dgfind,dgread};
#define BUFFER_SIZE 128
Except::Except(int type,const char * msg){
            this->type = type;
            strcpy(this->msg,msg);
        }
Polynomial::Polynomial(){unit = NULL;} //default constructor
Polynomial::Polynomial(const char * s) //polynomial string recognition
         {   
             unit = NULL;
             condition status = waiting;
             int dg=0,cf=0,sign=1;//1 - coef,2 - degree,0 - wait;
             for(int i=0;(s[i]!='\n')&&(s[i]!='\0');i++)
             {    switch(s[i]){
                     case '+':
                     case '-':
                         if (status == dgfind) {
                             Unit * buf = unit;
                             while(buf!=NULL){unit=unit->next;delete buf;buf = unit;};
                             throw Except(1,"don't read sign before degree\n");
                         }
                         if (status == xfind) dg = 1;
                         unit = add(cf*sign,dg,unit);//adding cell sign*coeff x ^ degree
                         cf = 0; dg = 0;
                         (s[i]=='+') ? sign = 1 : sign = -1; // set sign current member
                         status = waiting; // waiting for a new member
                         break;
                     case 'x': if((status==dgfind)||(status==xfind)||(status == dgread)){
                                   Unit * buf = unit;
                                   while(buf!=NULL){unit=unit->next;delete buf;buf = unit;};
                                   throw Except(1,"find x - uncorrected position\n");
                               }
                               if(status==waiting) cf = 1;
                               status = xfind;
                     case ' ':
                         break;
                     case '^':
                               if (status!=xfind){
                                   Unit * buf = unit;
                                   while(buf!=NULL){unit=unit->next;delete buf;buf = unit;};
                                   throw Except(1,"find ^ - uncorrected position\n");
                               }
                               status = dgfind;
                         break;
                     default:
                     if ((s[i]<'0') || (s[i]>'9')||(status == xfind)){
                         Unit * buf = unit;
                         while(buf!=NULL){unit=unit->next;delete buf;buf = unit;};
                         throw Except(1,"imput failed\n");//wrong character
                     }
                         switch(status){
                             case waiting: cf = s[i]-'0'; // first reading coeff 
                                     dg = 0; 
                                     status = cfread; 
                                     break;
                             case cfread: cf = cf*10+s[i]-'0'; // reading coeff
                                     break;
                             case dgfind: status = dgread;
                             case dgread: dg = dg*10+s[i]-'0'; // reading degree
                                     break;
                         }
                 }
             }
             if (status == dgfind){
                 Unit * buf = unit;
                 while(buf!=NULL){unit=unit->next;delete buf;buf = unit;};
                 throw Except(1,"don't read sign before degree\n");
             }
             if (status == xfind) dg = 1;
             if (status!=waiting) unit = add(cf*sign,dg,unit);
         };//construct basic+one parametr
         Polynomial::Polynomial(Polynomial & p)
         {
             Unit * q = unit;
             while(q!=NULL){unit=unit->next;delete q;q = unit;};
             unit=NULL;
             q = p.unit;
             while (q!=NULL)
             {
                 unit = add(q->coeff,q->degree,unit);
                 q = q->next;
             }
         };//construct copy*/
         void Polynomial::operator=(const Polynomial & p)
         {
             Unit * q = unit;
             while(q!=NULL){unit=unit->next;delete q;q = unit;};
             q = p.unit;
             while (q!=NULL)
             {
                 unit = add(q->coeff,q->degree,unit);//adding cell from p polynom to our polynom
                 q = q->next;
             }
         };
         Polynomial Polynomial::operator+(const Polynomial & p)
         {
             Polynomial Buf;
             Unit *one = this->unit, *two = p.unit;
             while (one!=NULL) {Buf.unit = add(one->coeff,one->degree,Buf.unit);one = one->next;};
             while (two!=NULL) {Buf.unit = add(two->coeff,two->degree,Buf.unit);two = two->next;};
             return Buf;
         };
         Polynomial Polynomial::operator-(const Polynomial & p)
         {
             Polynomial Buf;
             Unit *one = this->unit, *two = p.unit;
             while (one!=NULL) {Buf.unit = add(one->coeff,one->degree,Buf.unit);one = one->next;};
             while (two!=NULL) {Buf.unit = add((-1)*(two->coeff),two->degree,Buf.unit);two = two->next;};
             return Buf;
         };
         Polynomial Polynomial::operator*(const Polynomial & p)
         {
             Polynomial Buf;
             Unit *one = this->unit;
             while (one!=NULL)
             {
                 Unit * two = p.unit;
                 while (two!=NULL)
                 {
                     Buf.unit = add((two->coeff)*(one->coeff),(two->degree)+(one->degree),Buf.unit);
                     two = two->next;
                 }
                 one = one->next;
             }
             return Buf;
         };
         double Polynomial::DIntegr(double a,double b)
         {
             Polynomial Buf;
             Buf = (*this).Integr(); // find integr of our polynom
             //Buf.printpol();
             return Buf(b)-Buf(a);
         };//definite integral
         Polynomial Polynomial::Integr()
         {
             Polynomial Buf;
             Unit *one = this->unit;
             while (one!=NULL)
             {
                 Buf.unit = add((one->coeff)/(1+one->degree),1+one->degree,Buf.unit);
                 one = one->next;
             }
             return Buf;
         };
         Polynomial Polynomial::Def()
         {
             Polynomial Buf;
             Unit *one = this->unit;
             while (one!=NULL)
             {
                 Buf.unit = add((one->degree)*(one->coeff),(one->degree)-1,Buf.unit);
                 one = one->next;
             }
             return Buf;
         };
         double Polynomial::operator()(double x)
         {
             double s=0;
             struct Unit *point = unit;
             while (point != NULL)
             {
                 s+=degr(point->degree,point->coeff,x);//summed value of current cell
                 point = point->next; 
             }
             return s;
         }
         void Polynomial::printpol()
         {
             struct Unit * buf = unit;
             if (buf == NULL) cout << 0;
             while(buf!=NULL)
             {
                 if (buf->coeff > 0) cout << "+";
                 cout << buf->coeff; 
                 if (buf->degree) cout << "x^" << buf->degree;
                 buf = buf->next; 
             }
             cout << "\n";
             return;
         }
         Polynomial::~Polynomial(){
             struct Unit * buf = unit;
             while(buf!=NULL)
             {
                 unit=unit->next;
                 delete buf;
                 buf = unit;
             }//destructor
         }
