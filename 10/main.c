#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Node{
    int order;
    int job;
    int priority;
    //struct Node* sibling;
    struct Node *children_tail;
    struct Node *sib;
    struct Node *child;
} Node;

typedef struct Head_Node{
    int order;
    struct Head_Node* next;
    struct Head_Node* previous;
    Node* root;
} Head_Node;

typedef struct Head_List {
    Head_Node *head;
    int num;
    Head_Node *highest_pri;
} Head_List;

Head_Node *createHNode(int order) {
    Head_Node *n = (Head_Node *)malloc(sizeof(Head_Node));
    n->order = order;
    n->next = NULL;
    n->previous = NULL;
    n->root = NULL;
    return n;
}

Head_List *createHead_List() {
    Head_List *hl = (Head_List *)malloc(sizeof(Head_List));
    hl->head = createHNode(INT_MIN);
    hl->highest_pri = NULL;
    hl->num = 0;
    return hl;
}

Node *createNode(int order, int job, int priority) {
    Node *n = (Node *)malloc(sizeof(Node) );
    n->order = order;
    n->job = job;
    n->priority = priority;
    n->children_tail = NULL;
    n->sib = NULL;
    n->child = NULL;
    return n;
}

Head_List* BinomialHeapMerge(Head_List* H1, Head_List* H2){
    Head_List* H = createHead_List();
    Head_Node* H_p = H->head;
    Head_Node* H1_p = H1->head->next;
    Head_Node* H2_p = H2->head->next;
    Head_Node* temp;
    //Head_Node* temp2;

    if(H1_p == NULL){
        return H2;
    }else if (H2_p == NULL){
        return H1;
    }


    while(H1_p!=NULL && H2_p!=NULL){
        if(H1_p->order == H2_p->order){
            //update H
            temp = H1_p->next;
            H_p->next = H1_p;
            H1_p->previous = H_p;
            H_p->next->next = H2_p;
            H2_p->previous = H1_p;
            H_p = H_p->next->next;
            //move pivot
            H1_p = temp;
            H2_p = H2_p->next;

        }else if (H1_p->order > H2_p->order){

            H_p->next = H2_p;
            H2_p->previous = H_p;

            H_p = H_p->next;
            H2_p = H2_p->next;
        }else if (H2_p->order > H1_p->order){
            H_p->next = H1_p;
            H1_p->previous = H_p;

            H_p = H_p->next;
            H1_p = H1_p->next;
        }
    }
    //try lo
    if(H1_p==NULL && H2_p!=NULL){
        H_p->next = H2_p;
        H2_p->previous = H_p;
    }else if(H2_p==NULL && H1_p!=NULL){
        H_p->next = H1_p;
        H1_p->previous = H_p;
    }



    return H;

}

Head_List* BinomialHeapUnion(Head_List* H1, Head_List* H2){
    Head_List* H;
    Node* sibling;

    H = BinomialHeapMerge(H1, H2);
    H->num = H1->num + H2->num;
    if (H->head->next == NULL){
        return H;
    }

    Head_Node* prev;
    Head_Node* x;
    Head_Node* forward;
    prev = NULL;
    x = H->head->next;
    forward = x->next;
    H->highest_pri = H->head->next;
    //try lo
    if(x->root->priority > H->highest_pri->root->priority){
        H->highest_pri = x;
    }


    while(forward != NULL){
        //printf("E");
        if ((x->order != forward->order) || (forward->next != NULL && forward->next->order == x->order)){
            //printf("A");
            prev = x;
            x = forward;
            //initialling highest_pri
            if (x->root->priority > H->highest_pri->root->priority ){
                H->highest_pri = x;
            }

        }else{
            if (x->root->priority > forward->root->priority){
                //printf("B");
                //update
                x->next = forward->next;
                //printf("here");
                if(forward->next != NULL){
                    forward->next->previous = x;
                }

                // insert forward as a child & update order of x
                sibling = forward->root;
                free(forward);
                if(x->order == 0){
                    x->root->child = sibling;
                }else{
                    x->root->children_tail->sib = sibling;
                }
                x->root->children_tail = sibling;
                x->root->order++;
                x->order++;
                //in case
//                if(x->root->priority > H->highest_pri->root->priority){
//                    H->highest_pri = x;
//                }
//                if(x->next != NULL){
//                    x = x->next;
//                }


            }else{
                //printf("C");
                if(prev == NULL){
                    H->head->next = forward;
                    forward->previous = H->head;
                }else{
                    //printf("D");
                    prev->next = forward;
                    forward->previous = prev;
                }
                sibling = x->root;
                free(x);

                if(forward->order == 0){
                    forward->root->child = sibling;
                }else{
                    forward->root->children_tail->sib = sibling;
                }
                forward->root->children_tail = sibling;


                // insert x as a child & update order of forward
                forward->root->order++;
                forward->order++;

                //H->highest_pri_prev = x;

                x = forward;
                if(x->root->priority > H->highest_pri->root->priority){
                    H->highest_pri = x;
                }
            }
        }
        forward = x->next;
    }


    return H;
}


Head_List* BinomialHeapInsert(int job, int priority, int printer, Head_List* print){
    // Create New Heap
    Node* node = createNode(0,job, priority);
    Head_List* heap_plum = createHead_List();
    heap_plum->num++;
    Head_Node* new_head_node = createHNode(0);
    heap_plum->head->next = new_head_node;
    new_head_node->previous = heap_plum->head;
    new_head_node->root = node;

    //BinomialHeapUnion(pri[printer - 1], heap_plum);

    print = BinomialHeapUnion(print, heap_plum);
    return print;

}

Head_List* transfer(Head_List* print1, Head_List* print2){
    if(print2->head->next == NULL){
        return print1;
    }else if(print1->head->next == NULL){
        return print2;
    }
    print2 = BinomialHeapUnion(print1, print2);
    return print2;
}

Head_List* print_the_doc(Head_List* print1){
    printf("%d printed\n", print1->highest_pri->root->job);
    //take out the node
    Head_Node* lagerst = print1->highest_pri;
    if(print1->highest_pri->previous->order == INT_MIN){
        print1->head->next = lagerst->next;

        if(lagerst->next != NULL){
            lagerst->next->previous = print1->head;
        }

    }else{
        lagerst->previous->next =  lagerst->next;
        if(lagerst->next != NULL){
            lagerst->next->previous = lagerst->previous;
        }
    }

    Head_List* temp_heap = createHead_List();
    Head_Node* xp =temp_heap->head;
    Node* cursor = lagerst->root->child;
    if(lagerst->root->child != NULL){
        for(int i = 0; i < lagerst->order; i++){
            Head_Node* temp_HN = createHNode(i);
            temp_HN->root = cursor;
            xp->next = temp_HN;
            temp_HN->previous = xp;
            xp = xp->next;
            cursor = cursor->sib;
        }
        temp_heap->highest_pri = temp_heap->head->next;
    }
    free(lagerst->root);
    free(lagerst);
    print1->highest_pri = print1->head->next;


    print1 = BinomialHeapUnion(print1, temp_heap);
    print1->num--;

    return print1;

}


int main()
{
    int N, M, T;
    int job, priority, printer, printer2,input_printer, input_printer2;
    scanf("%d %d", &N, &M);
    Head_List* pri[N];
    //printf("okay");
    for (int i = 0; i < N; i++){
        pri[i] = createHead_List();
    }

    for (int i = 0; i < M; i++){
        scanf("%d ", &T);
        switch (T) {
            case 1:
                scanf("%d %d %d", &job, &priority, &printer);

                pri[printer - 1] = BinomialHeapInsert(job, priority, printer, pri[printer - 1]);
//              Head_Node* node = pri[printer - 1]->head->next;
//                printf("printers %d order: ", printer);
//                while(node!= NULL){
//                    printf("%d ", node->order);
//                    node = node->next;
//                }
//                printf("\n");
                printf("%d jobs waiting on printer %d\n", pri[printer - 1]->num, printer);
                //printf("highest priority = %d\n", pri[printer - 1]->highest_pri->root->priority);
//                if(pri[printer - 1]->highest_pri_prev->next->root->children != NULL){
//                    printf("highest job = %d\n", pri[printer - 1]->highest_pri_prev->next->root->job);
//                }


                break;
            case 2:
                scanf("%d", &printer);
                if (pri[printer - 1]->head->next == NULL){
                    printf("no documents in queue\n");
                    break;
                }
                pri[printer - 1] = print_the_doc(pri[printer - 1]);

//                Head_Node* cnode = pri[printer - 1]->head->next;
//                //printf("printers %d order: ", printer);
//                while(cnode!= NULL){
//                    printf("%d ", cnode->order);
//                    cnode = cnode->next;
//                }
//                printf("\n");
                //printf("%d jobs on printer %d after printing\n", pri[printer - 1]->num, printer);
                break;
            case 3:
                scanf("%d %d", &input_printer, &input_printer2);
//                printf("%d jobs origin on printer %d\n", pri[printer2 - 1]->num, printer2);
//                printf("%d jobs origin on printer %d\n", pri[printer - 1]->num, printer);
                //can open it if needed

                pri[input_printer2 - 1] = transfer(pri[input_printer - 1], pri[input_printer2 - 1]);

                //pri[printer2 - 1] = transfer(pri[printer - 1], pri[printer2 - 1]);
                pri[input_printer - 1] = createHead_List();
                printf("%d jobs waiting on printer %d after moving\n", pri[input_printer2 - 1]->num, input_printer2);
//                printf("%d jobs waiting on printer %d after moving\n", pri[printer - 1]->num, printer);//no need
                break;
        }
    }
/*
    for(int i = 800; i>0 ; i--){
        job = i;
        priority = i;
        printer = 1;
        pri[printer - 1] = BinomialHeapInsert(job, priority, printer, pri[printer - 1]);
        printf("%d jobs waiting on printer %d\n", pri[printer - 1]->num, printer);

        Head_Node* node = pri[printer - 1]->head->next;
        printf("printers %d order: ", printer);
        while(node!= NULL){
            printf("%d ", node->order);
            node = node->next;
        }
        printf("\n");
        printf("highest priority = %d\n", pri[printer - 1]->highest_pri->root->priority);
    }
    for(int i = 100; i> 0; i--){
        printer = 1;
        if (pri[printer - 1]->head->next == NULL){
            printf("no documents in queue\n");
            break;
        }

        pri[printer - 1] = print_the_doc(pri[printer - 1]);
    }*/
    for(int i = 0; i < N; i++){
        free(pri[i]);
    }
    return 0;
}
