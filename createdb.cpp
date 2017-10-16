#include <iostream>
#include <stdlib.h>
#include <mysql.h>

using namespace std;

int main(int argc, char *argv[]) {
	MYSQL *conn = mysql_init(NULL); 

	if (conn == NULL) {
		cout << "\nError in initializing MYSQL object." << endl;
		exit(1);
	}

	cout << "** Mysql handle: " << conn << endl;
	
	if (mysql_real_connect(conn, "localhost", "root", "imEhfl25!", NULL, 3306, NULL, 0) == NULL) {
		cout << "\nError in connecting mysql" << endl;
		mysql_close(conn);		
		exit(1);
	}

	if (mysql_query(conn, "CREATE DATABASE chatdb")) {
		cout << "\nError in creating new DB" << endl;
		mysql_close(conn);
		exit(1);
	}

	mysql_close(conn);
	exit(0);
}
