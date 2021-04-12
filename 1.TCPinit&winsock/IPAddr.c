#pragma comment(lib, "ws2_32")
#include<winsock2.h>
#include<ws2tcpip.h>
#include<stdio.h>

int main(int argc, char *argv[]) {
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0) return 1;

    char *ipv4test = "127.0.0.1";
    printf("IPv4 addr(before) = %s\n", ipv4test);

    printf("IPv4 addr(after) = %s\n", inet_addr(ipv4test));

    IN_ADDR ipv4num;
    ipv4num.s_addr = inet_addr(ipv4test);
    printf("IPv4 address(tarnsform) = %s\n", inet_ntoa(ipv4num));

    printf("\n");

}