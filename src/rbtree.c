#include "rbtree.h"
#include <stdlib.h>
rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree)); 
  //TODO: initialize struct if needed
  p->nil = (node_t *)malloc(sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->parent = NULL;
  p->nil->left = NULL;
  p->nil->right = NULL;
  p->nil->key = 0;
  p->root = p->nil;
  return p;
}

void delete_rbtree_node(node_t* nil, node_t* n){
  if (n != nil){
    delete_rbtree_node(nil, n->left);
    delete_rbtree_node(nil, n->right);
    free(n);
  }
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_rbtree_node(t->nil, t->root);
  free(t->nil);
  free(t);
}

node_t* rbtree_left_rotate(rbtree *t, node_t *n){
  node_t *m = n->right;
  n->right = m->left;
  if (m->left != t->nil) m->left->parent = n;  
  
  m->parent = n->parent;

  if (n->parent == t->nil) t->root = m;
  else if(n == n->parent->left) n->parent->left = m;
  else n->parent->right = m;
  
  m->left = n;
  n->parent = m;
  return m;
}

node_t* rbtree_right_rotate(rbtree *t, node_t *n){
  node_t *m = n->left;
  n->left = m->right;
  if(m->right != t->nil) m->right->parent = n;

  m->parent = n->parent;

  if(n->parent == t->nil) t->root = m;
  else if (n == n->parent->left) n->parent->left = m;
  else n->parent->right = m;

  m->right = n;
  n->parent = m;
  return m;
}

//flag is for rotate direction (0: left, 1: right)
void rbtree_recolor(int flag, node_t *n){
  if (flag == 0){
    n->color = (n->color == RBTREE_RED) ? RBTREE_BLACK : RBTREE_RED;
    n->left->color = (n->left->color == RBTREE_RED) ? RBTREE_BLACK : RBTREE_RED;
  }else{
    n->color = (n->color == RBTREE_RED) ? RBTREE_BLACK : RBTREE_RED;
    n->right->color = (n->right->color == RBTREE_RED) ? RBTREE_BLACK : RBTREE_RED;
  }
}

void rbtree_insert_check(rbtree *t,node_t * n){
  if (n == t->root)
    return;
  if (n->parent->color == RBTREE_BLACK)
    return;
  if (n->parent->color == RBTREE_RED)
  {
    // 1. sibling of parent == "BLACK" or t->nil -> ROTATE && RECOLOR
    // -> find out which rotate is needed
    // R0: me and parent // R1: parent and grandparent
    // 0: left rotate, 1: right rotate
    if (n->parent == n->parent->parent->left)
    { // n->parent: left child
      if (n->parent->parent->right->color == RBTREE_BLACK)
      {
        // parent sibling = black;
        int R0 = (n == n->parent->left) ? 0 : 1;
        int R1 = (n->parent == n->parent->parent->left) ? 0 : 1;
      
        if (R0 == 0 && R1 == 0)
        { // LL
          rbtree_right_rotate(t, n->parent->parent);
          rbtree_recolor(1,n->parent);
        }
        else if (R0 == 1 && R1 == 1)
        { // RR
          rbtree_left_rotate(t, n->parent->parent);
          rbtree_recolor(0,n->parent);
        }
        else if (R0 == 0 && R1 == 1)
        { // LR
          n = rbtree_right_rotate(t, n->parent);
          n = rbtree_left_rotate(t, n->parent);
          rbtree_recolor(0, n);
        }
        else if (R0 == 1 && R1 == 0)
        { // RL
          
          n = rbtree_left_rotate(t, n->parent);
          n = rbtree_right_rotate(t, n->parent);
          rbtree_recolor(1, n);
        }
      }
      else
      {
        // parent sibiling = red;
        n->parent->parent->right->color = RBTREE_BLACK;
        n->parent->color = RBTREE_BLACK;
        if (n->parent->parent != t->root)
        {
          if (n->parent->parent->color == RBTREE_RED)
            n->parent->parent->color = RBTREE_BLACK;
          else
            n->parent->parent->color = RBTREE_RED;
          rbtree_insert_check(t, n->parent->parent);
        }
      }
      }else{
        if(n->parent->parent->left->color == RBTREE_BLACK){
          int R0 = (n == n->parent->left) ? 0 : 1;
          int R1 = (n->parent == n->parent->parent->left) ? 0 : 1;

          if (R0 == 0 && R1 == 0){//LL
            rbtree_right_rotate(t, n->parent->parent);
            rbtree_recolor(1,n->parent);
          }else if (R0 == 1 && R1 == 1){//RR
            rbtree_left_rotate(t, n->parent->parent);
            rbtree_recolor(0,n->parent);
          }else if (R0 == 0 && R1 == 1){//LR
            n = rbtree_right_rotate(t, n->parent);
            n = rbtree_left_rotate(t, n->parent);
            rbtree_recolor(0,n);
          }else if (R0 == 1 && R1 == 0){//RL
            n = rbtree_left_rotate(t, n->parent);
            n = rbtree_right_rotate(t, n->parent);
            rbtree_recolor(1,n);
            
          } 
        }else{
          // 2. sibling of parent == "RED" -> make parent sibling BLACK && if GRANDPARENT IS NOT ROOT - RECOLOR GRANDPARENT AND RECHECK
          // 1) recolor(make RED) sibiling of parent and parent
          n->parent->parent->left->color = RBTREE_BLACK;
          n->parent->color = RBTREE_BLACK;
          if (n->parent->parent != t->root)
          {
            if (n->parent->parent->color == RBTREE_RED)
              n->parent->parent->color = RBTREE_BLACK;
            else
              n->parent->parent->color = RBTREE_RED;
            rbtree_insert_check(t, n->parent->parent);
          }
        }
      }
  }
}

node_t * rbtree_insert_node(rbtree *t, node_t *n, const key_t key){

  if (n->key > key){
    if (n->left != t->nil){
      return rbtree_insert_node(t, n->left, key);
    }else{
      node_t * newNode = (node_t *)malloc(sizeof(node_t));
      n->left = newNode;
      newNode->key = key;
      newNode->parent = n;
      newNode->left = t->nil;
      newNode->right = t->nil;
      newNode->color = RBTREE_RED;
      return newNode;
    }
  }else{
    if(n->right != t->nil){
      return rbtree_insert_node(t, n->right, key);
    }else{
      node_t *newNode = (node_t *)malloc(sizeof(node_t));
      n->right = newNode;
      newNode->key = key;
      newNode->parent = n;
      newNode->left = t->nil;
      newNode->right = t->nil;
      newNode->color = RBTREE_RED;
      return newNode;
    }
  }
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  //TODO: implement insert
  // no node in tree
  if (t->root == t->nil){
    t->root = (node_t *)malloc(sizeof(node_t));
    node_t *newNode = t->root;
    newNode->color = RBTREE_BLACK;
    newNode->parent = t->nil; //parent of root node = nil! 
    newNode->left = t->nil;
    newNode->right = t->nil;
    newNode->key = key;
    return newNode;
  }else
  {
    // at least one node in tree
    node_t *newNode = rbtree_insert_node(t, t->root, key);
    rbtree_insert_check(t, newNode);
    return newNode;
  }
}

node_t *rbtree_find_node(node_t* nil, node_t * n, const key_t key){
  if (n == nil){
    return NULL;
  }
  if(n->key > key)
    return rbtree_find_node(nil, n->left, key);
  else if(n->key < key)
    return rbtree_find_node(nil, n->right, key);
  else{
    return n;
  }
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find 
  return rbtree_find_node(t->nil, t->root, key);
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *n = t->root;
  if (n != t->nil)
  {
    while (n->left != t->nil){
      n = n->left;
    }
  }
  return n;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *n = t->root;
  if (n!= t->nil){
    while(n->right != t->nil){
      n = n->right;
    }
  }
  return n;
}

// helps us move subtree within the tree
// u: to be erased
// v: to be moved to replaced u
void rbtree_transplant_node(rbtree *t, node_t* u, node_t*v){
  // reset child address
  if (u->parent == t->nil) t->root = v;
  else if (u == u->parent->left) u->parent->left = v;
  else u->parent->right = v; 
  // reset parent address
  v->parent = u->parent; 
}

// 1. w is RED
// 2. w is BLACK, both w->left and w->right are BLACK
// 3. w is BLACK, w->left is RED and w->right is BLACK
// 4. w is BLACK and w->right is RED

// fixes any RBtree violations
int rbtree_erase_check(rbtree *t, node_t* x){
  while (x != t->root && x->color == RBTREE_BLACK){
    if (x == x->parent->left){
      node_t *w = x->parent->right;
      // 1. w is RED
      if (w->color == RBTREE_RED){
        x->parent->color = RBTREE_RED;
        w->color = RBTREE_BLACK;
        rbtree_left_rotate(t, x->parent);
        w = x->parent->right;
      }
      // 2. all the w, w->left, w->right are BLACK
      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }else{
        // 3. w is BLACK, w->right is BLACK, w->left is RED
        if(w->right->color == RBTREE_BLACK){
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rbtree_right_rotate(t, w);
          w = x->parent->right;
        }
        // 4. w is BLACk, w->right is RED w->left is BLACK
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        rbtree_left_rotate(t, x->parent);
        x = t->root;
      }
    }else{
      node_t *w = x->parent->left;
      // 1
      if (w->color == RBTREE_RED){
        x->parent->color = RBTREE_RED;
        w->color = RBTREE_BLACK;
        rbtree_right_rotate(t, x->parent);
        w = x->parent->left;
      }
      // 2(all BLACK)
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        // 3
        if (w->left->color == RBTREE_BLACK){
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rbtree_left_rotate(t, w);
          w = x->parent->left;
        }
        // 4
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        rbtree_right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
  return 0;
}

node_t * rbtree_min_node(rbtree *t, node_t *n){
  if (t->root == t->nil){
    return t->root;
  }
  if (n != t->nil)
  {
    while(n->left != t->nil){
      n = n->left;
    }
  }
  return n;
}

// erase case 
// 1. left child is NIL
  // if z(target) color is BLACK and its left child(x) is RED -> transplant(z, x)
  // since the original color of the target(now x) is BLACK -> erase_check(eventually, make x BLACK)
// 2. right child is NIL
  // if z(target) color is BLACK and its right child is RED -> transplant(z, x)
  // since the original color of the target(now x) is BLACK ->erase_check(eventually, make x BLACK)
// 3. neither child is NIL
  // find the minimum node of right subtree -> indicate that node y
  // indicate the right child of y, x
  // transplant (y, x)
  // transplant (z, y)


// erase the node
int rbtree_erase(rbtree *t, node_t *p) {
  //TODO: implement erase
  node_t *y = p;
  node_t *x;
  int y_original_color = y->color;

  // case 1: left child is NIL
  if (p->left == t->nil){
    x = p->right;
    rbtree_transplant_node(t, p, p->right);
  }else if (p->right == t->nil){
    // case 2: right child is NIL
    x = p->left;
    rbtree_transplant_node(t, p, p->left);
  }else{
    // case 3: neither child is NIL
    y = rbtree_min_node(t, p->right);
    y_original_color = y->color;
    x = y->right;

    if (y->parent == p)
      x->parent = y;
    else{
      rbtree_transplant_node(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    rbtree_transplant_node(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;

  }
  if(y_original_color == RBTREE_BLACK)
    rbtree_erase_check(t, x);
  free(p);
  return 0;
}

void rbtree_inorder(node_t* nil, key_t *arr, node_t *n, int* index){
  if (n == nil)
    return;
  rbtree_inorder(nil, arr, n->left, index);
  arr[(*index)++] = n->key;
  rbtree_inorder(nil, arr, n->right, index);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if (t->root != t->nil){
    int *index = (int *)malloc(sizeof(int));
    *index = 0;
    rbtree_inorder(t->nil, arr, t->root, index);
    free(index);
    return 0;
  }
  return 0;
}
