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

//i will be die.... die..... i willl..... diie.....

char resp[] = "HTTP/1.0 200 OK\r\n"
"Server: webserver-c\r\n"
"Content-type: text/html\r\n\r\n"
"<html>hello, world</html>\r\n";

void error(const char* _message);
void init(WSADATA* _window_socket_API_data, SOCKET* _server_socket);
void run(SOCKET* _server_socket);

int main() {
	WSADATA _window_socket_API_data;
	SOCKET _server_socket;
	init(&_window_socket_API_data, &_server_socket);
	while (true)
		run(&_server_socket);
	return 0;
}

void init(WSADATA* _window_socket_API_data, SOCKET* _server_socket) {
	if (WSAStartup(MAKEWORD(2, 2), _window_socket_API_data) != 0) error("Start-Up");
	*_server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (*_server_socket == INVALID_SOCKET) error("Invalid-Socket");
	SOCKADDR_IN _service_address;
	ZeroMemory(&_service_address, sizeof(SOCKADDR_IN));
	_service_address.sin_family = AF_INET;
	_service_address.sin_port = htons(PORT);
	_service_address.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(*_server_socket, (SOCKADDR*)&_service_address, sizeof(_service_address)) == -1)error("Binding");
	if (listen(*_server_socket, SOMAXCONN) == -1) error("Listening");
}

void run(SOCKET* _server_socket) {
	SOCKET _client_socket;
	SOCKADDR_IN _client_address;
	int _client_address_length = sizeof(_client_address);
	_client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_client_socket == -1) error("Client_Socket");
	_client_socket = accept(*_server_socket, (SOCKADDR*)(&_client_address), &_client_address_length);
	if (_client_socket < 0) error("Accept");
	char buffer[BUFFER];
	//if (read(_client_socket, buffer, BUFFER) < 0)error("Read");
	//closesocket(*_server_socket);
	char method[BUFFER], uri[BUFFER], version[BUFFER];
	sscanf(buffer, "%s %s %s", method, uri, version);
	printf("[%s:%u] %s %s %s\n", inet_ntoa(_client_address.sin_addr), ntohs(_client_address.sin_port), method, version, uri);
	try {
		if (write(_client_socket, resp, strlen(resp)) < 0) error("Write");
	}
	catch(char* error) {
		printf("ERROR");
	}
	closesocket(_client_socket);
	WSACleanup();
}

void error(const char* _message) {
	printf("ERROR: \"%s\"\n", _message);
	exit(-1);
}
