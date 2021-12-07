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
    testprint(t, t->root);
    key_t *res = calloc(4, sizeof(key_t));
    rbtree_to_array(t, res, 4);
    for (int i = 0; i < 4; i++)
{
    printf("i= %d, 값 :  %d\n", i, res[i]);
}
}