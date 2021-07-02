#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* inputFile = fopen("antiqs.in","r");
    int n;
    fscanf(inputFile,"%d\n",&n);
    fclose(inputFile);

    int a[n];
    a[0] = 0;
    a[1] = 1;
    for(int i=2; i<n; i++){
        a[i] = a[i/2];
        a[i/2] = i;
    }

    FILE* outputFile = fopen("antiqs.out","w");
    for(int i=0; i<n; i++){
        fprintf(outputFile,"%d ",a[i]+1);
    }
    fclose(outputFile);
    return 0;
}