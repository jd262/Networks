#include <iostream#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define SERVER "localhost"
#define BUFSIZE 1024
using namespace std;

int main(int argc, char *argv[])
{
        int SocketD, port, n;
	port = atoi(argv[1]);
	char buffer[BUFSIZE];
        SocketD = socket(AF_INET, SOCK_STREAM, 0);
	if (argc != 2) 
	{
       		cout << "usage error";
		exit(0);
        }
	if (SocketD < 0)
	{
		cout << "ERROR opening socket\n";
		exit(0);
	}	
	
	struct hostent *server;
	server = gethostbyname(SERVER);

        struct sockaddr_in server_addr;

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = INADDR_ANY;
	
	int addrlen = sizeof(server_addr);
	if(connect(SocketD, (struct sockaddr *) &server_addr, addrlen) < 0)
	{
		cout << "ERROR connecting\n";
		exit(0);
	} 
	if (server == NULL) 
	{
        	cout << "ERROR, no such host\n";
        	exit(0);
	}
	cout << "Enter a message: ";
	bzero(buffer, BUFSIZE);
	//string tmp;
	//cin >> tmp;
    	fgets(buffer, BUFSIZE, stdin);
	n = write(SocketD, buffer, strlen(buffer));
	if (n < 0)
	{
		cout << "ERROR writing to the socket\n";
		exit(0);
	}
	bzero(buffer, BUFSIZE);
	n = read(SocketD, buffer, BUFSIZE);
    	if (n < 0) 
	{
		cout << "ERROR reading from socket\n";
		exit(0);
	}
        cout << "Here's your message: " << buffer << endl;
	close(SocketD);
        return 0;
}
