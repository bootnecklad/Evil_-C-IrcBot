#include "irc.h"

void irc_init(irc_t* irc, const char* server, const char* port, const char* nick)
{
    privmsg_list = list_create();
    notice_list = list_create();

    irc->server = strdup(server);
    irc->port = strdup(port);
    irc->sockfd = connection_init(server, port);
    irc_send(irc, "NICK %s\r\n", nick);
    irc_send(irc, "USER %s * * :C IrcBot programmed by Nicholas Bailey, modified by bootnecklad\r\n", nick);
}

void irc_send(irc_t* irc, const char* format, ...)
{
    char buf[512];

    va_list ap;
    va_start(ap, format);
    vsnprintf(buf, 512, format, ap);
    va_end(ap);

    connection_send(irc->sockfd, buf, strlen(buf));
}

int irc_read_line(irc_t* irc, char* buffer, int max_length)
{
    int amount;
    int idx;
    int offset = 0;

    amount = connection_peek(irc->sockfd, buffer, max_length);

    buffer[amount] = 0;

    for(idx = 0; idx < amount; idx++)
    {
        if(buffer[idx] == '\n')
        {
            break;
        }
    }

    connection_recv(irc->sockfd, buffer, idx + 1);

    for(idx = 0; idx < amount; idx++)
    {
        if(buffer[idx] == '\n')
        {
            if(buffer[idx - 1] == '\r')
            {
                offset--;
                buffer[idx - 1] = 0;
            }

            offset--;
            buffer[idx] = 0;
            break;
        }
    }

    return idx + offset;
}

void irc_main_loop(irc_t* irc)
{
    char buffer[512];

    while(irc_read_line(irc, buffer, 512) > 0)
    {
        if(strstr(buffer, "005") != NULL)
        {
            break;
        }
    }

    while(irc_read_line(irc, buffer, 512) > 0)
    {
        char* user_info;
        char* command;
        char* arguments;

        if(strstr(buffer, "PING") == buffer)
        {
            buffer[1] = 'O';
            irc_send(irc, "%s\r\n", buffer);
        }

        if(strstr(buffer, "lol quit") != NULL)
        {
            return;
        }

        if(*buffer != ':')
        {
            continue;
        }

        user_info = strtok(buffer + 1, " ");
        command = strtok(NULL, " ");
        arguments = strtok(NULL, "\0");

        if(command == NULL)
            continue;

        if(strcmp("PRIVMSG", command) == 0)
        {
            //printf("K, it was PRIVMSG\n");
            message_t* message = malloc(sizeof(message_t));
            //printf("Mallocing\n");
            user_t* user = malloc(sizeof(user_t));
            //printf("Getting USER\n");

            message->channel = strtok(arguments, " ");
            message->message = strtok(NULL, "\0");
            message->message++;
            //printf("Parsed Message\n");

            user->nick = strtok(user_info, "!");
            user->user = strtok(NULL, "@");
            user->host = strtok(NULL, "\0");
            //printf("Parsed user\n");

            if(!list_isempty(privmsg_list))
            {
                //printf("list isn't empty\n");
                struct node* cur = NULL;

                //printf("Calling funcs.\n");
                for(cur = privmsg_list->start;
                    cur != NULL;
                    cur = cur->next)
                {
                    //printf("Calling function!\n");
                    if(cur->data != NULL)
                        ((fn_message_received) cur->data)(irc, user, message);
                    //printf("Called\n");
                }
            }

            free(message);
            free(user);
        }
    }
}

void irc_register(irc_t* irc, const char* command, void* message)
{
    if(strcmp(command, "PRIVMSG") == 0)
    {
        list_add(privmsg_list, message);
    }
    else if(strcmp(command, "NOTICE") == 0)
    {
        list_add(notice_list, message);
    }
}

void irc_cleanup(irc_t* irc)
{
    free(irc->server);
    free(irc->port);
    connection_close(irc->sockfd);

    list_destroy(notice_list);
    list_destroy(privmsg_list);
}
