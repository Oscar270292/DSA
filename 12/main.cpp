
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

long long *heap_increase_key(long long *Arr ,int hs, long long key){
    Arr[hs] = key;
    int p = hs/2;
    while(hs > 1 && Arr[p] < Arr[hs]) {
        long long temp = Arr[p];
        Arr[p] = Arr[hs];
        Arr[hs] = temp;
        hs = hs/2;
        p = hs/2;
    }
}

void max_heap_insert(long long *Arr, int *hea_size, long long key){
    *hea_size += 1;
    Arr[*hea_size] = LLONG_MIN;
    heap_increase_key(Arr, *hea_size, key);
}

int main() {
    long long *Arr = (long long*)malloc((1000000*sizeof(long long)));
    int hea_size = 0;

    int num;
    scanf("%d", &num);
    for(int i=0; i<num; i++){
        long long key;
        scanf("%lld", &key);
        max_heap_insert(Arr, &hea_size, key);
    }

    for(int k=1; k<=num; k++){
        printf("%lld ", Arr[k]);
    }


    return 0;
}

int bi_search(dungeon* dungeons[], const int path[], long long key, int l, int r, dungeon* current){
    int m = floor((l+r)/2);
    while(l<r){
        m = floor((l+r)/2);
        if(current->distance_to_root-dungeons[path[m]]->distance_to_root <= key){
            r = m;
        }
        else if (current->distance_to_root-dungeons[path[m]]->distance_to_root > key){
            l = m+1;
        }
        m = floor((l+r)/2);
    }
    return path[m];
}