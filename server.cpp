#include <iostream>
#include <stdio.h>
#include <mysql.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>

using namespace std;

void finish_with_error(MYSQL *conn) {
	fprintf(stderr, "%s\n", mysql_error(conn));
	mysql_close(conn);
	exit(1);
}

int main() {
	int listensd; 
	int connectsd; 
	int bufsize = 1024;
	char sendBuffer[bufsize];
	char recvBuffer[bufsize];

	struct sockaddr_in serverAddr; //containing server address 
	struct sockaddr_in clientAddr;
	socklen_t size;

	MYSQL *conn = mysql_init(NULL); //initializing object 

	if(conn == NULL) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}	

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

	if(mysql_real_connect(conn, "localhost", "solhee", "imEhfl25!", "chatdb", 3306, NULL, 0) == NULL) {
		finish_with_error(conn);
	}

	if(mysql_query(conn, "CREATE TABLE IF NOT EXISTS Chatlog(Name TEXT, Content TEXT)")) {
		finish_with_error(conn);
	}

	char strbuf[bufsize];
	char chatContent[bufsize];
	while(connectsd > 0) {
		memset(recvBuffer, 0, sizeof(recvBuffer));
		cout << "** Client: ";	
		recv(connectsd, recvBuffer, bufsize, 0);
		cout << recvBuffer << endl;
	
		if (*recvBuffer == '#')
			break;		

		//strcpy(chatContent, recvBuffer);

		sprintf(strbuf, "INSERT INTO Chatlog VALUES ('%s', '%s')", "Client", recvBuffer);

		if(mysql_query(conn, strbuf)){
			finish_with_error(conn);
		}

		memset (sendBuffer, 0, sizeof(sendBuffer));
		cout << "** Server: ";
		cin >> sendBuffer;
		send(connectsd, sendBuffer, bufsize, 0);

		if (*sendBuffer == '#')
			break;

		memset(chatContent, 0, sizeof(chatContent));
		memset(strbuf, 0, sizeof(strbuf));
		strcpy(chatContent, sendBuffer);
		sprintf(strbuf, "INSERT INTO Chatlog VALUES ('%s', '%s')", "Server", sendBuffer);

		if(mysql_query(conn, strbuf)){
			finish_with_error(conn);
		}
	}	

	cout << "=> Ended connection..." << endl;
	close(listensd);
	close(connectsd);
	mysql_close(conn);

	return 0;
}
