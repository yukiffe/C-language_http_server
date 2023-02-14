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

int connection_number = 0;
SOCKET* _connect_sockets[MAX_CONNECT];

void error(const char* _message);
void init(WSADATA* _window_socket_API_data, SOCKET* _listener, int* _return_value);
void run(SOCKET* _listener, FD_SET* _fd_set, SOCKET* _client_socket, int* _return_value);

int main() {
	WSADATA _window_socket_API_data;
	SOCKET _listen_socket;
	int _return_value;
	init(&_window_socket_API_data, &_listen_socket, &_return_value);
	FD_SET _fd_set;
	SOCKET _client_socket;
	while (true) {
		run(&_listen_socket, &_fd_set, &_client_socket, &_return_value);
	}
	return 0;
}

void init(WSADATA* _window_socket_API_data, SOCKET* _listener, int* _return_value) {
	if (WSAStartup(MAKEWORD(2, 2), _window_socket_API_data) != 0) {
		error("Start-Up");
	}
	*_listener = socket(AF_INET, SOCK_STREAM, 0);//4계층인터넷프로토콜, TCP/IP 사용
	if (*_listener == INVALID_SOCKET) {
		error("Invalid-Socket");
	}
	SOCKADDR_IN _listener_address;
	ZeroMemory(&_listener_address, sizeof(SOCKADDR_IN));
	_listener_address.sin_family = AF_INET;//IP4, ++++++나중에IP6도 커버하도록 해보자 
	_listener_address.sin_port = htons(PORT);
	_listener_address.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(*_listener, (SOCKADDR*)&_listener_address, sizeof(_listener_address)) == SOCKET_ERROR)
		error("Binding");
	if (listen(*_listener, MAX_CONNECT) == SOCKET_ERROR)
		error("Listening");
	u_long _on = 1;
	*_return_value = ioctlsocket(*_listener, FIONBIO, &_on);
	if (*_return_value == SOCKET_ERROR) {
		error("Asynchronous");
	}
}

void run(SOCKET* _listen_socket, FD_SET* _fd_set, SOCKET* _client_socket, int* _return_value) {
	SOCKADDR_IN _client_address;
	FD_ZERO(_fd_set);
	FD_SET(*_listen_socket, _fd_set);
	for (int i = 0; i < connection_number; i++) {
		FD_SET(*_connect_sockets[i], _fd_set);
	}
	*_return_value = select(0, _fd_set, nullptr, nullptr, nullptr);
	if (*_return_value == SOCKET_ERROR) {
		error("Select (Read/Write)");
	}

	if (FD_ISSET(*_listen_socket, _fd_set)) {
		int addrlen = sizeof(_client_address);
		*_client_socket = accept(*_listen_socket, (SOCKADDR*)&_client_address, &addrlen);
		printf("%s	%d\n", inet_ntoa(_client_address.sin_addr), ntohs(_client_address.sin_port));// 포트 이상함
		exit(1);
	}//테스트 서버 접속
}

void error(const char* _message) {
	printf("ERROR: \"%s\"\n", _message);
	exit(-1);//프로그램이 비정상적으로 끝났음을 표시
}