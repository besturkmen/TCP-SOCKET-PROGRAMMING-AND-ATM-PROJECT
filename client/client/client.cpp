#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<iostream>
#include<string>
#include<WS2tcpip.h>

#pragma comment(lib,"ws2_32.lib")

using namespace std;

typedef struct userInfo {
	int		userId;
	char	password[8];
} userInfo;

typedef struct serverReplyInfo {
	int	returnCode;
	char message[100];
} serverReplyInfo;

typedef struct processInfo {
	int	menuItem;
	int userId;
	int receiverId; // para gönderme işleminde para gönderilecek müşteri no
	int amount; // para gönderme ve çekme işleminde kullanılacak tutar 
} processInfo;

short SocketCreate(void)
{
	short hSocket;
	cout<<("Create the socket\n");
	hSocket = socket(AF_INET, SOCK_STREAM, 0);
	return hSocket;
}
//try to connect with server
int SocketConnect(int hSocket)
{
	int iRetval = -1;
	int ServerPort = 90190;
	struct sockaddr_in remote = { 0 };
	remote.sin_addr.s_addr = inet_addr("127.0.0.1"); //Local Host
	remote.sin_family = AF_INET;
	remote.sin_port = htons(ServerPort);
	iRetval = connect(hSocket, (struct sockaddr*)&remote, sizeof(struct sockaddr_in));
	return iRetval;
}
// Send the data to the server and set the timeout of 20 seconds
int SocketSend(int hSocket, char* Rqst, short lenRqst)
{
	int shortRetval = -1;
	struct timeval tv;
	tv.tv_sec = 20;  /* 20 Secs Timeout */
	tv.tv_usec = 0;
	if (setsockopt(hSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(tv)) < 0)
	{
		cout<<("Time Out\n");
		return -1;
	}
	shortRetval = send(hSocket, Rqst, lenRqst, 0);
	return shortRetval;
}
//receive the data from the server
int SocketReceive(int hSocket, char* Rsp, short RvcSize)
{
	int shortRetval = -1;
	struct timeval tv;
	tv.tv_sec = 20;  /* 20 Secs Timeout */
	tv.tv_usec = 0;
	if (setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv)) < 0)
	{
		cout<<("Time Out\n");
		return -1;
	}
	shortRetval = recv(hSocket, Rsp, RvcSize, 0);
	cout<<("Response %s\n", Rsp);
	return shortRetval;
}


userInfo GetUserInfo(void)
{
	int		id;
	char	psw[8];
	userInfo user;
	serverReplyInfo returnInfo;

	while (1) {
		cout<<("=============================================\n");
		cout<<("KULLANICI GİRİŞ\n");
		cout<<("=============================================\n");
		cout<<("Lütfen kullanıcı numaranızı giriniz :\n");
		cin >> id;
		cout<<("Lütfen şifrenizi giriniz :\n");
			cin>> psw ;
			int sonuc;
			
		// Burada password uzunluk vb kontroller yapılabilir.
		user.userId = id;
		strncpy(user.password, psw, strlen(psw));

		return(user);

		sonuc = SifreKontrol(id, psw);
		if (sonuc == 0) {
			return(0);  // Kullanıcı bilgileri doğru
		}
		else {
			return(1);  // Kullanıcı bilgileri hatalı
		}
	}
}

char* SerializeUserInfo(userInfo user)
{
	char userSerialize[30];
	// user serialize edilip char string olarak geri döndürülür
	return(userSerialize);
}

serverReplyInfo DeSerializeUserInfo(char* str)
{
	serverReplyInfo reply;
	// str ile gelen char string deserialize edilip reply structure ına atanır
	return reply;
}

char* SerializeProcessInfo(processInfo process)
{
	char processSerialize[200];
	// process serialize edilip char string olarak geri döndürülür
	return(processSerialize);
}

serverReplyInfo DeSerializeProcessInfo(char* str)
{
	serverReplyInfo reply;
	// str ile gelen char string deserialize edilip reply structure ına atanır
	return reply;
}

// Bakiye sorgulama ekranı 
int GetBalanceScreen(int userId, socketId)

{
	char* processStr;
	serverReplyInfo serverReply;
	processInfo process;

	process.menuItem = 1;
	process.userId = userId;

	processStr = SerializeProcessInfo(process);

	//Send data to the server
	SocketSend(hSocket, processStr, strlen(processStr));

	//Received the data from the server
	read_size = SocketReceive(socketId, server_reply, 200);
	serverReply = DeSerializeProcessInfo(server_reply);

	if (serverReply.returnCode != 0) {
		cout<<("İşlem sırasında hata oluştu: %s", serverReply.message);
		exit(0);
	}
}

// Para gonderme ekranı
int SendMoneyScreen(int userId, socketId)
{
	int receiverId;
	int amount;
	// Ekrandan para gönderilecek müşteri no ve gönderilecek tutar alınacak
	cout<<("Para Gönderilecek müşteri numarasını giriniz.\n");
	cin>>receiverId;
	cout<<("Gönderilecek tutarı giriniz.\n");
		cin>>amount;

	// Buraya tutar sıfır girilemez vb kontroller eklenebilir.

	char* processStr;
	serverReplyInfo serverReply;
	processInfo process;

	process.menuItem = 2;
	process.userId = userId;
	process.receiverId = receiverId;
	process.amount = amount;

	processStr = SerializeProcessInfo(process);

	//Send data to the server
	SocketSend(hSocket, processStr, strlen(processStr));

	//Received the data from the server
	read_size = SocketReceive(socketId, server_reply, 200);
	serverReply = DeSerializeProcessInfo(server_reply);

	if (serverReply.returnCode != 0) {
		cout<<("İşlem sırasında hata oluştu: %s", serverReply.message);
		exit(0);
	}
}

// Para çekme ekranı
int WithDrawScreen(int userId, socketId)
{
	int amount;

	// Ekrandan çekilecek para miktarı girilir.
	cout << ("Çekilecek tutarı giriniz.\n");
		cin>>amount;

	// Buraya tutar sıfır girilemez vb kontroller eklenebilir.

	char* processStr;
	serverReplyInfo serverReply;
	processInfo process;

	process.menuItem = 3;
	process.userId = userId;
	process.amount = amount;

	processStr = SerializeProcessInfo(process);

	//Send data to the server
	SocketSend(hSocket, processStr, strlen(processStr));

	//Received the data from the server
	read_size = SocketReceive(socketId, server_reply, 200);
	serverReply = DeSerializeProcessInfo(server_reply);

	if (serverReply.returnCode != 0) {
		cout<<("İşlem sırasında hata oluştu: %s", serverReply.message);
		exit(0);
	}
}

// Para yatırma ekranı
int DepositScreen(int userId, socketId)
{
	int amount;

	// Ekrandan yatırılacak para miktarı girilir.
	cout << ("Yatırılacak tutarı giriniz.\n");
		cin>>amount;

	// Buraya tutar sıfır girilemez vb kontroller eklenebilir.

	char* processStr;
	serverReplyInfo serverReply;
	processInfo process;

	process.menuItem = 4;
	process.userId = userId;
	process.amount = amount;

	processStr = SerializeProcessInfo(process);

	//Send data to the server
	SocketSend(hSocket, processStr, strlen(processStr));

	//Received the data from the server
	read_size = SocketReceive(socketId, server_reply, 200);
	serverReply = DeSerializeProcessInfo(server_reply);

	if (serverReply.returnCode != 0) {
		cout<<("İşlem sırasında hata oluştu: %s", serverReply.message);
		exit(0);
	}
}

int Menu(userInfo user, int socketId)
{
	int menuItem;

	while (1) {
		cout<<("=============================================\n");
		cout<<("MENU\n");
		cout<<("=============================================\n");
		cout<<("1. Bakiye Görme\n");
		cout<<("2. Para Gönderme\n");
	    cout<<("3. Para Çekme \n");
		cout<<("4. Para Yatırma \n");
		cout<<("5. Çıkış \n\n");
		cout<<("Lütfen yapmak istediğiniz işlemi seçiniz: ");

		cin>>menuItem;

		switch (menuItem) {
		case 1:
			/* Bakiye sorgulama */
			GetBalanceScreen(user.userId, socketId);
			break;
		case 2:
			/* Para gönderme ekranı */
			SendMoneyScreen(user.userId, socketId);
			break;
		case 3:
			/* Para çekme ekranı */
			WithDrawScreen(user.userId, socketId);
			break;
		case 4:
			/*Para yatırma ekranı */
			DepositScreen(user.userId, socketId);
			break;
		case 5:
			return(0);
		default:
			cout<<("Geçersiz seçim.\n");
			break;
		}
	}
	return(0);
}


int main(int argc, char* argv[])
{
	int hSocket, read_size;
	struct sockaddr_in server;
	char sendToServer[100] = { 0 };
	char server_reply[200] = { 0 };
	userInfo user;
	serverReplyInfo serverReply;
	char* userInfoStr;

	//Create socket
	hSocket = SocketCreate();
	if (hSocket == -1)
	{
		cout<<("Could not create socket\n");
		return 1;
	}
	cout<<("Socket is created\n");
	//Connect to remote server
	if (SocketConnect(hSocket) < 0)
	{
		cerr<<("connect failed.\n");
		return 1;
	}

	user = GetUserInfo();
	// Burada user structure serialize yapılıp server a gönderilecek
	// Server a char olarak gönderebildiğimiz için struct 
	// serialize edilip char olarak gönderilmeli
	userInfoStr = SerializeUserInfo(user);

	//Send data to the server
	SocketSend(hSocket, userInfoStr, strlen(userInfoStr));

	//Received the data from the server
	read_size = SocketReceive(hSocket, server_reply, 200);
	serverReply = DeSerializeUserInfo(server_reply);

	if (serverReply.returnCode != 0) {
		cout<<("Hatalı giriş: %s", serverReply.message);
		exit(0); // Kullanıcı bilgileri hatalı, işlemlere devam edilmez.
	}


	Menu(user, hSocket);
	closesocket(hSocket);
	shutdown(hSocket, 0);
	shutdown(hSocket, 1);
	shutdown(hSocket, 2);
	return 0;

}