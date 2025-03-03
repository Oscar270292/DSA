#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_K 1000006
#define MAX_N 1000006

// 將字符轉換為對應的值：A-Z -> 0-25, a-z -> 26-51
int char_to_value(char c) {
    if ('A' <= c && c <= 'Z') {
        return c - 'A';
    } else {
        return c - 'a' + 26;
    }
}

// 計算哈希值
long long compute_hash(char **arr, int j, int k, int d, int q) {
    long long hash_val = 0;
    for (int i = 0; i < k; ++i) {
        hash_val = (hash_val * d + char_to_value(arr[i][j])) % q;
    }
    return hash_val;
}

// KMP算法計算前綴函數
void compute_prefix_function(long long *P_prime, int m, int *pi) {
    int k = 0;
    pi[0] = 0;
    for (int i = 1; i < m; ++i) {
        while (k > 0 && P_prime[k] != P_prime[i]) {
            k = pi[k - 1];
        }
        if (P_prime[k] == P_prime[i]) {
            ++k;
        }
        pi[i] = k;
    }
}

// KMP搜索算法
void kmp_search(long long *T_prime, int n, long long *P_prime, int m, int *shifts, int *shift_count) {
    int pi[MAX_N];
    compute_prefix_function(P_prime, m, pi);

    int k = 0;
    for (int i = 0; i < n; ++i) {
        while (k > 0 && P_prime[k] != T_prime[i]) {
            k = pi[k - 1];
        }
        if (P_prime[k] == T_prime[i]) {
            ++k;
        }
        if (k == m) {
            shifts[(*shift_count)++] = i - m + 1;
            k = pi[k - 1];
        }
    }
}

// 驗證位移是否有效
void verify_shifts(char **T, char **P, int *shifts, int shift_count, int k, int m, int *valid_shifts, int *valid_count, int *spurious_shifts, int *spurious_count) {
    for (int i = 0; i < shift_count; ++i) {
        int s = shifts[i];
        int match = 1;
        for (int j = 0; j < k; ++j) {
            if (strncmp(&T[j][s], P[j], m) != 0) {
                match = 0;
                break;
            }
        }
        if (match) {
            valid_shifts[(*valid_count)++] = s;
        } else {
            spurious_shifts[(*spurious_count)++] = s;
        }
    }
}

int main() {
    int k, n, m, q;
    scanf("%d %d %d %d", &k, &n, &m, &q);

    char **T = (char **)malloc(k * sizeof(char *));
    char **P = (char **)malloc(k * sizeof(char *));
    for (int i = 0; i < k; ++i) {
        T[i] = (char *)malloc((n + 1) * sizeof(char));
        scanf("%s", T[i]);
    }
    for (int i = 0; i < k; ++i) {
        P[i] = (char *)malloc((m + 1) * sizeof(char));
        scanf("%s", P[i]);
    }

    long long T_prime[MAX_N], P_prime[MAX_N];
    int d = 52;

    for (int j = 0; j < n; ++j) {
        T_prime[j] = compute_hash(T, j, k, d, q);
    }
    for (int j = 0; j < m; ++j) {
        P_prime[j] = compute_hash(P, j, k, d, q);
    }

    for (int j = 0; j < n; ++j) {
        printf("%lld ", T_prime[j]);
    }
    printf("\n");
    for (int j = 0; j < m; ++j) {
        printf("%lld ", P_prime[j]);
    }
    printf("\n");

    int shifts[MAX_N], shift_count = 0;
    kmp_search(T_prime, n, P_prime, m, shifts, &shift_count);

    int valid_shifts[MAX_N], valid_count = 0;
    int spurious_shifts[MAX_N], spurious_count = 0;
    verify_shifts(T, P, shifts, shift_count, k, m, valid_shifts, &valid_count, spurious_shifts, &spurious_count);

    if (valid_count > 0) {
        for (int i = 0; i < valid_count; ++i) {
            printf("%d ", valid_shifts[i]);
        }
    } else {
        printf("-1");
    }
    printf("\n");

    if (spurious_count > 0) {
        for (int i = 0; i < spurious_count; ++i) {
            printf("%d ", spurious_shifts[i]);
        }
    } else {
        printf("-1");
    }
    printf("\n");

    for (int i = 0; i < k; ++i) {
        free(T[i]);
        free(P[i]);
    }
    free(T);
    free(P);

    return 0;
}
