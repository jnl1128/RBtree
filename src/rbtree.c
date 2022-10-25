#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>
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
  // if (t->root != t->nil){
    delete_rbtree_node(t->nil, t->root);
  // }
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
  printf("me: %d(%d), myparent: %d(%d)\n", n->key,n->color,  n->parent->key, n->parent->color);
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
      printf("my parent(%d) is left child\n", n->parent->key);
      if (n->parent->parent->right->color == RBTREE_BLACK)
      {
        // parent sibling = black;
        printf("my uncle(%d) is black\n", n->parent->parent->right->key);
        int R0 = (n == n->parent->left) ? 0 : 1;
        int R1 = (n->parent == n->parent->parent->left) ? 0 : 1;
        printf("R0: %d,R1: %d\n", R0, R1);

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
          printf("RL-----------------------------\n");
          n = rbtree_left_rotate(t, n->parent);
          printf("n: %d(%d), n->left:%d(%d)\n", n->key, n->color,n->left->key, n->left->color);

          printf("newly grand: %d(%p) parent: %d(%p)\n", n->parent->key,n->parent, n->key, n);

          n = rbtree_right_rotate(t, n->parent);
          printf("n: %d(%d), n->right:%d(%d)\n", n->key, n->color,n->right->key, n->right->color);
          rbtree_recolor(1, n);
          printf("n: %d(%d), n->right:%d(%d)\n", n->key, n->color,n->right->key, n->right->color);
          printf("-----------------------------\n");
        }
      }
      else
      {
        // parent sibiling = red;
        printf("my uncle is red\n");
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
        printf("my(%d) parent is right child\n", n->key);
        if(n->parent->parent->left->color == RBTREE_BLACK){
          printf("my uncle is black\n");
          int R0 = (n == n->parent->left) ? 0 : 1;
          int R1 = (n->parent == n->parent->parent->left) ? 0 : 1;
          printf("R0: %d, R1: %d\n", R0, R1);

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
          printf("my uncle is red(key: %d)\n", n->parent->parent->left->key);
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
            printf("grandparent is not root: %d(%d)\n", n->parent->parent->key, n->parent->parent->color);
            rbtree_insert_check(t, n->parent->parent);
          }
          else
            printf("my grandparent is root!\n");
        }
      }
  }
}

node_t * rbtree_insert_node(rbtree *t, node_t *n, const key_t key){

  if (n->key > key){
    if (n->left != t->nil){
      rbtree_insert_node(t, n->left, key);
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
      rbtree_insert_node(t, n->right, key);
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
    // printf("newNode: key(%d), color(%d)\n", newNode->key, newNode->color);
    rbtree_insert_check(t, newNode);
    return newNode;
  }
}

node_t *rbtree_find_node(const rbtree *t, node_t * n, const key_t key){
  if (n == t->nil)
    return NULL;

  if (n->key > key){
    rbtree_find_node(t, n->right, key);
  }
  else if (n->key < key){
    rbtree_find_node(t, n->left, key);
  }else{
    return n;
  }
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find 
  if (t->root == t->nil)
    return NULL;
  return rbtree_find_node(t, t->root, key);
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t* n;
  if (t->root != t->nil){
    n = t->root;
    node_t *pn;
    do{
      pn = n;
      n = n->left;
    } while (n->left != NULL);
    return pn;
  }
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *n;
  if (t->root != t->nil){
    n = t->root;
    node_t *pn;
    do{
      pn = n;
      n = n->right;
    } while (n->right != NULL);
    return pn;
  }
  return t->root;
}

// node_t * rbtree_erase_node(rbtree *t, node_t* n,node_t *p){
//   if (n == t->nil)
//     return NULL;
  
//   if (p->key < n->key){
//     n->left = rbtree_erase_node(t, n->left, p);
//   }
//   else if (p->key > n->key)
//   { 
//     n->right = rbtree_erase_node(t, n->right, p);
//   }
//   else{
//     if (n->color == RBTREE_RED){
//       if (n->left == t->nil){
//         node_t *tmp = n->right;
//         free(n);
//         return tmp;
//       }else if (n->right == t->nil){
//         node_t *tmp = n->left;
//         free(n);
//         return tmp;
//       }
//       rbtree *tt = new_rbtree();
//       tt->root = n->right;
//       node_t *tmp = rbtree_min(tt);
//       n->key = tmp->key;
//       n->right = rbtree_erase_node(tt, n->right, tmp->key);
//     }
//   }
// }



// u자리에 v를 놓기
void rbtree_transplant_node(rbtree *t, node_t* u, node_t*v){
  if (u->parent == t->nil) t->root = v;
  else if (u == u->parent->left) u->parent->left = v;
  else u->parent->right = v;
  v->parent = u->parent;
  free(u);
}

int rbtree_erase_check(rbtree *t, node_t* p){
  while (p != t->root && p->color == RBTREE_BLACK)
  {
    if(p == p->parent->left){
      node_t *n = p->parent->right;
      if (n->color == RBTREE_RED){
        n->color = RBTREE_BLACK;
        p->parent->color = RBTREE_RED;
        rbtree_left_rotate(t, p->parent);
        n = p->parent->right;
      }
      if (n->left->color == RBTREE_BLACK && n->right->color == RBTREE_BLACK){
        n->color = RBTREE_RED;
        p = p->parent;
      }else if(n->right->color == RBTREE_BLACK){
        n->left->color = RBTREE_BLACK;
        n->color = RBTREE_RED;
        rbtree_right_rotate(t, n);
        n = p->parent->right;
      }
      n->color = p->parent->color;
      p->parent->color = RBTREE_BLACK;
      n->right->color = RBTREE_BLACK;
      rbtree_left_rotate(t, p->parent);
      p = t->root;
    }else{
      node_t *n = p->parent->left;
      if (n->color == RBTREE_RED){
        n->color = RBTREE_BLACK;
        p->parent->color = RBTREE_RED;
        rbtree_right_rotate(t, p->parent);
        n = p->parent->left;
      }
      if(n->right->color == RBTREE_BLACK && n->left->color == RBTREE_BLACK){
        n->color = RBTREE_RED;
        p = p->parent;
      }else if (n->left->color == RBTREE_BLACK){
        n->right->color = RBTREE_BLACK;
        n->color = RBTREE_RED;
        rbtree_left_rotate(t, n);
        n = p->parent->left;
      }
      n->color = p->parent->color;
      p->parent->color = RBTREE_BLACK;
      n->left->color = RBTREE_BLACK;
      rbtree_right_rotate(t, p->parent);
      p = t->root;
    }
  }
  p->color = RBTREE_BLACK;
  return 0;
}

node_t * rbtree_min_node(rbtree *t, node_t *n){
  if (n != t->nil){
    node_t *pn;
    do{
      pn = n;
      n = n->left;
    } while (n->left != NULL);
    return pn;
  }
  return n;
}

int rbtree_erase(rbtree *t, node_t *p) {
  //TODO: implement erase
  if (t->root != t->nil){
    node_t *n = p;
    node_t *q;
    int n_origin_color = n->color;
    if(p->left == t->nil){
      q = p->right;
      rbtree_transplant_node(t, p, p->right);
    }else if(p->right == t->nil){
      q = p->left;
      rbtree_transplant_node(t, p, p->left);
    }
    else
    {
      n = rbtree_min_node(t, n->right);
      printf("NN: %p %d\n", n, n->key);
      n_origin_color = n->color;
      q = n->right;
      if (n->parent == p)
        q->parent = n;
      else{
        rbtree_transplant_node(t, n, n->right);
        n->right = p->right;
        n->right->parent = n;
      }
      rbtree_transplant_node(t, p, n);
      n->left = p->left;
      n->left->parent = n;
      n->color = p->color;
    }
    if (n_origin_color == RBTREE_BLACK)
      rbtree_erase_check(t, q);
    
    return 0;
  }
  return 0;
}


void rbtree_inorder(node_t* nil, key_t *arr, node_t *n, int* index){
  if (n == nil)
    return;
  rbtree_inorder(nil, arr, n->left, index);
  arr[(*index)++] = n->key;
  printf("key: %d, color: %d(0:red, 1:black)\n", n->key, n->color);
  printf("left: %d, right: %d parent: %d\n", n->left->key, n->right->key, n->parent->key);
  printf("---------------------------------\n");
  rbtree_inorder(nil, arr, n->right, index);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if (t->root != t->nil){
    int *index = (int *)malloc(sizeof(int));
    *index = 0;
    printf("root! %d %d\n", t->root->key, t->root->color);
    printf("root->left: %d, right: %d parent: %d\n", t->root->left->key, t->root->right->key, t->root->parent->key);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    rbtree_inorder(t->nil, arr, t->root, index);
    free(index);
    return 0;
  }
  return 0;
}
