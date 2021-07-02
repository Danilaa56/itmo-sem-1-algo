#include <stdio.h>
#include <stdlib.h>

typedef struct Leaf{
    int balance;
    int length;
    int key;
    int mark;
    int leftMark;
    int rightMark;
    struct Leaf* left;
    struct Leaf* right;
    struct Leaf* parent;
} Leaf;

struct WonderfulTree{
    Leaf* first;
} tree;

void getLengthAndBalance(Leaf* leaf){
    int l = 0;
    int r = 0;
    if(leaf->left){
        leaf->left->parent = leaf;
        getLengthAndBalance(leaf->left);
        l = leaf->left->length;
    }
    if(leaf->right){
        leaf->right->parent = leaf;
        getLengthAndBalance(leaf->right);
        r = leaf->right->length;
    }

    leaf->balance = r-l;
    leaf->length = r>l ? r+1 : l+1;
}

void defineLengthAndBalance(Leaf* leaf){
    int l = leaf->left ? leaf->left->length : 0;
    int r = leaf->right ? leaf->right->length : 0;

    leaf->balance = r-l;
    leaf->length = r>l ? r+1 : l+1;
}

Leaf* smallLeftRotate(Leaf* leaf){
    Leaf* right = leaf->right;

    leaf->right = right->left;
    if(leaf->right)
        leaf->right->parent = leaf;

    right->left = leaf;
    right->parent = leaf->parent;
    if(leaf->parent){
        if(leaf->parent->left == leaf)
            leaf->parent->left = right;
        else
            leaf->parent->right = right;
    }
    leaf->parent = right;

    defineLengthAndBalance(leaf);
    defineLengthAndBalance(right);
    return right;
}
Leaf* smallRightRotate(Leaf* leaf){
    Leaf* left = leaf->left;

    leaf->left = left->right;
    if(leaf->left)
        leaf->left->parent = leaf;

    left->right = leaf;
    left->parent = leaf->parent;
    if(leaf->parent){
        if(leaf->parent->left == leaf)
            leaf->parent->left = left;
        else
            leaf->parent->right = left;
    }
    leaf->parent = left;

    defineLengthAndBalance(leaf);
    defineLengthAndBalance(left);
    return left;
}

Leaf* bigLeftRotate(Leaf* leaf){
    Leaf* right = leaf->right;
    Leaf* rightLeft = right->left;

    right->left = rightLeft->right;
    if(right->left)
        right->left->parent = right;
    leaf->right = rightLeft->left;
    if(leaf->right)
        leaf->right->parent = leaf;

    rightLeft->left = leaf;
    rightLeft->right = right;
    rightLeft->parent = leaf->parent;
    if(leaf->parent){
        if(leaf->parent->left == leaf)
            leaf->parent->left = rightLeft;
        else
            leaf->parent->right = rightLeft;
    }
    leaf->parent = rightLeft;
    right->parent = rightLeft;

    defineLengthAndBalance(leaf);
    defineLengthAndBalance(right);
    defineLengthAndBalance(rightLeft);
    return rightLeft;
}
Leaf* bigRightRotate(Leaf* leaf){
    Leaf* left = leaf->left;
    Leaf* leftRight = left->right;

    left->right = leftRight->left;
    if(left->right)
        left->right->parent = left;
    leaf->left = leftRight->right;
    if(leaf->left)
        leaf->left->parent = leaf;

    leftRight->right = leaf;
    leftRight->left = left;
    leftRight->parent = leaf->parent;
    if(leaf->parent){
        if(leaf->parent->left == leaf)
            leaf->parent->left = leftRight;
        else
            leaf->parent->right = leftRight;
    }
    leaf->parent = leftRight;
    left->parent = leftRight;

    defineLengthAndBalance(leaf);
    defineLengthAndBalance(left);
    defineLengthAndBalance(leftRight);

    return leftRight;
}

Leaf* leftRotate(Leaf* leaf){
    return leaf->right->balance == -1 ? bigLeftRotate(leaf) : smallLeftRotate(leaf);
}
Leaf* rightRotate(Leaf* leaf){
    return leaf->left->balance == 1 ? bigRightRotate(leaf) : smallRightRotate(leaf);
}

void normalize(Leaf* leaf){
    defineLengthAndBalance(leaf);

    if(leaf->balance > 1){
        leaf = leftRotate(leaf);
    }else if(leaf->balance < -1){
        leaf = rightRotate(leaf);
    }

    if(leaf->parent){
        normalize(leaf->parent);
    }else{
        tree.first = leaf;
    }
}

void insert(Leaf* leaf){
    if(!tree.first){
        tree.first = leaf;
        leaf->balance = 0;
        leaf->length = 1;
        leaf->left = NULL;
        leaf->right = NULL;
        leaf->parent = NULL;
        return;
    }

    Leaf* parent = tree.first;
    int key = leaf->key;
    while(1){
        if(key < parent->key){
            if(parent->left){
                parent = parent->left;
                continue;
            }else{
                break;
            }
        }
        if(parent->key < key){
            if(parent->right){
                parent = parent->right;
                continue;
            }else{
                break;
            }
        }
        return;
    }

    leaf->length = 1;
    leaf->balance = 0;
    leaf->left = NULL;
    leaf->right = NULL;
    leaf->parent = parent;
    if(parent->key > key)
        parent->left = leaf;
    else
        parent->right = leaf;

    normalize(parent);
}
void delete(int key){
    Leaf* leaf = tree.first;
    while(leaf && leaf->key != key){
        if(leaf->key > key)
            leaf = leaf->left;
        else if(leaf->key < key)
            leaf = leaf->right;
        else break;
    }
    if(!leaf)
        return;

    Leaf* parent = leaf->parent;
    if(!leaf->left && !leaf->right) {
        if(parent){
            if (parent->left == leaf) {
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
            normalize(parent);
        }else{
            tree.first = NULL;
        }
    } else if(!leaf->left){
        leaf->right->parent = parent;
        if(parent){
            if(parent->left == leaf){
                parent->left = leaf->right;
            }else{
                parent->right = leaf->right;
            }
            normalize(parent);
        }else{
            tree.first = leaf->right;
        }
    } else {
        Leaf* R = leaf->left;
        while(R->right)
            R = R->right;

        Leaf* toNormalize;
        if(leaf->left == R){
            toNormalize = R;
        } else {
            toNormalize = R->parent;
            R->parent->right = R->left;
            if(R->left){
                R->left->parent = R->parent;
            }
            R->left = leaf->left;
            R->left->parent = R;
        }
        R->right = leaf->right;
        if(R->right)
            R->right->parent = R;

        R->parent = leaf->parent;
        if(leaf->parent){
            if(leaf->parent->left == leaf)
                leaf->parent->left = R;
            else
                leaf->parent->right = R;
        }
        R->length = leaf->length;

        normalize(toNormalize);
    }
}
int contains(int key){
    Leaf* leaf = tree.first;
    while(leaf && leaf->key != key){
        if(key < leaf->key)
            leaf = leaf->left;
        else if(leaf->key < key)
            leaf = leaf->right;
        else break;
    }
    if(!leaf)
        return 0;
    return 1;
}

int markNum = 1;
void markLeaves(Leaf* leaf){
    leaf->mark = markNum++;
    if(leaf->left){
        markLeaves(leaf->left);
        leaf->leftMark = leaf->left->mark;
    }
    if(leaf->right){
        markLeaves(leaf->right);
        leaf->rightMark = leaf->right->mark;
    }
}
void printLeaves(FILE* outputFile, Leaf* leaf){
    fprintf(outputFile, "%d %d %d\n", leaf->key, leaf->leftMark, leaf->rightMark);

    if(leaf->leftMark)
        printLeaves(outputFile, leaf->left);
    if(leaf->rightMark)
        printLeaves(outputFile, leaf->right);
}

void printTree(FILE* outputFile){
    if(!tree.first){
        return;
    }
    markNum = 1;
    markLeaves(tree.first);
    printLeaves(outputFile, tree.first);
}

int main() {
    FILE* inputFile = fopen("avlset.in","r");
    FILE* outputFile = fopen("avlset.out","w");

    int n;
    fscanf(inputFile,"%d", &n);

    Leaf* leaves = calloc(n+10, sizeof(Leaf));

    tree.first = NULL;

    char command[5];
    int key;
    for(int i=1;i<=n;i++){
        fscanf(inputFile,"%s %d", command, &key);
        Leaf* leaf;
        switch (command[0]) {
            case 'A':
                leaf = &leaves[i];
                leaf->key = key;
                insert(leaf);
                fprintf(outputFile, "%d\n", tree.first ? tree.first->balance : 0);
                //printf("%d\n", tree.first ? tree.first->balance : 0);
                break;
            case 'D':
                delete(key);
                fprintf(outputFile, "%d\n", tree.first ? tree.first->balance : 0);
                //printf("%d\n", tree.first ? tree.first->balance : 0);
                break;
            case 'C':
                fprintf(outputFile, "%c\n", "NY"[contains(key)]);
                //printf("%c\n", "NY"[contains(key)]);
                break;
        }
    }

    free(leaves);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}