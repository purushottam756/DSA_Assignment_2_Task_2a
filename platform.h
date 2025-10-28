#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdbool.h>
#include "post.h"
#include "comment.h"
#include "reply.h"

typedef struct Platform {
    post *head;           // oldest post
    post *tail;           // newest post
    post *lastViewedpost; // last viewed 
} Platform;

extern Platform *platform;

Platform *createplatform(void);
bool addpost(const char *username, const char *caption);
bool deletepost(int n);            
post *viewpost(int n);             
post *currpost(void);
post *nextpost(void);            
post *previouspost(void);         

bool addcomment(const char *username, const char *content);
bool deletecomment(int n);         
comment *viewcomments(void);      

bool addreply(const char *username, const char *content, int n);
bool deletereply(int n_comment, int m_reply);

#endif 
