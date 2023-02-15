#pragma warning(disable:4996) // 에러를 감춘다.
#pragma comment(lib, "ws2_32.lib") // 전처리 과정에서의 함수 참조

#include <stdio.h> // 기본
#include <Winsock2.h> // socket통신용 library ( io 제거 )

#define PORT 80 // http 기본 포트
#define BUFFER 1024 // 버퍼 크기

char buffer[BUFFER]; // 정보를 받아올 버퍼
char resp[] = "HTTP/1.0 200 OK\r\n"
"Server: webserver-c\r\n"
"Content-type: text/html\r\n\r\n"
"<html>hello, world</html>\r\n"; // html을 출력할 헤더와 바디

void error(const char* _message); // 에러를 출력해줄 함수

int main() {
	WSADATA _window_socket_API_data; //소켓 통신 API
	SOCKET _server_socket, _client_socket; // 서버와 클라이언트 소켓
	SOCKADDR_IN _server_address; // 서버 바인딩을 위한 구조체
	int _server_address_length = sizeof(_server_address); // 의 길이

	if (WSAStartup(MAKEWORD(2, 0), &_window_socket_API_data) != 0) error("Start-Up"); //소켓 통신 초기화
	_server_socket = socket(AF_INET, SOCK_STREAM, 0);  // 소켓 생성
	if (_server_socket == INVALID_SOCKET) error("Invalid-Socket"); // 소켓 검증
	_server_address.sin_family = AF_INET; // IP4 주소
	_server_address.sin_port = htons(PORT); // 연결 포트
	_server_address.sin_addr.s_addr = htonl(INADDR_ANY); // 주소
	if (bind(_server_socket, (SOCKADDR*)&_server_address, _server_address_length) != 0)error("Binding"); // 서버 바인딩
	if (listen(_server_socket, SOMAXCONN) != 0) error("Listening"); // 서버 리스닝

	while (true) { // 통신을 지속적으로 반복
		_client_socket = accept(_server_socket, (SOCKADDR*)(&_server_address), &_server_address_length); // 클라이언트 대기
		if (_client_socket < 0)error("Accept"); // 잘못된 클라이언트 검증
		recv(_client_socket, buffer, sizeof(buffer), 0); // 클라이언트의 정보 확인
		printf("%s", buffer); // 콘솔 출력
		send(_client_socket, resp, strlen(resp), 0); // 서버에서 클라이언트로 소켓 전송
		closesocket(_client_socket); // 클라이언트 연결 해제
	}
}

void error(const char* _message) {
	printf("ERROR: \"%s\"\n", _message);
	exit(-1);
}