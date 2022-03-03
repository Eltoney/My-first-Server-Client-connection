#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

short createSocket(void)
{
    short serverSocket;
    printf("creating the socket.....\n");
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    return serverSocket;
}
int BindCreatedSocket(int curSocket)
{
    int iRetval = -1;
    int ClientPort = 90190;
    struct sockaddr_in remote = {0};

    remote.sin_family = AF_INET;

    remote.sin_addr.s_addr = htonl(INADDR_ANY);
    remote.sin_port = htons(ClientPort);
    iRetval = bind(curSocket, (struct sockaddr *)&remote, sizeof(remote));
    return iRetval;
}

int main(int argc, char *argv[])
{
    int socket_desc, sock, clientLen, read_size;
    struct sockaddr_in server, client;
    char client_message[500] = {0};
    char message[500] = {0};
    const char *endMessage = "bye";

    socket_desc = createSocket();
    if (socket_desc == -1)
    {
        printf("No socket created");
        return 1;
    }
    printf("Socket created\n");
    if (BindCreatedSocket(socket_desc) < 0)
    {
        perror("bind failed");
        return 1;
    }
    printf("bind done\n");
    listen(socket_desc, 2);
    while (1)
    {
        printf("Waiting for incoming connection..\n");
        clientLen = sizeof(struct sockaddr_in);

        sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&clientLen);

        if (sock < 0)
        {
            perror("accept failed");
            return 1;
        }
        printf("Connection sucess\n");
        while (1)
        {

            memset(client_message, '\0', sizeof client_message);
            memset(message, '\0', sizeof message);

            if (recv(sock, client_message, 200, 0) < 0)
            {
                printf("recv failed");
                break;
            }
            int done = 0;
            if (strcmp(endMessage, client_message) == 0)
            {
               strcpy(message , "bye");
                done = 1;
            }
            else
            {

                printf("Client reply: %s\n\n", client_message);
                printf("Enter your message: ");
                gets(message);
            }
            if (send(sock, message, strlen(message), 0) < 0)
            {
                printf("Send failed");
                return 1;
            }
            if(done == 1){
                break;
            }
        }
    }
    close(sock);
    return 0;
}