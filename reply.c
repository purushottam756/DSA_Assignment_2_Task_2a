#include <stdlib.h>
#include <string.h>
#include "reply.h"

reply *createreply(const char *username, const char *content) {
    if (!username || !content) return NULL;
    reply *r = (reply *)malloc(sizeof(reply));
    if (!r) return NULL;
    r->username = strdup(username);
    r->content = strdup(content);
    r->next = NULL;
    return r;
}

reply *appendreply(reply *head, reply *newr) {
    if (!newr) return head;
    if (!head) return newr;
    reply *cur = head;
    while (cur->next) cur = cur->next;
    cur->next = newr;
    return head;
}

int cnt_replies(reply *head) {
    int c = 0;
    reply *cur = head;
    while (cur) { c++; cur = cur->next; }
    return c;
}
