#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void hit_count(char *S, char *C, int* n, int* m, long long hc[], long long pl[]){
    //get prefix function
    long long pi[*m];
    pi[0] = 0;
    pl[0] = 1;
    long long k = 0;

    for (int q = 2; q <= *m; q++) {
        while (k > 0 && C[k] != C[q - 1]) {
            k = pi[k-1];
        }
        if (C[k] == C[q - 1]) {
            k = k + 1;
        }
        pi[q-1] = k;
        pl[q-1] = 1;
        if(k > 0){
            pl[q-1] += pl[k-1];
        }
    }
    //print pefix function
//    for (int q = 1; q <= *m; q++){
//        printf("%lld", pi[q-1]);
//    }
//    printf("\n");
    long long qq = 0;
    long long ct = 0;
    for (int j = 0; j < *n; j++) {
        while (qq > 0 && C[qq] != S[j]) {
            qq = pi[qq - 1];
        }
        if (C[qq] == S[j]) {
            hc[qq]++;
            qq = qq + 1;
        }
        if (qq == *m) {
            //printf("Pattern occurs with shift %d\n", i - *m + 1);
            ct++;
            qq = pi[qq - 1];
        }

    }
    //printf("ct = %d\n", ct);
    //hc[*m-1] = ct;
    for(int l = *m-1; l >= 0; l--){
        //hc[l] += ct;
        if(pi[l] > 0){
            hc[pi[l]-1] += hc[l];
        }
    }

}


int main() {
    char *S = (char*)malloc((5000005) * sizeof(char));
    char *C = (char*)malloc((5000005) * sizeof(char));

//    printf("Enter string S: %s", S);
//    printf("Enter string C: %s", C);

    scanf("%s", S);
    scanf("%s", C);

    int n = strlen(S);
    int m = strlen(C);


//    for(int i = 0; C[i] != '\0'; i++){
//        printf("%c", C[i]);
//    }

    //printf("%lld %lld\n", n, m);
    long long hc[m];
    for(int h = 0; h < m; h++){
        hc[h] = 0;
    }
    long long pl[m];
    hit_count(S, C, &n, &m, hc, pl);
    //printf("Hit count = %d", ct);
//    printf("hc = ");
//    for(int s = 0; s < m; s++){
//        printf("%lld ", hc[s]);
//    }
//    printf("\n");
//
//    printf("pl = ");
//    for(int ss = 0; ss < m; ss++){
//        printf("%lld ", pl[ss]);
//    }
//    printf("\n");

    for(int sss = 0; sss < m; sss++){
        printf("%lld ", pl[sss] * hc[sss]);
        printf("\n");
    }

    return 0;
}