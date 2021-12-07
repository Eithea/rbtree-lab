#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

node_t *NODE(int);
node_t *searchdown(const rbtree *, node_t *, key_t);
void falldown(rbtree *, node_t *, node_t *, node_t *);
node_t *grandparent(rbtree *, node_t *);
node_t *uncle(rbtree *, node_t *);
node_t *bro(rbtree *, node_t *);
node_t *child(rbtree *, node_t *);
void insert(rbtree *, node_t *);
void resort(rbtree *, node_t *);
void recolor(rbtree *, node_t *);
void rotation(rbtree *, node_t *);
void cw(rbtree *, node_t *);
void ccw(rbtree *, node_t *);
node_t *leftest(const rbtree *, node_t *);
node_t *rightest(const rbtree *, node_t *);
void delete(rbtree *, node_t *);
void rotationD(rbtree *, node_t *);
void rebalance(rbtree *, node_t *);
void replace(rbtree *, node_t *);
int inorder(const rbtree *, node_t *, key_t *, int, const size_t);


void testprint(const rbtree *, node_t *);


node_t *NODE(int value)
{ 
    node_t *n = malloc(sizeof(node_t));
    n->key = value;
    n->color = RBTREE_RED;
    n->parent = NULL;
    n->left = NULL;
    n->right = NULL;
    return n;
}

rbtree *new_rbtree(void) 
{
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
    node_t *n = malloc(sizeof(node_t));
    n->key = 0;
    n->color = RBTREE_BLACK;
    n->parent = NULL;
    n->left = NULL;
    n->right = NULL;
    p->nil = n;
    p->root = n;
    return p;
}

node_t *searchdown(const rbtree *t, node_t *N, key_t value)
{
    if (N->key == value)
        return N;
    if (N == t->nil)
        return NULL;
    if (N->key < value)
        searchdown(t, N->right, value);
    else
        searchdown(t, N->left, value);
}

void falldown(rbtree *t, node_t * I, node_t *N, node_t *P)
{
    if (N == t->nil)
    {
        I->parent = P;
        I->left = t->nil;
        I->right = t->nil;
        if (P != t->nil && P->key < I->key)
            P->right = I;
        else if (P != t->nil && P->key >= I->key)
            P->left = I;
        else
            t->root = I;
    }
    else
    {
        if (N->key < I->key)
            falldown(t, I, N->right, N);
        else
            falldown(t, I, N->left, N);
    }
}

node_t *grandparent(rbtree *t, node_t *N)
{
    if (N == t->nil || N->parent == t->nil)
        return t->nil;
    return N->parent->parent;
}

node_t *uncle(rbtree *t, node_t *N)
{
    node_t *G = grandparent(t, N);
    if (G == t->nil)
        return t->nil;
    if (G->right == N->parent)
        return G->left;
    return G->right;
}

node_t *bro(rbtree *t, node_t *N)
{
    if (N == t->nil || N->parent == t->nil)
        return NULL;
    if (N == N->parent->left)
        return N->parent->right;
    return N->parent->left;
}

node_t *child(rbtree *t, node_t *N)
{
    node_t *C;
    if (N->left != t->nil)
        C = N->left;
    else if (N->right != t->nil)
        C = N->right;
    else
    {
        C = NODE(0);
        C->color = RBTREE_BLACK;
        C->parent = N;
    }
    return C;
}

void insert(rbtree *t, node_t *I)
{
    falldown(t, I, t->root, t->nil);
    resort(t, I);
}

void resort(rbtree *t, node_t *N)
{
    if (N->parent == t->nil)
        N->color = RBTREE_BLACK;
    else if (N->parent->color == RBTREE_RED)
        recolor(t, N);
}

void recolor(rbtree *t, node_t *N)
{
    node_t *U = uncle(t, N);
    node_t *G = grandparent(t, N);
    if (U != t->nil && U->color == RBTREE_RED)
    {
        U->color = RBTREE_BLACK;
        G->color = RBTREE_RED;
        N->parent->color = RBTREE_BLACK;
        resort(t, G);
    }
    else
        rotation(t, N);
}

void rotation(rbtree *t, node_t *N)
{
    node_t *G = grandparent(t, N);
    if (G->right == N->parent && N == N->parent->left)
    {
        cw(t, N);
        N = N->right;
    }
    else if (G->left == N->parent && N == N->parent->right)
    {
        ccw(t, N);
        N = N->left;
    }
    G = grandparent(t, N);
    G->color = RBTREE_RED;
    N->parent->color = RBTREE_BLACK;
    if (N == N->parent->left)
        cw(t, N->parent);
    else
        ccw(t, N->parent);
}

void cw(rbtree *t, node_t *N)
{
    node_t *G = grandparent(t, N);
    if (G != t->nil)
    {
        if (G->left == N->parent)
            G->left = N;
        else
            G->right = N;
    }
    N->parent->parent = N;
    N->parent->left = N->right;
    if (N->right != t->nil)
        N->right->parent = N->parent;
    N->right = N->parent;
    N->parent = G;
    if (G == t->nil)
        t->root = N;
}

void ccw(rbtree *t, node_t *N)
{
    node_t *G = grandparent(t, N);
    if (G != t->nil)
    {
        if (G->left == N->parent)
            G->left = N;
        else
            G->right = N;
    }
    N->parent->parent = N;
    N->parent->right = N->left;
    if (N->left != t->nil)
        N->left->parent = N->parent;
    N->left = N->parent;
    N->parent = G;
    if (G == t->nil)
        t->root = N;
}

node_t *leftest(const rbtree *t, node_t *N)
{
    if (N->left == t->nil)
        return N;
    return leftest(t, N->left);
}

node_t *rightest(const rbtree *t, node_t *N)
{
    if (N->right == t->nil)
        return N;
    return rightest(t, N->right);
}

void delete(rbtree *t, node_t *N)
{
    
    node_t *C = child(t, N);
    if (N->parent == t->nil)
    {
        if (C->left == NULL)
            t->root = t->nil;
        else
            t->root = C;
            C->color = RBTREE_BLACK;
            C->parent = t->nil;
    }
    else if (N->color == RBTREE_RED)
    {
        if (N == N->parent->left)
            N->parent->left = t->nil;
        else
            N->parent->right = t->nil;
    }
    else if (N->color == RBTREE_BLACK && C->color == RBTREE_RED)
    {
        C->parent = N->parent;
        C->color = RBTREE_BLACK;
        if (N == N->parent->left)
            N->parent->left = C;
        else
            N->parent->right = C;
    }
    else
    {
        C->parent = N->parent;
        if (N == N->parent->left)
            N->parent->left = C;
        else
            N->parent->right = C;
        rotationD(t, C);
        if (C == C->parent->left)
            C->parent->left = t->nil;
        else
            C->parent->right = t->nil;
    }  
    free(N);
    if (C->left == NULL)
        free(C);
}

void rotationD(rbtree *t, node_t *N)
{
    if (N->parent == t->nil)
        return;
    node_t *B = bro(t, N);
    if (B->color == RBTREE_RED)
    {
        B->color = RBTREE_BLACK;
        B->parent->color = RBTREE_RED;
        if (B == B->parent->left)
            cw(t, B);
        else
            ccw(t, B);
    }
    rebalance(t, N);
}

void rebalance(rbtree *t, node_t *N)
{
    node_t *B = bro(t, N);
    if (N->parent->color == RBTREE_BLACK && B->color == RBTREE_BLACK && B->left == t->nil && B->right == t->nil)
    {   
        B->color = RBTREE_RED;
        rotationD(t, N->parent);
        return;
    }
    replace(t, N);
}

void replace(rbtree *t, node_t *N)
{
    node_t *B = bro(t, N);
    if (N->parent->color == RBTREE_RED && B->color == RBTREE_BLACK && B->left == t->nil && B->right == t->nil)
    {   
        B->color = RBTREE_RED;
        N->parent->color = RBTREE_BLACK;
        return;
    }
    if (B->color ==RBTREE_BLACK)
    {
        if (N == N->parent->left && B->left->color == RBTREE_RED)
        {
            B->color = RBTREE_RED;
            B->left->color = RBTREE_BLACK;
            cw(t, B->left);
        }
        else if (N == N->parent->right && B->right->color == RBTREE_RED)
        {
            B->color = RBTREE_RED;
            B->right->color = RBTREE_BLACK;
            ccw(t, B->right);
        }
    }
    B = bro(t, N);
    B->color = N->parent->color;
    N->parent->color = RBTREE_BLACK;
    if (N == N->parent->left)
    {
        if (B->right != t->nil)
            B->right->color = RBTREE_BLACK;
        ccw(t, B);
    }
    else
    {
        if (B->left != t->nil)
            B->left->color = RBTREE_BLACK;
        cw(t, B);
    }
}

int inorder(const rbtree *t, node_t *N, key_t *arr, int i, const size_t n)
{   
    int index = i;
    if (N->left != t->nil)
    {
        index = inorder(t, N->left, arr, i, n);
    }
    if (index < n)
    {
        arr[index] = N->key;
        index = index + 1;
    }
    else
        return 0;
    if (N->right != t->nil)
        index = inorder(t, N->right, arr, index, n);
    return index;
}


void delete_rbtree(rbtree *t) 
{
    while (t->root != t->nil)
    {
        rbtree_erase(t, t->root);
    }
    free(t->nil);
    free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) 
{
    node_t *I = NODE(key);
    insert(t, I);
    return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) 
{
    node_t *f = searchdown(t, t->root, key);
    return f;
}

node_t *rbtree_min(const rbtree *t) 
{
    return leftest(t, t->root);
}

node_t *rbtree_max(const rbtree *t) 
{
    return rightest(t, t->root);
}

int rbtree_erase(rbtree *t, node_t *p) 
{
    node_t *N = searchdown(t, t->root, p->key);
    node_t *D;
    if (N == t->nil)
    return 0;
    if (N->left != t->nil && N->right != t->nil)
    {
        D = rightest(t, N->left);
        N->key = D->key;
    }
    else
        D = N;  
    delete(t, D);
    return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) 
{
    inorder(t, t->root, arr, 0, n);
    return 0;
}

void testprint(const rbtree *t, node_t *N)
{
    printf("값 : %d, 부모 : %d, 색 : %d\n", N->key, N->parent->key, N->color);
    if (N->left != t->nil)
        testprint(t, N->left);
    if (N->right != t->nil)
        testprint(t, N->right);
}

