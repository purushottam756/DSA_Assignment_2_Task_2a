#ifndef POST_H
#define POST_H

#include <stdbool.h>
#include "comment.h"

typedef struct post {
    char *username;
    char *caption;
    comment *comments;      // head of comment linked list
    struct post *prev;      // older post
    struct post *next;      // newer post
} post;

post *createpost(const char *username, const char *caption);

void freepostshallow(post *p);

#endif 
