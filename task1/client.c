#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/time.h>
#include <signal.h>

#define BUFFER_SIZE 128  //Constant size of send/recv message
int sockfd = 0;
void error(const char *errorMessage);
void                    //Signal handling
handler(int sig)
{
    close(sockfd);
    exit(0);
}
void clean_stdin(void)
{
int c;
do c = getchar(); 
while (c != '\n' && c != EOF);
}
int main()
{
  signal(SIGTERM,handler);
  signal(SIGINT,handler);
  signal(SIGTSTP,handler);
  struct sockaddr_in server;
  int size_struct_sockaddr_in = sizeof(struct sockaddr_in),port;
  char buffer[BUFFER_SIZE] = "Hello server.",name[BUFFER_SIZE]="",Mes[2*BUFFER_SIZE],c;
  /*--------------Input and processing port------------*/
  printf("Please, input your port: ");
  fgets(buffer,BUFFER_SIZE-1,stdin);
  for(int i=0;buffer[i]!='\n';i++) if ((buffer[i]<='0')||(buffer[i]>='9'))error("Port does not correct");
  port = atoi(buffer);
  /*--------------end processing port--------------*/
  /*--------------name handler-------------*/
  while (!strcmp(name,""))
  {
      printf("Please, input your name: ");
      fgets(Mes,2*BUFFER_SIZE-1,stdin);      //Save name of client for recv to server
      if (strlen(Mes)>BUFFER_SIZE-1) 
          {printf("Name too long.It will be shorting.Input enter\n");
           clean_stdin();}
      for(int i=0,j=0;(i<strlen(Mes))&&(j<BUFFER_SIZE);i++)
          if (Mes[i]!=' ') name[j++]=Mes[i];
      name[strlen(name)-1]='\0';
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
          memset(&Mes, 0, 2*BUFFER_SIZE);
          if (read(sockfd,Mes,2*BUFFER_SIZE-1))
              printf("%s",Mes);
          else break;
      }
      if(FD_ISSET(0,&readfds)){
          memset(&buffer, 0, BUFFER_SIZE);
          memset(&Mes,0,2*BUFFER_SIZE);
          fgets(Mes, 2*BUFFER_SIZE - 1, stdin);
          if (strlen(Mes)>BUFFER_SIZE-2) {printf("Too long message...\n");clean_stdin();continue;}
          if (!strcmp(Mes,"/exit\n")) break;
          if (!strcmp(Mes,"/help\n")) {printf("Commands:/help , /exit, /info\n");continue;}
          if (strcmp(Mes,"\n")) 
          {
              strcpy(buffer,Mes);
              write(sockfd, buffer, BUFFER_SIZE - 1);
          }
      }
  }
  printf("\n__________Dialog is over__________\n");
  close(sockfd);
  return 0;
}
 
void error(const char *errorMessage)
{
  printf("%s\n", errorMessage);
  handler(SIGINT);
  exit(EXIT_FAILURE);
}
