#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define total_N 1000000

typedef struct Dungeon{
    int length;
    long long treasure;
    int label;
    struct Dungeon *sib;
    struct Dungeon *next;
    struct Dungeon *child;
} Dungeon;

typedef struct Abyss{
    Dungeon *current;
    Dungeon *main; //考慮拆掉
    Dungeon *branch[total_N-1];
} Abyss;

Dungeon *createdungeon(int label){
    Dungeon *d = (Dungeon *)malloc(sizeof(Dungeon));
    d->next = NULL;
    d->sib = NULL;
    d->child = NULL;
    d->treasure = INT_MIN;
    d->label = label;
    d->length = 0;
    return d;
}

Abyss *createabyss(){
    Abyss *a = (Abyss *)malloc(sizeof(Abyss));
    for(int i = 0; i<total_N-1; i++){
        a->branch[i] = NULL;
    }
    a->current = NULL;
    a->main = NULL;
    return a;
}

void downstream(Dungeon **stack, int *top, Abyss *abyss) {
    if(abyss->current->next == NULL){
        printf("-1");
    }else{
        printf("%d", abyss->current->next->label);
        stack[*top] = abyss->current->next;
        (*top)++;
        abyss->current = abyss->current->next;
    }
    return;
}


void printDungeonDetails(Dungeon *dungeon, int level) {
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    if (dungeon) {
        printf("    Label: %d, Treasure: %lld, Length: %d\n", dungeon->label, dungeon->treasure, dungeon->length);
    }
}

void printDungeonHierarchy(Dungeon *dungeon, int level) {
    if (!dungeon) return;

    printDungeonDetails(dungeon, level);

    if (dungeon->child) {
        printDungeonHierarchy(dungeon->child, level + 1);
    }

    if (dungeon->sib) {
        printDungeonHierarchy(dungeon->sib, level);
    }
}

void printBranches(Dungeon **branches, int total) {
    for (int i = 0; i < total; i++) {
        if (branches[i]) {
            printf("Branch %d:\n", i);
            printDungeonHierarchy(branches[i]->child, 1);
        }
    }
}

void printAbyss(Abyss *abyss) {
    printf("Current Dungeon Status:\n");
    printDungeonDetails(abyss->current, 0);

    printf("\nMain Dungeon Structure:\n");
    Dungeon *current = abyss->main;
    while (current != NULL) {
        printDungeonDetails(current, 0);
        current = current->next;
    }

    printf("\nBranch Dungeons:\n");
    printBranches(abyss->branch, 100);  // Adjust this according to the actual number of branches
}


int main() {
    int N, M, Q;
    int ui, vi, l;
    int T;

    Abyss *abyss = createabyss();
    scanf("%d %d %d", &N, &M, &Q);

    for(int i = 0; i<N; i++){
        abyss->branch[i] = createdungeon(i);
    }

    for(int j = 0; j<M; j++){
        scanf("%d %d %d", &ui, &vi, &l);

        Dungeon* cursor = abyss->branch[ui];
        abyss->branch[vi]->length = l;
        if(cursor->next != NULL){
            if(cursor->child != NULL){

                cursor = cursor->child;
                while(cursor->sib != NULL){
                    cursor = cursor->sib;
                }
                cursor->sib = abyss->branch[vi];

            }else{
                cursor->child = abyss->branch[vi];
            }

        }else{
            cursor->next = abyss->branch[vi];
        }
        //abyss->branch[vi] = NULL;
    }
    //deal with main
//    if(abyss->branch[0]->child == NULL){
//        abyss->main = abyss->branch[0];
//    }else{
//        Dungeon* temp = abyss->branch[0]->child;
//        abyss->branch[0]->child = NULL;
//        abyss->main = abyss->branch[0];
//        abyss->branch[0] = temp;
//    }
    abyss->main = abyss->branch[0];
    abyss->current = abyss->branch[0];
    printAbyss(abyss);
    //printf("main = %d %d %d\n", abyss->main->label, abyss->main->next->label, abyss->main->next->next->label);
    //printf("branch = %d %d %d\n", abyss->branch[0]->label, abyss->branch[0]->child->label, abyss->branch[0]->child->sib->label);

    Dungeon* stack[total_N];
    int top = 0;
    stack[top] = abyss->current;
    top++;

    for (int k = 0; k < 1; k++){
        scanf("%d", &T);
        switch (T) {
            case 1:
                if(abyss->current->next == NULL){
                    printf("-1");
                }else{
                    if (abyss != NULL && abyss->current != NULL && abyss->current->next != NULL) {
                        printf("%d", abyss->current->next->label);
                        stack[top] = abyss->current->next;
                        (top)++;
                        abyss->current = abyss->current->next;
                    }


                }
        }
    }
    printAbyss(abyss);

    for(int i = 0; i<N; i++){
        free(abyss->branch[i]);
    }
    free(abyss);
    return 0;
}


