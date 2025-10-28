#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "post.h"

post *createpost(const char *username, const char *caption) {
    if (!username || !caption) return NULL;
    post *p = (post *)malloc(sizeof(post));
    if (!p) return NULL;
    p->username = strdup(username);
    p->caption = strdup(caption);
    p->comments = NULL;
    p->prev = p->next = NULL;
    return p;
}

void freepostshallow(post *p) {
    if (!p) return;
    if (p->username) free(p->username);
    if (p->caption) free(p->caption);
    free(p);
}
