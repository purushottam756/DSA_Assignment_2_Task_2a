#ifndef COMMENT_H
#define COMMENT_H

#include <stdbool.h>
#include "reply.h"

typedef struct comment {
    char *username;
    char *content;
    reply *replies;         // head of replies list (bonus)
    struct comment *next;   // next comment in the post's list
} comment;

comment *createcomment(const char *username, const char *content);

void freecommentAndReplies(comment *c);

comment *appendcomment(comment *head, comment *newc);

int countcomments(comment *head);

#endif 
