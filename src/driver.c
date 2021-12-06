#include "rbtree.h"

void main()
{
    rbtree *t = new_rbtree();
    rbtree_insert(t, 2);
    rbtree_insert(t, 1);
    rbtree_insert(t, 8);
    rbtree_insert(t, 9);
    rbtree_insert(t, 7);
    rbtree_insert(t, 6);
    rbtree_insert(t, 4);
    rbtree_insert(t, 5);
    node_t *f = rbtree_find(t, 4);
    rbtree_erase(t, f);
    f = rbtree_find(t, 1);
    rbtree_erase(t, f);
    f = rbtree_find(t, 8);
    rbtree_erase(t, f);
    testprint(t, t->root);
}