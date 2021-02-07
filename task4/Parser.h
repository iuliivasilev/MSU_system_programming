#include <iostream>
#include <string.h>
#include <cstdio>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <vector>
#include <stack>
#include "Scanner.h"
using namespace std;
/* Macros checking Lexem */
#define CheckLex(Lex) if(Curr_type == Lex) glex(); else throw Curr_lex;
class Parser{
    Lex Curr_lex;
    lex_type Curr_type;
    int Curr_value,Curr_param,Curr_tpIdnt,Flag_Ret,Def_param;
    Scanner scan;
    /*Stack <int, 1000> Stack_int;
    Stack <lex_type, 1000> Stack_type;*/
    void glex(){
        Curr_lex = scan.get_lex();
        Curr_type = Curr_lex.get_type();
        Curr_value = Curr_lex.get_value();
        if ((Curr_type == Lex_Idnt)&&(Curr_Func != -1))
            Curr_value = TF.Table[Curr_Func]->put_func(Curr_lex);
    }
    void Start();
    void Mult_Oper_prog();
    void Oper_prog();
    void Value_Assign_prog();
    void Value_prog();
    void Summand();
    void Multipl();
    void Def_Check();
    void Check_id();
    void Check_id_take();
    void Check_op();
    void Check_Param(int);
    void Check_not();
    void Check_Bool();
    void Check_Coord();
    void Check_Function();
    void Check_declare(lex_type type);
    void Check_type();
    void Compare_Elem();
    void Check_Idnt();
    public:
    Table_Function TF;
    int Curr_Func,Flag_Assign,Check_Func;
    Parser(const char * Name_Prog) : scan(Name_Prog),TF(){}
    void analyze();
    void Define_Var();
};

/* Semantic parser */
void Parser::Def_Check(){
     if(TF.Table[Curr_Func]->TIDF.Table[Curr_value]->get_declare())
         throw "twice";
     else{
          TF.Table[Curr_Func]->TIDF.Table[Curr_value]->put_declare();
          TF.Table[Curr_Func]->TIDF.Table[Curr_value]->put_type(Curr_type);
     }
};
void Parser::Check_id(){
     Check_Func = 0;
     if(TF.Table[Curr_Func]->TIDF.Table[Curr_value]->get_declare()){
          if ((!Flag_Assign)&&(!TF.Table[Curr_Func]->TIDF.Table[Curr_value]->get_assign())) throw "Not Assign";
          TF.Table[Curr_Func]->Stack_type.push(TF.Table[Curr_Func]->TIDF.Table[Curr_value]->get_type());
          Curr_tpIdnt = 1;
          TF.Table[Curr_Func]->prog.put_lex(Lex(Lex_Idnt, Curr_value));
          //cout << Curr_Func << Curr_value <<TF.Table[Curr_Func]->TIDF.Table[Curr_value]->name << endl;
     } else { 
         Check_Func = TF.Put_New_Function(TF.Table[Curr_Func]->TIDF.Table[Curr_value]->name);
         if (! TF.Table[Check_Func]->get_declare()) throw "Not declared";
         else {
              TF.Table[Curr_Func]->TIDF.Table[Curr_value]->put_assign();
              Flag_Assign = 0;
              Curr_tpIdnt = 2;
              TF.Table[Curr_Func]->Stack_type.push(TF.Table[Check_Func]->get_type());
              //TF.Table[Curr_Func]->prog.put_lex(Lex(Poliz_function, Check_Func));
              //cout << Curr_Func << Check_F <<TF.Table[Check_Func]->name <<"func"<< endl;
         }
     }
};
void Parser::Check_op(){
     lex_type t1,t2,op,t;
     t2 = TF.Table[Curr_Func]->Stack_type.pop();
     op = TF.Table[Curr_Func]->Stack_type.pop();
     t1 = TF.Table[Curr_Func]->Stack_type.pop();
     if ((op == Lex_Plus)||(op == Lex_Minus)
        ||(op == Lex_Multp)||(op == Lex_Slash)) t = Lex_Int;
     if ((op == Lex_Or)||(op == Lex_And)) t = Lex_Bool;
     if (((op == Lex_Great)||(op == Lex_Equal)||(op == Lex_GrEq)||
         (op == Lex_Low)||(op == Lex_LowEq)||(op == Lex_NotEq))&&(t1 == t2))
         TF.Table[Curr_Func]->Stack_type.push(Lex_Bool);
     else if (t1 == t2 && t1 == t) TF.Table[Curr_Func]->Stack_type.push(t);
     else throw "Wrong types in operation";
     TF.Table[Curr_Func]->prog.put_lex(Lex(op));
     return;
};
void Parser::Check_not(){
     if (TF.Table[Curr_Func]->Stack_type.pop() != Lex_Bool) throw "Wrong type in operation NOT";
     TF.Table[Curr_Func]->Stack_type.push(Lex_Bool);
     TF.Table[Curr_Func]->prog.put_lex(Lex(Lex_Not));
     return;
};
void Parser::Check_type(){
     if ( TF.Table[Curr_Func]->Stack_type.pop() != TF.Table[Curr_Func]->Stack_type.pop() ) throw "uncorrect types";
};
void Parser::Check_Bool(){
     if ( TF.Table[Curr_Func]->Stack_type.pop() != Lex_Bool ) throw "Need to Lex_Bool";
};
void Parser::Check_Coord(){
     if ( TF.Table[Curr_Func]->Stack_type.pop() != Lex_Coord ) throw "Need to Lex_Coord";
     TF.Table[Curr_Func]->Stack_type.push(Lex_Int);
};
void Parser::Check_id_take(){
     if(!TF.Table[Curr_Func]->TIDF.Table[Curr_value]->get_declare()) throw "Not declared variable";
     return;
};
void Parser::Check_Param(int Func_value){
     int Curr_F = (TF.Put_New_Function(TF.Table[Curr_Func]->TIDF.Table[Func_value]->name)),
         param = TF.Table[Curr_F]->get_param();
     while (param--){
         TF.Table[Curr_Func]->Stack_type.push(TF.Table[Curr_F]->TIDF.Table[param]->get_type());
         Check_type();
     };
};
void Parser::Check_declare(lex_type type){
     int i;
     while (!TF.Table[Curr_Func]->Stack_int.is_empty()){
         i = TF.Table[Curr_Func]->Stack_int.pop();
         if(TF.Table[Curr_Func]->TIDF.Table[i]->get_declare())
              throw "twice";
         else {
              TF.Table[Curr_Func]->TIDF.Table[i]->put_declare();
              TF.Table[Curr_Func]->TIDF.Table[i]->put_type(type);
              Curr_param++;
              if (Def_param) TF.Table[Curr_Func]->TIDF.Table[i]->put_assign();
         } 
     };
};
/* END Semantic parser*/
void Parser::Check_Idnt(){
    int Func_Val = Curr_value;
    if (Curr_type == Lex_Idnt){
        Check_id();//
        glex();
        if (Curr_type == Lex_LBr){
            if (Curr_tpIdnt != 2) throw "Ident is not function";
            TF.Table[Curr_Func]->Stack_type.push(Lex_Null);
            Flag_Assign = 0;
            glex();
            if ((Curr_type == Lex_Idnt)||(Curr_type == Lex_Number)||(Curr_type == Lex_LBr)){
                 Value_prog();
                 while (Curr_type == Lex_Comma){
                      glex();
                      Value_prog();
                 }
            };
            CheckLex(Lex_RBr);
            Check_Param(Func_Val);
            if (TF.Table[Curr_Func]->Stack_type.pop() != Lex_Null) 
                throw "Uncorrected quantity of paramert";
            TF.Table[Curr_Func]->prog.put_lex(Lex(Poliz_function, TF.Put_New_Function(TF.Table[Curr_Func]->TIDF.Table[Func_Val]->name)));
        } else {
          if (Curr_type == Lex_Colon){
            if (Curr_tpIdnt != 1) throw "function is not ident";
            glex();
            if ((Curr_type == Lex_OX)||(Curr_type == Lex_OY)){
                Check_Coord();
                TF.Table[Curr_Func]->prog.put_lex (Lex (Curr_type));
                glex();
            } else throw "do not find OX or OY";
          }
        };
    };
};
void Parser::Value_Assign_prog(){
    Flag_Assign = 1;
    int Value_Assign = Curr_value,Type_Assign = Curr_type,posid =TF.Table[Curr_Func]->prog.get_free();
    Value_prog();
    if (Curr_type == Lex_Assign){
        if (Flag_Assign){
            TF.Table[Curr_Func]->prog.put_lex(Lex (Poliz_address,Value_Assign), posid);   
            glex();
            Value_prog();
            Check_type();
            TF.Table[Curr_Func]->TIDF.Table[Value_Assign]->put_assign();
            TF.Table[Curr_Func]->prog.put_lex (Lex (Lex_Assign) );
        } else throw "Uncorrect Assign";
    } else {
        if ((Type_Assign == Lex_Idnt)&&(!TF.Table[Curr_Func]->TIDF.Table[Value_Assign]->get_assign())) {
            cout <<Value_Assign;
            throw "Not Assign";
        }
    }
};
void Parser::Value_prog(){
    Compare_Elem();
    if ((Curr_type == Lex_Great)||(Curr_type == Lex_GrEq)||(Curr_type == Lex_Equal)
       ||(Curr_type == Lex_Low)||(Curr_type == Lex_LowEq)||(Curr_type == Lex_NotEq)){
        Flag_Assign = 0;
        TF.Table[Curr_Func]->Stack_type.push(Curr_type);//
        glex();
        Compare_Elem();
        Check_op();
    };
};
void Parser::Compare_Elem(){
    Summand();
    while ((Curr_type == Lex_Plus)||(Curr_type == Lex_Minus)||(Curr_type == Lex_Or)){
        Flag_Assign = 0;
        TF.Table[Curr_Func]->Stack_type.push(Curr_type);//
        glex();
        Summand();
        Check_op();
    };
};
void Parser::Summand(){
    Multipl();
    while ((Curr_type == Lex_Multp)||(Curr_type == Lex_Slash)||(Curr_type == Lex_And)){
        Flag_Assign = 0;
        TF.Table[Curr_Func]->Stack_type.push(Curr_type);//
        glex();
        Multipl();
        Check_op();//
    };
};
void Parser::Multipl(){
    if (Curr_type == Lex_Idnt){
        Check_Idnt();
    } else {
    Flag_Assign = 0; 
    if (Curr_type == Lex_Number){
        TF.Table[Curr_Func]->Stack_type.push(Lex_Int);
        TF.Table[Curr_Func]->prog.put_lex ( Curr_lex );
        glex();
    } else if ((Curr_type == Lex_True)||(Curr_type == Lex_False)){
        TF.Table[Curr_Func]->Stack_type.push(Lex_Bool);
        TF.Table[Curr_Func]->prog.put_lex(Lex(Curr_type, Curr_type == Lex_True));//
        glex();
    } else if (Curr_type == Lex_Not){
        glex();
        Multipl();
        Check_not();//
    } else 
       if (Curr_type == Lex_LBr){
        glex();
        Value_prog();
        CheckLex(Lex_RBr);
       } else throw Curr_lex;
    };
};
void Parser::Define_Var(){
    TF.Table[Curr_Func]->Stack_int.reset();
    lex_type Cu_ty = Curr_type;
    if ((Curr_type == Lex_Int)||(Curr_type == Lex_Coord)||(Curr_type == Lex_Bool)){
        glex();
        while (1){
            int Value_Assign = Curr_value;
            if (Curr_type != Lex_Idnt) throw "!"; //+
            else {
                TF.Table[Curr_Func]->Stack_int.push(Curr_value);
                TF.Table[Curr_Func]->prog.put_lex(Lex(Poliz_address,Curr_value));
                glex();
            }
            if (Curr_type == Lex_Assign){
                TF.Table[Curr_Func]->Stack_type.push(Cu_ty);//
                glex();
                Value_prog();
                Check_type();//
                TF.Table[Curr_Func]->TIDF.Table[Value_Assign]->put_assign();
                TF.Table[Curr_Func]->prog.put_lex (Lex (Lex_Assign) );
            }
            if (Curr_type != Lex_Comma) break;
            glex();
        };
        CheckLex(Lex_Period);
        Check_declare(Cu_ty);
    }
};
void Parser::Oper_prog(){ // <operator> -> 
    int pl0, pl1, pl2, pl3;//
    if (Curr_type == Lex_If) { // if "(" <value> ")" <operator> [else <operator>]
        glex();
        CheckLex(Lex_LBr);
        Value_prog();
        Check_Bool();
        CheckLex(Lex_RBr);

        pl2 = TF.Table[Curr_Func]->prog.get_free ();//!
        TF.Table[Curr_Func]->prog.blank();//!
        TF.Table[Curr_Func]->prog.put_lex (Lex(Poliz_fgo));//!

        Oper_prog();

        pl3 = TF.Table[Curr_Func]->prog.get_free();//!
        TF.Table[Curr_Func]->prog.blank();//!
        TF.Table[Curr_Func]->prog.put_lex (Lex(Poliz_go));//!
        TF.Table[Curr_Func]->prog.put_lex (Lex(Poliz_label, TF.Table[Curr_Func]->prog.get_free()), pl2);//!

        if (Curr_type == Lex_Else){ 
            glex(); 
            Oper_prog();
            TF.Table[Curr_Func]->prog.put_lex(Lex(Poliz_label, TF.Table[Curr_Func]->prog.get_free()), pl3);//!
        } else TF.Table[Curr_Func]->prog.put_lex(Lex(Poliz_label, TF.Table[Curr_Func]->prog.get_free()), pl3);//!
    } else if (Curr_type == Lex_Either) { // Either <operator>{ or <operator>}
        int Secret,Posit,Curr;
        Stack <int,30> St_Iter,St_Beg;
        glex();
        St_Iter.push(TF.Table[Curr_Func]->prog.get_free ());
        TF.Table[Curr_Func]->prog.blank();
        TF.Table[Curr_Func]->prog.put_lex (Lex(Poliz_go));   
        St_Beg.push(TF.Table[Curr_Func]->prog.get_free ());///
        Oper_prog();
        St_Iter.push(TF.Table[Curr_Func]->prog.get_free());
        TF.Table[Curr_Func]->prog.blank();
        TF.Table[Curr_Func]->prog.put_lex (Lex(Poliz_go));

        while (Curr_type == Lex_Or){
            glex();
            St_Beg.push(TF.Table[Curr_Func]->prog.get_free ());
            Oper_prog();
            St_Iter.push(TF.Table[Curr_Func]->prog.get_free());
            TF.Table[Curr_Func]->prog.blank();
            TF.Table[Curr_Func]->prog.put_lex (Lex(Poliz_go));
        };
        Secret = rand() % St_Beg.size();
        Posit = TF.Table[Curr_Func]->prog.get_free();
        while (St_Iter.size()!= 1)
            TF.Table[Curr_Func]->prog.put_lex(Lex(Poliz_label, TF.Table[Curr_Func]->prog.get_free()),St_Iter.pop());//!
        while (St_Beg.size()!=0){
            Curr = St_Beg.pop();
            if (Secret == St_Beg.size()) Posit = Curr;
        };
        TF.Table[Curr_Func]->prog.put_lex(Lex(Poliz_label, Posit),St_Iter.pop());//!
    } else if (Curr_type == Lex_While) { //while "(" <value> ")" do <operator> "." 
        pl0=TF.Table[Curr_Func]->prog.get_free();//!
        glex();
        CheckLex(Lex_LBr);
        Value_prog();
        Check_Bool();
        CheckLex(Lex_RBr);

        pl1=TF.Table[Curr_Func]->prog.get_free();//!
        TF.Table[Curr_Func]->prog.blank();//!
        TF.Table[Curr_Func]->prog.put_lex(Lex(Poliz_fgo));//!

        CheckLex(Lex_Do);
        Oper_prog();
 
        TF.Table[Curr_Func]->prog.put_lex (Lex (Poliz_label, pl0));//!
        TF.Table[Curr_Func]->prog.put_lex (Lex ( Poliz_go));//!
        TF.Table[Curr_Func]->prog.put_lex (Lex ( Poliz_label, TF.Table[Curr_Func]->prog.get_free()), pl1);//!
    } else if (Curr_type == Lex_Take) { //Take "(" <ident> ")".
        glex();
        CheckLex(Lex_LBr);
        if (Curr_type == Lex_Idnt) { 
             //Check_id();
             TF.Table[Curr_Func]->prog.put_lex(Lex ( Poliz_address, Curr_value) );
             glex();
        } else throw "Need ident";//
        CheckLex(Lex_RBr);
        TF.Table[Curr_Func]->prog.put_lex (Lex (Lex_Take));//!
        CheckLex(Lex_Period);
    } else if ((Curr_type == Lex_Up)||(Curr_type == Lex_Down)//up.down.right.left.
          ||(Curr_type == Lex_Right)||(Curr_type == Lex_Left)||(Curr_type == Lex_Setbomb)){
        lex_type CT = Curr_type;
        glex();
        TF.Table[Curr_Func]->prog.put_lex (Lex (CT));//!
        CheckLex(Lex_Period);
    } else if (Curr_type == Lex_Return){
        glex();
        TF.Table[Curr_Func]->Stack_type.push(TF.Table[Curr_Func]->get_type());
        Value_prog();
        Check_type();
        TF.Table[Curr_Func]->prog.put_lex (Lex (Lex_Return));
        Flag_Ret = 1;
        CheckLex(Lex_Period);
    } else if (Curr_type == Lex_FigOpen){
         glex();
         Mult_Oper_prog();
         CheckLex(Lex_FigCls);
    } else if ((Curr_type == Lex_Int)||(Curr_type == Lex_Coord)
              ||(Curr_type == Lex_Bool)) 
        Define_Var();
    else if ((Curr_type == Lex_Idnt)||(Curr_type == Lex_Number)||(Curr_type == Lex_LBr)){
        Value_Assign_prog();
        CheckLex(Lex_Period);
    };
};
void Parser::Mult_Oper_prog(){ //предложения разделяются точками,действия запятыми.
     // <operators> -> { <operator> }
    while ((Curr_type == Lex_If)||(Curr_type == Lex_FigOpen)||(Curr_type == Lex_Up)||(Curr_type == Lex_Down)
          ||(Curr_type == Lex_Right)||(Curr_type == Lex_Left)||(Curr_type == Lex_Setbomb)||(Curr_type == Lex_Take)||(Curr_type == Lex_While)
          ||(Curr_type == Lex_Either)||(Curr_type == Lex_Idnt)||(Curr_type == Lex_Int)||(Curr_type == Lex_Coord)
          ||(Curr_type == Lex_Bool)||(Curr_type == Lex_Number)||(Curr_type == Lex_LBr)||(Curr_type == Lex_Return)){
        Oper_prog();
        TF.Table[Curr_Func]->prog.put_lex(Lex(Lex_Period));
    };
};
void Parser::Start(){ // <program> -> { NameFunc "{" <operators> "}" } Start "{" <operators> "}"
    int flag = 0;
    lex_type Cu_ty;
    while ((Curr_type == Lex_Int)||(Curr_type == Lex_Coord)||(Curr_type == Lex_Bool)){
        Curr_Func = -1;
        Def_param = 1;
        Flag_Ret = 0;
        Curr_param = 0;
        Cu_ty = Curr_type;
        glex();
        if (Curr_type == Lex_Idnt) Curr_Func = TF.Put_New_Function(TID.Table[Curr_value]->name,Cu_ty,true);
            else throw Curr_lex;
        //cout << TID.Table[Curr_value]->name << endl;
        glex();
        CheckLex(Lex_LBr);
        while((Curr_type == Lex_Int)||(Curr_type == Lex_Coord)||(Curr_type == Lex_Bool))
           { Define_Var(); TF.Table[Curr_Func]->prog.put_lex(Lex(Lex_Period));}
        Def_param = 0;
        CheckLex(Lex_RBr);
        TF.Table[Curr_Func]->put_param(Curr_param);//Setting parametrs
        CheckLex(Lex_FigOpen);
        Mult_Oper_prog();
        CheckLex(Lex_FigCls);
        if (!Flag_Ret) throw "Don't find RETURN!";
        TF.Table[Curr_Func]->prog.print();
        cout << endl;
    };
    CheckLex(Lex_Fin);
};
void Parser::analyze(){
    glex();
    Start();
    cout << "Yes!Program is correct" << endl;
};/*
int main(int argc,char * argv[]){
try{
    srand(time(NULL));
    Parser P(argv[1]);
    P.analyze();
    Executer Ex;
    Ex.execute(P.TF);
    return 0;
}catch(Lex Curr) { cout << Curr << endl;
}catch(const char * str){ cout << str << endl;
}catch(char ch){ cout << "uncorrected symbol:"<< ch<< endl;};
};*/
