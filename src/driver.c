#include "rbtree.h"

int main(int argc, char *argv[]) {
    
    // 트리 생성 함수
    rbtree *tree = new_rbtree();
    rbtree_insert(tree, 10);
    rbtree_insert(tree, 20);
    rbtree_insert(tree, 30);
    rbtree_insert(tree, 40);
    rbtree_insert(tree, 50);
    rbtree_insert(tree, 60);
    rbtree_insert(tree, 70);
    rbtree_insert(tree, 80);

    // tree 전위순회하며 노드 출력
    node_t *cur = tree->root;
    printf("postOrder=========\n");
    postOrder(tree, cur);
    printf("postOrder=========\n");

    // 노드 찾기 함수
    node_t *findedNode = rbtree_find(tree, 90);
    if(findedNode != NULL) {
        printf("finded Node : %d", findedNode->key);
    }
    else {
        printf("finded Node 못 찾았다\n");
    }

    // 가장 작은 값 찾기 함수
    node_t *miniNode = rbtree_min(tree);
    if(miniNode != NULL) {
        printf("min Node : %d\n", miniNode->key);
    }
    else {
        printf("min Node : 트리가 비어있다.");
    }

    // 가장 큰 값 찾기 함수
    node_t *maxNode = rbtree_max(tree);
    if(maxNode != NULL) {
        printf("max Node : %d\n", maxNode->key);
    }
    else {
        printf("max Node : 트리가 비어있다.");
    }

    // // 트리 + 노드들 지우기 함수
    // delete_rbtree(tree);
    // printf("delete 후 postOrder=========\n");
    // postOrder(tree, cur);
    // printf("delete 후 postOrder=========\n");

    // array에 담기 함수
    key_t *arr = (key_t *)malloc(5 * sizeof(key_t));
    rbtree_to_array(tree, arr, 5);
    for(int i = 0; i < 5; i++) {
        printf("array[%d] : %d\n", i, arr[i]);
    }

}

// 후위 순회하며 노드 출력
void postOrder(rbtree *t, node_t *cur) {
    if(cur == t->nil) {
        return;
    }

    postOrder(t, cur->left);
    postOrder(t, cur->right);
    printf("%d\n", cur->key);
}