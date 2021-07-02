#include <stdio.h>
#include <stdlib.h>

int quick_search(int* array, int left, int right, int index){
    if(left==right)
        return array[index];
    int key = array[(left+right)/2];
    int i = left;
    int j = right;
    int tmp;
    do{
        while(array[i] < key)
            i++;
        while(array[j] > key)
            j--;
        if(i<=j){
            tmp = array[i];
            array[i] = array[j];
            array[j] = tmp;
            i++;
            j--;
        }
    }while (j>=i);
    if(i<=index){
        return quick_search(array,i,right,index);
    }
    if(index<=j){
        return quick_search(array,left,j,index);
    }
    return array[index];
}

int main() {
    FILE* inputFile = fopen("kth.in","r");
    int n, k;
    fscanf(inputFile,"%d %d\n",&n,&k);
    int a,b,c;
    int* ar = malloc(sizeof(int)*n);
    fscanf(inputFile,"%d %d %d %d %d",&a,&b,&c,&ar[0],&ar[1]);
    fclose(inputFile);

    for(int i=2; i<n; i++)
        ar[i]  = a*ar[i-2] + b*ar[i-1] + c;

    FILE* outputFile = fopen("kth.out","w");
    fprintf(outputFile,"%d",quick_search(ar,0,n-1,k-1));
    fclose(outputFile);

    return 0;
}