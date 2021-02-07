#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include "Parser.h"
#define BUFFER_SIZE 128  //Constant size of send/recv message
#define Map_Size 15
using namespace std;

int sockfd = 0,myNumb=0;

struct Int_With_Player_Read{
    int St_posX,St_posY;
};
struct Int_With_Player_Write{
    char Map[Map_Size][Map_Size];
    int ScoreAll[4];
    int HealthAll[4];
};
int Where_I(struct Int_With_Player_Write *IWPW){
    int m = 0;
    for(int j = 0; j < Map_Size; j++){
        for (int i = 0; i < Map_Size;i++)
            if (IWPW->Map[i][j] == (myNumb + '0')){
                if (i == 0) m = 1;
                else if (IWPW->Map[i-1][j] == 'G') m = 1;
                if (i == Map_Size -1) m = m*10 + 2;
                else if (IWPW->Map[i+1][j] == 'G') m = m*10 + 2;
                if (j == 0) m = m*10 + 3;
                else if (IWPW->Map[i][j-1] == 'G') m = m*10 + 3;
                if (j == Map_Size -1) m = m*10 + 4;
                else if (IWPW->Map[i][j+1] == 'G') m = m*10 + 4;
                //cout << m << "Take value" << endl;
                return m;
            }
    }
};
int Where_Bomb(struct Int_With_Player_Write *IWPW){
    int m = 0;
    for(int j = 0; j < Map_Size; j++){
        for (int i = 0; i < Map_Size;i++)
            if (IWPW->Map[i][j] == (myNumb + '0')){
            for(int p = -1; p < 2; p++)
              for(int t = -1;t < 2; t++)
                 if ((i + p >= 0) && ( i + p <= Map_Size-1)
                   &&(j + t >= 0) && ( j + t <= Map_Size-1)){
                    if (IWPW->Map[i][j] == 'B') return 1;
                 }
                return 0;
            }
    }
};
void error(const char *errorMessage);
void handler(int sig){//Signal handling
    close(sockfd);
    exit(0);
}
int treatmentinp(){ //processing error in cin
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
void Print_Map(struct Int_With_Player_Write *IWPW){
     for(int i = 0; i < 4; i++)
         cout<<"Pl"<< i << ",score:" << IWPW->ScoreAll[i]<<'\t';
     cout << endl;
     for(int i = 0; i < 4; i++)
         cout<<"Pl"<< i << ",lives:" << IWPW->HealthAll[i]<<'\t';
     cout << endl;
     /* End print table of knowledge */
     for(int j = 0; j < Map_Size; j++){
         for (int i = 0; i < Map_Size;i++)
             cout << IWPW->Map[i][j] << ' ';
     cout <<endl;
     };
}
int main(int argc,char ** argv){
  signal(SIGTERM,handler);
  signal(SIGINT,handler);
  signal(SIGTSTP,handler);
  struct sockaddr_in server;
  int size_struct_sockaddr_in = sizeof(struct sockaddr_in),port;
  char buffer[BUFFER_SIZE] = "Hello server.",name[BUFFER_SIZE]="",c;
  /*--------------Input and processing port------------*/
  port = treatmentinp();
  /*--------------end processing port--------------*/
  /*--------------name handler-------------*/
  while (!strcmp(name,"")){
      cout << "Please, input your name: ";
      cin.getline(name,BUFFER_SIZE-1);   //Save name of client for recv to serve
      cin.clear(); // return cin to 'normal' mode
      cin.ignore(1000,'\n');
      for(int i=0,j=0;(i<strlen(name))&&(j<BUFFER_SIZE);i++)
          if (name[i]!=' ') name[j++]=name[i];
      name[strlen(name)]='\0';
      cout << name << endl;
  }
  /*--------------end name handler-------------*/
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
    error("socket()");
  memset(&server, 0, size_struct_sockaddr_in);
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  if (connect(sockfd, (struct sockaddr *)&server, size_struct_sockaddr_in) == -1)
    error("connect()");
  write(sockfd,name,BUFFER_SIZE-1);
  read(sockfd,&myNumb,sizeof(int));
  struct Int_With_Player_Write *IWPW = new struct Int_With_Player_Write;
  struct Int_With_Player_Read *IWPR = new struct Int_With_Player_Read;
  int * request = new(int);
  read(sockfd,IWPW,sizeof(struct Int_With_Player_Write));
  Print_Map(IWPW);
  if (argc == 1)
  for(;;)
  {
      fd_set readfds;
      int max_d = sockfd;
      FD_ZERO(&readfds);
      FD_SET(sockfd,&readfds);
      FD_SET(0,&readfds);
      int res = select(max_d+1,&readfds,NULL,NULL,NULL);
      if(FD_ISSET(sockfd,&readfds))
      {
          cout << endl;
          if (read(sockfd,IWPW,sizeof(struct Int_With_Player_Write))){
              Print_Map(IWPW);
          }
          else break;
      }
      if(FD_ISSET(0,&readfds)){
          * request = 0;
          cin.getline(buffer,BUFFER_SIZE); 
          if ((!strcmp(buffer,"left"))||(!strcmp(buffer,"right"))
             ||(!strcmp(buffer,"down"))||(!strcmp(buffer,"up")))
              * request = 1;
          if (!strcmp(buffer,"setbomb")) * request = 2;
          if (!strcmp(buffer,"/exit")) break;
          if (!strcmp(buffer,"/help")) {printf("Commands:/help , /exit, /info\n");continue;}
          if (* request) write(sockfd,request,sizeof(int));//define write error
          else cout << "Uncorrected input!\n";
          if (*request == 1){
              IWPR->St_posX = 0; IWPR->St_posY = 0;
              if (!strcmp(buffer,"left")) IWPR->St_posX = -1;
              if (!strcmp(buffer,"right")) IWPR->St_posX = 1;
              if (!strcmp(buffer,"down")) IWPR->St_posY = 1;
              if (!strcmp(buffer,"up")) IWPR->St_posY = -1;
              write(sockfd,IWPR,sizeof(struct Int_With_Player_Read));
          }
      }
  }else {
     int fd_lang[2],fd_client[2];
     if (pipe(fd_lang) == -1) throw "Error in pipe 1";
     if (pipe(fd_client) == -1) throw "Error in pipe 2";
     if (!fork()){ // language
         try{
             close(fd_lang[0]);
             close(fd_client[1]);
             srand(time(NULL));
             Parser P(argv[1]);
             P.analyze();
             Executer Ex(fd_client[0],fd_lang[1]);
             Ex.execute(P.TF);
             close(fd_lang[1]);
             close(fd_client[0]);
             return 0;
         }catch(Lex Curr) { cout << Curr << endl;
         }catch(const char * str){ cout << str << endl;
         }catch(char ch){ cout << "uncorrected symbol:"<< ch<< endl;};    
     } else {
     close(fd_lang[1]);
     close(fd_client[0]);        
     for(;;) {
         fd_set readfds;
         int max_d = sockfd;
         FD_ZERO(&readfds);
         FD_SET(sockfd,&readfds);
         FD_SET(fd_lang[0],&readfds);
         if (fd_lang[0] > max_d) max_d = fd_lang[0];
         int res = select(max_d+1,&readfds,NULL,NULL,NULL);
         if(FD_ISSET(sockfd,&readfds))
         {
             cout << endl;
             if (read(sockfd,IWPW,sizeof(struct Int_With_Player_Write))){
                 Print_Map(IWPW);
             }
             else break;
         }
         if(FD_ISSET(fd_lang[0],&readfds)){
             * request = 0;
             read(fd_lang[0], request, sizeof(int));
             if (*request >= 5) { 
                 if (*request == 5) *request = Where_I(IWPW);
                 else if (*request == 6) *request = Where_Bomb(IWPW);
                 write(fd_client[1], request, sizeof(int));
             } else {
             //cout << *request<<endl;
             write(sockfd,request,sizeof(int));//define write error
             if (*request == 1){
                 read(fd_lang[0], request, sizeof(int));
                 IWPR->St_posX = 0; IWPR->St_posY = 0;
                 if (*request == 1) IWPR->St_posX = -1;
                 if (*request == 2) IWPR->St_posX = 1;
                 if (*request == 3) IWPR->St_posY = 1;
                 if (*request == 4) IWPR->St_posY = -1;
                 write(sockfd,IWPR,sizeof(struct Int_With_Player_Read));
                }
             }
         }
       }
       close(fd_lang[0]);
       close(fd_client[1]);
       }
  }
  delete IWPW;
  delete IWPR;
  delete request;
  cout << "\n__________Dialog is over__________\n";
  close(sockfd);
  return 0;
}
 
void error(const char *errorMessage)
{
  printf("%s\n", errorMessage);
  handler(SIGINT);
  exit(EXIT_FAILURE);
}         
