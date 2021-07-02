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

Leaf* smallLeftRotate(Leaf* leaf){
    Leaf* right = leaf->right;

    leaf->right = right->left;
    if(leaf->right)
        leaf->right->parent = leaf;

    right->left = leaf;
    right->parent = leaf->parent;
    leaf->parent = right;

    if(right->parent){
        if(right->parent->left == leaf)
            right->parent->left = right;
        else
            right->parent->right = right;
    }

    if(right->balance==0){
        right->balance = -1;
        right->length++;
    }else{
        right->balance = 0;
    }
    leaf->length = right->length-1;
    return right;
}
Leaf* smallRightRotate(Leaf* leaf){
    Leaf* left = leaf->left;

    leaf->left = left->right;
    if(leaf->left)
        leaf->left->parent = leaf;
    left->right = leaf;
    left->parent = leaf->parent;

    if(left->parent){
        if(left->parent->left == leaf)
            left->parent->left = left;
        else
            left->parent->right = left;
    }

    if(left->balance==0){
        left->balance = 1;
        left->length++;
    }else{
        left->balance = 0;
    }
    leaf->length = left->length-1;
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

    rightLeft->balance = 0;
    rightLeft->length++;

    leaf->length = rightLeft->length-1;
    right->length = leaf->length;

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

    leftRight->balance = 0;
    leftRight->length++;

    leaf->length = leftRight->length-1;
    left->length = leaf->length;

    return leftRight;
}

Leaf* leftRotate(Leaf* leaf){
    return leaf->right->balance == -1 ? bigLeftRotate(leaf) : smallLeftRotate(leaf);
}
Leaf* rightRotate(Leaf* leaf){
    return leaf->left->balance == 1 ? bigRightRotate(leaf) : smallRightRotate(leaf);
}

void insert(Leaf* leaf){
    Leaf* parent = tree.first;
    int key = leaf->key;
    while(1){
        if(parent->key > key){
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
    leaf->parent = parent;
    if(parent->key > key){
        parent->left = leaf;
        if(parent->right){
            parent->balance = 0;
            return;
        }
        else
            parent->balance = -1;
    }
    else {
        parent->right = leaf;
        if (parent->left){
            parent->balance = 0;
            return;
        }
        else
            parent->balance = 1;
    }

    parent->length = 2;
    char isChanged = 1;

    while(isChanged){
        isChanged = 0;
        Leaf* next = parent->parent;
        if(next){
            if(next->length == parent->length){
                isChanged = 1;
                if(next->left == parent){
                    if(next->balance == -1) {
                        parent = rightRotate(next);
                    }else{
                        next->balance--;
                        next->length++;
                        parent = next;
                    }
                } else {
                    if(next->balance == 1){
                        parent = leftRotate(next);
                    }else{
                        next->balance++;
                        next->length++;
                        parent = next;
                    }
                }
            }
        }else{
            tree.first = parent;
        }
    }
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
    markNum = 1;
    markLeaves(tree.first);
    printLeaves(outputFile, tree.first);
}

int main() {
    FILE* inputFile = fopen("addition.in","r");

    int n;
    fscanf(inputFile,"%d", &n);
    if(n==0){
        fscanf(inputFile, "%d", &n);
        fclose(inputFile);

        FILE* outputFile = fopen("addition.out","w");
        fprintf(outputFile, "1\n%d 0 0\n", n);
        fclose(outputFile);

        return 0;
    }

    Leaf* leaves = calloc(n+10, sizeof(Leaf));

    int key, left, right;
    for(int i=1;i<=n;i++){
        fscanf(inputFile,"%d %d %d", &key, &left, &right);
        leaves[i].key = key;
        leaves[i].left = left ? &leaves[left] : 0;
        leaves[i].right = right ? &leaves[right] : 0;
    }
    tree.first = &leaves[1];

    int x;
    fscanf(inputFile, "%d", &x);

    fclose(inputFile);
    FILE* outputFile = fopen("addition.out","w");

    getLengthAndBalance(tree.first);
    tree.first->parent = NULL;

    Leaf* leaf = &leaves[0];
    leaf->key = x;
    leaf->balance = 0;
    leaf->length = 1;
    leaf->left = NULL;
    leaf->right = NULL;
    insert(leaf);

    fprintf(outputFile, "%d\n", n+1);

    printTree(outputFile);

    fclose(outputFile);
    free(leaves);

    return 0;
}