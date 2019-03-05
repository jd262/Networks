//Jacob Darabaris
//server


#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#define BUFSIZE 1024
#define PORT 61500
#define WEATHERPORT 80

using namespace std;

int main()
{
    int clientSocket, serverSocket, weatherServerSocket;
    char buf1[BUFSIZE];
    char buf2[BUFSIZE];
    string data, reply;
    
    struct sockaddr_in serverAddr;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY; //more specific
    
    int addrLen2 = sizeof(serverAddr);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    
    //if(bind(serverSocket, (struct sockaddr *) &serverAddr, addrLen) < 0)
    //{
    //    cout << "Error binding" << endl;
    //    exit(1);
    //}
    bind(serverSocket, (struct sockaddr *) &serverAddr, addrLen2);
    
    if(listen(serverSocket, 10) == 0)
        cout << "Server is listening on port: " << PORT << endl;
    else
        cout << "Server is not working" << endl;
    listen(serverSocket, 10);
    
    // vv WUNDERGROUND vv//
    weatherServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct hostent *server;
    server = gethostbyname("api.wunderground.com");
    struct sockaddr_in weatherServerAddr;
    int addrLen3 = sizeof(weatherServerAddr);
    bzero((char *) &weatherServerAddr, addrLen3);
    weatherServerAddr.sin_family = AF_INET;
    weatherServerAddr.sin_port = htons(WEATHERPORT);
    bcopy((char *)server->h_addr,
          (char *)&weatherServerAddr.sin_addr.s_addr,
          server->h_length);
   // ^^ WUNDERGROUND ^^ //
    
    while(1)
    {  // main accept() loop
        
        connect(weatherServerSocket, (struct sockaddr *)&weatherServerAddr, addrLen2); //CONNECT TO WUNDERGROUND
        
        bind(serverSocket, (struct sockaddr *) &serverAddr, addrLen2);
        struct sockaddr_in clientAddr;
        int addrLen1 = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, (socklen_t*)&addrLen1);
        cout << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs (clientAddr.sin_port) << endl;
        int receive = recv(clientSocket, buf1, BUFSIZE, 0);
        
        if(strncmp(buf1, "Evansville", 20) == 0)
        {
            send(weatherServerSocket, "GET /api/b83bbc9a3e39b91c/forecast/geolookup/conditions/q/IN/evansville.xml HTTP/1.0\r\nHost: api.wunderground.com\r\n\r\n", strlen("GET /api/b83bbc9a3e39b91c/forecast/geolookup/conditions/q/IN/evansville.xml HTTP/1.0\r\nHost: api.wunderground.com\r\n\r\n"), 0);
        }
        if(strncmp(buf1, "Chicago", 20) == 0)
        {
            send(weatherServerSocket, "GET /api/b83bbc9a3e39b91c/forecast/geolookup/conditions/q/IL/chicago.xml HTTP/1.0\r\nHost: api.wunderground.com\r\n\r\n", strlen("GET /api/b83bbc9a3e39b91c/forecast/geolookup/conditions/q/IL/chicago.xml HTTP/1.0\r\nHost: api.wunderground.com\r\n\r\n"), 0);
        }
        if(strncmp(buf1, "Miami", 20) == 0)
        {
            send(weatherServerSocket, "GET /api/b83bbc9a3e39b91c/forecast/geolookup/conditions/q/FL/miami.xml HTTP/1.0\r\nHost: api.wunderground.com\r\n\r\n", strlen("GET /api/b83bbc9a3e39b91c/forecast/geolookup/conditions/q/FL/miami.xml HTTP/1.0\r\nHost: api.wunderground.com\r\n\r\n"), 0);
        }

        while(data.find("</response>") == string::npos)
        {
            receive = recv(weatherServerSocket, buf2, BUFSIZE, 0);
            buf2[BUFSIZE - 1] = '\0'; //make sure it stops at the end
            data += buf2;
        }
            //send(clientfd, iter+1, receive, 0);
        data = data.substr(data.find("<response>"), (data.find("</response>") + 11));
        for(int i=0; i < 6; i++)
        {
            data = data.substr(data.find("<fcttext>") + 18, data.length());
            reply += data.substr(0, data.find("]")) + '~'; //divide up sections
            
        }
        char buffer[reply.length()]; //instead of buffer[BUFSIZE]
        strcpy(buffer, reply.c_str()); //assign char[] to string
        send(clientSocket, buffer, reply.length(), 0);
        //send(clientSocket, buf, receive, 0);
        string arr = string (buf1);
        cout << arr << endl;
        close(clientSocket);
    }
    
    close(weatherServerSocket);
    close(serverSocket);
    return 0;
}
