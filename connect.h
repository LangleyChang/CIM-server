#ifndef CONNECT_H_INCLUDED
#define CONNECT_H_INCLUDED

#include "bi_tree.h"

#define QUEUE 10
#define THREAD int
#define MAX_MSG 1024
#define MAX_BUFF sizeof(struct msg_pkt)
#define MAX_FR_LS 30
#define MAX_MULTI_CHAT 10

struct thread_args
{
    int client_sockfd;
};

enum FLAG{
    HELLO, // client say hello to server
    BYE,
    MSG, // client sends messages to friends
    FR_LS // client requires friend list
};

struct chat_msg
{
    int session_id;
    int size;
    unsigned long des_ip[MAX_MULTI_CHAT];
    char msg_str[MAX_MSG];
};

struct user_info
{
    unsigned long user_ip_addr;
    char user_name[MAX_USERNAME];
};

struct friends_info
{
    int size;
    struct user_info users[MAX_FR_LS];
};


struct msg_pkt
{
    int length;
    enum FLAG flag;
    union data{
        struct chat_msg message;
        struct friends_info friends;
    } msg_data;
};


#endif // CONNECT_H_INCLUDED
