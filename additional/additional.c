#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <string.h>
#include <Winsock2.h>

#define PORT 80
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
	SOCKADDR_IN _server_address;
	int _server_address_length = sizeof(_server_address);

	if (WSAStartup(MAKEWORD(2, 0), &_window_socket_API_data) != 0) error("Start-Up");
	_server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_socket == INVALID_SOCKET) error("Invalid-Socket");
	_server_address.sin_family = AF_INET;
	_server_address.sin_port = htons(PORT);
	_server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(_server_socket, (SOCKADDR*)&_server_address, _server_address_length) != 0)error("Binding");
	if (listen(_server_socket, SOMAXCONN) != 0) error("Listening");

	while (true) {
		_client_socket = accept(_server_socket, (SOCKADDR*)(&_server_address), &_server_address_length);
		if (_client_socket < 0)error("Accept");
		recv(_client_socket, buffer, sizeof(buffer), 0);
		printf("%s", buffer);
		if (buffer[0] == 'G')
			printf("%s\n", "GET");//서버에서 요청
		if (buffer[0] == 'P')
			printf("%s\n", "POST");//서버에 전달..느낌(?)
		char* referer = strstr(buffer, "Referer");
		char routh[BUFFER] = "";
		if (referer != NULL) {
			for (int i = 9; i < strlen(referer); i++) {
				routh[i - 9] = referer[i];
				if (referer[i] == '\n')
					break;
			}
			printf("%s\n", routh);

		}
		if (!strcmp(routh, "http://localhost/3\r\n")) {
			strcat(resp, "<p>3</p>");
			send(_client_socket, resp, strlen(resp), 0);
		}
		else {
			send(_client_socket, resp, strlen(resp), 0);
		}
		closesocket(_client_socket);
	}
}

void error(const char* _message) {
	printf("ERROR: \"%s\"\n", _message);
	exit(-1);
}
