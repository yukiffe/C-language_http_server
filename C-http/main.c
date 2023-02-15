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

#define PORT 3000 // 자동완성이 자꾸 3000가르켜서 변경
#define BUFFER 1024  

using namespace std;

/*
* C언어 API 사용 경험이 적고 코드가 너무 난잡해서
* 자고일어나면 기억리셋 + 보기어려운 관계로
* 쓸모 없는 코드를 제거하고 다시 쌓아갈 예정
* -> 서버의 실행, 클라이언트에 http 헤더+바디를 제공하는걸 최우선으로 지정
*/
SOCKET* _connect_sockets;

void error(const char* _message);
void init(WSADATA* _window_socket_API_data, SOCKET* _server_socket);
void run(SOCKET* _server_socket);

int main() {
	WSADATA _window_socket_API_data;
	SOCKET _server_socket;//listen->service로 의미 변경
	init(&_window_socket_API_data, &_server_socket);
	run(&_server_socket);
	return 0;
}	void error(const char* _message) {
	printf("ERROR: \"%s\"\n", _message);
	exit(-1);
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
	if (listen(*_server_socket, 10) == -1) error("Listening");
}


void run(SOCKET* _server_socket) {
	SOCKET _client_socket;
	SOCKADDR_IN _client_address;
	int _client_address_length = sizeof(_client_address);
	socklen_t client_length;
	char* buffer = "Hello, World!";

	_client_socket = accept(*_server_socket, (SOCKADDR*)&_client_address, &_client_address_length);
	printf("tcp 접속: %s", inet_ntoa(_client_address.sin_addr));
	int len = sizeof(buffer);
	send(_client_socket, buffer, sizeof(buffer), BUFFER);
	closesocket(_client_socket);
}
