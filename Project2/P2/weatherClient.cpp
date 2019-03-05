//Jacob Darabaris
//client

#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <sys/types.h>
#include <netdb.h>
#define BUFSIZE 1024

using namespace std;

int main(int argc, char *argv[])
{
    struct hostent *server;
    int clientSocket, port, n;
    char buffer[BUFSIZE];
    string dataHolder, str;
    vector<string>weatherData;
    
    if (argc != 3)
    {
        cout << "Usage error" << endl;
        exit(1);
    }
    
    server = gethostbyname(argv[1]); //gets IP address
    port = atoi(argv[2]);
    
    if(server == NULL)
    {
        cout << "Error: no such host" << endl;
        exit(2);
    }
    
    clientSocket = socket(AF_INET, SOCK_STREAM, 0); //socket(int domain, int type, int protocol)
    
    if(clientSocket < 0)
    {
        cout << "Error opening socket" << endl;
        exit(3);
    }
    struct sockaddr_in serverAddr;
    int addrLen = sizeof(serverAddr);
    bzero((char *) &serverAddr, addrLen);
    serverAddr.sin_family = AF_INET; //Sets address family to the internet
    serverAddr.sin_port = htons(port); //Sets port number
    //serverAddr.sin_addr.s_addr = server;
    bcopy((char *)server->h_addr,
          (char *)&serverAddr.sin_addr.s_addr,
          server->h_length);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero); //Change?
    
    if(connect(clientSocket, (struct sockaddr *) &serverAddr, addrLen) < 0)
    {
        cout << "Error connecting" << endl;
        exit(4);
    }
    while(1)
    {
        connect(clientSocket, (struct sockaddr *) &serverAddr, addrLen);
        bzero(buffer, BUFSIZE); //fill the buffer array with empty values
        string choice1 = "Evansville";
        string choice2 = "Chicago";
        string choice3 = "Miami";
        cout << "Select a city from the following:" << endl << choice1 << endl << choice2 << endl << choice3 << endl;
        bool validChoice = false;
        string city = "";
        //fgets(buffer, BUFSIZE, stdin);
    
        while (!validChoice)
        {
            fgets(buffer, BUFSIZE, stdin);
            //cout << buffer;
            buffer[strlen(buffer)-1] = '\0';
            city = buffer;
            if (city == choice1 || city == choice2 || city == choice3)
                {
                    validChoice = true;
                }
            else
                cout << "Please enter a valid city" << endl;
        }
        //cout << "You've selected: " << city <<endl;
        n = send(clientSocket, buffer, strlen(buffer), 0); //send(int socket, const void *message, size_t length, int flags);
    
        if (n < 0)
        {
            cout << "ERROR writing to the socket\n";
            exit(5);
        }
    
        n = recv(clientSocket, buffer, BUFSIZE, 0);
    
        if (n < 0)
        {
            cout << "ERROR reading from socket\n";
            exit(6);
        }
        dataHolder = buffer;
        stringstream ss(dataHolder);
        
        while (getline(ss, str, '~'))
        {
            weatherData.push_back(str);
        }
        
        cout << "Weather Forecast" << endl;
        for (int i = 0; i < weatherData.size(); i++)
            cout << "Day " << i << ": " << weatherData.at(i) << endl;

        //cout << "Here's your message: " << buffer << endl;
        close(clientSocket);
    }
    return 0;
}
