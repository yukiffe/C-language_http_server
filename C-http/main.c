#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <vector>
#include <Winsock2.h>
#include <winsock.h>
#include <Windows.h>
#include <io.h>

#define PORT 3000
#define BUFFER 1024  

char buffer[BUFFER];
char resp[] = "HTTP/1.0 200 OK\r\n"
"Server: webserver-c\r\n"
"Content-type: text/html\r\n\r\n"
"<html>hello, world</html>\r\n";

void error(const char* _message);

int main() {
	WSADATA _window_socket_API_data;
	SOCKET _server_socket, _client_socket;
	SOCKADDR_IN _server_address, _client_address;
	int _server_address_length = sizeof(_server_address);
	int _client_address_length = sizeof(_client_address);
	memset(&_server_address, '0', sizeof(SOCKADDR_IN));
	memset(&_client_address, '0', sizeof(SOCKADDR_IN));

	if (WSAStartup(MAKEWORD(2, 2), &_window_socket_API_data) != 0) error("Start-Up");
	_server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_socket == INVALID_SOCKET) error("Invalid-Socket");
	ZeroMemory(&_server_address, sizeof(SOCKADDR_IN));
	_server_address.sin_family = AF_INET;
	_server_address.sin_port = htons(PORT);
	_server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(_server_socket, (SOCKADDR*)&_server_address, _server_address_length) != 0)error("Binding");
	if (listen(_server_socket, SOMAXCONN) != 0) error("Listening");

	while (true) {
		_client_socket = socket(AF_INET, SOCK_STREAM, 0);
		_client_socket = accept(_server_socket, (SOCKADDR*)(&_server_address), &_server_address_length);
		if (_client_socket < 0) error("Accept");
		if (getsockname(_client_socket, (SOCKADDR*)&_client_address, &_client_address_length) < 0) { error("Server"); continue; }
		
		printf("SOCKET3");
		closesocket(_server_socket);
		printf("SOCKET4");
		//if (read(_client_socket, buffer,strlen(buffer)) < 0) { error("Read"); continue; }//하나만읽을꺼니까 while x
		printf("SOCKET5");
		char method[BUFFER], uri[BUFFER], version[BUFFER];
		sscanf(buffer, "%s %s %s", method, uri, version);
		printf("[%s:%u] %s %s %s\n", inet_ntoa(_client_address.sin_addr), ntohs(_client_address.sin_port), method, version, uri);
		printf("SOCKET7");
		if (write(_client_socket, resp, strlen(resp)) < 0) {error("Write"); continue;}
		printf("SOCKET8");
		close(_client_socket);
	}

	return 0;
}

void error(const char* _message) {
	printf("ERROR: \"%s\"\n", _message);
	exit(-1);
}
