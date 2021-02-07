#include <iostream>
#include <cstdio>
#include <cstring>
#define Map_Size 15
#define Buffer_Size 128
using namespace std;
struct Bomb{
    int posX,posY;
    int pid,NumbPlayer;//,force;
    Bomb *next;
};
class Player{
    friend class Environment;
    friend void send_Map(); //--based func for update_Map
    int posX,posY;
    int health,score;
    public:
    char * name;
    int add_socket;
    Player(int socket,char * name_str,int x=0,int y=0,int scr=0){
        posX = x;
        posY = y;
        name = new char[Buffer_Size];
        strcpy(name,name_str);
        health = 200;
        score = scr;
        add_socket = socket;
    }
    ~Player(){delete []name;};
};
class Environment{
    char Map[Map_Size][Map_Size];
    Bomb * Table_Bomb;
    void Damage_Bomb(int x,int y,int numb);
    void Damage_Player(int x,int y);
    public:
    int Envsize;
    Player * Table_Player[4];
    Environment(){
        this->Envsize = 0;
        for(int i=0;i<Map_Size;i++)
            for(int j=0;j<Map_Size;j++)
               Map[i][j] = 'G';
        Damage_Bomb(1,1,5);
        Damage_Bomb(1,Map_Size-2,5);
        Damage_Bomb(Map_Size-2,1,5);
        Damage_Bomb(Map_Size-2,Map_Size-2,5);
        Table_Bomb = NULL;
        for(int i = 0; i < 4; i++) Table_Player[i] = NULL;
    };
    ~Environment(){
       /* Clean Memory of Bombs*/
       Bomb * Buf_Tab = Table_Bomb;
       while (Buf_Tab!=NULL){
           Table_Bomb = Table_Bomb->next;
           delete Buf_Tab;
           Buf_Tab = Table_Bomb;
       }
       /* End CMoB*/
       for(int i=0;i<Envsize;i++)
       delete Table_Player[i];
    }
    void set_bomb(int);//command from player "set bomb"
    bool add_player(int numb_socket,char * name){
        int SetX = 0,SetY = 0;
        switch (Envsize){
            case 1: SetY = Map_Size-1; 
                break;
            case 3: SetY = Map_Size-1;
            case 2: SetX = Map_Size-1;
                break;
        }
        Table_Player[Envsize] = new Player(numb_socket,name,SetX,SetY);
        Envsize++;
        return (Envsize == 4);
    };
    Bomb * Clean_Table(Bomb * Table);
    void change_pos(int,int,int); //++
    void delete_player(int numb); //++find error in player
    void update_Map(); //++Paint on map players
    friend void send_Map(); //--based func for update_Map
};
