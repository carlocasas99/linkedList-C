// Project 5 - Fall 2019
// Maddie Brickell
//
// Warning: much the structure of this file is shamelessly copypasted from
// https://libcheck.github.io/check/doc/check_html/check_3.html

#include <stdio.h>
#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "test_utils.h"


// The number of mallocs in user code until the next one fails, or -1 for
// successful mallocs
int mallocs_until_fail = -1;

static void _lists_exactly_equivalent(List* a, FakeNode* wrapper) {

    ListNode* curr_a = a -> head;
    FakeNode* wrapper_node = wrapper;

    while(wrapper_node) {
        //ck_assert_ptr_eq(curr_a, wrapper_node -> node);
        ck_assert_ptr_eq(curr_a -> data, wrapper_node -> node -> data);
        if (wrapper_node -> node -> data) {
            ck_assert_int_eq(curr_a -> data -> pid, wrapper_node -> node -> data -> pid);
            ck_assert_str_eq(curr_a -> data -> name, wrapper_node -> node -> data -> name);
            ck_assert_int_eq(curr_a -> data -> priority, wrapper_node -> node -> data -> priority);
        }


        curr_a = curr_a -> next;

        FakeNode* temp = wrapper_node -> next;
        free(wrapper_node);
        wrapper_node = temp;
    }
}

static void reset_mallocs_until_fail(void) {
    mallocs_until_fail = -1;
}

static List *_create_baby_list(void) {
    List* baby_list = _create_list();
    _push_front(baby_list, _process_create("init", 99, 0));
    return baby_list;
}

static List *_create_longish_list(void) {
    List *longishLisst = _create_list();
    _push_front(longishLisst, _process_create("init", 100, 0));
    _push_front(longishLisst, _process_create("python", 50, 1));
    _push_front(longishLisst, _process_create("vim", 1, 2));
    return longishLisst;
}

/******************************************************************************/
/*************************** create_process() tests ******************************/
/******************************************************************************/
START_TEST(test_list_create_process_basic) {
    Process *p1 = create_process("init", 99);
    ck_assert(p1);
    ck_assert(p1->name);
    Process *p2 = create_process("python", 99);
    ck_assert_msg(p2->pid - p1->pid == 1, "PID assigned badly");
    _free_process(p2);
    _free_process(p1);
}
END_TEST

START_TEST(test_list_create_process_malloc_failure) {
    mallocs_until_fail = 0;
    Process *l1 = create_process("init", 300);
    ck_assert(!l1);
}
END_TEST
/******************************************************************************/
/*************************** create_list() tests ******************************/
/******************************************************************************/
START_TEST(test_list_create_list_basic) {
    struct list *l1 = create_list();
    ck_assert(l1);
    ck_assert(!l1->head);
    free(l1);
}
END_TEST

START_TEST(test_list_create_list_malloc_failure) {
    mallocs_until_fail = 0;
    struct list *l1 = create_list();
    ck_assert(!l1);
}
END_TEST


/******************************************************************************/
/*************************** create_node() tests ******************************/
/******************************************************************************/
START_TEST(test_list_create_node_basic) {
    Process* process = _process_create("emacs", 100, 0);
    ListNode* node = create_node(process);
    ck_assert(node);
    ck_assert(!node -> next);
    ck_assert_str_eq(node -> data -> name, process -> name);
    ck_assert_int_eq(node -> data -> pid, process -> pid);
    _free_process(process);
    free(node);
}
END_TEST

START_TEST(test_list_create_node_malloc_failure) {
    Process* processby = _process_create("code", 99, 0);
    mallocs_until_fail = 0;
    ListNode* node = create_node(processby);
    ck_assert(!node);
    _free_process(processby);
}
END_TEST

/******************************************************************************/
/************************** push_front() tests ***************************************/
/******************************************************************************/
START_TEST(test_list_push_front_null) {
    // both head and node null
    ck_assert_msg(!push_front(NULL, NULL), "push_front() incorrect behavious on NULL input");

    // node is null
    List *l1 = _create_list();
    ck_assert_msg(push_front(l1, NULL), "push_front() incorrect behavious on NULL input");

    ListNode *ln1 = _create_node(NULL);
    Process *proc1 = _process_create("init", 3, 0);
    // head null but data not null
    ck_assert_msg(!push_front(NULL, proc1), "push_front() incorrect behavious on NULL input");

    free(ln1);
    _free_process(proc1);
    _free_list_nodes_and_all_data(l1);

}
END_TEST

START_TEST(test_list_push_front) {

    List* main_list = _create_list();
    Process* proc_1 = _process_create("init", 3, 0);
    Process* proc_2 = _process_create("python", 2, 1);
    Process* proc_3 = _process_create("vim", 10, 2);
    ListNode* node_1 = _create_node(proc_1);
    ListNode* node_2 = _create_node(proc_2);
    ListNode* node_3 = _create_node(proc_3);
    ListNode* null_node = _create_node(NULL);

    FakeNode* test1 = _create_fake_list_with_processs(1, node_1);
    push_front(main_list, proc_1);
    ck_assert(main_list);
    _lists_exactly_equivalent(main_list, test1);

    FakeNode* test2 = _create_fake_list_with_processs(2, node_2, node_1);
    push_front(main_list, proc_2);
    ck_assert(main_list);
    _lists_exactly_equivalent(main_list, test2);

    FakeNode* test3 = _create_fake_list_with_processs(3, node_3, node_2, node_1);
    push_front(main_list, proc_3);
    ck_assert(main_list);
    _lists_exactly_equivalent(main_list, test3);

    FakeNode* nulldatatest = _create_fake_list_with_processs(4, null_node, node_3, node_2, node_1);
    push_front(main_list, NULL);
    ck_assert(main_list);
    _lists_exactly_equivalent(main_list, nulldatatest);


    

    _free_list_nodes_and_all_data(main_list);
    free(node_1);
    free(node_2);
    free(node_3);
    free(null_node);
}
END_TEST

/******************************************************************************/
/************************** push_back() tests ***************************************/
/******************************************************************************/
START_TEST(test_list_push_back_null) {
    // both head and node null
    ck_assert_msg(!push_back(NULL, NULL), "push_back() incorrect behavious on NULL input");

    // node is null
    List *l1 = _create_list();
    ck_assert_msg(push_back(l1, NULL), "push_back() incorrect behavious on NULL input");

    Process *proc1 = _process_create("init", 3, 0);
    ListNode *ln1 = _create_node(NULL);

    // head null but data not null
    ck_assert_msg(!push_back(NULL, proc1), "push_back() incorrect behavious on NULL input");

    free(ln1);
    _free_process(proc1);
    _free_list_nodes_and_all_data(l1);

}
END_TEST

START_TEST(test_list_push_back) {

    List* main_list = _create_list();
    Process* proc_1 = _process_create("init", 3, 0);
    Process* proc_2 = _process_create("python", 2, 1);
    Process* proc_3 = _process_create("vim", 10, 2);
    ListNode* node_1 = _create_node(proc_1);
    ListNode* node_2 = _create_node(proc_2);
    ListNode* node_3 = _create_node(proc_3);
    ListNode* null_node = _create_node(NULL);

    FakeNode* test1 = _create_fake_list_with_processs(1, node_1);
    push_back(main_list, proc_1);
    ck_assert(main_list);
    _lists_exactly_equivalent(main_list, test1);

    FakeNode* test2 = _create_fake_list_with_processs(2, node_1, node_2);
    push_back(main_list, proc_2);
    ck_assert(main_list);
    _lists_exactly_equivalent(main_list, test2);

    FakeNode* test3 = _create_fake_list_with_processs(3, node_1, node_2, node_3);
    push_back(main_list, proc_3);
    ck_assert(main_list);
    _lists_exactly_equivalent(main_list, test3);

    FakeNode* nulldatatest = _create_fake_list_with_processs(4, node_1, node_2, node_3, null_node);
    push_back(main_list, NULL);
    ck_assert(main_list);
    _lists_exactly_equivalent(main_list, nulldatatest);

    _free_list_nodes_and_all_data(main_list);
    free(node_1);
    free(node_2);
    free(node_3);
    free(null_node);
}
END_TEST

/******************************************************************************/
/**************************** remove_node tests *****************************/
/******************************************************************************/
START_TEST(test_list_remove_node_null) {
    Process *dataOut = (Process *) 0xDEADBEEFU;
    ck_assert(!remove_node(NULL, &dataOut, 0));

    List* l = _create_longish_list();
    ck_assert(!remove_node(l, NULL, 0));

    _free_list_nodes_and_all_data(l);
}
END_TEST


START_TEST(test_list_remove_node) {
    ListNode* node_1 = _create_node(_process_create("init", 2, 3));
    ListNode* node_2 = _create_node(_process_create("vim", 4, 50));
    ListNode* node_3 = _create_node(_process_create("emacs", 5, 10));

    List* main_list = _create_list_with_processs(3, node_1, node_2, node_3);
    Process *dataOut = NULL;

    // First off, we remove the node in the middle
    int result = remove_node(main_list, &dataOut, 50);
    ck_assert(result);
    ck_assert(dataOut);
    ck_assert_str_eq(dataOut -> name, "vim");
    _free_process(dataOut);
    FakeNode* remove_middle = _create_fake_list_with_processs(2, node_1, node_3);
    _lists_exactly_equivalent(main_list, remove_middle);

    // Next we remove the node at the end
    result = remove_node(main_list, &dataOut, 10);
    ck_assert(result);
    ck_assert(dataOut);
    ck_assert_str_eq(dataOut -> name, "emacs");
    _free_process(dataOut);
    FakeNode* remove_end = _create_fake_list_with_processs(1, node_1);
    _lists_exactly_equivalent(main_list, remove_end);

    // Finally we remove the front of the list
    result = remove_node(main_list, &dataOut, 3);
    ck_assert(result);
    ck_assert(dataOut);
    ck_assert_str_eq(dataOut -> name, "init");
    _free_process(dataOut);
    ck_assert(!main_list -> head);

    free(main_list);
}
END_TEST

///******************************************************************************/
///************************** copy_process() tests ********************************/
///******************************************************************************/
START_TEST(test_list_copy_process_null)
{
    ck_assert(!(copy_process(NULL, NULL)));

    Process* processby = malloc(sizeof(Process));
    ck_assert(!copy_process(NULL, &processby));

    ck_assert(!copy_process(processby, NULL));

    free(processby);
}
END_TEST

START_TEST(test_list_copy_process_malloc_failure)
{
    Process* processby = _process_create("init", 10, 0);
    Process *dataOut = (Process *) 0xDEADBEEFU;

    // checks for one malloc failure
    mallocs_until_fail = 0;
    ck_assert(!copy_process(processby, &dataOut));

    // checks for another malloc failure
    mallocs_until_fail = 1;
    ck_assert(!copy_process(processby, &dataOut));

    _free_process(processby);
}
END_TEST

START_TEST(test_list_copy_process)
{
    Process* processby = _process_create("init", 22, 0);
    Process *dataOut = (Process *) 0xDEADBEEFU;

    ck_assert(copy_process(processby, &dataOut));

    // Make sure that dataOut has the proper data
    ck_assert(dataOut);
    ck_assert_str_eq(dataOut -> name, "init");
    ck_assert_int_eq(dataOut -> pid, 0);
    ck_assert_int_eq(dataOut -> priority, 22);

    // Make sure that processby and dataOut aren't the same addresses
    ck_assert_ptr_ne(processby, dataOut);
    ck_assert_ptr_ne(processby -> name, dataOut -> name);

    _free_process(processby);
    _free_process(dataOut);
}
END_TEST

///******************************************************************************/
///************************** copy_list() tests ********************************/
///******************************************************************************/
START_TEST(test_list_copy_null_list) {
    // check if list is NULL
    ck_assert(!copy_list(NULL));

    // check if there is a list, but nothing in it :(
    List* new_list = malloc(sizeof(List));
    new_list -> head = NULL;
    ck_assert(!copy_list(NULL));

    free(new_list);
}
END_TEST

START_TEST(test_list_copy_list_malloc_failure) {
    List* l = _create_longish_list();

    // Lots of places for malloc to fail
    mallocs_until_fail = 0;
    ck_assert(!(copy_list(l)));

    mallocs_until_fail = 1;
    ck_assert(!(copy_list(l)));

    mallocs_until_fail = 2;
    ck_assert(!(copy_list(l)));

    _free_list_nodes_and_all_data(l);
}
END_TEST

START_TEST(test_list_copy_list_basic) {
    List* lil_list = _create_baby_list();
    List* copy = copy_list(lil_list);

    ck_assert(copy);
    ck_assert(copy -> head);
    ck_assert(copy -> head -> data);

    // make sure the data is the same
    ck_assert_int_eq(lil_list -> head -> data -> pid, copy -> head -> data -> pid);
    ck_assert_str_eq(lil_list -> head -> data -> name, copy -> head -> data -> name);

    // make sure the pointers are not the same
    // list pointers
    ck_assert_ptr_ne(lil_list, copy);
    // list node pointers
    ck_assert_ptr_ne(lil_list -> head, copy -> head);
    // list process pointers
    ck_assert_ptr_ne(lil_list -> head -> data, copy -> head -> data);
    // process names
    ck_assert_ptr_ne(lil_list -> head -> data -> name, copy -> head -> data -> name);

    // For students, ensure that the last node is null or you will get weird errors
    ck_assert(!lil_list -> head -> next);

    _free_list_nodes_and_all_data(lil_list);
    _free_list_nodes_and_all_data(copy);
}
END_TEST

START_TEST(test_list_copy_list_hard) {
    List* l = _create_longish_list();
    List* copy = copy_list(l);

    ck_assert_ptr_ne(l, copy);

    ck_assert(copy);
    ck_assert(copy -> head);
    ck_assert(copy -> head -> data);

    ListNode* curr = l -> head;
    ListNode* copy_curr = copy -> head;

    while(curr) {
        // assert a copy exists
        ck_assert(copy_curr);
        // assert data is correct
        ck_assert_int_eq(curr -> data -> pid, copy_curr -> data -> pid);
        ck_assert_int_eq(curr -> data -> priority, copy_curr -> data -> priority);
        ck_assert_str_eq(curr -> data -> name, copy_curr -> data -> name);

        // assert pointers are different
        ck_assert_ptr_ne(curr, copy_curr);
        ck_assert_ptr_ne(curr -> data, copy_curr -> data);
        ck_assert_ptr_ne(curr -> data -> name, copy_curr -> data -> name);

        copy_curr = copy_curr -> next;
        curr = curr -> next;
    }

    // Checks to make sure you set the last node's next pointer to null
    ck_assert(!copy_curr);
    _free_list_nodes_and_all_data(l);
    _free_list_nodes_and_all_data(copy);
}
END_TEST

///******************************************************************************/
///************************** destroy_process() tests *******************************/
///******************************************************************************/
START_TEST(test_list_destroy_process)
{
    destroy_process(NULL);

    // We're just going to let valgrind take care of these tests :)
    Process* processby = _process_create("init", 9, 0);
    destroy_process(processby);
}
END_TEST


/******************************************************************************/
/************************** destroy() tests ********************************/
/******************************************************************************/
START_TEST(test_list_destroy_NULL_list) {
    // Success if there is no error
    destroy(NULL);
}
END_TEST

// For all the tests below, we are relying on valgrind here
START_TEST(test_list_destroy_empty) {
    List* l = malloc(sizeof(List));
    l -> head = NULL;
    destroy(l);
}
END_TEST

START_TEST(test_list_destroy_baby) {
    List* l = _create_baby_list();
    destroy(l);
}
END_TEST

START_TEST(test_list_destroy_longer_list) {
    List* l = _create_longish_list();
    destroy(l);
}
END_TEST

///******************************************************************************/
///************************** compare_pid() tests ********************************/
///******************************************************************************/
START_TEST(test_list_compare_process_pid_null)
{
    // test if both are null
    ck_assert(!compare_pid(NULL, NULL));

    // a null
    Process* processby = _process_create("init", 9, 0);
    ck_assert_int_gt(compare_pid(NULL, processby), 0);

    // b null
    ck_assert_int_lt(compare_pid(processby, NULL), 0);

    _free_process(processby);
}
END_TEST

START_TEST(test_list_compare_process_pid)
{
    Process* processA = _process_create("init", 50, 9);
    Process* processB = _process_create("vim", 2, 9);
    Process* processC = _process_create("emacs", 3, 1);

    ck_assert_int_eq(compare_pid(processA, processB), 0);
    ck_assert_int_gt(compare_pid(processA, processC), 0);
    ck_assert_int_lt(compare_pid(processC, processA), 0);

    _free_process(processA);
    _free_process(processB);
    _free_process(processC);
}
END_TEST

///******************************************************************************/
///************************** compare_name() tests ********************************/
///******************************************************************************/
START_TEST(test_list_compare_process_name_null)
{
    // test if both are null
    ck_assert(!compare_name(NULL, NULL));

    // a null
    Process* processby = _process_create("init", 9, 0);
    ck_assert_int_gt(compare_name(NULL, processby), 0);

    // b null
    ck_assert_int_lt(compare_name(processby, NULL), 0);

    _free_process(processby);
}
END_TEST

START_TEST(test_list_compare_process_name)
{
    Process* processbyA = _process_create("vim", 9, 0);
    Process* processbyB = _process_create("vim", 9, 1);
    Process* processbyC = _process_create("vim2", 2, 2);

    ck_assert_int_eq(compare_name(processbyA, processbyB), 0);
    ck_assert_int_lt(compare_name(processbyA, processbyC), 0);
    ck_assert_int_gt(compare_name(processbyC, processbyA), 0);

    _free_process(processbyA);
    _free_process(processbyB);
    _free_process(processbyC);
}
END_TEST

///******************************************************************************/
///************************** swap_node() tests ********************************/
///******************************************************************************/
START_TEST(test_list_swap_node_null)
{
    // all null
    ck_assert(!swap_nodes(NULL, NULL, NULL));

    List* list = malloc(sizeof(List));
    ck_assert(!swap_nodes(NULL, NULL, list));

    ListNode* node = malloc(sizeof(ListNode));
    ck_assert(!swap_nodes(node, NULL, NULL));

    ck_assert(!swap_nodes(NULL, node, NULL));

    free(list);
    free(node);
}
END_TEST

START_TEST(test_list_swap_node_easy)
{
    ListNode* t = _create_node(_process_create("tetris", 68, 0));
    ListNode* c = _create_node(_process_create("crazy taxi", 50, 1));

    List* main_list = _create_list_with_processs(2, t, c);
    FakeNode* swap = _create_fake_list_with_processs(2, c, t);
    ck_assert(swap_nodes(t, c, main_list));
    _lists_exactly_equivalent(main_list, swap);

    _free_list_nodes_and_all_data(main_list);
}
END_TEST

START_TEST(test_list_swap_node_hard)
{
    ListNode* j = _create_node(_process_create("init", 68, 0));
    ListNode* m = _create_node(_process_create("vim", 50, 1));
    ListNode* a = _create_node(_process_create("python", 42, 2));
    ListNode* p = _create_node(_process_create("steam", 55, 3));


    List* main_list = _create_list_with_processs(4, a, j, m, p);


    // Swap 1
    swap_nodes(a, j, main_list);
    FakeNode* swap_1 = _create_fake_list_with_processs(4, j, a, m, p);
    ck_assert(swap_1);
    _lists_exactly_equivalent(main_list, swap_1);

    // Swap 2
    swap_nodes(a, p, main_list);
    FakeNode* swap_2 = _create_fake_list_with_processs(4, j, p, m, a);
    ck_assert(swap_2);
    _lists_exactly_equivalent(main_list, swap_2);

    // Swap 3
    swap_nodes(p, m, main_list);
    FakeNode* swap_3 = _create_fake_list_with_processs(4, j, m, p, a);
    ck_assert(swap_3);
    _lists_exactly_equivalent(main_list, swap_3);

    // Swap 4
    swap_nodes(a, j, main_list);
    FakeNode* swap_4 = _create_fake_list_with_processs(4, a, m, p, j);
    ck_assert(swap_4);
    _lists_exactly_equivalent(main_list, swap_4);

    // Swap 5
    swap_nodes(j, p, main_list);
    FakeNode* swap_5 = _create_fake_list_with_processs(4, a, m, j, p);
    ck_assert(swap_5);
    _lists_exactly_equivalent(main_list, swap_5);

    // Swap 6
    swap_nodes(j, m, main_list);
    FakeNode* swap_6 = _create_fake_list_with_processs(4, a, j, m, p);
    ck_assert(swap_6);
    _lists_exactly_equivalent(main_list, swap_6);

    _free_list_nodes_and_all_data(main_list);
}
END_TEST

///******************************************************************************/
///************************** sort() tests ********************************/
///******************************************************************************/
START_TEST(test_list_sort_null)
{
    // checks to make sure sorting is not null
    ck_assert(!sort(NULL, NULL));

    // checks to make sure the head is not null
    List* list = malloc(sizeof(List));
    list -> head = NULL;
    ck_assert(!sort(list, NULL));
    list -> head = malloc(sizeof(ListNode));
    ck_assert(!sort(list, NULL));

    free(list -> head);
    free(list);
}
END_TEST

START_TEST(test_list_sort_baby_pid)
{
    ListNode* j = _create_node(_process_create("java", 68, 1));
    ListNode* m = _create_node(_process_create("man", 50, 0));
    List* main_list = _create_list_with_processs(2, j, m);

    // Creating the function pointer for compare_pid
    int (*compare_func)(Process*, Process*) = &compare_pid;
    ck_assert(sort(main_list, compare_func));

    // Utilizing the wrapper function to ensure student's sorting properly
    FakeNode* basic_sort = _create_fake_list_with_processs(2, m, j);
    _lists_exactly_equivalent(main_list, basic_sort);

    _free_list_nodes_and_all_data(main_list);
}
END_TEST

START_TEST(test_list_sort_baby_name)
{
    // setting up for the test
    ListNode* j = _create_node(_process_create("init", 50, 0));
    ListNode* m = _create_node(_process_create("apache", 82, 1));
    List* main_list = _create_list_with_processs(2, j, m);

    // Function pointer
    int (*compare_func)(Process*, Process*) = &compare_name;
    ck_assert(sort(main_list, compare_func));

    // Utilizing the wrapper function to ensure proper sort
    FakeNode* basic_sort = _create_fake_list_with_processs(2, m, j);
    _lists_exactly_equivalent(main_list, basic_sort);

    _free_list_nodes_and_all_data(main_list);
}
END_TEST

START_TEST(test_list_sort_hard)
{
    // A whole lotta of setup (lovely)
    int (*compare_func_pid)(Process*, Process*) = &compare_pid;
    int (*compare_func_name)(Process*, Process*) = &compare_name;
    ListNode* a = _create_node(_process_create("sed", 12, 89));
    ListNode* b = _create_node(_process_create("mint", 1, 32));
    ListNode* c = _create_node(_process_create("mynt", 3,  5));
    ListNode* d = _create_node(_process_create("cat", 4, 15));
    ListNode* e = _create_node(_process_create("reboot", 15, 101));
    ListNode* f = _create_node(_process_create("tail", 6, 6));
    List* main_list = _create_list_with_processs(6, a, b, c, d, e, f);

    // Sort by pid
    ck_assert(sort(main_list, compare_func_pid));
    FakeNode* hard_sort = _create_fake_list_with_processs(6, c, f, d, b, a, e);
    _lists_exactly_equivalent(main_list, hard_sort);

    // Sort by name
    ck_assert(sort(main_list, compare_func_name));
    FakeNode* hard_sort_2 = _create_fake_list_with_processs(6, d, b, c, e, a, f);
    _lists_exactly_equivalent(main_list, hard_sort_2);

    _free_list_nodes_and_all_data(main_list);
}
END_TEST

///******************************************************************************/
///************************** list_to_array() tests ********************************/
///******************************************************************************/
START_TEST(test_list_list_to_array_null)
{
    ck_assert(!list_to_array(NULL, 0));

    List* temp = malloc(sizeof(List));
    temp -> head = NULL;
    ck_assert(!list_to_array(temp, 0));

    free(temp);
}
END_TEST

START_TEST(test_list_list_to_array_malloc_failure)
{
    List* l = _create_longish_list();

    mallocs_until_fail = 0;
    ck_assert(!list_to_array(l, 3));

    _free_list_nodes_and_all_data(l);
}
END_TEST

START_TEST(test_list_list_to_array_baby)
{
    Process* proc = _process_create("init", 89, 0);
    List* main_list = _create_list_with_processs(1, _create_node(proc));
    Process** process_array = list_to_array(main_list, 1);

    ck_assert(process_array);
    ck_assert_ptr_eq(proc, *process_array);
    ck_assert_ptr_eq(proc -> name, (*process_array) -> name);
    ck_assert_str_eq(proc -> name, (*process_array) -> name);
    ck_assert_int_eq(proc -> pid, (*process_array) -> pid);

    _free_process(proc);
    free(process_array);
}
END_TEST

START_TEST(test_list_list_to_array_hard)
{
    Process* init = _process_create("init", 15, 0);
    Process* python = _process_create("python", 6, 1);
    Process* tetris = _process_create("tetris", 10, 2);
    Process* sed = _process_create("sed", 42, 3);

    List* main_list = _create_list_with_processs(4, _create_node(init), _create_node(python), _create_node(tetris), _create_node(sed));

    Process* fake[] = {init, python, tetris, sed};

    Process** process_array = list_to_array(main_list, 4);

    ck_assert(process_array);

    for(int i = 0; i < 4; i++) {
        Process* my_process = fake[i];
        Process* student_process = *(process_array + i);
        ck_assert_ptr_eq(my_process, student_process);
        ck_assert_ptr_eq(my_process -> name, student_process -> name);
        ck_assert_str_eq(my_process -> name, student_process -> name);
        ck_assert_int_eq(my_process -> pid, student_process -> pid);
        _free_process(my_process);
    }

    free(process_array);
}
END_TEST

///******************************************************************************/
///************************** apply() tests ********************************/
///******************************************************************************/
int realloc_used = 0;

START_TEST(test_make_idle_realloc)
{
    Process* python = _process_create("python", 6, 1);

    mallocs_until_fail = 0;

    ck_assert(!make_idle(python));
    ck_assert(realloc_used);
    realloc_used = 0;
    _free_process(python);
}
END_TEST


START_TEST(test_make_idle_null)
{
    ck_assert(!make_idle(NULL));

    Process* proc = malloc(sizeof(Process));
    proc->name = NULL;
    ck_assert(!make_idle(proc));

    free(proc);
}
END_TEST

START_TEST(test_make_idle)
{
    Process* init = _process_create("init", 15, 0);
    Process* python = _process_create("python (idle)", 6, 1);
    Process* tetris = _process_create("tetris", 10, 2);
    Process* sed = _process_create("sed (idle)", 42, 3);

    make_idle(init);
    ck_assert_str_eq(init -> name, "init (idle)");

    make_idle(python);
    ck_assert_str_eq(python -> name, "python (idle)");

    make_idle(tetris);
    ck_assert_str_eq(tetris -> name, "tetris (idle)");

    make_idle(sed);
    ck_assert_str_eq(sed -> name, "sed (idle)");

    _free_process(init);
    _free_process(python);
    _free_process(tetris);
    _free_process(sed);


}
END_TEST

START_TEST(test_make_active_null)
{
    ck_assert(!make_idle(NULL));

    Process* proc = malloc(sizeof(Process));
    proc->name = NULL;
    ck_assert(!make_idle(proc));

    free(proc);
}
END_TEST

START_TEST(test_make_active)
{
    Process* init = _process_create("init (idle)", 15, 0);
    Process* python = _process_create("python", 6, 1);
    Process* tetris = _process_create("tetris (idle)", 10, 2);
    Process* sed = _process_create("sed", 42, 3);

    make_active(init);
    ck_assert_str_eq(init -> name, "init");

    make_active(python);
    ck_assert_str_eq(python -> name, "python");

    make_active(tetris);
    ck_assert_str_eq(tetris -> name, "tetris");

    make_active(sed);
    ck_assert_str_eq(sed -> name, "sed");
    _free_process(init);
    _free_process(python);
    _free_process(tetris);
    _free_process(sed);

}
END_TEST

START_TEST(test_make_active_realloc)
{
    Process* python = _process_create("python (idle)", 6, 1);

    mallocs_until_fail = 0;

    ck_assert(!make_active(python));
    ck_assert(realloc_used);
    realloc_used = 0;
    _free_process(python);
}
END_TEST


START_TEST(test_list_apply)
{
    // A whole lotta of setup (lovely)
    int (*make_idle_ptr)(Process*) = &make_idle;
    int (*make_active_ptr)(Process*) = &make_active;
    ListNode* a = _create_node(_process_create("sed", 12, 89));
    ListNode* b = _create_node(_process_create("mint", 1, 32));
    ListNode* c = _create_node(_process_create("mynt", 3,  5));
    ListNode* d = _create_node(_process_create("cat", 4, 15));
    ListNode* e = _create_node(_process_create("reboot", 15, 101));
    ListNode* f = _create_node(_process_create("tail", 6, 6));
    List* main_list = _create_list_with_processs(6, a, b, c, d, e, f);

    map_inplace(main_list, make_idle_ptr);

    ListNode *curr = main_list->head;
    ck_assert_str_eq(curr->data->name, "sed (idle)");
    curr = curr->next;
    ck_assert_str_eq(curr->data->name, "mint (idle)");
    curr = curr->next;
    ck_assert_str_eq(curr->data->name, "mynt (idle)");
    curr = curr->next;
    ck_assert_str_eq(curr->data->name, "cat (idle)");
    curr = curr->next;
    ck_assert_str_eq(curr->data->name, "reboot (idle)");
    curr = curr->next;
    ck_assert_str_eq(curr->data->name, "tail (idle)");

    map_inplace(main_list, make_active_ptr);

    curr = main_list->head;
    ck_assert_str_eq(curr->data->name, "sed");
    curr = curr->next;
    ck_assert_str_eq(curr->data->name, "mint");
    curr = curr->next;
    ck_assert_str_eq(curr->data->name, "mynt");
    curr = curr->next;
    ck_assert_str_eq(curr->data->name, "cat");
    curr = curr->next;
    ck_assert_str_eq(curr->data->name, "reboot");
    curr = curr->next;
    ck_assert_str_eq(curr->data->name, "tail");


    _free_list_nodes_and_all_data(main_list);
}
END_TEST



Suite *list_suite(void) {
    Suite *s = suite_create("Linked List");

    // create_process() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_create_process_basic);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_create_process_malloc_failure);

    // create_list() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_create_list_basic);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_create_list_malloc_failure);

    // create_node() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_create_node_basic);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_create_node_malloc_failure);

    // add_node() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_push_back_null);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_push_back);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_push_front_null);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_push_front);

    // remove_node() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_remove_node_null);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_remove_node);

    // copy_process() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_copy_process_malloc_failure);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_copy_process_null);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_copy_process);

    // copy_list() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_copy_null_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_copy_list_malloc_failure);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_copy_list_basic);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_copy_list_hard);

    // destroy_process() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_destroy_process);

    // destroy() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_destroy_NULL_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_destroy_empty);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_destroy_baby);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_destroy_longer_list);

    // compare() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_compare_process_pid_null);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_compare_process_pid);

    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_compare_process_name_null);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_compare_process_name);

     // swap_node() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_swap_node_null);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_swap_node_easy);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_swap_node_hard);

    // sort() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_sort_null);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_sort_baby_pid);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_sort_baby_name);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_sort_hard);

    // list_to_array() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_list_to_array_null);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_list_to_array_malloc_failure);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_list_to_array_baby);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_list_to_array_hard);

    // make_idle() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_make_idle);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_make_idle_null);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_make_idle_realloc);

    // make_active() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_make_active);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_make_active_null);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_make_active_realloc);

    // apply() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_apply);

    return s;
}
