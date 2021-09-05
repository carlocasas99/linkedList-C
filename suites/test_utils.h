#include "../list.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
// This is used for testing, you do not need to worry about it :)
#define tcase_hack(suite, setup_fixture, teardown_fixture, func) { \
    TCase *tc = tcase_create(STRINGIFY(func)); \
    tcase_add_checked_fixture(tc, setup_fixture, teardown_fixture); \
    tcase_add_test(tc, func); \
    suite_add_tcase(s, tc); \
}

// FakeNode used for testing
typedef struct fake {
    ListNode* node;
    struct fake* next;
} FakeNode;

typedef ListNode node;

//-----------------------Helper functions used for testing--------------//
extern Process* _process_create(char*, int, int);

extern void _free_list_nodes_and_all_data(List*);
extern void _free_list_nodes(ListNode*);
extern void _free_process(Process*);
extern int _push_front(List *, Process *);
extern int _push_back(List *, ListNode *);
extern ListNode* _get_node(Process*, List*);

extern List* _create_list_with_processs(int, ...);
extern FakeNode* _create_fake_list_with_processs(int, ...);

extern node *_create_node(Process *data);
extern struct list *_create_list(void);