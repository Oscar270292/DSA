#include <stdio.h>

int ct = 0;
int max;

void swap(long long *a, long long *b) {
    if (ct < max) {
        long long temp = *a;
        *a = *b;
        *b = temp;
        ct++;
    }
}

int partition(long long arr[], int l, int r) {
    long long p = arr[l];
    int i = l + 1;
    int j = r;

    while (i < j && ct < max) {
        while (i <= r && arr[i] <= p){
            i++;
        }
        while (j >= l && arr[j] > p){
            j--;
        }


        if (i >= j) {
            break;
        }

        swap(&arr[i], &arr[j]);
    }

    if (l < j && arr[l] > arr[j])
        swap(&arr[l], &arr[j]);

    return j;
}

void quickSort(long long  arr[], int l, int r) {
    if (l < r && ct < max) {
        int m = partition(arr, l, r);
        quickSort(arr, l, m - 1);
        quickSort(arr, m + 1, r);
    }
}

int main() {
    int N;
    scanf("%d %d", &N, &max);

    long long arr[N];
    for (int i = 0; i < N; i++) {
        scanf("%lld", &arr[i]);
    }

    quickSort(arr, 0, N-1);

    printf("The array is ");
    for (int i = 0; i < N; i++) {
        printf("%lld ", arr[i]);
    }

    if (ct <= max) {
        printf("after %d swaps.\n", ct);
    } else {
        printf("after %d swaps.\n", max);
    }

    return 0;
}
