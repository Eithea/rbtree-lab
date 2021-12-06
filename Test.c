#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

typedef enum
{
  RED, BLACK
} color_t;

typedef int key_t;

typedef struct node
{
  color_t color;
  key_t key;
  struct node *parent, *left, *right;
} node;

typedef struct {
  node *root;
  node *nil;
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);
node *rbtree_insert(rbtree *, const key_t);
node *rbtree_find(const rbtree *, const key_t);
node *rbtree_min(const rbtree *);
node *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node *);
int rbtree_to_array(const rbtree *, key_t *, const size_t);

node *NODE(int);
node *searchdown(const rbtree *, node *, key_t);
node *falldown(rbtree *, node *, node *, node *);
node *grandparent(rbtree *, node *);
node *uncle(rbtree *, node *);
node *bro(rbtree *, node *);
node *child(rbtree *, node *);
void insert(rbtree *, node *, key_t);
void resort(rbtree *, node *);
void recolor(rbtree *, node *);
void rotation(rbtree *, node *);
void cw(rbtree *, node *);
void ccw(rbtree *, node *);
node *leftest(const rbtree *, node *);
node *rightest(const rbtree *, node *);



node *NODE(int value)
{ 
  node *n = malloc(sizeof(node));
  n->key = value;
  n->color = RED;
  n->parent = NULL;
  n->left = NULL;
  n->right = NULL;
  return n;
}

rbtree *new_rbtree(void) 
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = NODE(0);
  p->nil->color = BLACK;
  p->root = p->nil;
  return p;
}

node *falldown(rbtree *t, node * I, node *N, node *P)
{
  if (N == t->nil)
  {
      I->parent = P;
      if (P != t->nil && P->key < I->key)
          P->right = I;
      else if (P != t->nil && P->key >= I->key)
          P->left = I;
    else 
  }
  else
  {
      if (N->key < I->key)
          N->right = falldown(t, I, N->right, N);
      else
          N->left = falldown(t, I, N->left, N);
  }
  return N;
}

node *grandparent(rbtree *t, node *N)
{
  if (N == t->nil || N->parent == t->nil)
      return t->nil;
  return N->parent->parent;
}

node *uncle(rbtree *t, node *N)
{
  node *G = grandparent(t, N);
  if (G == t->nil)
      return t->nil;
  if (G->right == N->parent)
      return G->left;
  return G->right;
}


void insert(rbtree *t, node *P, key_t value)
{
  node *I = NODE(value);
  t->root = falldown(t, I, t->root, P);
  resort(t, I);
}

void resort(rbtree *t, node *N)
{
  if (N->parent == t->nil)
      N->color = BLACK;
  else if (N->parent->color == RED)
      recolor(t, N);
}

void recolor(rbtree *t, node *N)
{
  node *U = uncle(t, N);
  node *G = grandparent(t, N);
  if (U != t->nil && U->color == RED)
  {
      U->color = BLACK;
      G->color = RED;
      N->parent->color = BLACK;
      resort(t, G);
  }
  else
      rotation(t, N);
}

void rotation(rbtree *t, node *N)
{
  node *G = grandparent(t, N);
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
}

void cw(rbtree *t, node *N)
{
    node *G = grandparent(t, N);
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

void ccw(rbtree *t, node *N)
{
    node *G = grandparent(t, N);
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

node *rbtree_insert(rbtree *t, const key_t key) 
{
  insert(t, t->nil, key);
  return t->root;
}

void main()
{
    rbtree *t = new_rbtree();
    rbtree_insert(t, 5);
    printf("%d", t->root->key);
}