#ifndef __IRC_H__
#define __IRC_H__

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "connection.h"

typedef struct
{
    char* server;
    char* port;
    SOCKET sockfd;
} irc_t;

typedef struct
{
    char* nick;
    char* user;
    char* host;
} user_t;

typedef struct
{
    char* channel;
    char* message;
} message_t;

typedef void (*fn_message_received)(irc_t* irc, user_t* user, message_t* message);

struct list* privmsg_list;
struct list* notice_list;

void irc_init(irc_t* irc, const char* server, const char* port);
void irc_send(irc_t* irc, const char* format, ...);
int  irc_read_line(irc_t* irc, char* buffer, int max_length);
void irc_register(irc_t* irc, const char* command, void* received);
void irc_main_loop(irc_t* irc, const char* nick);
void irc_cleanup(irc_t* irc);

#endif // __IRC_H__
