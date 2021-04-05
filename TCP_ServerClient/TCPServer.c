#pragma comment(lib, "ws2_32")
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#define SERVERPORT 9000
#define BUFSIZE 512

// ���� �Լ� ���� ��� �� ���
void err_quit(char *msg) {
    LPVOID lpMshBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMshBuf, 0, NULL);
    MessageBox(NULL, (LPCTSTR)lpMshBuf, msg, MB_ICONERROR);
    LocalFree(lpMshBuf);
    exit(1);
}

void err_display(char *msg) {
    LPVOID lpMshBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMshBuf, 0, NULL);
    printf("[%S] %s", msg, (char *)lpMshBuf);
    LocalFree(lpMshBuf);
}

int main(int argc, char *agrv[]) {
    int retval;

    // ���� �ʱ�ȭ
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;
    
    // socket()
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
    if(retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if(retval == SOCKET_ERROR) err_quit("listen()");

    // ������ ��ſ� ����� ����
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZ + 1];

    while(1) {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR *)&client_sock, &addrlen);
        if(client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        // ������ Ŭ���̾�Ʈ ���� ���
        printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ� = %s, ��Ʈ ��ȣ = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        // Ŭ���̾�Ʈ�� ������ ���
        while(1) {
            // ������ �ޱ�
            retval = recv(client_sock, buf, BUFSIZE, 0);
            if(retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            }
            else if(retval == 0) break;

            // ���� ������ ���
            buf[retval] = '\0';
            printf("[TCP%s : %d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

            // ������ ������
            retval = send(client_sock, buf, retval, 0);
            if(retval == SOCKET_ERROR) {
                err_display("send()");
                break;
            }
        }

        // closesocket()
        closesocket(client_sock);
        printf("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = %s, ��Ʈ ��ȣ = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    }

    // closesocket()
    closesocket(listen_sock);

    // ���� ����
    WSACleanup();
    return 0;
}