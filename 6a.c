#include <stdio.h>
#include <stdlib.h>

#define POWER2 4
#define MASK 15

unsigned int getHash(int x){
    return x;
}

typedef struct Node{
    struct Node** nodes;
} Node;

typedef struct Set{
    Node** nodes;
} Set;

Set set;

void initSet() {
    set.nodes = calloc(sizeof(Node *), 1<<POWER2);
}

Node* createNode(){
    Node* node = malloc(sizeof(Node));
    node->nodes = calloc(sizeof(Node*),1<<POWER2);
    return node;
}

void insert(int value){
    unsigned int hash = getHash(value);

    Node** nodes = set.nodes;

    for(int f=1;f<32/POWER2;f++){
        unsigned byte = hash&MASK;
        hash >>= POWER2;
        if(nodes[byte]==NULL)
            nodes[byte] = createNode();
        nodes = nodes[byte]->nodes;
    }

    if(nodes[hash]==NULL) {
        nodes[hash] = malloc(sizeof(int));
        *(int*)nodes[hash] = value;
    }
}

void delete(int value){
    unsigned int hash = getHash(value);

    Node** nodes = set.nodes;

    for(int f=1;f<32/POWER2;f++){
        unsigned byte = hash&MASK;
        hash >>= POWER2;
        if(nodes[byte]==NULL)
            return;
        nodes = nodes[byte]->nodes;
    }
    if(nodes[hash]==NULL)
        return;

    free(nodes[hash]);
    nodes[hash] = NULL;
}

int exists(int value){
    unsigned int hash = getHash(value);

    Node** nodes = set.nodes;

    for(int f=0;f<32/POWER2;f++){
        unsigned byte = hash&MASK;
        hash >>= POWER2;
        if(nodes[byte]==NULL)
            return 0;
        nodes = nodes[byte]->nodes;
    }
    return 1;
}

int main() {
    FILE* inputFile = fopen("set.in","r");
    char command[40];
    int x;

    FILE* outputFile = fopen("set.out","w");

    initSet();

    while(fscanf(inputFile, "%s %d",command, &x)!=-1){
        switch (command[0]) {
            case 'i':
                insert(x);
                break;
            case 'd':
                delete(x);
                break;
            case 'e':
                fprintf(outputFile, "%s\n", exists(x) ? "true" : "false");
                break;
        }
    }
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}