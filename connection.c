#include "connection.h"

#ifdef _WIN32
	void socketStartup()
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 0), &wsaData);
	}
	void socketCleanup()
	{
		WSACleanup();
	}
#else
	void socketStartup()
	{
		//not implemented
	}
	void socketCleanup()
	{
		//not implemented
	}
#endif

SOCKET connection_init(const char* server, const char* port)
{
    struct addrinfo hints, *p, *servinfo;
	int rv;
	
	socketStartup();
	SOCKET sockfd = -1;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(server, port, &hints, &servinfo)) != 0)
    {
        return -1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            continue;
        }

        if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
#ifdef _WIN32
            closesocket(sockfd);
#else
            close(sockfd);
#endif
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    return sockfd;
}

int connection_send(SOCKET sockfd, const char* buffer, int max_length)
{
    return send(sockfd, buffer, max_length, 0);
}

int connection_peek(SOCKET sockfd, char* buffer, int max_length)
{
    return recv(sockfd, buffer, max_length, MSG_PEEK);
}

int connection_recv(SOCKET sockfd, char* buffer, int max_length)
{
    return recv(sockfd, buffer, max_length, 0);
}

int connection_close(SOCKET sockfd)
{
#ifdef _WIN32
    int x = closesocket(sockfd);
#else
    int x = close(sockfd);
#endif
    socketCleanup();

    return x;
}
