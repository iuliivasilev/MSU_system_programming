#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#define BUFFER_SIZE 128
 
int sockfd = 0, new_sockfd = 0,opt=1;
struct cell {
    int add_socket;
    char *name;
    struct cell * next;
};
struct cell* MainTable=NULL,*point;
struct cell*
Add_Table(int sock,char *name,struct cell* Table)
{
    if (Table==NULL)
    {
        Table = malloc(sizeof(struct cell));
        Table->add_socket = sock;
        Table->name = malloc(sizeof(char)*BUFFER_SIZE);
        strcpy(Table->name,name);
        //printf("Opened %d %s\n",Table->add_socket,Table->name);
        Table->next = NULL;
        return Table;
    }
    Table->next = Add_Table(sock,name,Table->next);
    return Table;
}
struct cell*
Rem_Table(int sock,struct cell* Table)
{
    if (Table == NULL) return NULL;
    if (Table->add_socket == sock)
    {
        struct cell * buf;
        buf = Table;
        Table = Table->next;
        printf("%s - left\n",buf->name);
        free(buf->name);
        free(buf); 
        return Table;
    } 
    Table->next = Rem_Table(sock,Table->next);
    return Table;
}
void
funcinfo(int sock,struct cell* Table)
{
    struct cell* buf = Table;
    char Mes[2*BUFFER_SIZE]="Current persons on server:\n";
    for(;buf!=NULL;buf = buf->next) {strcat(Mes,buf->name);strcat(Mes,"\n");}
    write(sock,Mes,2*BUFFER_SIZE-1);
    return;
}
int
testname(char * name,struct cell* Table)
{
    struct cell* buf = Table;
    for(;buf!=NULL;buf = buf->next)
        if (!strcmp(buf->name,name)) return 0;
    return 1;
}
char*
findname(struct cell* Table,int sock)
{
    struct cell * buf=Table;
    for(;buf != NULL; buf = buf->next)
        if (buf->add_socket == sock) return buf->name;
    return "nothing";
}
void error(const char *errorMessage);
void 
printMes(char *buf)
{
        struct cell *Point;
        Point = MainTable;
        while (Point!=NULL)
        {
           write(Point->add_socket, buf, 2*BUFFER_SIZE - 1);
           Point = Point->next;
        }
}
void 
handler(int sig)
{
    for (point = MainTable;point!=NULL;point = point->next)
      close(point->add_socket);
    close(sockfd);
    exit(0);
}
int main()
{
  signal(SIGTERM,handler);
  signal(SIGINT,handler);
  signal(SIGTSTP,handler);
  struct sockaddr_in server, client;
  time_t rawtime; //send time of message
  struct tm * timeinfo; // struct for time
  int size_struct_sockaddr_in = sizeof(struct sockaddr_in),port,status=0;
  char buffer[BUFFER_SIZE],name[BUFFER_SIZE],Mes[2*BUFFER_SIZE];
  /*--------------Input and processing port------------*/
  printf("Please, input your port: ");
  fgets(buffer,BUFFER_SIZE-1,stdin);
  for(int i=0;buffer[i]!='\n';i++) if ((buffer[i]<='0')||(buffer[i]>='9'))error("Port does not correct");
  port = atoi(buffer);
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
  if (listen(sockfd, 4) == -1)
    error("listen()");
  for(;;)
  {
      fd_set readfds;
      int max_d = sockfd;
      FD_ZERO(&readfds);
      FD_SET(0,&readfds);
      FD_SET(sockfd,&readfds);
      for (point = MainTable;point!=NULL;point = point->next)
      {
          FD_SET(point->add_socket,&readfds);
          if (point->add_socket>max_d) max_d = point->add_socket;
      }
      int res = select(max_d+1,&readfds,NULL,NULL,NULL);
      if(FD_ISSET(sockfd,&readfds)){
          new_sockfd = accept(sockfd, (struct sockaddr *)&client,&size_struct_sockaddr_in);
          memset(&name,0,BUFFER_SIZE);
          if (read(new_sockfd,name,BUFFER_SIZE-1)&&testname(name,MainTable))
          {
              printf("%s joined\n",name);
              MainTable = Add_Table(new_sockfd,name,MainTable);
          }else  close(new_sockfd);
      }
      for (point = MainTable;point!=NULL;point = point->next)
          if(FD_ISSET(point->add_socket,&readfds)){
              memset(&buffer, 0, BUFFER_SIZE);
              memset(&Mes,0,2*BUFFER_SIZE);
              if (read(point->add_socket,buffer,BUFFER_SIZE-1))
              {
                   if (!strcmp(buffer,"/info\n")) funcinfo(point->add_socket,MainTable);
                   else {
                       time ( &rawtime );//current date in seconds
                       timeinfo = localtime ( &rawtime ); 
                       strftime (Mes,BUFFER_SIZE,"%X___<<",timeinfo);
                       strcat(Mes,findname(MainTable,point->add_socket));
                       strcat(Mes,">>::");
                       strcat(Mes,buffer);
                       if (status) printf("Message:%s",Mes);
                       printMes(Mes);
                   }
              }
              else 
              {
                  close(point->add_socket);
                  MainTable = Rem_Table(point->add_socket,MainTable);
              }
          }
      if(FD_ISSET(0,&readfds)){
          memset(&buffer, 0, BUFFER_SIZE);
          fgets(buffer, BUFFER_SIZE - 1, stdin);
          if (!strcmp(buffer,"/exit\n")) break;
          if (!strcmp(buffer,"/chshow\n")) status ? status=0 : status++;
          if (!strcmp(buffer,"/help\n")) printf("Commands:/help , /exit, /chshow \n");
      };
  }
  for (point = MainTable;point!=NULL;point = point->next)
      close(point->add_socket);
  close(sockfd);
  return 0;
}
 
void error(const char *errorMessage)
{
  printf("%s\n", errorMessage);
  handler(SIGINT);
  exit(EXIT_FAILURE);
}
