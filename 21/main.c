#include <stdio.h>
#include <stdlib.h>

// 定義節點結構
struct Node {
    char color;
    int key;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
};

// 定義紅黑樹結構
struct RBT {
    struct Node* root;
    struct Node* nil;
};

// 創建新節點
struct Node* newNode(struct RBT* T, char color, int key) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->color = color;
    node->key = key;
    node->left = T->nil;
    node->right = T->nil;
    node->parent = T->nil;
    return node;
}

// 左旋操作
void LeftRotate(struct RBT* T, struct Node* x) {
    struct Node* y = x->right;
    x->right = y->left;
    if (y->left != T->nil)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == T->nil)
        T->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// 右旋操作
void RightRotate(struct RBT* T, struct Node* x) {
    struct Node* y = x->left;
    x->left = y->right;
    if (y->right != T->nil)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == T->nil)
        T->root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

// 修正插入後的紅黑樹
void insertFixup(struct RBT* T, struct Node* z) {
    while (z->parent->color == 'R') {
        if (z->parent == z->parent->parent->left) {
            struct Node* y = z->parent->parent->right;
            if (y->color == 'R') {
                z->parent->color = 'B';
                y->color = 'B';
                z->parent->parent->color = 'R';
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    LeftRotate(T, z);
                }
                z->parent->color = 'B';
                z->parent->parent->color = 'R';
                RightRotate(T, z->parent->parent);
            }
        } else {
            struct Node* y = z->parent->parent->left;
            if (y->color == 'R') {
                z->parent->color = 'B';
                y->color = 'B';
                z->parent->parent->color = 'R';
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    RightRotate(T, z);
                }
                z->parent->color = 'B';
                z->parent->parent->color = 'R';
                LeftRotate(T, z->parent->parent);
            }
        }
    }
    T->root->color = 'B';
}

// 插入新節點到紅黑樹
void insert(struct RBT* T, struct Node* z) {
    struct Node* y = T->nil;
    struct Node* x = T->root;
    while (x != T->nil) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    z->parent = y;
    if (y == T->nil)
        T->root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;
    z->left = T->nil;
    z->right = T->nil;
    // 這裡我們不自動設置顏色，保留節點的初始顏色
    if (z->color == 'R' && z->parent->color == 'R') {
        insertFixup(T, z);
    }
}

// 打印每個節點的父節點、左子節點和右子節點
void printTreeStructure(struct RBT* T, struct Node* node) {
    if (node != T->nil) {
        printf("Node %d (%c): Parent = %d, Left = %d, Right = %d\n",
               node->key,
               node->color,
               node->parent != T->nil ? node->parent->key : -1,
               node->left != T->nil ? node->left->key : -1,
               node->right != T->nil ? node->right->key : -1);
        printTreeStructure(T, node->left);
        printTreeStructure(T, node->right);
    }
}

// 查找節點
struct Node* findNode(struct RBT* T, struct Node* root, int key) {
    if (root == T->nil || root->key == key)
        return root;
    if (key < root->key)
        return findNode(T, root->left, key);
    else
        return findNode(T, root->right, key);
}

// 執行操作
void executeOperations(struct RBT* T, int M, char* ops, int* keys) {

    for (int i = 0; i < M; i++) {
        char op = ops[i];
        int key = keys[i];
        struct Node* node = findNode(T, T->root, key);
//        printf("Node %d (%c): Parent = %d, Left = %d, Right = %d\n",
//               node->key,
//               node->color,
//               node->parent != T->nil ? node->parent->key : -1,
//               node->left != T->nil ? node->left->key : -1,
//               node->right != T->nil ? node->right->key : -1);

        if (op == 'L') {
            LeftRotate(T, node);
//            printf("Node %d (%c): Parent = %d, Left = %d, Right = %d\n",
//                   node->key,
//                   node->color,
//                   node->parent != T->nil ? node->parent->key : -1,
//                   node->left != T->nil ? node->left->key : -1,
//                   node->right != T->nil ? node->right->key : -1);
        } else if (op == 'R') {
            RightRotate(T, node);
//            printf("Node %d (%c): Parent = %d, Left = %d, Right = %d\n",
//                   node->key,
//                   node->color,
//                   node->parent != T->nil ? node->parent->key : -1,
//                   node->left != T->nil ? node->left->key : -1,
//                   node->right != T->nil ? node->right->key : -1);
        } else if (op == 'P') {
            int d = 0;
            int bd = 0;
            struct Node* temp = node;
//            if(node->color = 'B' && node != T->root){
//                bd--;
//            }
            while(node != T->root){
                node = node->parent;
                d++;
                if(node->color == 'B'){
                    bd++;
                }

            }
            printf("%d %d %d %d\n",
                   temp->left != T->nil ? temp->left->key : -1,
                   temp->right != T->nil ? temp->right->key : -1,
                   bd, d);
        }
    }
}

// 主函數
int main() {
    int N, M;
    scanf("%d", &N);

    struct RBT T;
    T.nil = (struct Node*)malloc(sizeof(struct Node));
    T.nil->color = 'B';
    T.nil->left = T.nil->right = T.nil->parent = NULL;
    T.root = T.nil;

    char* colors = (char*)malloc(N * sizeof(char));
    int* keys = (int*)malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) {
        scanf(" %c %d", &colors[i], &keys[i]);
    }

    for (int i = 0; i < N; i++) {
        struct Node* newNodePtr = newNode(&T, colors[i], keys[i]);
        insert(&T, newNodePtr);
    }

    // 打印樹的結構
    //printf("Tree structure after build:\n");
    //printTreeStructure(&T, T.root);

    scanf("%d", &M);
    char* ops = (char*)malloc(M * sizeof(char));
    int* opKeys = (int*)malloc(M * sizeof(int));
    for (int i = 0; i < M; i++) {
        scanf(" %c %d", &ops[i], &opKeys[i]);
    }

    executeOperations(&T, M, ops, opKeys);

    return 0;
}
