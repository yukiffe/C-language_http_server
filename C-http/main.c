//(나중) read_me기재: 모든 변수는 _포함 -> socket()같은 함수들 때문(비카멜케이스)

#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib") //소켓2 라이브러리를 위해서,,,

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <vector> //필요할것같은것들은 미리미리 다 가져오기
#include <Winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h> //<sys/socket>같은것들은 이걸로 변경
//이것들 오류인지도 모르고 폭풍삽질..

#define IP = "127.0.0.1" //내컴
#define PORT 80 //굳이 포트 달기 귀찮아서 이걸로
#define BUFFER 1024 //넉넉하게 1KB(맞나?)
#define MAX_CONNECT 0x00000002 // 연결은 2개까지만
int connection_number = 0;//위에랑 같이해서 초반 0개 시작
//아무도 안읽을것같지만
//자꾸 ===쓰고(typescript) 주석#쓰고
//클래스만드는데 public a()이런식으로쓰고 미칠껏같네요 홍홍

using namespace std;
//enum IP_version {
//	ipv4 = 0,
//	ipv6
//};//아이피 종류도 신경써주고

//struct Socket {
//	SOCKET socket;
//	IP_version version;
//	char buffer[BUFFER + 1];
//	int size;
//};//소켓 정보 담아줄꺼
//제외: 사유 안씀 -> 나중에쓸수도?

void error(const char* _message);//블로그들 찾아보니까 에러 처리해주는게 있었음
								//각 단계마다 에러 출력
void init(WSADATA* _wsaData, SOCKET* _socket);
void run(SOCKET* _socket, FD_SET* _fd_set, SOCKET* _client_socket, int* _return_value);
//어떨결에 만들다보니까 포인터로 다 떄려박아버린 코드덜..

int accept();//
int read();//
int write();//나머지도 run()함수로 통일
int close();

int main() {
	WSADATA _window_socket_API_data;
	SOCKET _socket;
	init(&_window_socket_API_data, &_socket);//초반 초기화는 이걸로 다 해줄꺼
	u_long _on = 1;
	int _return_value = ioctlsocket(_socket, FIONBIO, &_on);
	if (_return_value == SOCKET_ERROR) {
		error("Asynchronous");
	}//비동기로 소켓으로 변경
	FD_SET _fd_set;//읽기/쓰기/에러발생 대기
	SOCKET _client_socket;
	while (true) {
		run(&_socket, &_fd_set, &_client_socket, &_return_value);
	}
	return 0;
}

void init(WSADATA* _window_socket_API_data, SOCKET* _socket) {
	if (WSAStartup(MAKEWORD(2, 2), _window_socket_API_data) != 0) {
		error("Start-Up");
	}//전체API 기초 세팅
	*_socket = socket(AF_INET, SOCK_STREAM, 0);//4계층인터넷프로토콜, TCP/IP 사용
	if (*_socket == INVALID_SOCKET) {
		error("Invalid-Socket");
	}//소켓 생성
	SOCKADDR_IN _socket_address;//소켓 주소 구조체
	ZeroMemory(&_socket_address, sizeof(SOCKADDR_IN));
	_socket_address.sin_family = AF_INET;//IP4, ++++++나중에IP6도 커버하도록 해보자 
	_socket_address.sin_port = htons(PORT);//포트랑를 16비트로
	_socket_address.sin_addr.s_addr = htonl(INADDR_ANY);//주소를 16비트 0으로 떄려박기
	//소켓 정보 
	if (bind(*_socket, (SOCKADDR*)&_socket_address, sizeof(_socket_address)) == SOCKET_ERROR)
		error("Binding");//기본 바인딩
	if (listen(*_socket, MAX_CONNECT) == SOCKET_ERROR)
		error("Listening");//연결은 2개까지만 받을꺼임. 나중에 테스트 예정
	//초기화 끗
}

void run(SOCKET* _socket, FD_SET* _fd_set, SOCKET* _client_socket, int* _return_value) {
	FD_ZERO(_fd_set);
	FD_SET(*_socket, _fd_set);// 초기화
	SOCKET* _connect_sockets = new SOCKET[MAX_CONNECT];
	//참고, 소켓은 ip4 기준
	for (int i = 0; i < connection_number; i++) {
		FD_SET(_connect_sockets[i], _fd_set);//소켓을 차곡차곡~
	}
	*_return_value = select(0, _fd_set, nullptr, nullptr, nullptr);
	if (*_return_value == SOCKET_ERROR) {
		error("Select (Read/Write)");
	}

	if (FD_ISSET(*_socket, _fd_set)) {
		int addrlen = sizeof(_socket);
		SOCKADDR_IN clientaddr4;
		*_connect_sockets = accept(*_socket, (SOCKADDR*)&clientaddr4, &addrlen);
		printf("[TCPv4 서버] 클라이언트 접속 IP : %s		%d\r\n",
			inet_ntoa(clientaddr4.sin_addr), ntohs(clientaddr4.sin_port));//아이피, 포트 이상함
		exit(1);/*AddSocketInfo(_connect_sockets, false);*/
	}//테스트 서버 접속
}

void error(const char* _message) {
	printf("ERROR: \"%s\"\n", _message);
	exit(-1);//프로그램이 비정상적으로 끝났음을 표시
}