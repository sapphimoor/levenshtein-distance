#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min(int, int, int);

int main(int argc, char *argv[]) {
    int i, j, n0, n1, cost;
    int *n;
    int **score;
    char **sentence;
    FILE *fp;

    if(argc < 3) {
        printf(" **Error: format: input 2 files after execution command.\n");
        exit(1);
    }
    
    n = (int*) malloc(sizeof(int) * 3);
    sentence = (char**) malloc(sizeof(char*) * 3);
    n[0] = n[1] = 256;
    for(i=0; i<2; i++) {
        if((fp=fopen(argv[i+1], "r")) == NULL) {
            printf(" **Error: input file: cannot open the file \"%s\".\n", argv[i+1]);
            for(j=0; j<3; j++) free(sentence[j]);
            free(sentence); free(n);
            exit(1);
        }
        n[2] = n[i] >> 1;
        sentence[i] = (char*) calloc(n[i], sizeof(char));
        sentence[2] = (char*) calloc(n[2], sizeof(char));
        sentence[i][0] = ' ';
        while(fgets(sentence[2], n[2], fp) != NULL) {
            if(strlen(sentence[i]) + strlen(sentence[2]) > n[i] - 1) {
                n[i] <<= 1; n[2] <<= 1;
                sentence[i] = (char*) realloc(sentence[i], sizeof(char) * n[i]);
                sentence[2] = (char*) realloc(sentence[2], sizeof(char) * n[2]);
                if(sentence[i] == NULL || sentence[2] == NULL) {
                    printf(" **Error: allocation: memory is insufficient.\n");
                    for(j=0; j<3; j++) free(sentence[j]);
                    free(sentence); free(n);
                    fclose(fp);
                    exit(1);
                }
            }
            strcat(sentence[i], sentence[2]);
        }
        n[i] = strlen(sentence[i]);
    }
    n0 = n[0]; n1 = n[1];

    score = (int**) calloc(n0, sizeof(int*));
    for(i=0; i<n0; i++) {
        score[i] = (int*) calloc(n1, sizeof(int));
        score[i][0] = i;
    }
    for(j=1; j<n1; j++) score[0][j] = j;
    
    for(i=1; i<n0; i++) {
        for(j=1; j<n1; j++) {
            cost = sentence[0][i]==sentence[1][j] ? 0 : 1;
            score[i][j] = min(score[i-1][j] + 1, score[i][j-1] + 1, score[i-1][j-1] + cost);
        }
    }

    for(i=0; i<2; i++) printf("# of letters in \"%s\": %d\n", argv[i+1], n[i] - 2);
    printf("levenshtein distance: %d\n", score[n0-1][n1-1]);

    for(i=0; i<3; i++) free(sentence[i]);
    for(i=0; i<n0; i++) free(score[i]);
    free(sentence); free(score); free(n);

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