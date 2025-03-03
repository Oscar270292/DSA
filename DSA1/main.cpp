#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_LEVEL 60
#define MAX_M 100000

typedef struct Node {
    long long value;
    struct Node* forward[MAX_LEVEL];
    struct Node* below;
} Node;

typedef struct SkipList {
    int level;
    Node *header;
} SkipList;

Node *createNode(int level, long long value) {
    Node *n = (Node *)malloc(sizeof(Node) );
    n->value = value;
    n->below = NULL;
    return n;
}

SkipList *createSkipList() {
    SkipList *sl = (SkipList *)malloc(sizeof(SkipList));
    sl->level = 0;
    sl->header = createNode(MAX_LEVEL, INT_MIN);
    for (int i = 0; i < MAX_LEVEL; i++) {
        sl->header->forward[i] = NULL;
    }
    return sl;
}

void printSkipList(SkipList *sl) {
    printf("Skip List Structure:\n");
    printf("Skip List Level:%d\n", sl->level);
    for (int i = sl->level; i >= 0; i--) {
        Node *node = sl->header->forward[i];
        printf("Level %d: ", i);
        while (node != NULL) {
            printf("%d", node->value);
            if (node->below != NULL) {
                printf("d");  // 表示有 below 指针指向下一层
            } else {
                printf("  ");  // 表示没有 below 指针
            }
            node = node->forward[i];
        }
        printf("\n");
    }
}



int coinFlip(long long k, int i) {
    return (k >> (i-1)) &1;
}

void insert(SkipList *sl, long long value) {
    Node *update[MAX_LEVEL];
    Node *x = sl->header;
    Node *prevNode = NULL;
    int lvl = sl->level;

    // initial updates
    for (int i = 0; i < MAX_LEVEL; i++) {
        update[i] = x;
    }

    while (lvl > 0) {
        while (x->forward[lvl] != NULL && x->forward[lvl]->value >= value) {
            x = x->forward[lvl];
        }
        update[lvl] = x;
        if (x->below!=NULL){
            x = x->below;
        }
        lvl--;
    }

    while (x->forward[lvl] != NULL && x->forward[lvl]->value >= value){
        x = x->forward[lvl];
    }
    update[lvl] = x;
    //printf("rihghghghght");
    // 在底层插入新节点
    Node *newNode = createNode(MAX_LEVEL, value);
    newNode->forward[0] = update[0]->forward[0];
    update[0]->forward[0] = newNode;

    // 确定新节点应该存在的层数，并逐层插入新节点
    lvl = 1;
    prevNode = newNode;
    while (coinFlip(value, lvl) && lvl < MAX_LEVEL) {
        Node *newNode = createNode(MAX_LEVEL, value);
        newNode->forward[lvl] = update[lvl]->forward[lvl];
        update[lvl]->forward[lvl] = newNode;

        // 设置 below 指针
        update[lvl]->forward[lvl]->below = prevNode;
        prevNode = newNode;
        //printf("bel = %lld, lvl = %d\n", update[lvl]->forward[lvl]->below->value, lvl);
        lvl++;
    }

    // 更新跳表的最高层
    if (lvl - 1 > sl->level) {
        sl->level = lvl - 1;
    }
    printSkipList(sl);

}

void fastGet(SkipList *sl, long long value) {//new
    Node *x = sl->header;
    int lvl = sl->level;

    if(x->forward[0] == NULL || value > x->forward[0]->value){
        printf("-1\n");
        return;
    }


    while (lvl > 0) {
        while (x->forward[lvl] != NULL && x->forward[lvl]->value >= value) {
            printf("%lld ", x->forward[lvl]->value);
            x = x->forward[lvl];
        }

        if (x->below!=NULL){
            printf("%lld ", x->below->value);
            x = x->below;
        }
        lvl--;
    }
    while (x->forward[lvl] != NULL && x->forward[lvl]->value >= value){
        printf("%lld ", x->forward[lvl]->value);
        x = x->forward[lvl];
    }
    printf("\n");
}



void slowGet(SkipList *sl, long long value) {
    Node *x = sl->header->forward[0];

    if (x == NULL || value > x->value) {
        printf("-1\n");
        return;
    }
    while (x != NULL && x->value >= value) {
        printf("%lld ", x->value);
        x = x->forward[0];
    }
    /*
    if(i != 0){
        for (int j = 0; j < i; ++j) {
                printf("%lld ", result[j]);
            }
            printf("\n");*/
    printf("\n");
    //printSkipList(sl);
}





void removeNode(SkipList *sl, long long data) {
    Node *x = sl->header;
    Node *del;
    Node *update[MAX_LEVEL];

    // 1. 查找节点
    for (int i = sl->level; i >= 0; i--) {


        while (x->forward[i]!= NULL && x->forward[i]->value > data) {
            x = x->forward[i];
        }


        if (x->forward[i] != NULL) {
            if(x->forward[i]->value == data){
                del = x->forward[i];
                x->forward[i] = del->forward[i];  // 移除节点
                free(del);  // 释放节点内存
            }
        }

        if (x->below!=NULL){
            x = x->below;
        }
    }



    // 3. 更新顶层
    for (int i = sl->level; i >= 0; i--){
        if (sl->header->forward[i] == NULL) {
            sl->level--; // 降低层数
        }
    }


}



void freeSkipList(SkipList *sl) {
    Node *current = sl->header;
    while (current != NULL) {
        Node *temp = current;
        current = current->forward[0];
        free(temp);
    }
    free(sl);
}

int main() {
    int M, t;
    long long k;
    SkipList *sl = createSkipList();

    scanf("%d", &M);
    for (int i = 0; i < M; i++) {
        scanf("%d %lld", &t, &k);
        switch (t) {

            case 1:
                slowGet(sl, k);
                break;
            case 2:
                fastGet(sl, k);
                break;

            case 3:
                insert(sl, k);
                break;

            case 4:
                removeNode(sl, k);
                break;
        }
    }

    printSkipList(sl);


    freeSkipList(sl);
    return 0;
}
