#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include "ftp_client.h"

int ftp_clifd;

int main()
{
	struct sockaddr_in servaddr;
	const char* hostname;
	int opt;
	WORD wSocketVersion;
	WSADATA wsaData;

	wSocketVersion = MAKEWORD(2, 2);
	if (WSAStartup(wSocketVersion, &wsaData))
	{
		printf(ERRHEAD"WSA init failed!\n");
		exit(-1);
	}

	if ((ftp_clifd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf(ERRHEAD"socket error!\n");
		WSACleanup();
		exit(-1);
	}
	opt = SO_REUSEADDR;
	setsockopt(ftp_clifd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

	memset(&servaddr, 0, sizeof(servaddr));
	hostname = "127.0.0.1";
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(hostname);
	servaddr.sin_port = htons(21);
	if (connect(ftp_clifd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		printf(ERRHEAD"connect error!\n");
		//close(ftp_clifd);
		WSACleanup();
		getchar();
		exit(-1);
	}
	printf("connect success.\n");
	getchar();

    return 0;
}

int get_local_port_random()
{
	srand(time(NULL));
	return rand() % 50001 + 10000;
}