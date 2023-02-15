#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <vector>
#include <Winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h>

#define IP = "127.0.0.1"
#define PORT 80
#define BUFFER 1024 
#define MAX_CONNECT 0x00000002 

using namespace std;

int _connection_count = 0;
SOCKET* _connect_sockets[MAX_CONNECT];


void error(const char* _message) {
	printf("ERROR: \"%s\"\n", _message);
	exit(-1);
}

void init(WSADATA* _window_socket_API_data, SOCKET* _service_socket, int* _return_value) {
	if (WSAStartup(MAKEWORD(2, 2), _window_socket_API_data) != 0) error("Start-Up");
	*_service_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (*_service_socket == INVALID_SOCKET) error("Invalid-Socket");
	SOCKADDR_IN _listener_address;
	ZeroMemory(&_listener_address, sizeof(SOCKADDR_IN));
	_listener_address.sin_family = AF_INET;
	_listener_address.sin_port = htons(PORT);
	_listener_address.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(*_service_socket, (SOCKADDR*)&_listener_address, sizeof(_listener_address)) == SOCKET_ERROR)error("Binding");
	if (listen(*_service_socket, MAX_CONNECT) == SOCKET_ERROR) error("Listening");
	u_long _on = 1;
	*_return_value = ioctlsocket(*_service_socket, FIONBIO, &_on);
	if (*_return_value == SOCKET_ERROR) error("Asynchronous");
}

void run(SOCKET* _service_socket, int* _return_value) {
	FD_SET _fd_set;
	SOCKET _client_socket;
	SOCKADDR_IN _client_address;
	FD_ZERO(&_fd_set);
	FD_SET(*_service_socket, &_fd_set);
	for (int i = 0; i < _connection_count; i++) {
		FD_SET(*_connect_sockets[i], &_fd_set);
	}
	*_return_value = select(0, &_fd_set, nullptr, nullptr, nullptr);
	if (*_return_value == SOCKET_ERROR) {
		error("Select (Read/Write)");
	}
	if (FD_ISSET(*_service_socket, &_fd_set)) {
		int client_address_length = sizeof(_client_address);
		_client_socket = accept(*_service_socket, (SOCKADDR*)&_client_address, &client_address_length);
		printf("connect: %s\n", inet_ntoa(_client_address.sin_addr));
		if (_connection_count < MAX_CONNECT) {
			SOCKET* socket = new SOCKET;
			_connect_sockets[_connection_count++] = socket;
		}
		else {
			printf("연결 초과");
		}
	}
}

int main() {
	WSADATA _window_socket_API_data;
	SOCKET _service_socket;//listen->service로 의미 변경
	int _return_value;
	init(&_window_socket_API_data, &_service_socket, &_return_value);
	while (true) {
		run(&_service_socket, &_return_value);
	}
	return 0;
}