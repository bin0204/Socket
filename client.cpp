#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

int main()
{
  /* ---------- INITIALIZING VARIABLES ---------- */

    /*
       1. client is a file descriptor to store the values
       returned by the socket system call and the accept
       system call.
       2. portNum is for storing port number on which
       the accepts connections
       3. isExit is bool variable which will be used to
       end the loop
       4. The client reads characters from the socket
       connection into a dynamic variable (buffer).
       5. A sockaddr_in is a structure containing an internet
       address. This structure is already defined in netinet/in.h, so
       we don't need to declare it again.
        DEFINITION:
        struct sockaddr_in
        {
          short   sin_family;
          u_short sin_port;
          struct  in_addr sin_addr;
          char    sin_zero[8];
        };
        6. serv_addr will contain the address of the server
    */
    int client;
    int portNum = 8080;
    int bufsize = 1024;
    char buffer[bufsize];
    char ip[] = "127.0.0.1";
    struct sockaddr_in server_addr;
    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0)
    {
        cout << "Error establishing socket" << endl;
        exit(1);
    }
    cout << "=> Socket client has been created" << endl;
    server_addr.sin_family = AF_INET;
    cout << "=> Connection to the ip address: ";
    cin >> ip;
    cout << "=> Connection to the server port number: ";
    cin >> portNum;
    server_addr.sin_addr.s_addr = inet_addr((char*)ip);
    server_addr.sin_port = htons(portNum);
    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
      cout << "=> Error connection to the server ipaddress: " << ip << " and port number: " << portNum << endl;
    }

    /*
        The connect function is called by the client to
        establish a connection to the server. It takes
        three arguments, the socket file descriptor, the
        address of the host to which it wants to connect
        (including the port number), and the size of this
        address.
        This function returns 0 on success and -1
        if it fails.
        Note that the client needs to know the port number of
        the server but not its own port number.
    */
    cout << "=> Awaiting confirmation from the server..." << endl;
    recv(client, buffer, bufsize, 0);
    cout << buffer << endl;
    cout << "=> Connection confirmed, you are good to go" << endl;
    cout << "=> Enter # to end the connection\n" << endl;
    cout << "~File Name: ";
    cin >> buffer;
    if(buffer[0] == '#'){
      cout << "Connection terminated." << endl;
      close(client);
      return 0;
    }
    send(client, buffer, bufsize, 0);
    recv(client, buffer, bufsize, 0);
    cout << "~" << buffer << endl;
    recv(client, buffer, bufsize, 0);
    while(buffer[0] != 'e'){
      cout << "~" << buffer << endl;
      recv(client, buffer, bufsize, 0);
    }
    cout << "Connection terminated." << endl;
    close(client);
    return 0;
}
