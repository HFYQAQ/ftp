#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ftp_server.h"

int ftp_servfd;

int main()
{
	struct sockaddr_in servaddr;
	pthread_t threadId;
	ARG arg;
	int opt;
	int opt_size;

	if ((ftp_servfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror(ERRHEAD"socket error");
		exit(-1);
	}
	opt = SO_REUSEADDR;
	opt_size = sizeof(opt);
	setsockopt(ftp_servfd, SOL_SOCKET, SO_REUSEADDR, &opt, &opt_size);

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(FTP_SERVER_PORT);
	if (bind(ftp_servfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr)) == -1)
	{
		perror(ERRHEAD"bind error");
		exit(-1);
	}

	if (listen(ftp_servfd, LISTEN_BACKLOG) == -1)
	{
		perror(ERRHEAD"listen error");
		exit(-1);
	}
	printf(LOGHEAD"ftp server started.\n");

	for (; ;)
	{
		if ((arg.clifd = accept(ftp_servfd, (struct sockaddr*)arg.cliaddr, sizeof(struct sockaddr))) == -1)
		{
			perror(ERRHEAD"accept error");
			exit(-1);
		}

		if (pthread_create())
		{
			
		}
	}

	return 0;
}