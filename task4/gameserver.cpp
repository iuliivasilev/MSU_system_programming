#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <signal.h>
#include "space.h"
#define BUFFER_SIZE 128
#define Map_Size 15
using namespace std;
int sockfd = 0, new_sockfd = 0,opt=1;
Environment Env;
/* Interact with player */
struct Int_With_Player_Read{
    int St_posX,St_posY;
};
struct Int_With_Player_Write{
    char Map[Map_Size][Map_Size];
    int ScoreAll[4];
    int HealthAll[4];
};
/* End */
void Func_Interact_User(int SockNum,int Numb){
    struct Int_With_Player_Read *IWPR = new struct Int_With_Player_Read;
    int * request = new int;
    * request = 0;
    if (!read(SockNum,request,sizeof(int))){
         close(SockNum);
         Env.delete_player(Numb);//add func
         return;
    }
    switch (*request){
        case 0 : //catch error
          //throw;
          break;
        case 1 :
          if (!read(SockNum,IWPR,sizeof(struct Int_With_Player_Read))){
              close(SockNum);
              Env.delete_player(Numb);//add func
              return;
          }
          Env.change_pos(Numb,IWPR->St_posX,IWPR->St_posY);
          break;
        case 2 :
          Env.set_bomb(Numb);
          break;
        /* We can adding here anything*/
    }
    delete IWPR;
    delete request;
    return;
}
void send_Map(){
    struct Int_With_Player_Write * IWPW = new struct Int_With_Player_Write;
    for (int i = 0;i < Map_Size; i++)
          for (int j = 0; j < Map_Size ; j++)
              IWPW->Map[i][j] = Env.Map[i][j];
    for (int i = 0;i < 4; i++){
        IWPW->ScoreAll[i] = (i < Env.Envsize) ? Env.Table_Player[i]->score : 0;
        IWPW->HealthAll[i] = (i < Env.Envsize) ? Env.Table_Player[i]->health : 0;
    }
    for (int i = 0;i < Env.Envsize; i++)
      write(Env.Table_Player[i]->add_socket,IWPW,sizeof(struct Int_With_Player_Write));
    delete IWPW;
    return;
}
int treatmentinp() //processing error in cin
{
    int quan;
    while(1){
        cout << "Please, input your port: ";
        cin >> quan;
        if (cin.fail()) // if the previous extraction was failed
        {
            cout << "Port does not correct!Please,repeat input!\n";
            cin.clear(); // return cin to 'normal' mode
            cin.ignore(1000,'\n'); 
            // delete the values of the previous input from the input buffer
        }
        else break;
    }
    cin.ignore(1000,'\n');
    return quan;
}
void error(const char *errorMessage);
void handler(int sig)
{    
    for (int i=0;i < Env.Envsize; i++)
      close(Env.Table_Player[i]->add_socket);
    close(sockfd);
    exit(0);
}
int main(int argc, char ** argv)
{
  signal(SIGTERM,handler);
  signal(SIGINT,handler);
  signal(SIGTSTP,handler);
  struct sockaddr_in server, client;
  socklen_t size_struct_sockaddr_in = sizeof(struct sockaddr_in);
  char buffer[BUFFER_SIZE],name[BUFFER_SIZE],Mes[2*BUFFER_SIZE];
  /*--------------Input and processing port------------*/
  int port = treatmentinp();
  /*--------------end processing port--------------*/
  sockfd = socket(AF_INET, SOCK_STREAM, 0); 
  if (sockfd == -1)
    error("socket()");
  setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)); // remove the freeze servers
  memset(&server, 0, size_struct_sockaddr_in);
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = INADDR_ANY;
  if (bind(sockfd, (struct sockaddr *)&server, size_struct_sockaddr_in) == -1)
    error("bind()");
  if (listen(sockfd, 5) == -1)
    error("listen()");
  /*--------------registration users---------------*/
  for(;;)
  {   
      int buf_int;
      fd_set readfds;
      FD_SET(0,&readfds);
      FD_SET(sockfd,&readfds);
      int res = select(sockfd+1,&readfds,NULL,NULL,NULL);
      if(FD_ISSET(sockfd,&readfds)){
          new_sockfd = accept(sockfd, (struct sockaddr *)&client,&size_struct_sockaddr_in);
          memset(&name,0,BUFFER_SIZE);
          if (read(new_sockfd,name,BUFFER_SIZE-1))
          {
              cout << name << " joined\n";
              buf_int = Env.Envsize+1;
              write(new_sockfd,&buf_int,sizeof(int)); // send the number of player
              if (Env.add_player(new_sockfd,name)) break;
          }else  close(new_sockfd);
      }
      if(FD_ISSET(0,&readfds)){
          cin.getline(buffer,BUFFER_SIZE);
          if (!strcmp(buffer,"/start")) break;
      }
  };
  cout << "Quantity player:" << Env.Envsize << endl;
  /*--------------end------------------------------*/
  Env.update_Map(); //Paint map before writing
  send_Map();
  for(;;)
  {
      if (Env.Envsize == 0) break;
      if (Env.Envsize == 1) {cout << "Player " << Env.Table_Player[0]->name << " win\n";break;}
      fd_set readfds;
      int max_d = sockfd;
      FD_ZERO(&readfds);
      FD_SET(0,&readfds);
      FD_SET(sockfd,&readfds);
      for (int i = 0 ;i < Env.Envsize; i++)
      {
          FD_SET(Env.Table_Player[i]->add_socket,&readfds);
          if (Env.Table_Player[i]->add_socket > max_d) max_d = Env.Table_Player[i]->add_socket;
      }
      int res = select(max_d+1,&readfds,NULL,NULL,NULL);
      if (FD_ISSET(sockfd,&readfds)){ //ignored new connected
          new_sockfd = accept(sockfd, (struct sockaddr *)&client,&size_struct_sockaddr_in);
          close(new_sockfd);
      }
      for (int i = 0;i < Env.Envsize; i++)
          if(FD_ISSET(Env.Table_Player[i]->add_socket,&readfds)){
              Func_Interact_User(Env.Table_Player[i]->add_socket,i);
          }
      if(FD_ISSET(0,&readfds)){
          memset(&buffer, 0, BUFFER_SIZE);
          cin.getline(buffer,BUFFER_SIZE);//input
          if (!strcmp(buffer,"/exit")) break;
          if (!strcmp(buffer,"/help")) cout << "Commands:/help , /exit, /chshow \n";
      };
      Env.update_Map(); //Paint map before writing
      send_Map();
  }
  cout << "__________The game is overed__________________\n";
  for (int i = 0;i < Env.Envsize; i++)
      close(Env.Table_Player[i]->add_socket);
  close(sockfd);
  return 0;
}
 
void error(const char *errorMessage)
{
  cout << errorMessage << endl;
  handler(SIGINT);
  exit(EXIT_FAILURE);
}
