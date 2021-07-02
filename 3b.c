#include <stdio.h>
#include <stdlib.h>

int isHeap(int* ar, int length){
    for(int i=1;i<=length;i++){
        if((i<<1)<=length){
            if(ar[i-1]<ar[(i<<1)-1]){
                //printf("%d %d\n",ar[i-1],ar[(i<<1)-1]);
                return 0;
            }
        }else
            return 1;
        if((i<<1)<length){
            if(ar[i-1]<ar[(i<<1)]){
                //printf("%d %d\n",ar[i-1],ar[i<<1]);
                return 0;
            }
        }else
            return 1;
    }
    return 1;
}

void heapify(int* ar, int i, int length){
    int left = 2*i;
    int right = 2*i+1;
    int largest = i;
    if(left<=length && ar[left-1]>ar[largest-1]){
        largest = left;
    }
    if(right<=length && ar[right-1]>ar[largest-1]){
        largest = right;
    }
    if(largest!=i){
        int tmp = ar[largest-1];
        ar[largest-1] = ar[i-1];
        ar[i-1] = tmp;
        heapify(ar, largest, length);
    }
}

void showArray(int* ar, int length){
    for(int f=0;f<length;f++){
        printf("%d ",ar[f]);
    }
    printf("\n");
}

void heapSort(int* ar, int length){
    for(int i=length/2;i>0;i--)
        heapify(ar,i,length);
    for(int i=length-1;i>=0;i--){
        int tmp = ar[i];
        ar[i] = ar[0];
        ar[0] = tmp;
        heapify(ar, 1, --length);
    }
}

int main() {
    FILE* inputFile = fopen("sort.in","r");
    int n;
    fscanf(inputFile,"%d\n",&n);
    int ar[n];
    for(int f=0;f<n;f++)
        fscanf(inputFile, "%d ",&ar[f]);
    fclose(inputFile);

    heapSort(ar, n);

    FILE* outputFile = fopen("sort.out","w");
    for(int i=0; i<n; i++){
        fprintf(outputFile,"%d ",ar[i]);
    }
    fclose(outputFile);
    return 0;
}

