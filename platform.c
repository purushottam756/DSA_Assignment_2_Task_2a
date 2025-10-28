#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform.h"

Platform *platform = NULL;

Platform *createplatform(void) {
    if (platform) return platform; // already created
    platform = (Platform *)malloc(sizeof(Platform));
    platform->head = platform->tail = platform->lastViewedpost = NULL;
    return platform;
}

static post *get_nth_recentpost(int n) {
    if (!platform || n <= 0) return NULL;
    post *cur = platform->tail;
    int idx = 1;
    while (cur && idx < n) { cur = cur->prev; idx++; }
    return cur;
}

// Add a post to tail
bool addpost(const char *username, const char *caption) {
    if (!platform) return false;
    post *p = createpost(username, caption);
    if (!p) return false;
    if (!platform->head) {
        platform->head = platform->tail = p;
    } else {
        p->prev = platform->tail;
        platform->tail->next = p;
        platform->tail = p;
    }
    platform->lastViewedpost = p; // new post becomes lastViewedpost
    return true;
}

// free all comments and replies of a post 
static void clearcomments(comment *head) {
    comment *cur = head;
    while (cur) {
        comment *tmp = cur->next;
        // free replies
        reply *r = cur->replies;
        while (r) {
            reply *rt = r->next;
            if (r->username) free(r->username);
            if (r->content) free(r->content);
            free(r);
            r = rt;
        }
        if (cur->username) free(cur->username);
        if (cur->content) free(cur->content);
        free(cur);
        cur = tmp;
    }
}

// Delete the nth recent post , also clear comments/replies
bool deletepost(int n) {
    if (!platform) return false;
    post *target = get_nth_recentpost(n);
    if (!target) return false;

    if (target->prev) target->prev->next = target->next;
    else platform->head = target->next; // target as head
    if (target->next) target->next->prev = target->prev;
    else platform->tail = target->prev; // target was tail

    if (platform->lastViewedpost == target) {
        platform->lastViewedpost = platform->tail; // most recent post or NULL
    }

    // free comments and replies
    clearcomments(target->comments);
    // free post strings and post structure
    if (target->username) free(target->username);
    if (target->caption) free(target->caption);
    free(target);
    return true;
}

// Return pointer to nth recent post and update lastViewedpost
post *viewpost(int n) {
    if (!platform) return NULL;
    post *p = get_nth_recentpost(n);
    if (!p) return NULL;
    platform->lastViewedpost = p;
    return p;
}

// Return lastViewedpost
post *currpost(void) {
    if (!platform) return NULL;
    if (!platform->lastViewedpost) platform->lastViewedpost = platform->tail;
    return platform->lastViewedpost;
}

// post which was posted just before posting the lastViewedpost
post *nextpost(void) {
    if (!platform) return NULL;
    post *cur = currpost();
    if (!cur) return NULL;
    if (cur->prev) {
        platform->lastViewedpost = cur->prev;
        return platform->lastViewedpost;
    }
    // if lastViewedpost is first post, return it 
    platform->lastViewedpost = cur;
    return cur;
}

// post which was posted just after posting the lastViewedpost
post *previouspost(void) {
    if (!platform) return NULL;
    post *cur = currpost();
    if (!cur) return NULL;
    if (cur->next) {
        platform->lastViewedpost = cur->next;
        return platform->lastViewedpost;
    }
    platform->lastViewedpost = cur;
    return cur;
}

// Add comment to lastViewedpost
bool addcomment(const char *username, const char *content) {
    if (!platform) return false;
    post *p = currpost();
    if (!p) return false;
    comment *c = createcomment(username, content);
    if (!c) return false;
    p->comments = appendcomment(p->comments, c);
    return true;
}

// get nth recent comment from a comment list 
static comment *getNthRecentcomment(comment *head, int n) {
    if (!head || n <= 0) return NULL;
    int cnt = 0;
    comment *cur = head;
    while (cur) { cnt++; cur = cur->next; }
    int idxFromStart = cnt - n + 1; 
    if (idxFromStart <= 0) return NULL;
    cur = head;
    int idx = 1;
    while (cur && idx < idxFromStart) { cur = cur->next; idx++; }
    return cur;
}

// Delete nth recent comment of lastViewedpost 
bool deletecomment(int n) {
    if (!platform) return false;
    post *p = currpost();
    if (!p || !p->comments) return false;
    comment *head = p->comments;
    comment *target = getNthRecentcomment(head, n);
    if (!target) return false;
    if (target == head) {
        p->comments = head->next;
        freecommentAndReplies(target);
        return true;
    }
    comment *prev = head;
    while (prev->next && prev->next != target) prev = prev->next;
    if (prev->next == target) {
        prev->next = target->next;
        freecommentAndReplies(target);
        return true;
    }
    return false;
}

// Return comments head for lastViewedpost 
comment *viewcomments(void) {
    if (!platform) return NULL;
    post *p = currpost();
    if (!p) return NULL;
    return p->comments;
}

// Add reply to the nth recent comment 
bool addreply(const char *username, const char *content, int n) {
    if (!platform) return false;
    post *p = currpost();
    if (!p || !p->comments) return false;
    comment *target = getNthRecentcomment(p->comments, n);
    if (!target) return false;
    reply *r = createreply(username, content);
    if (!r) return false;
    target->replies = appendreply(target->replies, r);
    return true;
}

// get nth recent reply in a reply list 
static reply *getNthRecentreply(reply *head, int n) {
    if (!head || n <= 0) return NULL;
    int cnt = 0;
    reply *cur = head;
    while (cur) { cnt++; cur = cur->next; }
    int idxFromStart = cnt - n + 1;
    if (idxFromStart <= 0) return NULL;
    cur = head;
    int idx = 1;
    while (cur && idx < idxFromStart) { cur = cur->next; idx++; }
    return cur;
}

// Delete mth recent reply for nth recent comment 
bool deletereply(int n_comment, int m_reply) {
    if (!platform) return false;
    post *p = currpost();
    if (!p || !p->comments) return false;
    comment *c = getNthRecentcomment(p->comments, n_comment);
    if (!c || !c->replies) return false;
    reply *head = c->replies;
    reply *target = getNthRecentreply(head, m_reply);
    if (!target) return false;
    if (target == head) {
        c->replies = head->next;
        if (target->username) free(target->username);
        if (target->content) free(target->content);
        free(target);
        return true;
    }
    reply *prev = head;
    while (prev->next && prev->next != target) prev = prev->next;
    if (prev->next == target) {
        prev->next = target->next;
        if (target->username) free(target->username);
        if (target->content) free(target->content);
        free(target);
        return true;
    }
    return false;
}
