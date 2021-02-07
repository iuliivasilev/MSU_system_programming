#include <iostream>
#include <sys/types.h> 
#include <sys/wait.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "space.h"
#define Map_Size 15
#define Buffer_Size 128
using namespace std;
void Environment::change_pos(int Numb,int x,int y){
    Map[Table_Player[Numb]->posX][Table_Player[Numb]->posY] = ' ';
    if ((Table_Player[Numb]->posX + x < Map_Size) 
       && (Table_Player[Numb]->posX + x >= 0)
       && (Table_Player[Numb]->posY + y < Map_Size) 
       && (Table_Player[Numb]->posY + y >= 0)
       && (Map[Table_Player[Numb]->posX+x][Table_Player[Numb]->posY+y] != 'G')){
         Table_Player[Numb]->posX += x;
         Table_Player[Numb]->posY += y;
      }
    return;
}
void Environment::set_bomb(int Numb){//command from player "set bomb"
    int Bomb_pid;
    if (Bomb_pid = fork()){
        Bomb * Buf_Bomb = new Bomb;
        Buf_Bomb->NumbPlayer = Numb;
        Buf_Bomb->posX = Table_Player[Numb]->posX;
        Buf_Bomb->posY = Table_Player[Numb]->posY;
        Buf_Bomb->pid = Bomb_pid;
        Buf_Bomb->next = Table_Bomb;
        Table_Bomb = Buf_Bomb;
    } else {
        sleep(6);
        exit(0);
    };
    return;
}
void Environment::Damage_Player(int x,int y){
    for(int numb = 0; numb < Envsize; numb++)
       if ((Table_Player[numb]->posX == x)&&(Table_Player[numb]->posY == y)){
           Table_Player[numb]->health-=40; cout << "Damage ->"<< numb<<endl;
           if (Table_Player[numb]->health <= 0){
               close(Table_Player[numb]->add_socket);
               delete_player(numb);
           }
       }
    return;
}
void Environment::Damage_Bomb(int x,int y,int numb){
    for(int i = -1; i < 2; i++)
        for(int j = -1;j < 2; j++){
           if ((x + i >= 0) && ( x + i <= Map_Size-1)
               &&(y + j >= 0) && ( y + j <= Map_Size-1)){
               if ((Map[x+i][y+j] != ' ')&&(numb < Envsize))//
                   Table_Player[numb]->score++;
               Damage_Player(x+i,y+j);
               Map[x+i][y+j] = 'H';
           }
        }
    return;
}
Bomb * Environment::Clean_Table(Bomb * Table)
{
    int rez,status;
    Bomb * Buf_Table;
    if (Table == NULL) return NULL;
    if ((rez = waitpid(Table->pid,&status,WNOHANG))>0)
    {
        cout << "Bomb with PID:" << Table->pid <<  " end!\n";
        Buf_Table = Table;
        Damage_Bomb(Buf_Table->posX,Buf_Table->posY,Buf_Table->NumbPlayer);
        Table = Clean_Table(Table->next);
        //animation
        delete Buf_Table;
    } else {
        Map[Table->posX][Table->posY] = 'B';
        Table->next = Clean_Table(Table->next);
    }
    return Table;
}
void Environment::delete_player(int numb){ //find error in player
    Map[Table_Player[numb]->posX][Table_Player[numb]->posY] = ' ';//
    cout << "Player: "<< Table_Player[numb]->name << " is gone\n";
    delete Table_Player[numb];
    Envsize--;
    if (Envsize == 0) return;
    Table_Player[numb] = Table_Player[Envsize];
    Table_Player[Envsize] = NULL;
    return;
}
void Environment::update_Map(){//sending map to player
    Table_Bomb = Clean_Table(Table_Bomb);
    for(int i = 0;i < Map_Size; i++)
        for(int j = 0;j < Map_Size; j++){
            if (Map[i][j] == '*') Map[i][j] = ' ';//
            if (Map[i][j] == 'H') Map[i][j] = '*';
        }
    for(int i=0;i < Envsize;i++)
      Map[Table_Player[i]->posX][Table_Player[i]->posY] = i+1 + '0';
}; 
//void Environment::send_Map(int sock_addr); //send map through socket to players
