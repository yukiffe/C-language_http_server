#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <vector> //필요할것같은것들은 미리미리 다 가져오기
#include <Winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h> //<sys/socket>같은것들은 이걸로 변경

#define IP = "127.0.0.1" //내컴
#define PORT 80 //굳이 포트 달기 귀찮아서 이걸로
#define BUFFER 1024 //넉넉하게 1KB(맞나?)

enum IP_version {
	ipv4 = 0,
	ipv6
};//아이피 종류도 신경써주고

struct Socket {
	SOCKET socket;
	IP_version version;
	char buffer[BUFFER + 1];
	int size;
};//소켓 틀

void error() {

}//블로그들 찾아보니까 에러 처리해주는게 있었음

int socket();
int bind();
int listen();
int accept();
int read();
int write();
int close();

int main() {
	WSADATA wsaData;

	while (true) {

	}
	return 0;
}