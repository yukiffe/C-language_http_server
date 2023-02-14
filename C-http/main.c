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
#define CONNECT 0x00000002 // 연결은 2개까지만
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

void error(const char* message);//블로그들 찾아보니까 에러 처리해주는게 있었음
void init(WSADATA* wsaData, SOCKET* socket);

int socket();//안씀->init대체, 삭제예정
int bind();//바인딩도 init으로 대체 -> 굳이구현x
int listen();//바인딩이랑 리스닝은 따라다니므로 init에 또 집어넣기
int accept();
int read();
int write();
int close();//이걸 쓸지날이 올지를 모르겠넹..

int main() {
	WSADATA _window_socket_API_data;
	SOCKET _socket;
	init(&_window_socket_API_data, &_socket);//초반 초기화는 이걸로 다 해줄꺼

	while (true) {

	}
	return 0;
}

void init(WSADATA* _window_socket_API_data, SOCKET* _socket) {
	if (WSAStartup(MAKEWORD(2, 2), _window_socket_API_data) != 0) {
		error("Start-Up");
	}//전체API 기초 세팅
	*_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (*_socket == INVALID_SOCKET) {
		error("Invalid-Socket");
	}//소켓 생성
	SOCKADDR_IN _socket_address;//소켓 주소 구조체
	ZeroMemory(&_socket_address, sizeof(SOCKADDR_IN));
	_socket_address.sin_family = AF_INET;//4계층
	_socket_address.sin_port = htons(PORT);//포트랑를 16비트로
	_socket_address.sin_addr.s_addr = htonl(INADDR_ANY);//주소를 16비트 0으로 떄려박기
	//소켓 정보 
	if (bind(*_socket, (SOCKADDR*)&_socket_address, sizeof(_socket_address)) == SOCKET_ERROR)
		error("Binding");//기본 바인딩
	if (listen(*_socket, CONNECT) == SOCKET_ERROR)
		error("Listening");//연결은 2개까지만 받을꺼임. 나중에 테스트 예정
	//초기화 끗
}

void error(const char* message) {
	printf("ERROR: \"%s\"\n", message);
	exit(-1);//프로그램이 비정상적으로 끝났음을 표시

}