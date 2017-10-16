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

int main() {
	int clientsd; 
	int bufsize=1024;
	char recvBuffer[bufsize];
	char sendBuffer[bufsize];
	char* ip = "127.0.0.1";
	int portNum = 6743;

	struct sockaddr_in serverAddr;
	
	clientsd = socket(AF_INET, SOCK_STREAM, 0);

	if(clientsd == -1) {
		cout << "\nError in creating socket" << endl;
		exit(1);
	}

	cout << "\n=> Socket client is created..." << endl;
	
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNum);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if( connect(clientsd, (struct sockaddr *)&serverAddr, sizeof(serverAddr))== 0) {
		cout << "=> Connection to the server port: " << portNum << endl;	
	}

	recv(clientsd, recvBuffer, bufsize, 0);
	cout << recvBuffer;
	cout << "=> Connection confirmed! To end the connection, enter #." << endl;

	while(clientsd>0) {
		
		memset(sendBuffer, 0, sizeof(sendBuffer));
		cout << "** Client: ";	
		cin >> sendBuffer;
		send(clientsd, sendBuffer, bufsize, 0);

		if(*sendBuffer == '#')
			break;
		
		memset(recvBuffer, 0, sizeof(recvBuffer));
		cout << "** Server: ";
		recv(clientsd, recvBuffer, bufsize,0);
		cout << recvBuffer << endl;

		if(*recvBuffer == '#')
			break;
	}

	cout << "=> Ended connection..." << endl;
	close(clientsd);
	return 0;

}
