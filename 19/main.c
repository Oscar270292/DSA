#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


typedef struct node {
    long long val;
    int t_val;
    int h_val;
    long long sum;
    int lazy;
    int left_r;
    int right_r;
    struct node* left;
    struct node* right;
} node;

typedef struct treap {
    struct node* root;
} treap;

typedef struct Pair {
    node *first;
    node *second;
} Pair;

node* createnode(long long val, int t_val, int h_val) {
    node *n = (node *)malloc(sizeof(node));
    n->val = val;
    n->t_val = t_val;
    n->h_val = h_val;
    n->sum = val;
    n->lazy = 0;
    n->left_r = t_val;
    n->right_r = t_val;
    n->left = NULL;
    n->right = NULL;
    return n;
}

treap* createtreap(){
    treap *t = (treap *)malloc(sizeof(treap));
    t->root = NULL;
    return t;
}

Pair makePair(node *first, node *second) {
    Pair p;
    p.first = first;
    p.second = second;
    return p;
}

void push(node* node){
    if(node == NULL) return;
    if (node->lazy != 0){
        node->t_val += node->lazy;
        if (node->left != NULL) node->left->lazy += node->lazy;
        if (node->right != NULL) node->right->lazy += node->lazy;
        node->lazy = 0;
    }
}

void pull(node* node){
    if(node == NULL) return;
    if(node->left == NULL && node->right == NULL){
        node->left_r = node->t_val + node->lazy;
        node->right_r = node->left_r;
    }
    if(node->left != NULL){
        node->left_r = node->left->left_r + node->left->lazy;
    }else{
        node->left_r = node->t_val + node->lazy;
    }
    if(node->right != NULL){
        //node->right_r = node->right->right_r + node->right->lazy;
        node->right_r = node->right->right_r + node->right->lazy;
    }else{
        node->right_r = node->t_val + node->lazy;
    }
}


Pair Split(node *root, int k) {
    if (root == NULL) {
        return makePair(NULL, NULL);
    }
    push(root);
    //pull(root);
    if (root->t_val <= k) {
        Pair split = Split(root->right, k);

        root->right = split.first;
        pull(root);
        // Update the sum for the current root
        root->sum = root->val + (root->left ? root->left->sum : 0) + (root->right ? root->right->sum : 0);
        return makePair(root, split.second);
    } else {
        Pair split = Split(root->left, k);

        root->left = split.second;
        pull(root);
        // Update the sum for the current root
        root->sum = root->val + (root->left ? root->left->sum : 0) + (root->right ? root->right->sum : 0);
        return makePair(split.first, root);
    }
}

node* Merge(node* l, node* r){
    if (l == NULL) {
        //printf("A");
        return r;
    }
    if (r == NULL) {
        //printf("B");
        return l;
    }
    //push(l);
    //pull(l);
    //push(r);
    //pull(r);
    if (l->h_val > r->h_val) {
        //printf("C");
        push(l);
        l->right = Merge(l->right, r);
        pull(l);
        // Update sum assuming children might be NULL
        l->sum = l->val;
        if (l->left != NULL) l->sum += l->left->sum;
        if (l->right != NULL) l->sum += l->right->sum;
        return l;
    } else {
        //printf("D");
        push(r);
        r->left = Merge(l, r->left);
        pull(r);
        // Update sum assuming children might be NULL
        r->sum = r->val;
        if (r->left != NULL) r->sum += r->left->sum;
        if (r->right != NULL) r->sum += r->right->sum;
        return r;
    }
}

int getRandom(int max) {
    return rand() % (max + 1);
}

void insert(treap* treap1, int k, int a, int N_MAX){
    treap* treap_a = createtreap();
    treap* treap_b = createtreap();
    Pair  p = Split(treap1->root, k);
    treap_a->root = p.first;
    treap_b->root = p.second;
    //printf("aRoot = %d, bRoot = %d\n", treap_a->root->val, treap_b->root->val);
    if (treap_b->root != NULL) treap_b->root->lazy += 1;

    int randomNum = getRandom(N_MAX);
    node* new_node = createnode(a, k+1, randomNum);

    treap_a->root = Merge(treap_a->root, new_node);
    treap1->root = Merge(treap_a->root, treap_b->root);
    free(treap_a);
    free(treap_b);
}

void delete(treap* treap1, int k){
    treap* treap_a = createtreap();
    treap* treap_b = createtreap();
    Pair  p = Split(treap1->root, k-1);
    treap_a->root = p.first;
    treap_b->root = p.second;
    //printf("aRoot = %d, bRoot = %d\n", treap_a->root->val, treap_b->root->val);

    Pair p1 = Split(treap_b->root, k);
    treap_b->root = p1.second;
    free(p1.first);
    //printf("aRoot = %d, bRoot = %d\n", p1.first->val, treap_b->root->val);
    if (treap_b->root != NULL) treap_b->root->lazy -= 1;

    treap1->root = Merge(treap_a->root, treap_b->root);
    free(treap_a);
    free(treap_b);
}

long long calSum(node* root_n, int ll, int rr){
    if (root_n == NULL) return 0;
    push(root_n);
    pull(root_n);
    if (root_n->left_r == ll && root_n->right_r == rr) {

        return root_n->sum;
    }

    long long c_sum = 0;
    int min, max;
    int kc = (root_n->t_val);
    // 如果当前节点的位置在[l, r]范围内，加入当前节点的值
    if (kc >= ll && kc <= rr) {
        c_sum += root_n->val;
    }

    // 如果左子树有可能在[l, r]范围内，递归计算左子树的和
    if (ll < kc) {
        if(kc - 1 < rr){
            min = kc - 1;
        }else{
            min = rr;
        }
        c_sum += calSum(root_n->left, ll, min);
    }

    // 如果右子树有可能在[l, r]范围内，递归计算右子树的和
    if (rr > kc) {
        if(ll > kc + 1){
            max = ll;
        }else{
            max = kc + 1;
        }
        c_sum += calSum(root_n->right, max, rr);
    }

    return c_sum;
}

void printInOrder(node* root) {
    if (root != NULL) {
        printInOrder(root->left);
        //printf("Node Value: %lld, t_val: %lld, h_val: %lld, sum: %lld, lazy: %d\n", root->val, root->t_val, root->h_val, root->sum, root->lazy);
        printf("Node Value: %lld, lr: %d, rr: %d, sum: %lld, t_val: %d, lazy: %d\n", root->val, root->left_r, root->right_r, root->sum, root->t_val, root->lazy);
        if (root->left != NULL)
            printf("    Left Child: %lld\n", root->left->val);
        else
            printf("    Left Child: NULL\n");

        if (root->right != NULL)
            printf("    Right Child: %lld\n", root->right->val);
        else
            printf("    Right Child: NULL\n");
        printInOrder(root->right);
    }
}

void InOrder_val(node* root) {
    if (root != NULL) {
        InOrder_val(root->left);
        printf("%lld ", root->val);
        InOrder_val(root->right);
    }
}

void printTreap(treap* t) {
    if (t != NULL && t->root != NULL) {
        printf("Treap nodes in order:\n");
        printInOrder(t->root);
    } else {
        printf("Treap is empty.\n");
    }
}


int main() {
    int N, tot;
    int T;
    scanf("%d %d", &N, &T);
    tot = N;
    srand(21);

    treap* treap1 = createtreap();
    node* node1;
    int arr;
    for(int i = 0;i<N;i++){
        scanf("%d", &arr);
        int randomNumber = getRandom(INT_MAX);
        node1 = createnode(arr, i+1, randomNumber);
        treap1->root = Merge(treap1->root, node1);
        //printf("\n");
    }
    //printTreap(treap1);

    int op;
    int k1, k2, a;
    int ll, rr;
    long long sum;
    for (int j = 0; j < T; j++) {
        scanf("%d ", &op);
        switch (op) {
            case 1:
                scanf("%d", &k1);
                delete(treap1, k1);
                tot--;
                break;
            case 2:
                scanf("%d %d", &k2, &a);
                insert(treap1, k2, a, INT_MAX);
                tot++;
                break;
            case 3:
                scanf("%d %d", &ll, &rr);
                sum = calSum(treap1->root, ll, rr);
                printf("%lld\n", sum);
                //printTreap(treap1);
                break;
        }
        //printTreap(treap1);
    }



    printf("%d\n", tot);
    InOrder_val(treap1->root);


    return 0;
}
