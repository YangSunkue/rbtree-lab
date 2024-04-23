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
  node_t *x;
  node_t *y = p;
  color_t y_origin_color = y->color;

  // 자녀가 1개 이하일 때 
  if(p->left == t->nil) {
    x = p->right;
    rbtree_transplant(t, p, p->right);
  }
  else if(p->right == t->nil) {
    x = p->left;
    rbtree_transplant(t, p, p->left);
  }

  // 자녀가 2개일 때
  else {
    // 후임자 y에 담기
    y = node_successor(t, p);
    y_origin_color = y->color;

    // x는 y자식이다
    x = y->right;

    // y가 삭제노드 자식이면 x는 y의 right에 그대로 연결되어 있다
    if(y->parent == p) {
      x->parent = y;
    }
    else {
      // 아닐 경우 y자식을 y자리에 대체한다
      rbtree_transplant(t, y, y->right);

      // 삭제노드 right 자식을 y에 달기
      y->right = p->right;
      y->right->parent = y;
    }
    
    // y를 삭제노드 자리로 옮기기
    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  // 노드 삭제 후 메모리 반환
  free(p);
  p = NULL;

  // y 원래 자리를 x가 대체했는데 삭제된 색이 black이라면 x가 doubly black 또는 red and black 이므로 fixup 실행
  if(y_origin_color == RBTREE_BLACK) {
    rbtree_erase_fixup(t, x);
  }
  return 0;
}



// x가 red(red and black)라면 while문 넘겨서 바로 black
// x가 black(doubly black)이라면 while문 진행
void rbtree_erase_fixup(rbtree *t, node_t *p) {

  // p가 root거나 red and black이라면 조건 없이 black으로 바꾸기
  while(p != t->root && p->color == RBTREE_BLACK) {
    node_t *w;

    // 형제 지정
    if(p == p->parent->left) {
      w = p->parent->right;
      
      // case 1 : 형제가 red일 경우, 색깔 바꾸고 부모기준 돌리기
      // 형제가 black 되었으므로 case 2, 3, 4 중 한개 하기
      if(w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        p->parent->color = RBTREE_RED;

        left_rotate(t, p->parent);
        // 돌렸으니 형제 재지정
        w = p->parent->right;
      }

      // case 2 : 형제가 black인데 형제 자식도 다 black인 경우, 나/형제꺼 black 올리기
      // 올린후 부모 기준으로 "처음부터" 시작하기
      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        // doubly는 정상 black이 되니까 색깔변경 안함, 형제만 red가 됨
        w->color = RBTREE_RED;
        // 부모에게 extra black 줬으니 부모부터 다시 시작한다
        p = p->parent;
      }
      // case 3: 형제의 red자식이 꺾여있을 경우
      else if(w->right->color == RBTREE_BLACK) {
        // 형제와 형제자식 색 바꾸고 회전 ( 1자로 만들기 )
        w->left->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        right_rotate(t, w);

        // case 3 이후엔 case 4를 꼭 해야 한다.
        // 형제 재지정 후 case 4 실행
        w = p->parent->right;
        
        // 형제는 부모 색 물려받기
        // 부모/형제자식은 black으로 만들고 회전
        // 이러면 doubly는 없어진다.
        w->color = p->parent->color;
        p->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, p->parent);
        
        // case 4를 했으면 무조건 doubly가 없어지므로, while문 탈출을 위해 root를 넣어준다.
        p = t->root;
      }

      // case 4 : 형제는 black, 형제 red자식이 1자일 경우
      else {
        // 형제는 부모 색 물려받기
        // 부모/형제자식은 black으로 만들고 회전
        // 이러면 doubly는 없어진다.
        w->color = p->parent->color;
        p->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, p->parent);
        
        // case 4를 했으면 무조건 doubly가 없어지므로, while문 탈출을 위해 root를 넣어준다.
        p = t->root;
      }
    }


    //////////////////////////반대 경우//////////////////////////////////
    else {
      w = p->parent->left;
      
      // case 1 : 형제가 red일 경우, 색깔 바꾸고 부모기준 돌리기
      // 형제가 black 되었으므로 case 2, 3, 4 중 한개 하기
      if(w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        p->parent->color = RBTREE_RED;

        right_rotate(t, p->parent);
        // 돌렸으니 형제 재지정
        w = p->parent->left;
      }

      // case 2 : 형제가 black인데 형제 자식도 다 black인 경우, 나/형제꺼 black 올리기
      // 올린후 부모 기준으로 "처음부터" 시작하기
      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        // doubly는 정상 black이 되니까 색깔변경 안함, 형제만 red가 됨
        w->color = RBTREE_RED;
        // 부모에게 extra black 줬으니 부모부터 다시 시작한다
        p = p->parent;
      }
      // case 3: 형제의 red자식이 꺾여있을 경우
      else if(w->left->color == RBTREE_BLACK) {
        // 형제와 형제자식 색 바꾸고 회전 ( 1자로 만들기 )
        w->right->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        left_rotate(t, w);
        // case 3 이후엔 case 4를 꼭 해야 한다.



        // 형제 재지정 후 case 4 실행
        w = p->parent->left;
        
        // 형제는 부모 색 물려받기
        // 부모/형제자식은 black으로 만들고 회전
        // 이러면 doubly는 없어진다.
        w->color = p->parent->color;
        p->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, p->parent);
        
        // case 4를 했으면 무조건 doubly가 없어지므로, while문 탈출을 위해 root를 넣어준다.
        p = t->root;
      }

      // case 4 : 형제는 black, 형제 red자식이 1자일 경우
      else {
        // 형제는 부모 색 물려받기
        // 부모/형제자식은 black으로 만들고 회전
        // 이러면 doubly는 없어진다.
        w->color = p->parent->color;
        p->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, p->parent);
        
        // case 4를 했으면 무조건 doubly가 없어지므로, while문 탈출을 위해 root를 넣어준다.
        p = t->root;
      }
    }
  }
  p->color = RBTREE_BLACK;
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



// 노드 대체 함수, v를 u자리로 옮긴다
void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {

  // u가 root였을 경우 v를 root로 설정
  // 아니라면 left, right 중 맞는 자리로 설정
  if(u->parent == t->nil) {
    t->root = v;
  }
  else if(u == u->parent->left) {
    u->parent->left = v;
  }
  else {
    u->parent->right = v;
  }
  
  // 부모 설정
  v->parent = u->parent;
}



// 특정 노드의 후임자 찾기
node_t *node_successor(rbtree *t, node_t *node) {

  // TODO: implement find
  node_t *cur = node->right;
  node_t *mini;

  // 후임자가 없을 경우 NULL 반환
  if(cur == t->nil) {
    return NULL;
  }

  while(cur != t->nil) {
    mini = cur;
    cur = cur->left;
  }

  return mini;
}