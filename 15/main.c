#include <stdio.h>
#include <stdlib.h>


int time;
int ans = 0;
int check = 0;
int tot_check = 1;


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
    int scc_lab;
    struct node* head;
    struct node* tail;
} a_list;

typedef struct t_s {
    struct node* head;
} t_s;

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
    al->scc_lab = -1;
    al->head = NULL;
    al->tail = NULL;
    return al;
}

t_s* createts(){
    t_s *ts = (t_s *)malloc(sizeof(t_s));
    ts->head = NULL;
    return ts;
}

void DFS_Visit(a_list* a[], int ind, t_s* top){
    time++;
    a[ind]->discover = time;
    a[ind]->color = 1;

    node* cur = a[ind]->head;
    while (cur != NULL) {
        if (a[cur->id]->color == 0) {
            cur->E = 'T';
            a[cur->id]->parent = ind;
            DFS_Visit(a, cur->id, top);
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

    node* temp = a[ind];
    temp->next = top->head;
    top->head = temp;
}

void DFS(a_list* a[], int B, t_s* top){

    time = 0;
    for (int i = 0; i < B; i++) {
        if (a[i]->color == 0) {
            DFS_Visit(a, i, top);
        }
    }
}

void DFS_t_Visit(a_list* a_T[], int ind, t_s* top, t_s* top_T) {
    time++;
    a_T[ind]->discover = time;
    a_T[ind]->color = 1;

    node *cur = a_T[ind]->head;
//    if(cur!= NULL){
//        if(a_T[cur->id]->scc_lab == ans && ans != 0){
//            check = 1;
//        }
//    }


    while (cur != NULL) {
        if(a_T[cur->id]->scc_lab == ans && ans != 0){
            check = 1;
        }
        if (a_T[cur->id]->color == 0) {


            cur->E = 'T';
            a_T[cur->id]->parent = ind;
            DFS_t_Visit(a_T, cur->id, top, top_T);
        } else if (a_T[cur->id]->color == 1) {
            cur->E = 'B';
        } else {
            if (a_T[ind]->discover >= a_T[cur->id]->discover) {
                cur->E = 'C';
            } else {
                cur->E = 'F';
            }

        }
        cur = cur->next;
    }


    a_T[ind]->color = 2;
    a_T[ind]->scc_lab = (ans+1);
    time++;
    a_T[ind]->finish = time;

    node* temp_T = a_T[ind];
    temp_T->next = top_T->head;
    top_T->head = temp_T;
}



void DFS_t(a_list* a_T[], int B, t_s* top, t_s* top_T, int cc){
    time = 0;
    node* cus = top->head;
    int i = 0;
    while(cus != NULL){
        if (a_T[cus->id]->color == 0) {
            DFS_t_Visit(a_T, cus->id, top, top_T);
            ans++;

            if(check != 1 && i != 0){
                tot_check = 0;
                //printf("A = %d\n", cus->id);
            }
            check = 0;
        }
        cus = cus->next;
        i++;

    }

}


void printAList(a_list* list) {
    printf("List ID: %d, scc_lab = %d\n", list->a_id+1, list->scc_lab);
    node* cursor = list->head;
    while (cursor != NULL) {
        printf("ID: %d, E: %c -> ", cursor->id+1, cursor->E);
        cursor = cursor->next;
    }
    printf("NULL\n");
}

void printAllLists(a_list** a, int V) {
    for (int i = 0; i < V; i++) {
        printf("List %d:\n", i+1);
        printAList(a[i]);
    }
}

void printTS(t_s* ts) {
    node* current = ts->head;
    printf("Topological Sort Order (Based on Finish Times):\n");
    while (current != NULL) {
        printf("Node ID: %d -> ", current->id + 1); // Assuming node ID should be 1-based in output
        current = current->next;
    }
    printf("NULL\n");
}



int main() {
    int B;
    int A;
    int mode;
    scanf("%d %d %d", &B, &A, &mode);

    a_list* a[B];
    a_list* a_T[B];
    for (int i = 0; i < B; i++) {
        a[i] = createlist(i);
        a_T[i] = createlist(i);
    }

    int ui, vi;
    for (int j = 0; j < A; j++){
        scanf("%d %d", &ui, &vi);
        ui--;
        vi--;
        node* cursor = createnode(vi);
        if (a[ui]->head == NULL) {
            a[ui]->head = cursor;
            a[ui]->tail = a[ui]->head;
        } else {
            a[ui]->tail->next = cursor;
            a[ui]->tail = a[ui]->tail->next;
        }


        node* cursor2 = createnode(ui);
        if (a_T[vi]->head == NULL) {
            a_T[vi]->head = cursor2;
            a_T[vi]->tail = a_T[vi]->head;
        } else {
            a_T[vi]->tail->next = cursor2;
            a_T[vi]->tail = a_T[vi]->tail->next;
        }


    }

    t_s* top = createts();
    t_s* top_T = createts();
    DFS(a, B, top);
    int cc = top->head->id;
    DFS_t(a_T, B, top, top_T, cc);

    printf("%d\n", ans);
    if(mode == 2){
        printf("%d\n", tot_check);
    }

//    printf("SCC = %d\n", ans);
//    printf("mode2 = %d\n", tot_check);
//    printAllLists(a, B);
//    printf("===================\n");
//    printAllLists(a_T, B);
//    printf("===================\n");
//    printTS(top);
//    printf("===================\n");
//    printTS(top_T);



    return 0;
}
