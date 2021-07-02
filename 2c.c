#include <stdio.h>
#include <stdlib.h>

static long long res;

void merge(int* array, int center, int length, int* cache){
    for(int f=0;f<length;f++){
        cache[f] = array[f];
    }
    int leftCount = center;
    int rightCount = length-center;

    int* leftArray = cache;
    int* rightArray = cache+center;
    while(leftCount>0||rightCount>0){
        if(*rightArray<*leftArray){
            *array = *rightArray;
            rightArray++;
            array++;
            res += leftCount;
            if(--rightCount==0){
                for(int i = 0; i < leftCount; i++){
                    *array = *leftArray;
                    leftArray++;
                    array++;
                }
                return;
            }
        }else{
            *array = *leftArray;
            leftArray++;
            array++;
            if(--leftCount==0){
                for(int i = 0; i < rightCount; i++){
                    *array = *rightArray;
                    rightArray++;
                    array++;
                }
                return;
            }
        }
    }
}

void merge_sort(int* array, int length, int* cache){
    if(length==1)
        return;
    if(length==2){
        if(array[1]<array[0]) {
            int tmp = array[1];
            array[1] = array[0];
            array[0] = tmp;
            res++;
        }
        return;
    }
    if(cache==NULL){
        cache = malloc(sizeof(void*)*length);
        merge_sort(array,length/2,cache);
        merge_sort(array+length/2,(length+1)/2,cache);
        merge(array, length/2, length,cache);
        free(cache);
        return;
    }else{
        merge_sort(array,length/2,cache);
        merge_sort(array+length/2,(length+1)/2,cache);
        merge(array, length/2, length,cache);
    }
}

int main() {
    FILE* inputFile = fopen("inversions.in","r");

    int n;
    fscanf(inputFile,"%d\n",&n);

    int array[n];
    for(int i=0;i<n;i++){
        fscanf(inputFile,"%d ",&array[i]);
    }

    fclose(inputFile);

    res = 0;
    merge_sort(array,n, NULL);

    FILE* outputFile = fopen("inversions.out","w");
    fprintf(outputFile,"%lld",res);
    fclose(outputFile);
    return 0;
}