#include <stdio.h>
#include <stdlib.h>


typedef struct Leaf{
    struct Leaf* leftChild;
    struct Leaf* rightChild;
    int value;
} Leaf;

typedef struct Tree{
    Leaf* first;
} Tree;

void placeLeaf(Leaf* dst, Leaf* leaf){
    if(leaf->value < dst->value){//left;
        if(dst->leftChild){
            placeLeaf(dst->leftChild, leaf);
        }else{
            dst->leftChild = leaf;
        }
    }else if(leaf->value > dst->value){//right
        if(dst->rightChild){
            placeLeaf(dst->rightChild, leaf);
        }else{
            dst->rightChild = leaf;
        }
    }
}

void insert(Tree* tree, int value){
    Leaf* leaf = malloc(sizeof(Leaf));
    leaf->value = value;
    leaf->leftChild = NULL;
    leaf->rightChild = NULL;
    if(!tree->first){
        tree->first = leaf;
    }else{
        placeLeaf(tree->first, leaf);
    }
}

void deleteLeaf(Leaf* leaf, int value){
    if(value < leaf->value){
        if(leaf->leftChild){
            if(leaf->leftChild->value == value){
                Leaf* leftSubChild = leaf->leftChild->leftChild;
                Leaf* rightSubChild = leaf->leftChild->rightChild;
                leaf->leftChild = NULL;
                free(leaf->leftChild);
                /*if(leftSubChild){
                    if(rightSubChild){
                        placeLeaf(rightSubChild, leftSubChild);

                    }else{
                        leaf->leftChild = leftSubChild;
                    }
                }else{
                    leaf->leftChild = rightSubChild;
                }*/
                if(leftSubChild)
                    placeLeaf(leaf, leftSubChild);
                if(rightSubChild)
                    placeLeaf(leaf, rightSubChild);
            }else{
                deleteLeaf(leaf->leftChild, value);
            }
        }
    }
    if(value > leaf->value){
        if(leaf->rightChild){
            if(leaf->rightChild->value == value){
                Leaf* leftSubChild = leaf->rightChild->leftChild;
                Leaf* rightSubChild = leaf->rightChild->rightChild;
                leaf->rightChild = NULL;
                free(leaf->rightChild);
                if(leftSubChild)
                    placeLeaf(leaf, leftSubChild);
                if(rightSubChild)
                    placeLeaf(leaf, rightSubChild);
            }else{
                deleteLeaf(leaf->rightChild, value);
            }
        }
    }
}

void delete(Tree* tree, int value){
    Leaf* oldFirst = tree->first;
    if(oldFirst){
        if(oldFirst->value == value){
            if(oldFirst->leftChild){
                if(oldFirst->rightChild){
                    tree->first = oldFirst->rightChild;
                    placeLeaf(tree->first, oldFirst->leftChild);
                    free(oldFirst);
                }else{
                    tree->first = oldFirst->leftChild;
                    free(oldFirst);
                }
            }else{
                tree->first = oldFirst->rightChild;
                free(oldFirst);
            }
        }else{
            deleteLeaf(tree->first, value);
        }
    }
}

int check(struct Leaf* leaf, int min, int max){
    if(!leaf)
        return 1;
    int value = leaf->value;
    if(value<min || value>max)
        return 0;
    if(!check(leaf->leftChild, min, value-1))
        return 0;
    if(!check(leaf->rightChild, value+1, max))
        return 0;
    return 1;
}


void printLeaf(Leaf* leaf, int level){
    if(!leaf)
        return;
    for(int i=0;i<level;i++)
        printf(" ");
    printf("%d\n", leaf->value);
    printLeaf(leaf->leftChild, level+2);
    printLeaf(leaf->rightChild, level+2);
}

void printTree(Tree* tree){
    if(tree->first){
        printLeaf(tree->first, 0);
    }else{
        printf("Empty tree\n");
    }
}

int exists(Tree* tree, int value){
    Leaf* leaf = tree->first;
    while(leaf){
        if(leaf->value == value){
            return 1;
        }
        if(value < leaf->value)
            leaf = leaf->leftChild;
        else
            leaf = leaf->rightChild;
    }
    return 0;
}

#define NONE 1010101010
int next(Leaf* leaf, int value){
    if(!leaf)
        return NONE;

    int l = next(leaf->leftChild, value);
    if(l!=NONE){
        return l;
    }
    if(leaf->value > value)
        return leaf->value;
    return next(leaf->rightChild, value);
}


int prev(Leaf* leaf, int value){
    if(!leaf)
        return NONE;

    int r = prev(leaf->rightChild, value);
    if(r!=NONE){
        return r;
    }
    if(leaf->value < value)
        return leaf->value;
    return prev(leaf->leftChild, value);
}



int main() {
    FILE* inputFile = fopen("bstsimple.in","r");
    char command[40];
    int x;

    FILE* outputFile = fopen("bstsimple.out","w");

    Tree tree;
    tree.first = NULL;

    while(fscanf(inputFile, "%s %d",command, &x)!=-1){
        int tmp;
        switch (command[0]) {
            case 'i':
                insert(&tree, x);
                break;
            case 'd':
                delete(&tree, x);
                break;
            case 'e':
                fprintf(outputFile, "%s\n", exists(&tree, x) ? "true" : "false");
                break;
            case 'n':
                tmp = next(tree.first, x);
                if(tmp == NONE)
                    fprintf(outputFile, "none\n");
                else
                    fprintf(outputFile,"%d\n", tmp);
                break;
            case 'p':
                tmp = prev(tree.first, x);
                if(tmp == NONE)
                    fprintf(outputFile,"none\n");
                else
                    fprintf(outputFile,"%d\n", tmp);
                break;
        }
        //printf("Tree state is: %d\n", check(tree.first,-1000000000, 1000000000));
        //printTree(&tree);
    }
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}