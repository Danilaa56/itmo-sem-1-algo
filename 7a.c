#include <stdio.h>
#include <stdlib.h>
#define problem = 2000000000

struct Leaf* ar;

struct Leaf{
    int balance;
    int length;
    int key;
    struct Leaf* left;
    struct Leaf* right;
};

void getBalance(struct Leaf* leaf){
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

int main() {
    FILE* inputFile = fopen("addition.out","r");
    int n;
    fscanf(inputFile,"%d", &n);

    FILE* outputFile = fopen("balance.out","w");
    ar = calloc(200002, sizeof(struct Leaf));


    int key, left, right;
    for(int i=1;i<=n;i++){
        fscanf(inputFile,"%d %d %d", &key, &left, &right);
        ar[i].key = key;
        ar[i].left = left ? &ar[left] : 0;
        ar[i].right = right ? &ar[right] : 0;
    }

    getBalance(&ar[1]);
    for(int i=1; i <= n; i++)
        fprintf(outputFile, "%d\n", ar[i].balance);

    free(ar);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}