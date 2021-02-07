# Created by Vasilev Iulii, 209 groups 
## Implemented version of the task: сhat ##
## Description
* The program allows users to communicate with each other. 
* Users messages are sent to the server where they are glued with names and sent to all clients.
* The function of notifying about current users has also been implemented:
* The client can send an “informational” message that returns the name information of all excluded clients to the client.
* If the client provokes errors, for example, sends a SIGINT signal, the server disconnects the client, sends a message to all active users, and continues.
* In addition, if there is a problem with system calls, the client also exits and the server displays an error.
* Shutting down the server forces it to shut down all clients in an orderly fashion and display them in the appropriate shutdown message.
---
### 1/03/2018 last update: ###
1. Include port
2. Test unique name.
3. Test long name,test long message
4. Create operation /exit for server and client programm
5. Handle empty name/message + delete blank from name.
6. Add time in message.
7. Add command /help in server,client.
8. Corrected ports input.
9. Add command /chshow in server.Activ: switch of showing dialog in server(maybe used for server owner).
10. Processing port in server,client
---
## Special commands
###
1. **server.c** file contain command
***setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt))***
it is described in third book from Stolyarov A.V., page 215, theme "System socket in OS Unix".
The command provide remove the freese servers, because after finishing work, 
server some times exists in operating memory and waits activity from clients.
2. Files **server.c** and **client.c** use command ***select()***, provide working without parallelization of processes and save time.
3. Used library: <stdio.h>, <stdlib.h>, <string.h>, <unistd.h>, <sys/socket.h>, <sys/types.h>, <netdb.h>, <sys/time.h>, <signal.h>.
---
## Compile 
###
In order to compile files, you need to open a Linux terminal window and compile files using makefile: 
* ***make server - compile server***
* ***make client - compile client.***
---
## Run
* ***make runserver*** <- turn on the server in console
* ***make run client*** <- for client connections enable console client
* The program will ask for the user name, and then connect it to the chat.
###
