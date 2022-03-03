#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

short createSocket(void)
{
    short clientSocket;
    printf("Creating the socket\n");
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    return clientSocket;
}

int connectSocket(int curSocket)
{
    int iRetVal = -1;
    int serverPort = 90190;
    struct sockaddr_in remote = {0};
    remote.sin_addr.s_addr = inet_addr("127.0.0.1");
    remote.sin_family = AF_INET;
    remote.sin_port = htons(serverPort);
    iRetVal = connect(curSocket, (struct sockaddr *)&remote, sizeof(struct sockaddr_in));
    return iRetVal;
}

int SocketSend(int curSocket, char *Rqst, short lenRqst)
{
    int shortRetVal = -1;
    struct timeval tv;
    tv.tv_sec = 20;
    tv.tv_usec = 0;
    if (setsockopt(curSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv)) < 0)
    {
        printf("Time out\n");
        return -1;
    }
    shortRetVal = send(curSocket, Rqst, lenRqst, 0);
    return shortRetVal;
}

int SocketReceive(int curSocket, char *Rsp, short RvcSize)
{
    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 20;
    tv.tv_usec = 0;

    if (setsockopt(curSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv)) < 0)
    {
        printf("Time out\n");
        return -1;
    }
    shortRetval = recv(curSocket, Rsp, RvcSize, 0);
    printf("Response %s\n", Rsp);
    return shortRetval;
}

int main(int argc, char *argv[])
{
    int curSockt, read_size;
    struct sockaddr_in server;
    char SendToServer[500] = {0};
    char server_reply[500] = {0};
    const char *endMessage = "bye";

    curSockt = createSocket();
    if (curSockt == -1)
    {
        printf("Failed to create socket\n");
        return 1;
    }
    printf("Socket is created\n");
    if (connectSocket(curSockt) < 0)
    {
        perror("connect failed\n");
        return 1;
    }
    printf("Sucessfully conected with server\n");
    while (1)
    {
        memset(server_reply, '\0', sizeof server_reply);
        memset(SendToServer, '\0', sizeof SendToServer);
        printf("Enter the Message: ");
        gets(SendToServer);
        // Send data to the server
        SocketSend(curSockt, SendToServer, strlen(SendToServer));
        // Received the data from the server
        read_size = SocketReceive(curSockt, server_reply, 200);
        printf("Server Response : %s\n\n", server_reply);
        if (strcmp(server_reply, endMessage) == 0)
        {
            break;
        }
    }
    close(curSockt);
    shutdown(curSockt, 0);
    shutdown(curSockt, 1);
    shutdown(curSockt, 2);
    return 0;
}