#include <stdio.h>
#include <stdlib.h>

typedef struct com {
    int trans_num;
    long long raise;
    long long bonus_sum;
    struct com* parent;
} com;

typedef struct emp {
    long long acb;
    long long sal;
    struct com* com;
} emp;



com* MakeSet(){
    com* c = (com *)malloc(sizeof(com));
    c->parent = c;
    c->bonus_sum = 0;
    c->raise = 0;
    c->trans_num = 0;
    return c;
}

emp* createemp(com* c, long long s){
    emp* e = (emp *)malloc(sizeof(emp));
    e->acb = 0;
    e->com = c;
    e->sal = s;
    return e;
}


com* FindSet(com* com1, com* delivery){
    if(com1 != com1->parent){
        com1->parent = FindSet(com1->parent, delivery);

        com1->bonus_sum += (delivery->bonus_sum + com1->raise * delivery->trans_num);
        com1->trans_num += delivery->trans_num;
        com1->raise+= delivery->raise;

        delivery->trans_num = com1->trans_num;
        delivery->raise = com1->raise;
        delivery->bonus_sum = com1->bonus_sum;

    }
    return com1->parent;
}

void merge(emp* employee[], int x, int y){

    com* temp1 = employee[x]->com;
    com* del = MakeSet();
    temp1->parent = FindSet(temp1, del);

    com* temp2 = employee[y]->com;
    del = MakeSet();
    temp2->parent = FindSet(temp2, del);
    if(temp1->parent == temp2->parent){
        return;
    }

    com* temp3 = MakeSet();
    temp1->parent->parent = temp3;
    temp2->parent->parent = temp3;


}

void transfer(emp* employee[], int k){
    emp* temp_staff = employee[k];
    com* temp_com = temp_staff->com;
    com* del = MakeSet();
    temp_com->parent = FindSet(temp_com, del);
    temp_com->parent->trans_num++;
    temp_com->parent->bonus_sum += temp_com->parent->raise;

}

void raise(emp* employee[],int r,  int k){
    com* temp1 = employee[k]->com;
    com* del = MakeSet();
    temp1->parent = FindSet(temp1, del);
    temp1->parent->raise += r;
}

void quit(emp* employee[], int k, long long c){
    com* temp1 = employee[k]->com;
    com* del = MakeSet();
    temp1->parent = FindSet(temp1, del);
    long long b;
    if(employee[k]->com != employee[k]->com->parent){
        b = employee[k]->com->bonus_sum + employee[k]->com->parent->bonus_sum + employee[k]->com->parent->trans_num * employee[k]->com->raise;
        //tot_bonus = employee[i]->com->bonus_sum + employee[i]->com->parent->trans_num * employee[i]->com->raise;
        b = (employee[k]->com->trans_num + employee[k]->com->parent->trans_num) * employee[k]->sal + b;
    }else{
        b = employee[k]->com->bonus_sum;
        //tot_bonus = employee[i]->com->bonus_sum + employee[i]->com->parent->trans_num * employee[i]->com->raise;
        b = (employee[k]->com->trans_num) * employee[k]->sal + b;
    }
    b += employee[k]->acb;
    com* temp = MakeSet();
    employee[k] = createemp(temp, c);
    employee[k]->acb += b;
}



int main() {
    int N, Q;
    scanf("%d %d", &N, &Q);

    long long s;
    emp* employee[N];
    com* temp;
    for(int i = 0; i<N; i++){
        scanf("%lld", &s);
        temp = MakeSet();
        employee[i] = createemp(temp, s);
    }

    int op, x, y, k, k2, r, k1;
    long long c;
    for (int j = 0; j < Q; j++){
        scanf("%d ", &op);
        switch (op) {
            case 1:
                scanf("%d %d", &x, &y);
                merge(employee, x-1, y-1);
                break;
            case 2:
                scanf("%d %d", &k2, &r);
                raise(employee, r, k2-1);
                break;
            case 3:
                scanf("%d", &k);
                transfer(employee, k-1);
                break;
            case 4:
                scanf("%d %lld", &k1, &c);
                quit(employee, k1-1, c);
                break;
        }
    }

    for(int i = 0; i<N; i++){
        long long tot_paid, tot_bonus;
        com* del = MakeSet();
        employee[i]->com->parent = FindSet(employee[i]->com, del);
        if(employee[i]->com != employee[i]->com->parent){
            tot_bonus = employee[i]->com->bonus_sum + employee[i]->com->parent->bonus_sum + employee[i]->com->parent->trans_num * employee[i]->com->raise;
            //tot_bonus = employee[i]->com->bonus_sum + employee[i]->com->parent->trans_num * employee[i]->com->raise;
            tot_paid = (employee[i]->com->trans_num + employee[i]->com->parent->trans_num) * employee[i]->sal + tot_bonus;
        }else{
            tot_bonus = employee[i]->com->bonus_sum;
            //tot_bonus = employee[i]->com->bonus_sum + employee[i]->com->parent->trans_num * employee[i]->com->raise;
            tot_paid = (employee[i]->com->trans_num) * employee[i]->sal + tot_bonus;
        }

        if (employee[i]->acb != 0){
            tot_paid += employee[i]->acb;
        }
        printf("%lld ", tot_paid);
    }
    printf("\n");
    return 0;
}
