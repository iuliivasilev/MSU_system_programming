#Created by Vasiliev Yulii,209 groups#
##Implemented version of the task: Chat (number 1 in word file)##
###/in last(1/03/2018) added:###
* 1.Include port
* 2.Test unique name.
* 3.Test long name,test long message
* 4.Create operation /exit for server and client programm
* 5.Handle empty name/message + delete blank from name.
* 6.Add time in message.
* 7.Add command /help in server,client.
* 8.Corrected ports input.
* 9.Add command /chshow in server.Activ: switch of showing dialog in server(maybe used for server owner).
* 10.Processing port in server,client

###README file write on english because in my virtual machine Russian language not installed###
##_______________________________Part one: special commands._____________________________##
##1)In server.c file i use command ##
 setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)).
it is described in third book Stolyarov,page 215,theme "System socket in OS Unix".
The command provide remove the freese servers, because after finishing work, 
server some times exists in operating memory and waits activity from clients.
##2)In server.c and client.c i use command select(), provide working without parallelization of processes and save time.##
##3)Used library:## 
### <stdio.h> <stdlib.h> <string.h> <unistd.h> <sys/socket.h>### 
###<sys/types.h> <netdb.h> <sys/time.h> <signal.h> ###
###for work with sockets,strings,signals.###
##_______________________________Part two: compile._______________________________________##
In order to compile files,you need to open a Linux terminal window and compile files using makefile: make server - compile server, make client - compile client.
To get started, first turn on the server in one terminal window: make runserver.
For client connections enable the terminal client:
make run client .The program will ask for the user name, and then connect it to the chat.
##_______________________________Part three: work.____________________________________________##
The program provides an opportunity to communicate with customers.
Their messages are sent to a server where they are glued together with their names and sent to all clients.
The info function is also implemented (the client simply sends an "info" message), which returns information about the names of all the excluded clients to the client.If a client fails, such as sending a SIGINT signal, the server disables the client, sends the message to the others, and continues.Disabling signal to the server causes it to orderly shutdown all clients and displays them in the relevant message about the end of work.Also, when problems occur with system calls, the client also shuts down and displays an error.
