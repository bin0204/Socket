#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>


using namespace std;

void *task(void *);
int server;
bool isExit = false;
int bufsize = 1024;
char buffer[1024];
struct stat fileStas;
char filename[1024];
pthread_t thread[3];

int main()
{
  /*Initializing Variables*/
  //store the values returned by the socket system call and the accept system call
  int client;
  char ip[] = "127.0.0.1";
  int portNum = 8080;
  struct sockaddr_in server_addr;
  socklen_t size;

  client = socket(AF_INET, SOCK_STREAM, 0);
  if (client < 0)
  {
      cout << "Error establishing socket" << endl;
      exit(1);
  }
  /*socket() function creates a new socket
    It takes 3 arguments,

      a. AF_INET : address domain of the socket
      b. SOCK_STREAM : Type of socket. a stream socket
      in which characters are read in a continuous stream (TCP)
      c. Third is a protocol argument : should always be 0.
      The OS will choose the most appropiate protocol

      If the socket call fails, it returns -1.
      Otherwise, it will return a small integer
  */
  cout << "=> Socket server has been created" << endl;
  server_addr.sin_family = AF_INET;
  //server_addr.sin_addr.s_addr = htons(INADDR_ANY);
  server_addr.sin_addr.s_addr = inet_addr((char*)ip);
  server_addr.sin_port = htons(portNum);
  if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0)
  {
      cout << "=> Error binding connection, the socket has already been established" << endl;
      return -1;
  }
  size = sizeof(server_addr);
  cout << "=> Looking for clients..." << endl;
  listen(client, 3);
  int numThread = 0;
  while(numThread < 3){
    server = accept(client,(struct sockaddr *)&server_addr,&size);
    if (server < 0){
      cout << "=> Error on accepting..." << endl;
      break;
    }
    else {
        strcpy(buffer, "=> Server connected");
        send(server, buffer, bufsize, 0);
        cout << "=> Connected with the client, you are good to go" << endl;
    }
    pthread_create(&thread[numThread], NULL, task, NULL);
    numThread++;
  }
  for(int count=0;count<3;count++){
    pthread_join(thread[count],NULL);
  }
  cout << "=>Server closed" << endl;
  close(client);
  return 0;
}

void *task(void* justPutHere){
  recv(server, buffer, bufsize, 0);
  if(buffer[0] != '#'){
    stat(buffer, &fileStas);
    char* size = (char*) std::to_string(fileStas.st_size).c_str();
    send(server, size, bufsize, 0);
    ifstream infile(buffer);
    string stringLine = "";
    while(!infile.eof()){
      getline(infile, stringLine);
      char* line = (char*) stringLine.c_str();
      send(server, line, bufsize, 0);
    }
  }
  send(server, "e", bufsize, 0);
  cout << "=>Connection with client terminated" << endl;
  close(server);
}
