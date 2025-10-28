#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comment.h"

comment *createcomment(const char *username, const char *content) {
    if (!username || !content) return NULL;
    comment *c = (comment *)malloc(sizeof(comment));
    if (!c) return NULL;
    c->username = strdup(username);
    c->content = strdup(content);
    c->replies = NULL;
    c->next = NULL;
    return c;
}

void freecommentAndReplies(comment *c) {
    if (!c) return;
    reply *r = c->replies;
    while (r) {
        reply *tmp = r->next;
        if (r->username) free(r->username);
        if (r->content) free(r->content);
        free(r);
        r = tmp;
    }
    if (c->username) free(c->username);
    if (c->content) free(c->content);
    free(c);
}

comment *appendcomment(comment *head, comment *newc) {
    if (!newc) return head;
    if (!head) return newc;
    comment *cur = head;
    while (cur->next) cur = cur->next;
    cur->next = newc;
    return head;
}

int countcomments(comment *head) {
    int cnt = 0;
    comment *cur = head;
    while (cur) { cnt++; cur = cur->next; }
    return cnt;
}
