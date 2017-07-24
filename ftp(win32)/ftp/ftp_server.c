#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "ftp_server.h"

int ftp_servfd;

int main()
{
	struct sockaddr_in servaddr;
	HANDLE thread_handle;
	ARG arg;
	int cliaddr_size;
	int opt;
	WORD wSocketVersion;
	WSADATA wsaData;
	
	wSocketVersion = MAKEWORD(2, 2);
	if (WSAStartup(wSocketVersion, &wsaData))
	{
		printf(ERRHEAD"WSA init failed!\n");
		exit(-1);
	}

	if ((ftp_servfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf(ERRHEAD"socket error!\n");
		WSACleanup();
		exit(-1);
	}
	opt = SO_REUSEADDR;
	setsockopt(ftp_servfd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(FTP_SERVER_PORT);
	if (bind(ftp_servfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr)) == SOCKET_ERROR)
	{
		printf(ERRHEAD"bind error!\n");
		close(ftp_servfd);
		WSACleanup();
		exit(-1);
	}

	if (listen(ftp_servfd, LISTEN_BACKLOG) == SOCKET_ERROR)
	{
		printf(ERRHEAD"listen error!\n");
		close(ftp_servfd);
		WSACleanup();
		exit(-1);
	}
	printf(LOGHEAD"ftp server started.\n");

	cliaddr_size = sizeof(struct sockaddr_in);
	for (; ;)
	{
		if ((arg.clifd = accept(ftp_servfd, (struct sockaddr*)&arg.cliaddr, &cliaddr_size)) == INVALID_SOCKET)
			continue;

		if (!CreateThread(NULL, 0, handle_client, (void*)&arg, 0, &thread_handle))
		{
			printf(ERRHEAD"create thread error!\n");
			close(ftp_servfd);
			WSACleanup();
			exit(-1);
		}
	}
	close(ftp_servfd);
	WSACleanup();

    return 0;
}

DWORD WINAPI handle_client(LPVOID lpParam)
{
	ARG cliinfo = *(ARG*)lpParam;

	printf(LOGHEAD"a new client connected.\n"
		"		fd  :%15d\n"
		"		ip  :%15s\n"
		"		port:%15d\n",
		cliinfo.clifd,
		inet_ntoa(cliinfo.cliaddr.sin_addr),
		ntohs(cliinfo.cliaddr.sin_port));

	ExitThread(0);
}