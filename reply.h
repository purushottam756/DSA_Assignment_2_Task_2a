#ifndef reply_H
#define reply_H

typedef struct reply {
    char *username;
    char *content;
    struct reply *next;
} reply;

reply *createreply(const char *username, const char *content);

reply *appendreply(reply *head, reply *newr);

int cnt_replies(reply *head);

#endif 
