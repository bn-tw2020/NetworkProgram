#pragma comment(lib, "ws2_32")
#include<stdio.h>
#include<winsock2.h>

int main(int argc, char * argv[]) {
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

    u_short x1 = 0x1234;
    u_long y1 = 0x12345678;
    u_short x2;
    u_long y2;

    printf("[host byte] -> [network byte]\n");
    printf("0x%x -> 0x%x\n", x1, x2 = htons(x1));
    printf("0x%x -> 0x%x\n", y1, y2 = htonl(y1));

    printf("[network byte] -> [host byte]\n");
    printf("0x%x -> 0x%x\n", x2, htons(x2));
    printf("0x%x -> 0x%x\n", y2, htonl(y1));

    printf("\n[fault example]\n");
    printf("0x%x -> 0x%x\n", x1, htonl(x1));

    WSACleanup();
    return 0;
}