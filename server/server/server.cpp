#include<iostream>
#include<string>
#include <WS2tcpip.h>
#include<chrono>
#include<thread>
#include<iomanip>
#include<sstream>


#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable : 4996)
using namespace std;
short SocketCreate(void)
{
    short hSocket;
    cout<<("Create the socket\n");
    hSocket = socket(AF_INET, SOCK_STREAM, 0);
    return hSocket;
}
int BindCreatedSocket(int hSocket)
{
    int iRetval = -1;
    int ClientPort = 90190;
    struct sockaddr_in  remote = { 0 };
    /* Internet address family */
    remote.sin_family = AF_INET;
    /* Any incoming interface */
    remote.sin_addr.s_addr = htonl(INADDR_ANY);
    remote.sin_port = htons(ClientPort); /* Local port */
    iRetval = bind(hSocket, (struct sockaddr*)&remote, sizeof(remote));
    return iRetval;
}
int main(int argc, char* argv[])
{
    int socket_desc, sock, clientLen, read_size;
    struct sockaddr_in server, client;
    char client_message[200] = { 0 };
    char message[100] = { 0 };
    const char* pMessage = "hello aticleworld.com";
    //Create socket
    socket_desc = SocketCreate();
    if (socket_desc == -1)
    {
        cout<<("Could not create socket");
        return 1;
    }
    cout<<("Socket created\n");
    //Bind
    if (BindCreatedSocket(socket_desc) < 0)
    {
        //print the error message
        cerr<<("bind failed.");
        return 1;
    }
    cout<<("bind done\n");
    //Listen
    listen(socket_desc, 3);
    //Accept and incoming connection
    while (1)
    {
        cout<<("Waiting for incoming connections...\n");
        clientLen = sizeof(struct sockaddr_in);
        //accept connection from an incoming client
        sock = accept(socket_desc, (struct sockaddr*)&client, (socklen_t*)&clientLen);
        if (sock < 0)
        {
            cerr<<("accept failed");
            return 1;
        }
        cout<<("Connection accepted\n");
        memset(client_message, '\0', sizeof client_message);
        memset(message, '\0', sizeof message);
        //Receive a reply from the client
        if (recv(sock, client_message, 200, 0) < 0)
        {
            cout<<("recv failed");
            break;
        }

        // Burada gelen mesaj kontrol edilerek yapılmak istenilen işlem 
        // bulunacak. Sonra hangi işlem yapılacak ise onun fonksiyonuna yönlendirilecek
        cout<<("Client reply : %s\n", client_message);
        if (strcmp(pMessage, client_message) == 0)
        {
            strcpy(message, "Hi there !");
        }
        else
        {
            strcpy(message, "Invalid Message !");
        }
        // Send some data
        if (send(sock, message, strlen(message), 0) < 0)
        {
            cout<<("Send failed");
            return 1;
        }
        closesocket(sock);
       

        //do clean Up
        WSACleanup();
       
    }
    return 0;
}