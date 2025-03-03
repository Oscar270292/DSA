#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <tgmath.h>

#define total_N 1000000

typedef struct Dungeon{
    long long up_length;
    long long root_length;
    long long treasure;
    int up_index;
    int index;
    struct sib* head;
    struct sib* tail;
    int if_parent;
} Dungeon;

typedef struct sib{
    int label;
    struct sib* next;
}sib ;

Dungeon *createDungeon(int index){
    Dungeon *d = (Dungeon *)malloc(sizeof(Dungeon));
    d->treasure = INT_MIN;
    d->index = index;
    d->head = NULL;
    d->tail = NULL;
    d->up_length = 0;
    d->root_length = 0;
    d->if_parent = 0;
    d->up_index = INT_MIN;
    return d;
}

sib *createsib(int parent_label){
    sib *a = (sib *)malloc(sizeof(sib));
    a->label = parent_label;
    a->next = NULL;
    return a;
}

Dungeon* downstream(int stack[], Dungeon* main[], int* top, Dungeon* cursor, long long ac_length[]) {
    if(cursor->head == NULL){
        printf("-1\n");
    }else{
        printf("%d\n", cursor->head->label);
        (*top)++;
        stack[*top] = cursor->head->label;
        cursor = main[cursor->head->label];
        ac_length[*top] = cursor->root_length;
    }
    return cursor;
}

Dungeon* upstream(int stack[], Dungeon* main[], int* top, Dungeon* cursor){
    if(*top == 0){
        printf("-1\n");
    }else{
        (*top)--;
        printf("%d\n", stack[*top]);
        cursor = main[stack[*top]];
        if(cursor->head->next == NULL){
            cursor->tail = NULL;
        }
        cursor->head = cursor->head->next;
    }
    return cursor;
}

void plan(long long pl, long long ac_length[], int *top, int stack[]){
    int l = 0;
    int r = *top;
    long long tot = ac_length[*top];

    int mid;
    long long val;
//    while(l < r){
//        mid = ((l+r)/2);
//        val = ac_length[mid];
//
//        if (pl < (tot - val)){
//            l = mid + 1;
//        }else if(pl >= (tot - val)){
//            r = mid;
//        }
//        mid = ((l+r)/2);
//    }
//    printf("%d\n", stack[mid]);

    while(l < r){
        mid = l + (r - l) / 2;  // 只在這裡計算一次 mid
        val = ac_length[mid];

        if (pl < (tot - val)){
            l = mid + 1;
        }else{
            r = mid;
        }
    }
    // 現在 l 和 r 應該是相等的，因此 l 或 r 都可以使用
    printf("%d\n", stack[l]);


    return;
}

//void plan(long long pl, long long ac_length[], int *top, int stack[]){
//    int l = 0;
//    int r = *top;
//    long long tot = ac_length[*top];
//
//    int mid;
//    int res = -1;
//    long long val;
//    while(l <= r){
//        mid = ((l+r)/2);
//        val = ac_length[mid];
//
//        if (pl < (tot - val)){
//            l = mid + 1;
//        }else if(pl > (tot - val)){
//            res = mid;
//            r = mid - 1;
//        }else if(pl == (tot - val)){
//            res = mid;
//            break;
//        }
//    }
//    //printf("mid = %d\n", mid);
//    if (res != -1){
//        printf("%d\n", stack[res]);
//    }else{
//        printf("%d\n", stack[mid]);
//    }
//
//    return;
//
//}



void free_sib_list(sib* head) {
    sib* current = head;
    while (current != NULL) {
        sib* temp = current;
        current = current->next;
        free(temp);
    }
}

void free_all_resources(Dungeon** main, int N) {
    for (int i = 0; i < N; i++) {
        if (main[i] != NULL) {
            free_sib_list(main[i]->head); // 釋放 sib 鏈表
            free(main[i]);                // 釋放 Dungeon 結構
        }
    }
}



int main() {
    int N, M, Q;
    int ui, vi, l;
    int op;
    scanf("%d %d %d", &N, &M, &Q);
    Dungeon* main[N];

    for(int i = 0; i<N; i++){
        main[i] = createDungeon(i);
    }
    //create map
    for(int j = 0; j<M; j++){
        scanf("%d %d %d", &ui, &vi, &l);

        if (main[ui]->if_parent == 0){
            main[ui]->if_parent = 1;
            sib* first_sib = createsib(vi);
            main[ui]->head = first_sib;
            main[ui]->tail = first_sib;
        }else{
            sib* child_sib = createsib(vi);
            main[ui]->tail->next = child_sib;
            main[ui]->tail = main[ui]->tail->next;
        }

        main[vi]->up_index = ui;
        main[vi]->up_length = l;
        main[vi]->root_length = main[ui]->root_length + l;
    }

    int stack[N];
    int top = -1;
    stack[0] = 0;
    top++;
    Dungeon* cursor = main[0];

    //for case 3
    long long ac_length[N];
    ac_length[0] = 0;
    long long pl;

    for (int k = 0; k < Q; k++){
        scanf("%d", &op);
        switch (op) {
            case 1:
                cursor = downstream(stack, main, &top, cursor, ac_length);
                break;
            case 2:
                cursor = upstream(stack, main, &top, cursor);
                break;
            case 3:
                scanf(" %lld", &pl);
                plan(pl, ac_length, &top, stack);
                break;
        }

    }

    printf("top = %d\n", top);
    printf("stack = ");
    for (int i = 0; i < top+1; i++){
        printf("%d ", stack[i]);
    }
    printf("\n");
    printf("ac_length = ");
    for (int i = 0; i < top+1; i++){
        printf("%d ", ac_length[i]);
    }
    printf("\n");


    free_all_resources(main, N);
}