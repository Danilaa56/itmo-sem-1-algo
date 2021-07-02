#include <stdio.h>

int findLeft(int* ar, int num, int left, int right){
    if(left==right)
        if(ar[left]==num)
            return left;
        else
            return -2;
    if(right-left==1){
        if(ar[left]==num)
            return left;
        if(ar[right]==num)
            return right;
        return -2;
    }
    if(ar[(left+right)/2]>=num){
        return findLeft(ar, num, left, (left+right)/2);
    }else{
        return findLeft(ar, num, (left+right)/2, right);
    }
}
int findRight(int* ar, int num, int left, int right){
    if(left==right)
        if(ar[left]==num)
            return left;
        else
            return -2;
    if(right-left==1){
        if(ar[right]==num)
            return right;
        if(ar[left]==num)
            return left;
        return -2;
    }
    if(ar[(left+right)/2]<=num){
        return findRight(ar, num, (left+right)/2, right);
    }else{
        return findRight(ar, num, left, (left+right)/2);
    }
}

int main() {
    FILE* inputFile = fopen("binsearch.in","r");
    FILE* outputFile = fopen("binsearch.out","w");

    int n;
    fscanf(inputFile, "%d", &n);
    int ar[n];
    for(int i=0;i<n;i++)
        fscanf(inputFile, "%d", &ar[i]);

    int m;
    fscanf(inputFile, "%d", &m);
    for(int i=0;i<m;i++){
        int num;
        fscanf(inputFile, "%d", &num);
        int left = findLeft(ar, num, 0, n-1)+1;
        if(left==-1)
            fprintf(outputFile,"-1 -1\n");
        else
            fprintf(outputFile,"%d %d\n", left, findRight(ar, num, left-1, n-1)+1);
    }

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}