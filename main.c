#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "platform.h"

char *readLine(void) {
    size_t size = 256;
    char *buf = malloc(size);
    if (!buf) return NULL;
    if (!fgets(buf, (int)size, stdin)) { free(buf); return NULL; }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
    return buf;
}

void printpost(post *p) {
    if (!p) {
        printf("post does not exist.\n");
        return;
    }
    printf("%s - %s\n", p->username, p->caption);
}

void printcomments(comment *head) {
    if (!head) {
        printf("(no comments)\n");
        return;
    }
    comment *c = head;
    int cnum = 1;
    while (c) {
        printf("%d) %s: %s\n", cnum, c->username, c->content);
        reply *r = c->replies;
        while (r) {
            printf("   -> %s: %s\n", r->username, r->content);
            r = r->next;
        }
        c = c->next;
        cnum++;
    }
}

int main(void) {
    createplatform();
    printf("platform created successfully.\n");
    printf("Type 'help' for commands, 'exit' to quit.\n");

    while (1) {
        printf(">> ");
        char *cmd = readLine();
        if (!cmd) break;

        // Exit
        if (strcmp(cmd, "exit") == 0) {
            free(cmd);
            break;
        }

        // Help menu
        else if (strcmp(cmd, "help") == 0) {
            printf("Available Commands:\n");
            printf(" add post\n delete post\n view post\n");
            printf(" current post\n next post\n previous post\n");
            printf(" add comment\n delete comment\n view comments\n");
            printf(" add reply\n delete reply\n");
            printf(" exit\n");
        }

        else if (strcmp(cmd, "add post") == 0) {
            printf("Enter username: ");
            char *username = readLine();
            printf("Enter caption: ");
            char *caption = readLine();
            if (addpost(username, caption)) {
                printf("post added successfully.\n");
                post *p = currpost();
                printpost(p);
            } else printf("Failed to add post.\n");
            free(username);
            free(caption);
        }

        else if (strcmp(cmd, "delete post") == 0) {
            printf("Enter post number to delete: ");
            char *nstr = readLine();
            int n = atoi(nstr);
            if (deletepost(n))
                printf("post #%d deleted.\n", n);
            else
                printf("post does not exist.\n");
            free(nstr);
        }

        else if (strcmp(cmd, "view post") == 0) {
            printf("Enter post number to view: ");
            char *nstr = readLine();
            int n = atoi(nstr);
            post *p = viewpost(n);
            if (p) {
                printpost(p);
                printf("comments:\n");
                printcomments(p->comments);
            } else printf("post does not exist.\n");
            free(nstr);
        }

        else if (strcmp(cmd, "current post") == 0) {
            post *p = currpost();
            if (p) {
                printpost(p);
                printf("comments:\n");
                printcomments(p->comments);
            } else printf("No posts yet.\n");
        }

        else if (strcmp(cmd, "previous post") == 0) {
            post *p = previouspost();
            if (p) {
                printpost(p);
                printf("comments:\n");
                printcomments(p->comments);
            } else printf("previous post does not exist.\n");
        }

        else if (strcmp(cmd, "next post") == 0) {
            post *p = nextpost();
            if (p) {
                printpost(p);
                printf("comments:\n");
                printcomments(p->comments);
            } else printf("Next post does not exist.\n");
        }

        else if (strcmp(cmd, "add comment") == 0) {
            printf("Enter your username: ");
            char *username = readLine();
            printf("Enter comment content: ");
            char *content = readLine();
            if (addcomment(username, content)) {
                printf("comment added by %s: %s\n", username, content);
                printf("Updated comments:\n");
                printcomments(viewcomments());
            } else printf("Failed to add comment.\n");
            free(username);
            free(content);
        }

        else if (strcmp(cmd, "delete comment") == 0) {
            printf("Enter comment number to delete: ");
            char *nstr = readLine();
            int n = atoi(nstr);
            if (deletecomment(n)) {
                printf("comment #%d deleted.\n", n);
                printf("Remaining comments:\n");
                printcomments(viewcomments());
            } else printf("comment does not exist.\n");
            free(nstr);
        }

        else if (strcmp(cmd, "view comments") == 0 || strcmp(cmd, "view all comments") == 0) {
            printf("All comments on current post:\n");
            printcomments(viewcomments());
        }

        else if (strcmp(cmd, "add reply") == 0) {
            printf("Enter your username: ");
            char *username = readLine();
            printf("Enter reply content: ");
            char *content = readLine();
            printf("Enter comment number to reply to: ");
            char *nstr = readLine();
            int n = atoi(nstr);
            if (addreply(username, content, n)) {
                printf("reply added by %s to comment #%d.\n", username, n);
                printf("Updated comments:\n");
                printcomments(viewcomments());
            } else printf("Failed to add reply.\n");
            free(username);
            free(content);
            free(nstr);
        }

        else if (strcmp(cmd, "delete reply") == 0) {
            printf("Enter comment number: ");
            char *nstr = readLine();
            printf("Enter reply number: ");
            char *mstr = readLine();
            int n = atoi(nstr), m = atoi(mstr);
            if (deletereply(n, m)) {
                printf("reply #%d from comment #%d deleted.\n", m, n);
                printf("Updated comments:\n");
                printcomments(viewcomments());
            } else printf("reply does not exist.\n");
            free(nstr);
            free(mstr);
        }

        else {
            printf("Unknown command. Type 'help' for list.\n");
        }

        free(cmd);
    }

    printf("Exiting platform\n");
    return 0;
}
