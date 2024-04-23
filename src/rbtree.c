#include "rbtree.h"
#include <stdlib.h>


// 트리 생성
rbtree *new_rbtree(void) {

  // 트리, nil노드 할당
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;

  // tree의 nil과 root 노드를 방금 만든 nil로 설정
  p->nil = nil;
  p->root = nil;

  return p;
}



// 트리 삭제1
void delete_rbtree(rbtree *t) {

  // TODO: reclaim the tree nodes's memory
  node_t *cur = t->root;

  // 노드 지우기
  postOrderDelete(t, cur);
  free(t->nil);
  t->nil = NULL;

  // 트리 지우기
  free(t);
  t = NULL;
}



// 트리 삭제2
void postOrderDelete(rbtree *t, node_t *cur) {
    if(cur == t->nil) {
        return;
    }

    postOrderDelete(t, cur->left);
    postOrderDelete(t, cur->right);
    
    // 메모리 해제
    free(cur);
    cur = NULL;
}


// key 입력받아서 node_t 형태로 tree에 삽입
node_t *rbtree_insert(rbtree *t, const key_t key) {

  // TODO: implement insert
  // z노드에 key 넣고 로직 시작
  node_t *z = (node_t *)malloc(sizeof(node_t));
  node_t *y;
  node_t *x;
  z->key = key;
  y = t->nil;
  x = t->root;

  // y는 leaf node, x는 leaf까지 가기 위한 임시변수
  // leaf node 까지 파고들기
  while(x != t->nil) {
    y = x;
    if(z->key < x->key) {
      x = x->left;
    }
    else {
      x = x->right;
    }
  }

  // z의 부모를 y로 설정하고 left/right 배정하기
  // 부모 노드가 없다면 z를 root로 설정한다
  z->parent = y;
  if(y == t->nil) {
    t->root = z;
  }
  else if(z->key < y->key) {
    y->left = z;
  }
  else {
    y->right = z;
  }

  // z의 자식을 nil로 설정, color Red로 지정
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  // 삽입 후 트리 재조정
  insert_fixup(t, z);

  return t->root;
}



// 삽입 후 트리 재조정 함수
void insert_fixup(rbtree *t, node_t *z) {
  
  // red가 연속되지 않을 때 까지 반복한다
  while(z->parent->color == RBTREE_RED) {
    node_t *y;

    // 왼쪽/오른쪽 기준 잡기
    if(z->parent == z->parent->parent->left) {
      y = z->parent->parent->right; // 삼촌을 y에 저장
      
      // case 1 : 삼촌이 RED일 경우
      if(y->color == RBTREE_RED) {

        // 부모삼촌 <-> 할배 색깔 뒤집기
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;

        // 뒤집은 후 할배기준 다시 확인하기
        z = z->parent->parent;
        continue;
      }

      // case 2 : 삼촌이 BLACK인데 꺾여있을 경우
      else if(z == z->parent->right) {

        //부모기준 회전하기 + case 3 진행
        z = z->parent;
        left_rotate(t, z);

        // case 2 수행 후에 case 3 수행
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }

      // case 3 : 삼촌이 BLACK인데 1자일 경우
      else {
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    else {
      // 삼촌을 y에 저장
      y = z->parent->parent->left;

      // case 1 : 삼촌 RED일 때
      if(y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;

        // 뒤집은 후 할배기준 다시 확인
        z = z->parent->parent;
      }

      // case 2 : 삼촌 BLACK인데 꺾여있을 때
      else if(z == z->parent->left) {
        z = z->parent;

        // 부모기준 회전 후 case 3 실행
        right_rotate(t, z);

        // case 3
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }

      // case 3 : 삼촌 BLACK인데 1자일 때
      else {
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}



// 특정 key에 해당하는 노드 찾기
node_t *rbtree_find(const rbtree *t, const key_t key) {

  // TODO: implement find
  node_t *cur = t->root;

  while(cur != t->nil) {
    // key를 찾았을 경우
    if(cur->key == key) {
      return cur;
    }
    // cur->key보다 작으면 left, 크면 right 탐색
    else if(cur->key > key) {
      cur = cur->left;
    }
    else {
      cur = cur->right;
    }
  }

  // 해당하는 key가 없을 경우 NULL 반환
  return NULL;
}



// 가장 작은 값 찾기
node_t *rbtree_min(const rbtree *t) {

  // TODO: implement find
  node_t *cur = t->root;
  node_t *mini;

  // tree가 비어있을 경우 NULL 반환
  if(cur == t->nil) {
    return NULL;
  }

  while(cur != t->nil) {
    mini = cur;
    cur = cur->left;
  }

  return mini;
}



// 가장 큰 값 찾기
node_t *rbtree_max(const rbtree *t) {

  // TODO: implement find
  node_t *cur = t->root;
  node_t *maxx;

  if(cur == t->nil) {
    return NULL;
  }

  while(cur != t->nil) {
    maxx = cur;
    cur = cur->right;
  }

  return maxx;
}



int rbtree_erase(rbtree *t, node_t *p) {

  // TODO: implement erase
  // node_t *cur = t->root;
  
  return 0;
}



// 주어진 arr에 node를 오름차순으로 넣기, 최대 n개 까지만1
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {

  // TODO: implement to_array
  node_t *cur = t->root;
  size_t *index;
  size_t x = 0;
  index = &x;

  inOrder(t, arr, n, index, cur);
  return 0;
}



// 주어진 arr에 node를 오름차순으로 넣기, 최대 n개 까지만2
void inOrder(const rbtree *t, key_t *arr, const size_t n, size_t *index, node_t *cur) {

  if(cur == t->nil) {
    return;
  }

  inOrder(t, arr, n, index, cur->left);
  if(*index < n) {
    arr[*index] = cur->key;
    *index = *index + 1;
  }
  else {
    return;
  }
  inOrder(t, arr, n, index, cur->right);
}


// 좌회전
void left_rotate(rbtree *t, node_t *x) {

  // y 설정
  node_t *y;
  y = x->right;

  // 서브트리 옮겨달기
  x->right = y->left;
  // y왼쪽 자식이 nil이 아니라면, 부모 설정해주기 ( nil이라면 parent 설정이 의미없다 )
  if(y->left != t->nil) {
    y->left->parent = x;
  }

  // x 부모를 y에게 주기
  // x가 root 였다면 y를 root로
  // 그게 아니라면 부모의 왼쪽/오른쪽 자식 중 하나로 달기
  y->parent = x->parent;
  if(x->parent == t->nil) {
    t->root = y;
  }
  else if(x == x->parent->left) {
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }

  // 옮길 거 다 옮겼으니 x, y 관계 역전
  y->left = x;
  x->parent = y;
}



// 우회전
void right_rotate(rbtree *t, node_t *y) {
  
  // x 설정
  node_t *x;
  x = y->left;

  // 서브트리 옮겨달기
  y->left = x->right;
  if(x->right != t->nil) {
    x->right->parent = y;
  }

  // 부모 옮겨달기
  x->parent = y->parent;
  if(y->parent == t->nil) {
    t->root = x;
  }
  else if(y == y->parent->left) {
    y->parent->left = x;
  }
  else {
    y->parent->right = x;
  }

  // 옮겨달았으니 x, y 관계역전
  x->right = y;
  y->parent = x;
}