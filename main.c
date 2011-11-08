#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "irc.h"
#include "connection.h"
#include "list.h"

irc_t irc;

void handle_signal(int sig)
{
    irc_cleanup(&irc);
}

void my_message_received(irc_t* irc, user_t* user, message_t* message)
{
    printf("[%s][%s]: %s\n", message->channel, user->nick, message->message);
    if(strstr(message->message, ".") == message->message)
    {
        char* command;
        char* arguments;

       // printf("Starts with !\n");

        command = strtok(message->message + 1, " ");
        arguments = strtok(NULL, "\0");

      //  printf("%s\n%s\n", command, arguments);

        if(strcmp(command, "say") == 0)
        {
            irc_send(irc, "PRIVMSG %s :%s\r\n", message->channel, arguments);
        }
        else if(strcmp(command, "join") == 0)
        {
            irc_send(irc, "JOIN %s\r\n", arguments);
        }
        else if(strcmp(command, "part") == 0)
        {
            irc_send(irc, "PART %s\r\n", arguments);
        }
		else if(strcmp(command, "grab") == 0)
		    irc_send(irc, "PRIVMSG %S :Grabbed quote!\r\n", message ->channel, arguments);
		    irc_send(irc, "PRIVMSG %S :%S\r\n", message->channel, arguments);
    }
}

int main()
{
    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);
    signal(SIGABRT, handle_signal);

    irc_init(&irc, "irc.strictfp.com", "6667");

    irc_register(&irc, "PRIVMSG", my_message_received);
    irc_send(&irc, "JOIN #stl\r\n");
    irc_main_loop(&irc, "qdbot");
    irc_cleanup(&irc);

    return 0;
}