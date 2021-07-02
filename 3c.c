#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int shift;

void merge(char** ar, int center, int length, char** cache){
    for(int f=0;f<length;f++){
        cache[f] = ar[f];
    }
    int leftCount = center;
    int rightCount = length-center;

    char** leftArray = cache;
    char** rightArray = cache+center;
    while(leftCount>0||rightCount>0){
        if(strcmp((*rightArray)+shift,(*leftArray)+shift)<0){
            *ar = *rightArray;
            rightArray++;
            ar++;
            if(--rightCount==0){
                for(int i = 0; i < leftCount; i++){
                    *ar = *leftArray;
                    leftArray++;
                    ar++;
                }
                return;
            }
        }else{
            *ar = *leftArray;
            leftArray++;
            ar++;
            if(--leftCount==0){
                for(int i = 0; i < rightCount; i++){
                    *ar = *rightArray;
                    rightArray++;
                    ar++;
                }
                return;
            }
        }
    }
}

void merge_sort(char** ar, int length, char** cache){
    if(length==1)
        return;
    if(length==2){
        if(strcmp((ar[1])+shift,(ar[0])+shift)<0) {
            char* tmp = ar[1];
            ar[1] = ar[0];
            ar[0] = tmp;
        }
        return;
    }
    if(cache==NULL){
        cache = malloc(sizeof(char*)*length);
        merge_sort(ar,length/2,cache);
        merge_sort(ar+length/2,(length+1)/2,cache);
        merge(ar, length/2, length,cache);
        free(cache);
        return;
    }else{
        merge_sort(ar,length/2,cache);
        merge_sort(ar+length/2,(length+1)/2,cache);
        merge(ar, length/2, length,cache);
    }
}

int main() {
    FILE* inputFile = fopen("radixsort.in","r");
    int n, k, m;
    fscanf(inputFile,"%d", &n);
    fscanf(inputFile,"%d", &m);
    fscanf(inputFile,"%d", &k);

    shift = m-k;

    char* array[n];
    int mem = sizeof(char)*m;
    for(int i=0;i<n;i++){
        array[i] = malloc(mem);
        fscanf(inputFile,"%s\n",array[i]);
    }

    fclose(inputFile);

    merge_sort(array, n, NULL);

    FILE* outputFile = fopen("radixsort.out","w");
    fprintf(outputFile,"%s",array[0]);
    for(int i=1;i<n;i++){
        fprintf(inputFile,"\n%s",array[i]);
    }
    fclose(outputFile);
    return 0;
}