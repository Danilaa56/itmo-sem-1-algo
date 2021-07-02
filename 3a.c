#include <stdio.h>
#include <stdlib.h>

void no(){
    FILE* outputFile = fopen("isheap.out","w");
    fprintf(outputFile,"NO");
    fclose(outputFile);
    exit(0);
}

int main() {
    FILE* inputFile = fopen("isheap.in","r");
    int n;
    fscanf(inputFile,"%d\n",&n);
    int ar[n];
    for(int f=0;f<n;f++)
        fscanf(inputFile, "%d ",&ar[f]);
    fclose(inputFile);

    for(int i=1;i<=n;i++){
        if((i<<1)<=n){
            if(ar[i-1]>ar[(i<<1)-1]){
                no();
            }
        }else
            break;

        if((i<<1)<n){
            if(ar[i-1]>ar[(i<<1)]){
                no();
            }
        }else
            break;
    }

    FILE* outputFile = fopen("isheap.out","w");
    fprintf(outputFile,"YES");
    fclose(outputFile);
    return 0;
}

