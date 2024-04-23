#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;

typedef int key_t;

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);

int rbtree_to_array(const rbtree *, key_t *, const size_t);

// rotate 함수
void left_rotate(rbtree *, node_t *);
void right_rotate(rbtree *, node_t *);

// 삽입 fixup 함수
void insert_fixup(rbtree *, node_t *);

// 트리 print 함수
void postOrder(rbtree *, node_t *);

// 노드, 트리 통째로 삭제 함수
void postOrderDelete(rbtree *, node_t *);

// 중위 순회 + arr에 담기 함수
void inOrder(const rbtree *t, key_t *arr, const size_t n, size_t *index, node_t *cur);

// 삭제 fixup 함수
void rbtree_erase_fixup(rbtree *, node_t *);

// 노드 대체 함수
void rbtree_transplant(rbtree *, node_t *, node_t *);

// 후임자 찾기 함수
node_t *node_successor(rbtree *, node_t *);


#endif  // _RBTREE_H_
