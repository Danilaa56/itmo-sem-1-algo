#include <stdio.h>
#include <stdlib.h>

struct Leaf* ar;

struct Leaf{
    struct Leaf* leftChild;
    struct Leaf* rightChild;
    int value;
};

int check(struct Leaf* leaf, int min, int max){
    int value = leaf->value;
    if(value<min || value>max)
        return 0;
    if(leaf->leftChild){
        if(value <= leaf->leftChild->value)
            return 0;
        if(!check(leaf->leftChild, min, value-1))
            return 0;
    }
    if(leaf->rightChild){
        if(value >= leaf->rightChild->value)
            return 0;
        if(!check(leaf->rightChild, value+1, max))
            return 0;
    }
    return 1;
}

int main() {
    FILE* inputFile = fopen("check.in","r");
    int n;
    fscanf(inputFile,"%d", &n);

    FILE* outputFile = fopen("check.out","w");
    if(n==0){
        fprintf(outputFile, "YES");
        fclose(outputFile);
        fclose(inputFile);
        exit(0);
    }

    ar = calloc(200002, sizeof(struct Leaf));


    int key, left, right;
    for(int i=1;i<=n;i++){
        fscanf(inputFile,"%d %d %d", &key, &left, &right);
        ar[i].value = key;
        if(left)
            ar[i].leftChild = &ar[left];
        if(right)
            ar[i].rightChild = &ar[right];
    }
    fprintf(outputFile, "%s", check(&ar[1], -1000000001, 1000000001) ? "YES" : "NO");

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}