#ifndef _FTP_SERVER_H_
#define _FTP_SERVER_H_

#define APPNAME "FTP"

#define LOGHEAD "["APPNAME"]: "
#define ERRHEAD "[ERROR]: "

#define FTP_SERVER_PORT 21
#define LISTEN_BACKLOG 5

typedef struct arg {
	int clifd;
	struct sockaddr_in cliaddr;
} ARG;

DWORD WINAPI handle_client(LPVOID lpParam);

#endif