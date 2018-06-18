#pragma once


#ifndef WINSOCK_H

#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

#define WINSOCK_H

#endif

#ifndef WINDOWS_H

#define WINDOWS_H
#include<Windows.h>

#endif

#ifndef SOCK_PORT

#define SOCK_PORT 27015 //默认端口号

#endif


bool GetHostInf(struct hostent* hostinf) {   //获取主机信息
	char host_name[100];
	if (gethostname(host_name, sizeof(host_name)) != 0) {
		return false;
	}
	hostinf = gethostbyname(host_name);
	return true;
}
/*
bool isIP(char *a) {
	char *b = "192.168.1.";
	for (int i = 0; i <= strlen(b) - 1; i++) {
		if (a[i] != b[i])
			return false;
	}
	return true;
}
*/

#ifndef HOST_TEXT

bool GetHostAddr(char *addr) {
	char host_name[256] = { 0 };
	char ip[20] = { 0 };
	char *a = ip;
	gethostname(host_name, sizeof(host_name));
	HOSTENT *host_entry = gethostbyname(host_name);

	if (host_entry != NULL&& host_entry->h_addr_list[0] != NULL)
		sprintf(addr, "%s", inet_ntoa(*(struct in_addr *)host_entry->h_addr_list[0]));
	else return false;
	return true;
	/*
	for (int i = 0; host_entry != NULL && host_entry->h_addr_list[i] != NULL; ++i) {
		sprintf(addr, "%s", inet_ntoa(*(struct in_addr *)host_entry->h_addr_list[i]));
		//strcpy_s(addr ,strlen(ip),ip);
		printf("[IP]\t%s\n[Name]\t%s\n\n", addr, host_name);	//*****text*****
		//if (isIP(addr)) {
			//sprintf_s(addr,strlen(a),"%s", a);
			//return true;
	//}
	}
	return false;
	*/
}

#else

bool GetHostAddr(char *addr) {
	strcpy(addr, "127.0.0.1");
	return true;
}

#endif

bool GetHostName(char *name) {	//获取主机名字
	if (gethostname(name, sizeof(name)) != 0) {
		return false;
	}
	return true;
}

bool InitSocket()	//初始化socket
{
	WSADATA wsaData;
	int err;
	err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0) {
		return false;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		WSACleanup();
		return false;
	}
	return true;
}


inline void EndSock() {	//结束sock
	WSACleanup();
}

#ifdef SOCKRUN_TCP

bool StartListen(SOCKET *s, char *sServerIP) {	//开始监听端口

	*s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in sockaddr = { 0 };
	sockaddr.sin_addr.S_un.S_addr = inet_addr(sServerIP);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(SOCK_PORT);

	if (bind(*s, (SOCKADDR *)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR) {
		closesocket(*s);
		return false;
	}

	if (listen(*s, SOMAXCONN) == SOCKET_ERROR) {
		closesocket(*s);
		return false;
	}
	return true;
}

bool StartConnect(SOCKET *s, char *sServerIP) {

	*s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in sockaddr = { 0 };
	sockaddr.sin_addr.S_un.S_addr = inet_addr(sServerIP);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(SOCK_PORT);

	if (connect(*s, (SOCKADDR *)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR) {
		closesocket(*s);
		return false;
	}
	return true;
}




#else
#ifdef SOCKRUN_UDP
bool StartListen(SOCKET *s, char *sServerIP) {	//开始监听端口

	*s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	struct sockaddr_in sockaddr = { 0 };
	sockaddr.sin_addr.S_un.S_addr = inet_addr(sServerIP);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(SOCK_PORT);

	if (bind(*s, (SOCKADDR *)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR) {
		return false;
	}

	if (listen(*s, SOMAXCONN) == SOCKET_ERROR) {
		return false;
	}
	return true;
}

bool StartConnect(SOCKET *s, char *sServerIP) {

	*s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	struct sockaddr_in sockaddr = { 0 };
	sockaddr.sin_addr.S_un.S_addr = inet_addr(sServerIP);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(SOCK_PORT);

	if (connect(*s, (SOCKADDR *)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR) {
		closesocket(*s);
		return false;
	}
	return true;
}

#endif
#endif