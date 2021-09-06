#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min(int, int, int);

int main(int argc, char *argv[]) {
    int n1, n2, n, i, j, cost;
    int **score;
    char *sentence1, *sentence2, *text;
    FILE *fp1, *fp2;

    if(argc < 3) {
        printf(" **Error: format: input 2 files after execution command.\n");
        exit(1);
    }
    if((fp1=fopen(argv[1], "r")) == NULL) {
        printf(" **Error: input file: cannot open the file \"%s\".\n", argv[1]);
        exit(1);
    }
    if((fp2=fopen(argv[2], "r")) == NULL) {
        printf(" **Error: input file: cannot open the file \"%s\".\n", argv[2]);
        fclose(fp1);
        exit(1);
    }

    n1 = n2 = 256;

    n = n1 >> 1;
    sentence1 = (char*) calloc(n1, sizeof(char));
    text = (char*) calloc(n, sizeof(char));
    sentence1[0] = ' ';
    while(fgets(text, n, fp1) != NULL) {
        if(strlen(sentence1) + strlen(text) > n1 - 1) {
            n1 <<= 1; n <<= 1;
            sentence1 = (char*) realloc(sentence1, sizeof(char) * n1);
            text = (char*) realloc(text, sizeof(char) * n);
            if(sentence1 == NULL || text == NULL) {
                printf(" **Error: allocation: memory is insufficient.\n");
                free(sentence1); free(text);
                fclose(fp1); fclose(fp2);
                exit(1);
            }
        }
        strcat(sentence1, text);
    }
    n1 = strlen(sentence1) + 1;

    n = n2 >> 1;
    sentence2 = (char*) calloc(n2, sizeof(char));
    text = (char*) calloc(n, sizeof(char));
    sentence2[0] = ' ';
    while(fgets(text, n, fp2) != NULL) {
        if(strlen(sentence2) + strlen(text) > n2 - 1) {
            n2 <<= 1; n <<= 1;
            sentence2 = (char*) realloc(sentence2, sizeof(char) * n2);
            text = (char*) realloc(text, sizeof(char) * n);
            if(sentence2 == NULL || text == NULL) {
                printf(" **Error: allocation: memory is insufficient.\n");
                free(sentence1); free(sentence2); free(text);
                fclose(fp1); fclose(fp2);
                exit(1);
            }
        }
        strcat(sentence2, text);
    }
    n2 = strlen(sentence2) + 1;

    fclose(fp1); fclose(fp2);

    score = (int**) calloc(n1, sizeof(int*));
    for(i=0; i<n1; i++) {
        score[i] = (int*) calloc(n2, sizeof(int));
        score[i][0] = i;
    }
    for(j=1; j<n2; j++) score[0][j] = j;
    
    for(i=1; i<n1; i++) {
        for(j=1; j<n2; j++) {
            cost = sentence1[i]==sentence2[j] ? 0 : 1;
            score[i][j] = min(score[i-1][j] + 1, score[i][j-1] + 1, score[i-1][j-1] + cost);
        }
    }

    printf("# of letters in \"%s\": %d\n", argv[1], n1 - 2);
    printf("# of letters in \"%s\": %d\n", argv[2], n2 - 2);
    printf("levenshtein distance: %d\n", score[n1-1][n2-1]);


    free(sentence1); free(sentence2); free(text);

    return 0;
}

int min(int a, int b, int c) {
    if(a < b) {
        if(a < c) return a;
        return c;
    }
    if(b < c) return b;
    return c;
}