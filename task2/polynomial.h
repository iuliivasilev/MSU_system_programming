#include <iostream>
#include <cstdio>
class Except:public std::exception{
    public:
        int type;//type of exception
        char msg[256];//message of error
        Except(int type,const char * msg);
    };
class Polynomial{
         struct Unit{
             int degree;
             double coeff;
             Unit * next;
             } *unit;
         Unit * add(double coef,int deg,Unit *start)
         {
             if (coef==0) return start; // we don't store cell with 0 coeff
             Unit *buf;
             if (start==NULL) { //hit in end of list
                 start = new struct Unit;
                 start->degree = deg;
                 start->coeff = coef;
                 start->next = NULL;
                 return start;
             }//in list we store sort cell from high degree to low
             if (start->degree > deg) {start->next = add(coef,deg,start->next);return start;};
             if (start->degree == deg) { // find cell with same degree
                  start->coeff+=coef;
                  if (start->coeff == 0){buf = start;start = start->next;delete buf;}
                  return start;
             }
             buf = start; //find degree < out degree = > add our sell firstly
             start = new struct Unit;
             start->degree = deg;
             start->coeff = coef;
             start->next = buf;
             return start;
         };
         double degr(int g,double c,double x) const //exponentiation
         {
             double x1=1.0;
             while(g>0) {x1*=x;g--;}
             return x1*c;
         }
    public:
         Polynomial();
         Polynomial(const char * s);
         Polynomial(Polynomial & p);
         void operator=(const Polynomial & p);
         Polynomial operator+(const Polynomial & p);
         Polynomial operator-(const Polynomial & p);
         Polynomial operator*(const Polynomial & p);
         double DIntegr(double a,double b);
         Polynomial Integr();
         Polynomial Def();
         double operator()(double x);
         void printpol();
         ~Polynomial();
    };
