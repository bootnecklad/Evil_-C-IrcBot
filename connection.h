#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <string.h>

#ifndef _WIN32
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <netdb.h>
#else
# include <windows.h>
# include <Ws2tcpip.h>
# include <winsock2.h>
#endif

#ifndef _WIN32
typedef int SOCKET;
#endif

SOCKET connection_init(const char* server, const char* port);

int connection_send(SOCKET sockfd, const char* buffer, int max_length);
int connection_recv(SOCKET sockfd, char* buffer, int max_length);
int connection_peek(SOCKET sockfd, char* buffer, int max_length);
int connection_close(SOCKET sockfd);

#endif // __CONNECTION_H__
