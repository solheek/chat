#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main() {
	int listensd; 
	int connectsd; 
	int bufsize = 1024;
	char sendBuffer[bufsize];
	char recvBuffer[bufsize];

	struct sockaddr_in serverAddr; //containing server address 
	struct sockaddr_in clientAddr;
	socklen_t size;

	listensd = socket(PF_INET, SOCK_STREAM, 0); //IPv4, tcp socket created

	if(listensd == -1) { //failed to create socket
		cout << "\nError in creating socket" << endl;
		exit(1);
	}

	cout << "\n=> Socket server is created..." << endl;

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET; //IPv4
	serverAddr.sin_port = htons(6743); //converting port
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); 

	if( bind(listensd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
		cout << "\nError in binding" << endl;
		return -1;
	}

	cout << "=> Looking for clients..." << endl;

	if(listen(listensd, 5) == -1) { //listen on the socket for connections
		cout << "\nError in listening" << endl;
		exit(1);
	}	

	size = sizeof(clientAddr);
	connectsd = accept(listensd, (struct sockaddr*)&clientAddr, &size);
	
	strcpy(sendBuffer, "=> Server is connected...\n");
	send(connectsd, sendBuffer, bufsize, 0);

	cout << "=> Connection is confirmed! To end the connection, enter #." << endl;

	while(connectsd > 0) {
		memset(recvBuffer, 0, sizeof(recvBuffer));
		cout << "** Client: ";	
		recv(connectsd, recvBuffer, bufsize, 0);
		cout << recvBuffer << endl;

		if (*recvBuffer == '#')
			break;

		cout << "** Server: ";
		cin >> sendBuffer;
		send(connectsd, sendBuffer, bufsize, 0);

		if (*sendBuffer == '#')
			break;
	}	

	cout << "=> Ended connection..." << endl;
	close(listensd);
	close(connectsd);

	return 0;
}
