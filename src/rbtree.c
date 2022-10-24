#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree)); 
  //TODO: initialize struct if needed
  p->nil = (node_t *)malloc(sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->parent = NULL;
  p->nil->left = NULL;
  p->nil->right = NULL;
  p->root = p->nil;
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t->root != t->nil){
    if (t->root->left == t->nil && t->root->right == t->nil)
      free(t->root);
    else{
      rbtree *tt = new_rbtree();
      if(t->root->left != t->nil){
        tt->root = t->root->left;
        delete_rbtree(tt);
      }
      if(t->root->right != t->nil){
        tt->root = t->root->right;
        delete_rbtree(tt);
      }
    }
  }
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
    n->left->color = (n->left->color =- RBTREE_RED) ? RBTREE_BLACK : RBTREE_RED;
  }else{
    n->color = (n->color == RBTREE_RED) ? RBTREE_BLACK : RBTREE_RED;
    n->right->color = (n->left->color = -RBTREE_RED) ? RBTREE_BLACK : RBTREE_RED;
  }
}

void rbtree_check(rbtree *t,node_t * n){
  // if parent of new node == "RED"
  if (n->parent->color == RBTREE_RED){
    // 1. sibling of parent == "BLACK" or t->nil -> ROTATE && RECOLOR
    // -> find out which rotate is needed
    // R0: me and parent // R1: parent and grandparent
    // 0: left rotate, 1: right rotate
    int R0 = (n == n->parent->left) ? 0 : 1;
    int R1 = (n->parent == n->parent->parent->left) ? 0 : 1;
    
    // ???? Is it necessary to check whether n->parent is root or not?
    //LL or RR
    if (R0 == 0 && R1 == 0){//LL
      rbtree_right_rotate(t, n->parent->parent);
    }else if (R0 == 1 && R1 == 1){//RR
      rbtree_left_rotate(t, n->parent->parent);
    }else if (R0 == 0){//LR
      n = rbtree_right_rotate(t, n->parent);
      rbtree_recolor(1, n);
      n = rbtree_left_rotate(t, n->parent);
      rbtree_recolor(0,n);
    }else{//RL
      n = rbtree_left_rotate(t, n->parent);
      rbtree_recolor(0,n);
      n = rbtree_right_rotate(t, n->parent);
      rbtree_recolor(1,n);
   }

    // 2. sibling of parent == "RED" -> RECOLOR && if GRANDPARENT IS NOT ROOT - RECOLOR GRANDPARENT AND RECHECK
    // 1) recolor(make RED) sibiling of parent and parent
    n->parent->parent->left->color = RBTREE_BLACK;
    n->parent->parent->right->color = RBTREE_BLACK;
    if (n->parent->parent != t->root){
      if (n->parent->parent->color == RBTREE_RED)
        n->parent->parent->color = RBTREE_BLACK;
      else
        n->parent->parent->color = RBTREE_RED;
      rbtree_check(t, n->parent->parent);
    }
  }
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  //TODO: implement insert
  node_t* newNode;
  // no node in tree
  if (t->root == t->nil){
    t->root = (node_t *)malloc(sizeof(node_t));
    newNode = t->root;
    newNode->color = RBTREE_BLACK;
    newNode->parent = t->nil; //parent of root node = nil! 
    newNode->left = t->nil;
    newNode->right = t->nil;
    newNode->key = key;
    return t->root;
  }
  else
  {
    // at least one node in tree
    if (key > t->root->key){
      if (t->root->left != t->nil){
        rbtree *tt = new_rbtree();
        tt->root = t->root->left;
        rbtree_insert(tt, key);
      }
      else{
        t->root->left = (node_t *)malloc(sizeof(node_t));
        newNode = t->root->left;
        newNode->color = RBTREE_RED;
        newNode->parent = t->root;
        newNode->left = t->nil;
        newNode->right = t->nil;
        newNode->key = key;
      }
    }
    else{
      if (t->root->right != t->nil){
        rbtree *tt = new_rbtree();
        tt->root = t->root->left;
        rbtree_insert(tt, key);
      }
      else{
        t->root->right = (node_t *)malloc(sizeof(node_t));
        newNode = t->root->right;
        newNode->color = RBTREE_RED;
        newNode->parent = t->root;
        newNode->left = t->nil;
        newNode->right = t->nil;
        newNode->key = key;
      }
    }
  }
  rbtree_check(t, newNode);
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
  // // TODO: implement find
  // node_t *n;
  // if (t->root != t->nil){
  //   n = t->root;
  //   rbtree *tt = new_rbtree();
  //   if (key < n->key){
  //     tt->root = n->left;
  //     n->left = rbtree_find(tt, key);
  //   }else if (key > n->key){
  //     tt->root = n->right;
  //     n->right = rbtree_find(tt, key);
  //   }
  // }
  // return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // // TODO: implement find
  // node_t* n;
  // if (t->root != t->nil){
  //   n = t->root;
  //   node_t *pn;
  //   do{
  //     pn = n;
  //     n = n->left;
  //   } while (n != t->nil);
  //   return pn->key;
  // }
  // return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // // TODO: implement find
  // node_t *n;
  // if (t->root != t->nil){
  //   n = t->root;
  //   node_t *pn;
  //   do{
  //     pn = n;
  //     n = n->right;
  //   } while (n != t->nil);
  //   return pn->key;
  // }
  // return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
