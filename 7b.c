#include <stdio.h>
#include <stdlib.h>

struct Leaf* leaves;

typedef struct Leaf{
    int balance;
    int length;
    int key;
    int mark;
    struct Leaf* left;
    struct Leaf* right;
} Leaf;

void getBalance(Leaf* leaf){
    int l = 0;
    int r = 0;
    if(leaf->left){
        getBalance(leaf->left);
        l = leaf->left->length;
    }
    if(leaf->right){
        getBalance(leaf->right);
        r = leaf->right->length;
    }

    leaf->balance = r-l;
    leaf->length = r>l ? r+1 : l+1;
}

void justGetLengthAndBalance(Leaf* leaf){
    int l = 0;
    int r = 0;
    if(leaf->left){
        l = leaf->left->length;
    }
    if(leaf->right){
        r = leaf->right->length;
    }

    leaf->balance = r-l;
    leaf->length = r>l ? r+1 : l+1;
}

Leaf* smallLeftRotate(Leaf* leaf){
    Leaf* right = leaf->right;
    Leaf* rightLeft = leaf->right->left;

    leaf->right = rightLeft;
    justGetLengthAndBalance(leaf);

    right->left = leaf;
    justGetLengthAndBalance(right);
    
    return right;
}

Leaf* bigLeftRotate(Leaf* leaf){
    Leaf* right = leaf->right;
    Leaf* rightLeft = right->left;

    Leaf* rightLeftLeft = rightLeft->left;
    Leaf* rightLeftRight = rightLeft->right;

    leaf->right = rightLeftLeft;
    justGetLengthAndBalance(leaf);

    right->left = rightLeftRight;
    justGetLengthAndBalance(right);

    rightLeft->left = leaf;
    rightLeft->right = right;
    justGetLengthAndBalance(rightLeft);
    
    return rightLeft;
}

Leaf* leftRotate(Leaf* leaf){
    if(leaf->right->balance == -1){
        return bigLeftRotate(leaf);
    } else {
        return smallLeftRotate(leaf);
    }
}

int markNum = 1;

void markLeaves(Leaf* leaf){
    leaf->mark = markNum++;
    if(leaf->left)
        markLeaves(leaf->left);
    if(leaf->right)
        markLeaves(leaf->right);
}

void printTree(FILE* outputFile, Leaf* leaf){
    int l = 0;
    int r = 0;
    if(leaf->left)
        l = leaf->left->mark;
    if(leaf->right)
        r = leaf->right->mark;
    
    fprintf(outputFile, "%d %d %d\n", leaf->key, l, r);
    
    if(l)
        printTree(outputFile, leaf->left);
    if(r)
        printTree(outputFile, leaf->right);
}

int main() {
    FILE* inputFile = fopen("rotation.in","r");
    int n;
    fscanf(inputFile,"%d", &n);

    FILE* outputFile = fopen("rotation.out","w");
    leaves = calloc(200002, sizeof(struct Leaf));

    int key, left, right;
    for(int i=1;i<=n;i++){
        fscanf(inputFile,"%d %d %d", &key, &left, &right);
        leaves[i].key = key;
        leaves[i].left = left ? &leaves[left] : 0;
        leaves[i].right = right ? &leaves[right] : 0;
    }

    getBalance(&leaves[1]);
    Leaf* firstLeaf = leftRotate(&leaves[1]);

    markNum = 1;
    markLeaves(firstLeaf);

    fprintf(outputFile, "%d\n", n);
    printTree(outputFile, firstLeaf);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}