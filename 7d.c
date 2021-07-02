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
    if(leaf->parent){
        if(leaf->parent->left == leaf)
            leaf->parent->left = right;
        else
            leaf->parent->right = right;
    }
    leaf->parent = right;

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
    if(leaf->parent){
        if(leaf->parent->left == leaf)
            leaf->parent->left = left;
        else
            leaf->parent->right = left;
    }
    leaf->parent = left;

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

void normalize(Leaf* leaf){
    int length = leaf->length;
    int r = 0;
    int l = 0;
    if(leaf->right)
        r = leaf->right->length;
    if(leaf->left)
        l = leaf->left->length;
    leaf->balance = r-l;
    leaf->length = (leaf->balance<0) ? l+1 : r+1;

    if(leaf->balance > 1){
        leaf = leftRotate(leaf);
    }else if(leaf->balance < -1){
        leaf = rightRotate(leaf);
    }

    if(leaf->parent){
        normalize(leaf->parent);
        //if(length != leaf->length)
            //normalize(leaf->parent);
    }else{
        tree.first = leaf;
    }
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
    FILE* inputFile = fopen("deletion.in","r");

    int n;
    fscanf(inputFile,"%d", &n);

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
    FILE* outputFile = fopen("deletion.out","w");

    getLengthAndBalance(tree.first);
    tree.first->parent = NULL;

    delete(x);

    fprintf(outputFile, "%d\n", n-1);
    printTree(outputFile);

    fclose(outputFile);
    free(leaves);

    return 0;
}