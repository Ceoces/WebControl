#pragma warning(disable:4996)

#define SOCKRUN_TCP
#define SOCK_PORT 27015
#define PASSWORD "123456"
#define HOST_TEXT


#include<stdio.h>
#include<string.h>
#include"log.h"
#include"SockRun.h"
#include"ThreadRun.h"
#include"shutdown.h"
#include"msg.h"




int main() {


	//同时只能有一个程序实例运行
	SetLastError(0);
	HANDLE hmutex = CreateMutex(NULL, FALSE, (LPCSTR)L"5D0E714F-8C2E-94D3-E819-92C54FDD2EDC");
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return 0;
	}
	SetLastError(0);

	system("color 02");

	SOCKET s;
	char buf[1024] = { 0 };
	char hostaddr[20 + 1] = { 0 };
	char hostname[20 + 1] = { 0 };

	FILE *flog = NewLog();

	if (!InitSocket()) {
		printf("Init Error.\nError code:%ld\n", WSAGetLastError());
		system("pause>nul");
		exit(1);
	}


	if (!GetHostAddr(hostaddr)) {
		if (WSAGetLastError() == 0)
			printf("Can't find right addr in your AP.\n");
		else
			printf("Get host addr error.\nError code:%ld\n", WSAGetLastError());
		system("pause>nul");
		exit(1);
	}
	else
		printf("Host addr:%s\n", hostaddr);

	if (!StartListen(&s, hostaddr)) {
		printf("Listen error.Error code:%ld\n", WSAGetLastError());
		system("pause>nul");
		closesocket(s);
		WSACleanup();
		return 1;
	}
	else
		printf("Port:%d\n", SOCK_PORT);

	struct sockaddr ClientAddr = { 0 };
	int ClientAddrLen = sizeof(ClientAddr);

	while (1) {
		//接受客户端连接,同时只能有一个客户端连接。
		printf("\nWaiting for connection...\n");
		struct sockaddr_in ConnAddr = { 0 };
		int ConnAddrlen = sizeof(ConnAddr);
		SOCKET AcceptSocket = accept(s, &ClientAddr, &ClientAddrLen);
		setsockopt(AcceptSocket, IPPROTO_TCP, SO_RCVBUF, (const char *)buf, sizeof(buf));
		if (AcceptSocket == INVALID_SOCKET) {
			printf("Accept error.\nError code:%ld\n", WSAGetLastError());
			closesocket(s);
			WSACleanup();
			return 1;
		}
		else {
			getpeername(AcceptSocket, (sockaddr*)&ConnAddr, &ConnAddrlen);
			AddLog(flog, inet_ntoa((struct in_addr)ConnAddr.sin_addr),LOG_DEVICECONNECT);
			printf("\nConn: %s Port: %d\n", inet_ntoa((struct in_addr)ConnAddr.sin_addr),ntohs(ConnAddr.sin_port));
		}
		printf("Authentication password...\n");
		if (GetPassword(&AcceptSocket)) {		//验证密码
			AddLog(flog, "right", LOG_PASSWORD);
			printf("Password Right!\n");
			int len = 0;
			do {
				len = recv(AcceptSocket, buf, sizeof(buf), 0);
				if (len > 0) {
					AddLog(flog, buf, LOG_CMD);
					DoMsg(buf);
					break;
				}
				//printf("%s", buf);
			} while (1);
			shutdown(AcceptSocket, SD_BOTH);
			AddLog(flog, NULL, LOG_BREAK);
			printf("Connect break.\n");
			continue;
		}
		else {
			AddLog(flog, "wrong", LOG_PASSWORD);
			AddLog(flog, NULL, LOG_BREAK);
			printf("Connect break.\n");
			shutdown(AcceptSocket, SD_BOTH);	//验证失败关闭连接
		}
	}
	fclose(flog);
	closesocket(s);
	WSACleanup();
	//printf("%ld\n", WSAGetLastError());
	system("pause");
	return 0;
}