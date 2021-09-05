/**
 * CS 2110 - Fall 2019 - Project 5
 *
 * Do not modify this file!
 *
 * struct list.h
 */

#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <string.h>

// This is just to remove compiler warnings associated with unused variables.
// Delete calls to this as you implement functions.
#define UNUSED_PARAMETER(x) (void)(x)

/**************************
 ** Datatype definitions **
 **************************/

// Process definitions

typedef struct process {
    char *name;
    int priority;
    int pid;
} Process;

typedef struct list_node {
    struct list_node *next;
    struct list_node * prev;
    Process *data;
} ListNode;

typedef struct list {
    ListNode *head;
} List;


/***************************************************
** Prototypes for struct list library functions.  **
**                                                **
** For more details on their functionality,       **
** check list.c.                                  **
***************************************************/

ListNode *create_node(Process *);
Process *create_process(char *, int);
List *create_list(void);
int push_front(List *, Process *);
int push_back(List *, Process *);
int remove_node(List *, Process **, int);
void destroy_process(Process *);
void destroy(List *);
int copy_process(Process *, Process **);
List *copy_list(List *);
int compare_pid(Process *, Process *);
int compare_name(Process *, Process *);
int swap_nodes(ListNode *, ListNode *, List *);
int sort(List *, int (*compare_func)(Process *a, Process *b));
int make_idle(Process*);
int make_active(Process*);
int map_inplace(List *, int (*map_func)(Process *));
Process **list_to_array(List*, int);

#endif
