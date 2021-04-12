#pragma comment(lib, "ws2_32")
#include<stdio.h>
#include <winsock2.h>

void err_quit(char *msg) {
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
    MessageBox(NULL, (LPTSTR)lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}

int main(int argc, char *argv[]) {
    // 윈속 초기화
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;
    MessageBox(NULL, "winsock init success", "INFO", MB_OK);

    // socket
    SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(tcp_sock == INVALID_SOCKET) err_quit("socket()");
    MessageBox(NULL, "TCP socket generate success", "INFO", MB_OK);

    // closesocket()
    closesocket(tcp_sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}