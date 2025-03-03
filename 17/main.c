#include <stdio.h>
#include <stdlib.h>

int time;


typedef struct node {
    int id;
    char E;
    struct node* next;
} node;

typedef struct a_list {
    int a_id;
    int color; // white 0 gray 1 black 2
    int discover;
    int finish;
    int parent;
    struct node* head;
} a_list;

node* createnode(int id) {
    node *n = (node *)malloc(sizeof(node));
    n->id = id;
    n->E = 'N';
    n->next = NULL;
    return n;
}

a_list* createlist(int a_id) {
    a_list *al = (a_list *)malloc(sizeof(a_list));
    al->a_id = a_id;
    al->color = 0;
    al->discover = 0;
    al->finish = 0;
    al->parent = -1;
    al->head = NULL;
    return al;
}

void DFS_Visit(a_list* a[], int ind){
    time++;
    a[ind]->discover = time;
    a[ind]->color = 1;

    node* cur = a[ind]->head;
    while (cur != NULL) {
        if (a[cur->id]->color == 0) {
            cur->E = 'T';
            a[cur->id]->parent = ind;
            DFS_Visit(a, cur->id);
        }else if(a[cur->id]->color == 1){
            cur->E = 'B';
        }else{
            if(a[ind]->discover >= a[cur->id]->discover){
                cur->E = 'C';
            }else{
                cur->E = 'F';
            }

        }
        cur = cur->next;
    }
    a[ind]->color = 2;
    time++;
    a[ind]->finish = time;
}

void DFS(a_list* a[], int V){

    time = 0;
    for (int i = 0; i < V; i++) {
        if (a[i]->color == 0) {
            DFS_Visit(a, i);
        }
    }
}


void printAList(a_list* list) {
    printf("List ID: %d\n", list->a_id);
    node* cursor = list->head;
    while (cursor != NULL) {
        printf("ID: %d, E: %c -> ", cursor->id, cursor->E);
        cursor = cursor->next;
    }
    printf("NULL\n");
}

void printAllLists(a_list** a, int V) {
    for (int i = 0; i < V; i++) {
        printf("List %d:\n", i);
        printAList(a[i]);
    }
}

int main() {
    int V;
    int deg;
    int num;
    scanf("%d", &V);

    a_list* a[V];
    for (int i = 0; i < V; i++) {
        a[i] = createlist(i);
    }

    for (int j = 0; j < V; j++) {
        scanf("%d", &deg);
        node* cursor = NULL;
        for (int k = 0; k < deg; k++) {
            scanf("%d", &num);
            num--;
            if (a[j]->head == NULL) {
                a[j]->head = createnode(num);
                cursor = a[j]->head;
            } else {
                cursor->next = createnode(num);
                cursor = cursor->next;
            }
        }
    }

    DFS(a,V);

    for (int s = 0; s < V; s++){
        printf("%d ", s+1);
        node* current = a[s]->head;
        while(current != NULL){
            if(current->E == 'N'){
                continue;
            }else{
                printf("%c", current->E);
            }
            current = current->next;
        }
        printf("\n");
    }

    //printAllLists(a, V);

    return 0;
}
