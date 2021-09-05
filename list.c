/**
 * CS 2110 - Fall 2019 - Project 5
 *
 * @author Elton Pinto
 *
 * list.c: Complete the functions!
 */

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!-IMPORTANT-!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * For any function that must use malloc, if malloc returns NULL, the function
 * itself should return NULL if needs to return something (or return 0 if
 * the function returns a int).
 */

// Do not add ANY additional includes!!!
#include "list.h"
// This is used so that you guys can print stuff out
#include <stdio.h>

/* You should NOT have any global variables. */


/** create_process
 *
 * Helper function that creates a struct process by allocating memory for it on the heap
 *
 * If malloc returns NULL, you should return NULL to indicate failure
 *
 * @param name A pointer to char that points to the name of the process
 * ( be sure to malloc space for the string this points to and
 * copy this string over to the malloc-ed space in the process you return )
 * @param priority The priority of the process
 * @param pid The pid (process id) of the process
 * @return a struct process
 */
Process *create_process(char *name, int priority) {
    static int total_pid = -1;
    Process *result = (Process *) malloc(sizeof(Process));
    if (!result) return NULL;
    char *namecpy = (char *) malloc((strlen(name) + 1) * sizeof(char));
    if (!namecpy) {
        free(result);
        return NULL;
    }
    strcpy(namecpy, name);
    result->name = namecpy;
    result->priority = priority;
    total_pid++;
    result->pid = total_pid;
    return result;
}


/** create_node
 *
 * Helper function that creates a struct list_node by allocating memory for it on the heap.
 * Be sure to initialize its next pointer to NULL and prev pointer to NULL.
 *
 * If malloc returns NULL, you should return NULL to indicate failure.
 *
 * @param data a void pointer to data the user wants to store in the struct list
 * @return a struct list_node
 */
ListNode *create_node(Process *data) {
    ListNode *result = (ListNode *) malloc(sizeof(ListNode));
    if(!result) return NULL;
    result->next = NULL;
    result->prev = NULL;
    result->data = data;
    return result;
}


/** create_list
 *
 * Creates a struct list by allocating memory for it on the heap.
 * Be sure to initialize head to NULL.
 *
 * If malloc returns NULL, you should return NULL to indicate failure.
 *
 * @return a pointer to a new struct list or NULL on failure
 */
List *create_list(void)
{
    List *result = (List *) malloc(sizeof(List));
    if (!result) return NULL;
    result->head = NULL;
    return result;
}


/** push_front
 *
 * Adds a ListNode to the front of the list
 *
 * @param list a pointer to the List structure
 * @param proc The process to add to the list
 * @return
 *      0, if
 *          - the list is NULL
 *          - any other failure in the function
 *      1, otherwise
 *
 * Note: You must add the process to the list even if it is NULL
 */
int push_front(List *list, Process *node) {
    if (list == 0) return 0;
    ListNode *new_node = create_node(node);
    if (new_node == NULL) return 0;
    new_node->next = list->head;
    list->head = new_node;
    return 1;
}


/** push_back
 *
 * Adds the ListNode to the back of the list
 *
 * @param list a pointer to the List structure
 * @param proc The process to add to the list
 * @return
 *      0, if
 *          - the list is NULL
 *          - any other failure in the function
 *      1, otherwise
 *
 * Note: You must add the process to the list even if it is NULL
 */
int push_back(List *list, Process *node) {
    if (list == NULL) return 0;
    ListNode *new_node = create_node(node);
    
    if (new_node == NULL){ 
        return 0;
    } else if (list->head == NULL) {
        list->head = new_node;
        return 1;
    }
    ListNode *curr = list->head;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = new_node;
    new_node->prev = curr;
    return 1;
}

/** remove_node
  *
  * Remove the first node with the passed in `pid`
  *
  * The node should be removed completely, but the node's data should be returned through the `dataOut` parameter
  *
  * @param list a pointer to the struct list structure
  * @param dataOut a double pointer to return the Process data that is being removed from the list
  * @param pid The pid of the process that you want to remove
  * @return
  *     0, if
  *         - the struct list is NULL
  *         - there is no process with the specified pid
  *         - the dataOut is NULL
  *     1, otherwise
  */
int remove_node(List *list, Process **dataOut, int pid) {
    if (list == 0|| dataOut == 0) return 0;
    ListNode *curr = list->head;
    while (curr != NULL) {
        if (curr->data->pid == pid) {
            if (curr == list->head) {
                *dataOut = curr->data;
                list->head = curr->next;
                free(curr);
            } else {
                *dataOut = curr->data;
                curr->prev->next = curr->next;
                if (curr->next != NULL) {
                    curr->next->prev = curr->prev;
                }
                free(curr);
            }
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

/** destroy_process
 *
 * Destroys all data in the process. Make sure to free any process attributes that have memory allocated for them.
 *
 * @param proc The process to destroy
*/
void destroy_process(Process *proc) {
    if (proc == NULL) return;
    if (proc->name != NULL) {
        free(proc->name);
    }
    free(proc);    
    return;
}

/** destroy
  *
  * Destroys the list. This frees the list, nodes within the list, and data within the nodes
  * (`destroy_proc` can help with free'ing the data within the nodes).
  *
  * Make sure to check that the list is not NULL
  *
  * @param list A pointer to the struct list structure
  */
void destroy(List *list) {
    if (list == 0) return;
    ListNode *curr = list->head;
    ListNode *del = curr;
    while (curr != NULL) {
        del = curr;
        curr = curr->next;
        destroy_process(del->data);
        free(del);
    }
    free(list);
    return;
}

/** copy_processs
 *
 * A function that will **deep** copy the process and all its attributes.
 *
 * For any malloc failures, you must not leak memory. This means that
 * if any memory failures occur, before you return 0, you must go back
 * and free any data that you allocated memory for.
 *
 *
 * @param from The process to copy
 * @param to The address of the pointer that will point to the copy
 *           of the process
 * @return
 *      0, if
 *          - malloc fails
 *          - from is NULL
 *          - to is NULL
 *      1, otherwise
 */
int copy_process(Process *from, Process **to) {
    if (from == NULL || to == NULL) return 0;
    Process *result = (Process *) malloc(sizeof(Process));
    if (!result) return 0;
    char *namecpy = (char *) malloc((strlen(from->name) + 1) * sizeof(char));
    if (!namecpy) {
        free(result);
        return 0;
    }
    strcpy(namecpy, from->name);
    result->name = namecpy;
    result->priority = from->priority;
    result->pid = from->pid;
    *to = result;
    return 1;
}

/** copy_list
 *
 * Create a new list structure, new nodes, and new deep copies of the data
 * (use the copy_process function for this).
 *
 * If listToCopy is NULL or empty return NULL. For memory allocations
 * failures, including if copy_process returns 0 (indicating a memory
 * allocation failure), your code must not leak memory. This means that if any
 * memory allocation failures occur, before you return NULL, you must go back
 * and free all data in the new list, nodes in the new list, and
 * the new list itself (you may find the destroy function useful for
 * this).
 *
 *
 * @param listToCopy A pointer to the struct list structure to make a copy of
 * @return The list structure created by copying the old one, or NULL on
 *         failure (includes any of the parameters being null)
 */
List *copy_list(List *listToCopy)
{
    if (listToCopy == NULL || listToCopy->head == NULL) return NULL;
    List *result = create_list();
    if (!result) return NULL;
    ListNode *nodeToCopy = listToCopy->head;
    ListNode *curr;
    ListNode *prev;
    Process **procCop = malloc(sizeof(Process **));
    if (!procCop) {
        destroy(result);
        free(procCop);
        return NULL;
    }
    int isHead = 1;
    while (nodeToCopy != NULL) {
        if (!copy_process(nodeToCopy->data, procCop)) {
            destroy(result);
            free(procCop);
            return NULL;
        }
        curr = create_node(*procCop);
        if (!curr) {
            destroy_process(*procCop);
            destroy(result);
            free(procCop);
            return NULL;
        }
        if (!isHead) {
            curr->prev = prev;
            prev->next = curr;
        } else {
            result->head = curr;
            isHead = 0;
        }
        nodeToCopy = nodeToCopy->next;
        prev = curr;
    }
    free(procCop);
    return result;
}

/** compare_process
 *
 * This method compares the pids of two processes
 * A null process has a larger pid than a non-null process
 *
 * @param a A process
 * @param b Another process
 * @return >0 if process a's pid > process b's pid
 *          0 if process a's pid = process b's pid
 *         <0 if process a's pid < process b's pid
 **/
// TODO doc
int compare_pid(Process *a, Process *b) {  
    if (a == NULL && b == NULL) {
        return 0;
    } else if ((a == NULL && b != NULL) || (a != NULL && b == NULL)) {
        return (a == NULL ? 1 : -1);
    } else {
        return (a->pid - b->pid);
    }
}

/** compare_name
 *
 * This method compares the name of two processes
 * A null process has a larger name than a non-null process
 * (Hint) Take a look at the strcmp man page
 *
 * @param a A process
 * @param b Another process
 * @return >0 if process a's name > process b's name
 *          0 if process a's name = process b's name
 *         <0 if process a's name < process b's name
 **/
int compare_name(Process *a, Process *b) {
    if (a == NULL && b == NULL) {
        return 0;
    } else if ((a == NULL && b != NULL) || (a != NULL && b == NULL)) {
        return (a == NULL ? 1 : -1);
    } else {
        return strcmp(a->name, b->name);
    }
}

/** swap_nodes
 *
 * This method swaps two nodes in place. For example if our list was
 * A -> B -> C -> D and we called swap_nodes(b, d, list), the resulting list would be
 * A -> D -> C -> B
 * Note: The nodes will either be NULL or will exist in the list
 *
 * @param node_a A node to swap
 * @param node_b The other node to swap
 * @param list The list within which the nodes will be swapped
 * @return 0, if
 *              - any of the parameters are null
 *              - node_a = node_b
 *         1, otherwise
**/
int swap_nodes(ListNode *node_a, ListNode *node_b, List *list) {
    if (node_a == NULL || node_b == NULL || list == NULL) return 0;
    if (node_a == node_b) return 0;
    //if the nodes are right next to each other
    if (node_a->next == node_b) {
        node_a->next = node_b->next;
        node_b->prev = node_a->prev;
        node_a->prev = node_b;
        node_b->next = node_a;
        if (node_a->next) {
            node_a->next->prev = node_a;
        }
        if (node_b->prev) {
            node_b->prev->next = node_b;
        } else {
            list->head = node_b;
        }
        return 1;
    } else if (node_b->next == node_a) {
        node_b->next = node_a->next;
        node_a->prev = node_b->prev;
        node_b->prev = node_a;
        node_a->next = node_b;
        if (node_b->next) {
            node_b->next->prev = node_b;
        }
        if (node_a->prev) {
            node_a->prev->next = node_a;
        } else {
            list->head = node_a;
        }
        return 1;
    }

    //otherwise I make a temp node to help with the swap
    ListNode *a_temp = node_a->prev;
    ListNode *b_temp = node_b->prev;
    if (!a_temp) {
        list->head = node_b;
    } else {
        a_temp->next = node_b;
    }
    if (!b_temp) {
        list->head = node_a;
    } else {
        b_temp->next = node_a;
    }

    node_a->next = node_b->next;
    node_b->prev = node_a->prev;
    node_a->next->prev = node_b;
    node_a->prev = node_b;


    
    return 1;
}

/** sort
 *
 * A function to sort the nodes in ascending order (in place) using the comparator function passed in.
 * Recommend using bubble sort (Hint: Utilize the swap_nodes() function above)
 *
 * @param list The list to sort
 * @param compare_func A function pointer that denotes which compare method (written above) will be utilized.
 *                     Assume that the return value of the function pointer will be the return value of the comparator methods
 *                     written above
 * @return
 *      0, if
 *          - list is null
 *          - the list is empty
 *          - compare_func is null
 *      1, otherwise
 */
int sort(List *list, int (*compare_func)(Process *a, Process *b)) {
    if (list == NULL || list->head == NULL || compare_func == NULL) return 0;
    int swapped = 0;
    ListNode *curr = list->head;
    do {
        swapped = 0;
        curr = list->head;
        while (curr->next != NULL) {
            if ((*compare_func)(curr->data, curr->next->data) > 0) {
                swap_nodes(curr, curr->next, list);
                swapped = 1;
            }
            curr = curr->next;
        }

    } while (swapped);
    return 1;
}

/** list_to_array
 *
 * This function will create and utilize a new structure to represent our processes.
 * First, create an array of process pointers, one for each process in the linked list
 * Next, iterate through the linked list, setting the process pointer to its new home in the array
 * and removing the linked list structure (Hint: You want to free the linked list but not the process itself)
 * Finally, return the process array
 *
 * @param list The linked list to be converted to an array
 * @param size The number of processes in the linked list
 * @return
 *      NULL, if
 *              - the list is null
 *              - the list is empty
 *              - there is a malloc failure
 *      An array to process pointers, otherwise
 */
Process **list_to_array(List *list, int size) {
    if (list == NULL || size <= 0) return NULL;
    Process **proc_arr = malloc(sizeof(Process *) * size);
    if (!proc_arr || list->head == NULL) return NULL;
    ListNode *curr = list->head;
    for (int i = 0; i < size; i++) {
        if (curr != NULL) {
            *(proc_arr + i) = curr->data;
            curr = curr->next;
            if (curr->prev != NULL) {
                free(curr->prev);
            }
        }
    }
    free(curr);
    free(list);
    return proc_arr;
}


/** make_idle
 *
 * Append the string " (idle)" to the end of the name
 * of the process passed in.
 *
 * If the string " (idle)" already exists, do not include it
 *
 * You **must** use `realloc`.
 * You **can** use any functions in <string.h>
 *
 * @param proc A pointer to a Process struct
 * @return
 *      0, if
 *          - proc is NULL
 *          - the name of the proc is NULL
 *          - any other failure (for example, if `realloc` fails)
 *      1, otherwise
 */
int make_idle(Process *proc) {
    if (proc == NULL || proc->name == NULL) return 0;
    if (!strstr(proc->name, " (idle)")) {
        char *idle_name = realloc(proc->name, strlen(proc->name) + 8);
        if (!idle_name) return 0;
        strncpy(idle_name, proc->name, strlen(proc->name));
        strncat(idle_name, " (idle)", 7);
        free(proc->name);
        proc->name = idle_name;
    }
    return 1;
}

/** make_active
 *
 * Remove the string " (idle)" from the end of the name of
 * of the process passed in, if it exists
 *
 * You **must** use `realloc`.
 * You **can** use any functions in <string.h>
 *
 * @param proc A pointer to a Process struct
 * @return
 *      0, if
 *          - proc is NULL
 *          - the name of the proc is NULL
 *          - any other failure (for example, if `realloc` fails)
 *      1, otherwise
 */
int make_active(Process *proc) {
    if (proc == NULL || proc->name == NULL) return 0;
    char *idle = strstr(proc->name, " (idle)");
    if (idle) {
        *idle = 0;
    }
    char *new_name = realloc(proc->name, strlen(proc->name) + 1);
    if (!new_name) {
        return 0;
    }
    if (!strncpy(new_name, proc->name, strlen(proc->name))) return 0;
    free(proc->name);
    proc->name = new_name;
    return 1;
}

/** apply
 *
 * Invokes `func` on each process of the linked list
 *
 * @param list A pointer to the list structure
 * @param func A pointer to the function to be invoked on each node
 * @return
 *      0, if
 *          - list is NULL
 *          - func is NULL
 *      1, otherwise
 */
int map_inplace(List *list, int (*map_func)(Process *)) {
    if (list == NULL || map_func == NULL) return 0;
    ListNode *curr = list->head;
    while (curr != NULL) {
        (*map_func)(curr->data);
        curr = curr->next;
    }
    return 1;
}
