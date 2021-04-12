#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    50

void err_quit(char *msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}

void err_display(char *msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    printf("[%s] %s", msg, (char *)lpMsgBuf);
    LocalFree(lpMsgBuf);
}

int main(int argc, char *argv[])
{
    int retval;

    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
    serveraddr.sin_port = htons(SERVERPORT);

    char buf[BUFSIZE];
    char *testdata[] = {
        "안녕하세요",
        "반가워요",
        "오늘따라 할 이야기가 많을 것 같네요",
        "저도 그렇네요",
    };
    int len;

    for(int i = 0; i < 4; i++) {
        // socket()
        SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock == INVALID_SOCKET) err_quit("socket()");

        // connect()
        retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
        if(retval == SOCKET_ERROR) err_quit("connect()");

        len = strlen(testdata[i]);
        strncpy(buf, testdata[i], len);
        retval = send(sock, buf, len, 0);
        if(retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }
        printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

        // closesocket()
        closesocket(sock);
    }

    // closewinsock()
    WSACleanup();
    return 0;
}
