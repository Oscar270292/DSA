#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_R 2
#define INITIAL_Q 0
#define ALPHABET_SIZE 26

typedef struct Node {
    char* str;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} Bucket;

Bucket* table;
int r, q;

unsigned long long compute_hash(const char* str) {
    unsigned long long hash = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        hash = hash * ALPHABET_SIZE + (str[i] - 'a');
    }
    return hash;
}

unsigned int get_least_significant_bits(unsigned long long num, int bits) {
    return num & ((1U << bits) - 1);
}

void insert_into_bucket(Bucket* bucket, const char* str) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->str = strdup(str);
    new_node->next = bucket->head;
    bucket->head = new_node;
}

void split_bucket(int bucket_index) {
    Bucket new_bucket = {NULL};
    Node* curr = table[bucket_index].head;
    table[bucket_index].head = NULL;

    while (curr) {
        Node* next = curr->next;
        unsigned int new_hash = get_least_significant_bits(compute_hash(curr->str), r + 1);
        if (new_hash == bucket_index + (1 << r)) {
            insert_into_bucket(&new_bucket, curr->str);
        } else {
            insert_into_bucket(&table[bucket_index], curr->str);
        }
        free(curr);
        curr = next;
    }

    table[bucket_index + (1 << r)] = new_bucket;
}

void insert(const char* str) {
    unsigned long long hash = compute_hash(str);
    unsigned int bucket_index = get_least_significant_bits(hash, r);

    if (bucket_index < q) {
        bucket_index = get_least_significant_bits(hash, r + 1);
    }

    insert_into_bucket(&table[bucket_index], str);

    if (table[bucket_index].head && table[bucket_index].head->next) {
        q++;
        if (q == (1 << r)) {
            r++;
            q = 0;
        }
        split_bucket(q - 1);
    }
}

void print_buckets() {
    for (int i = 0; i < (1 << r) + q; ++i) {
        if (table[i].head) {
            Node* curr = table[i].head;
            while (curr) {
                printf("%s ", curr->str);
                curr = curr->next;
            }
            printf("\n");
        } else {
            printf("-1\n");
        }
    }
}

int main() {
    int N;
    scanf("%d", &N);

    table = (Bucket*)calloc((1 << INITIAL_R), sizeof(Bucket));
    r = INITIAL_R;
    q = INITIAL_Q;

    char buffer[65];
    for (int i = 0; i < N; ++i) {
        scanf("%s", buffer);
        insert(buffer);
    }

    print_buckets();

    for (int i = 0; i < (1 << r) + q; ++i) {
        Node* curr = table[i].head;
        while (curr) {
            Node* next = curr->next;
            free(curr->str);
            free(curr);
            curr = next;
        }
    }
    free(table);

    return 0;
}

