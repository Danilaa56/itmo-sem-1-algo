#include <stdio.h>
#include <stdlib.h>

struct Leaf* ar;

struct Leaf{
    int leftChildIndex;
    int rightChildIndex;
};

struct Tree{
    struct Leaf* first;
};

int getLength(struct Leaf* leaf){
    int l = 0;
    int r = 0;
    if(leaf->leftChildIndex)
        l = getLength(&ar[leaf->leftChildIndex]);
    if(leaf->rightChildIndex)
        r = getLength(&ar[leaf->rightChildIndex]);
    if(l>r)
        return l + 1;
    else
        return r + 1;
}

int main() {
    FILE* inputFile = fopen("height.in","r");
    int n;
    fscanf(inputFile,"%d", &n);

    FILE* outputFile = fopen("height.out","w");
    if(n==0){
        fprintf(outputFile, "0");
        fclose(outputFile);
        fclose(inputFile);
        exit(0);
    }
    ar = calloc(200002, sizeof(struct Leaf));
    int* castedAr = (int*)ar;

    int max = 1;
    int key, left, right;
    for(int i=1;i<=n;i++){
        fscanf(inputFile,"%d %d %d", &key, &left, &right);
        castedAr[i<<1] = left;
        castedAr[(i<<1)+1] = right;
    }
    fprintf(outputFile, "%d", getLength(&ar[1]));

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}