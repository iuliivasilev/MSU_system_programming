#include <iostream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <cstdio>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <vector>
using namespace std;

enum lex_type 
{ Lex_Null, Lex_Start, Lex_Idnt, Lex_If, Lex_Else, //0 - 4
  Lex_Up, Lex_Right, Lex_Down, Lex_Left, Lex_Setbomb,// 5 - 9
  Lex_Fin,Lex_Func, // 10-11
  Lex_Take, Lex_Do, Lex_While,Lex_Either,Lex_Or, // 12 - 16
  Lex_Int,Lex_Coord,Lex_OX,Lex_OY,Lex_Number, // 17 - 21
  Lex_Plus,Lex_Minus,Lex_Multp,Lex_Slash, // 22-25
  Lex_Great,Lex_GrEq, Lex_Equal, Lex_Assign,Lex_Low, Lex_LowEq, Lex_NotEq,// 26 - 32
  Lex_Period, Lex_Comma, Lex_Semicolon,// "." "," ";" 33 - 35
  Lex_Colon,Lex_Dash,Lex_FigOpen,Lex_FigCls,Lex_LBr,Lex_RBr,// ":" "-" "{" "}" "(" ")" 36 - 41
  Lex_And,Lex_Not,Lex_Bool,Lex_True,Lex_False, // 42 - 46
  Poliz_label,Poliz_address,Poliz_function,Poliz_go,Poliz_fgo,Lex_Return//47-52
};
class Lex {
    lex_type type;
    int value;
    public:
        Lex(lex_type N_type = Lex_Null, int N_value = 0): value(N_value), type(N_type){};
        int get_value() { return value; }
        lex_type get_type() { return type; }
        friend ostream & operator << (ostream &s,Lex l){
            s << '(' << l.value << ',' << l.type << ");";
            return s;
        }
};
///////////Class Poliz /////////////////////////////
class Poliz{
    Lex * p;
    int size;
    int free;
    public:
    Poliz (int max_size){
        p = new Lex [size = max_size];
        free = 0;
    }
    ~Poliz() { delete [] p; }
    void put_lex (Lex l){
        p [free] = l;
        free++;
    }
    void put_lex (Lex l, int place) { p [place] = l; }
    void blank() { free++; }
    int get_free () { return free; }
    Lex & operator[] (int index){
        if ( index > size )
          throw "POLIZ:out of array";
        else
            if (index > free )
               throw "POLIZ:indefinite element of array";
            else
            return p[index];}
    void print (){
        for ( int i = 0; i < free; ++i )
            cout << p[i];
    }
};
///////////Define stack/////////////////////////////
template <class T,int max_size>
class Stack
{
    T s [max_size];
    int top;
public:
    Stack() { top = 0; }
    void reset () { top = 0; }
    void push (T i);
    T pop ();
    bool is_empty() { return top == 0; }
    bool is_full() { return top == max_size; }
    int size();
};
template <class T, int max_size >
void Stack <T,max_size >::push(T i)
{
  if ( !is_full() )
    s [top++] = i;
  else
    throw "Stack_is_full";
}
template <class T, int max_size >
int Stack <T,max_size >::size()
{
  return top;
}
template <class T, int max_size>
T Stack <T, max_size>::pop ()
{
  if ( !is_empty() )
    return s[--top];
  else
    throw "stack_is_empty";
}
/////////////////////////////////////////////////
class Ident{
     char *name;
     bool declare;
     bool assign;
     lex_type type;
     int value;
   public:
     Ident( const char * New_name , int New_value=0):value(New_value){
          declare = false;
          assign = false;
          name = new char(strlen(New_name)+1);
          strcpy(name,New_name);
     };
     bool get_assign(){ return assign;};
     void put_assign(){ assign = true; return;};
     int get_value(){ return value;};
     void put_value(int val){ value = val; return;};
     bool get_declare(){ return declare;};
     void put_declare(){ declare = true; return;};
     void put_type(lex_type ntype){ type = ntype; return;};
     lex_type get_type(){return type;};
     ~Ident(){
         delete [] name;
     };
   friend class Table_Ident;
   friend class Parser;
   friend class Function;
};
class Table_Ident{
     friend class Parser;
     friend class Function;
     friend class Executer;
     vector<Ident *> Table;
     public:
     Table_Ident(){};
     int Put_New_Ident(const char * New_name){
         for(int i=0; i < Table.size();i++)
            if (!strcmp(Table.at(i)->name,New_name)) return i;
         Ident *id = new Ident(New_name);
         Table.push_back(id);
         return Table.size()-1;
     };
     ~Table_Ident(){
       for(int i=0; i < Table.size();i++)
          delete Table.at(i);
       Table.clear();
     };
};
Table_Ident TID;
/////////
class Function{
   friend class Parser;
   friend class Executer;
     char *name;
     lex_type type;
     bool declare;
     int quant_param;
     Table_Ident TIDF;
     Stack <int, 100> Stack_int;
     Stack <lex_type, 100> Stack_type;
   public:
     Poliz prog;
     bool get_declare(){
          return declare;
     };
     void put_param(int par){
          quant_param = par;
          return;
     };
     int get_param(){
          return quant_param;
     };
     Function( const char * New_name ,lex_type New_type = Lex_Null,bool dec=false):type(New_type),declare(dec),prog(1000){
          name = new char(strlen(New_name)+1);
          strcpy(name,New_name);
     };
     int put_func(Lex Curr_Lex){
          return TIDF.Put_New_Ident(TID.Table[Curr_Lex.get_value()]->name);
     };
     lex_type get_type(){return type;};
     ~Function(){
         delete [] name;
     };
   friend class Table_Function;
};
class Table_Function{
     friend class Parser;
     friend class Executer;
     vector<Function *> Table;
     public:
     Table_Function(){};
     int Put_New_Function(const char * New_name,lex_type New_type = Lex_Null,bool dec=false){
         for(int i=0; i < Table.size();i++)
            if (!strcmp(Table.at(i)->name,New_name)) return i;
         Function *func = new Function(New_name,New_type,dec);
         Table.push_back(func);
         return Table.size()-1;
     };
     int Find_Function(const char * New_name){
         for(int i=0; i < Table.size();i++)
            if (!strcmp(Table.at(i)->name,New_name)) return i;
         return -1;
     };
     ~Table_Function(){
       for(int i=0; i < Table.size();i++)
          delete Table.at(i);
       Table.clear();
     };
};
///////////////////
/*____________Table of identificatiors_______*/
const char * Table_SpecWd [50] = {
  "",
  //"Start",
  "if","else","Up","Right","Down","Left","Setbomb",
  "Take","do","while","either","or","int","coord","OX","OY",
  "and","not","bool","true","false","return", NULL
}; //clear view

lex_type Inter_Resp_Words [50] = {
  Lex_Null, 
  //Lex_Start,
  Lex_If, Lex_Else, 
  Lex_Up, Lex_Right, Lex_Down, Lex_Left, Lex_Setbomb,
  Lex_Take,Lex_Do,Lex_While,Lex_Either,
  Lex_Or,Lex_Int,Lex_Coord,Lex_OX,Lex_OY,
  Lex_And,Lex_Not,Lex_Bool,Lex_True,Lex_False,Lex_Return//
};
 /*___________Taблица разделителей___________*/
const char * Table_Delim [50] = {
  "",
  "+","-","*","/",">",">=","==","=",
  "<","<=","!=",".",",",";",
  ":","{","}","(",")",NULL
}; //clear view
lex_type Inter_Resp_Delim [50] = {
  Lex_Null, Lex_Plus,Lex_Minus,Lex_Multp,Lex_Slash, // actions
  Lex_Great,Lex_GrEq, Lex_Equal,Lex_Assign, Lex_Low, Lex_LowEq, Lex_NotEq, //comparison
  Lex_Period, Lex_Comma, Lex_Semicolon,// "." "," ";"
  Lex_Colon,Lex_FigOpen, Lex_FigCls,Lex_LBr,Lex_RBr// ":" "-" "{" "}" "(" ")"
};//internal representation
/*_________________End_______________________*/


class Scanner 
{
    enum state { Start, Idnt, Number, Comment, Gr_Eq_Li, Other, NotEqual };
    state CS;
    ifstream fp;
    char c, buf [ 80 ];
    int buf_top;
    /* FUNCTIONS */
    void clear (){
        buf_top = 0;
        for ( int j = 0; j < 80; j++ )
           buf[j] = '\0';
    }
    void add (){
        buf [ buf_top++ ] = c;
    }
    int find(const char *buf, const char **list ){
        int i = 0;
        while (list[i]){
        if ( !strcmp(buf, list[i]) )
            return i;
        ++i;
        }
        return 0;
    }
    void gc () {
       c = fp.get();
    }
    /* END of FUNC*/
public:
    Scanner ( const char * name_file ){
        fp.open ( name_file);
        CS = Start;
        clear();
        gc();
    }
    Lex get_lex ();
};

Lex Scanner::get_lex () {
  int d, j;
  CS = Start;
  for(;;){
    switch(CS)
    {
      case Start:
        if ( c ==' ' || c == '\n' || c == '\r' || c == '\t' ) 
          gc();
        else if ( ((c >= 'a')&&(c <='z')) || ((c >= 'A')&&(c <='Z')) ){
          clear();
          add();
          gc();
          CS = Idnt;
        }
        else if ( (c >= '0' )&&( c <= '9') )
        { 
          d = c - '0';
          gc();
          CS = Number;
        }
        else if ( c == '[' )
        {
          gc();
          CS = Comment;
        }
        else if ( c == '=' || c == '<' || c == '>' )
        {
          clear(); 
          add(); 
          gc(); 
          CS = Gr_Eq_Li; 
        }
        else if (!fp.good())
          return Lex(Lex_Fin);
        else if (c == '!')
        {
          clear();
          add();
          gc();
          CS = NotEqual;
        }
        else 
          CS = Other;
        break;
      case Idnt:
        if ( ((c >= 'a')&&(c<='z')) || ((c >= 'A')&&(c<='Z')) || ((c >= '0' )&&( c <= '9')) ) 
        {
          add(); 
          gc();
        }
        else 
        if ( j = find(buf, Table_SpecWd) )
          return Lex (Inter_Resp_Words[j], j);
        else {
          j = TID.Put_New_Ident(buf);
          return Lex (Lex_Idnt, j);
        }
        break;
      case Number:
        if ((c >= '0' )&&( c <= '9')) {
          d = d * 10 + (c - '0'); gc();
        } else return Lex ( Lex_Number, d);
        break;
      case Comment:
        if ( c == ']' )
        {
          gc();
          CS = Start;
        } else 
        if ( !fp.good() || c == '[' ) throw c;
        else  gc();
        break;
      case Gr_Eq_Li:
        if ( c == '=') {
          add();
          gc();
          j = find( buf, Table_Delim );
          return Lex ( Inter_Resp_Delim[j], j);
        } else {
          j = find ( buf, Table_Delim );
          return Lex ( Inter_Resp_Delim[j], j );
        }
        break;
      case NotEqual:
        if (c == '=') {
          add();
          gc();
          j = find( buf, Table_Delim );
          return Lex ( Lex_NotEq, j );
        } else throw '!';
        break;
      case Other:
        clear();
        add();
        if ( j = find ( buf, Table_Delim) ) {
          gc();
          return Lex ( Inter_Resp_Delim[j], j );
        } else throw c;
      break;
    }
  }
}
///////////////////////////////////////
class Executer{
    Lex pc_el;
    int fd_read;
    int fd_write;
    public:
    void execute(Table_Function & T);
    Executer(int fdr = 0,int fdwr = 1):fd_read(fdr),fd_write(fdwr){};
};
 
void Executer::execute ( Table_Function & Tab_Fun ){
    Stack < int, 100 > args;
    int i, j, index = 0,Curr_Func = Tab_Fun.Find_Function("Start"),flagSt = 0;
    if (Curr_Func == -1) throw "Not find main func Start";
    int  size = Tab_Fun.Table[Curr_Func]->prog.get_free();
    while ( index < size ){
      j = 0; i = 1;
      pc_el = Tab_Fun.Table[Curr_Func]->prog [ index ];
      switch ( pc_el.get_type () )
      {
        case Lex_True: 
        case Lex_False: 
        case Lex_Number: 
        case Poliz_address:
          flagSt = 1;
        case Poliz_label:
          args.push ( pc_el.get_value () );
          break;
        case Lex_Idnt:
          i = pc_el.get_value ();
          if ( Tab_Fun.Table[Curr_Func]->TIDF.Table[i]->get_assign () )
          {
            args.push ( Tab_Fun.Table[Curr_Func]->TIDF.Table[i]->get_value () );
            flagSt = 1;
            break;
          } else throw "POLIZ: indefinite identifier";
        case Lex_Not:
          args.push( !args.pop() );
          break;
        case Lex_Or:
          i = args.pop();
          args.push ( args.pop() || i );
          break;
        case Lex_And:
          i = args.pop();
          args.push ( args.pop() && i );
          break;
        case Poliz_go:
          flagSt = 0;
          index = args.pop()-1;
          break;
        case Poliz_fgo:
          flagSt = 0;
          i = args.pop();
          if ( !args.pop() ) index = i-1;
          break;
        case Lex_Setbomb: i++;
        case Lex_Up:j++;
        case Lex_Down:j++;
        case Lex_Right:j++;
        case Lex_Left:j++;
          sleep(1);
          write(fd_write, &i, sizeof(int));
          if (i == 1) write(fd_write, &j, sizeof(int));
          //cout << "change coord" << endl;
          break;
        case Lex_Take:
          {
            int k=5;
            i = args.pop ();
            //Interact with server
            if ( Tab_Fun.Table[Curr_Func]->TIDF.Table[i]->get_type() == Lex_Bool )
                k++;
            write(fd_write, &k, sizeof(int));
            read(fd_read, &k, sizeof(int));
            Tab_Fun.Table[Curr_Func]->TIDF.Table[i]->put_value (k);
            Tab_Fun.Table[Curr_Func]->TIDF.Table[i]->put_assign ();
            flagSt = 0;
            break;
          }
        case Lex_Plus:
          args.push ( args.pop() + args.pop() );
          break;
        case Lex_Multp:
          args.push ( args.pop() * args.pop() );
          break;
        case Lex_Minus:
          i = args.pop();
          args.push ( args.pop() - i );
          break;
        case Lex_Slash:
          i = args.pop();
          if (!i){
            args.push(args.pop() / i);
            break;
          } else throw "POLIZ:divide by zero";
        case Lex_Equal:
          args.push ( args.pop() == args.pop() );
          break;
        case Lex_Low:
          i = args.pop();
          args.push ( args.pop() < i);
          break;
        case Lex_Great:
          i = args.pop();
          args.push ( args.pop() > i );
          break;
        case Lex_LowEq:
          i = args.pop();
          args.push ( args.pop() <= i );
          break;
        case Lex_GrEq:
          i = args.pop();
          args.push ( args.pop() >= i );
          break;
        case Lex_NotEq:
          i = args.pop();
          args.push ( args.pop() != i );
          break;
        case Lex_Period://
          if (flagSt) args.pop();
          flagSt = 0;
          break;
        case Poliz_function:{
          i = pc_el.get_value();//Number Function
          int par = Tab_Fun.Table[i]->get_param(),val;
          //cout << par << "PARAM"<<endl;
          while (par != 0 ){
               val = args.pop();
               cout << val << " value param" <<endl;
               Tab_Fun.Table[i]->TIDF.Table[par-1]->put_value(val);
               Tab_Fun.Table[i]->TIDF.Table[par-1]->put_assign();
               par--; 
          }
          args.push(Curr_Func);
          args.push(index);
          Curr_Func = i;
          index = -1;
          break;}
        case Lex_Return:
          flagSt = 0;
          i = args.pop();//Take Value of Return
          if (args.size() <= 1) cout << i << endl;
          else {
              index = args.pop(); // read current position in high func
              Curr_Func = args.pop();
              flagSt = 1;
              args.push(i);
          }
          break;
        case Lex_Assign:
          flagSt = 0;
          i = args.pop();
          //cout << i << "value\n";
          j = args.pop();
          //cout << j << "var\n";
          Tab_Fun.Table[Curr_Func]->TIDF.Table[j]->put_value(i);
          Tab_Fun.Table[Curr_Func]->TIDF.Table[j]->put_assign(); 
          break;       
        default: throw "POLIZ: unexpected elem";
    }//end of switch
    cout << Curr_Func << "FUNC" << index << "Id " << args.size() << "Stack\n";
    index++;
  };//end of while
  cout << "Fin Execute" << endl;
}
