#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POWER2 4
#define MASK 15
#define hash_t unsigned unsigned long

short magic[4] = {0x1234,0x5678,0x9abc,0xdef0};

unsigned long long getHash(char* x) {
    unsigned long long hash = 0;
    while(*x){
        hash = (hash<<2) + magic[*x&3]**x;
        x++;
    }
    return hash;
}

typedef struct Branch{
    struct Branch** branches;
} Branch;

typedef struct Node{
    void* value;
    struct Node* next;
    struct Node* prev;
    Branch* parent;
} Node;

typedef struct LinkedMap{
    int length;
    Node* lastNode;
    struct Branch** branches;
} LinkedMap;

Branch mainMap;

void initSet() {
    mainMap.branches = calloc(sizeof(Branch *), 1<<POWER2);
}

Branch* createBranch(){
    Branch* node = malloc(sizeof(Branch));
    node->branches = calloc(sizeof(Branch*),1<<POWER2);
    return node;
}

LinkedMap* createLinkedMap(){
    LinkedMap* map = malloc(sizeof(LinkedMap));
    map->branches = calloc(sizeof(Branch*),1<<POWER2);
    map->length = 0;
    map->lastNode = NULL;
    return map;
}

void destroyNode(LinkedMap* linkedMap, Node* node){
    if(node->prev)
        node->prev->next = node->next;
    if(node->next)
        node->next->prev = node->prev;
    if(linkedMap->lastNode == node)
        linkedMap->lastNode = node->prev;
    free(node->value);
    free(node);
}
void destroyBranch(Branch* branch, int depth){
    if(depth>0)
        for(int f=0;f<1<<POWER2;f++){
            if(branch->branches[f])
                destroyBranch(branch->branches[f],depth-1);
        }
    free(branch);
}

void destroyLinkedMap(LinkedMap* linkedMap){
    Node* node = linkedMap->lastNode;
    while(node){
        Node* tmp = node->prev;
        free(node->value);
        free(node);
        node = tmp;
    }
    for(int f=0;f<1<<POWER2;f++){
        if(linkedMap->branches[f])
            destroyBranch(linkedMap->branches[f],14);
    }
}

void insert(LinkedMap* linkedMap, char* value){
    unsigned long long hash = getHash(value);

    Branch** branches = linkedMap->branches;

    for(int f=1;f<64/POWER2;f++){
        unsigned byte = hash&MASK;
        hash >>= POWER2;
        if(branches[byte]==NULL)
            branches[byte] = createBranch();
        branches = branches[byte]->branches;
    }

    if(branches[hash]==NULL){
        Node* node = malloc(sizeof(Node));
        node->prev = linkedMap->lastNode;
        if(linkedMap->lastNode)
            linkedMap->lastNode->next = node;
        node->next = NULL;
        node->value = malloc(30);
        strcpy((char*)node->value,value);
        linkedMap->lastNode = node;

        branches[hash] = (Branch*)node;
        linkedMap->length++;
    } else {
        Node* node = (Node*)branches[hash];
        strcpy((char*)node->value,value);
    }
}

void deleteFromLinkedMap(LinkedMap* linkedMap, char* value){
    unsigned long long hash = getHash(value);

    Branch** branches = linkedMap->branches;

    for(int f=1;f<64/POWER2;f++){
        unsigned byte = hash&MASK;
        hash >>= POWER2;
        if(branches[byte]==NULL)
            return;
        branches = branches[byte]->branches;
    }
    if(branches[hash]==NULL)
        return;

    destroyNode(linkedMap, (Node*)branches[hash]);
    branches[hash] = NULL;
    linkedMap->length--;
}

void put(char* key, char* value){
    unsigned long long hash = getHash(key);

    Branch** branches = mainMap.branches;

    for(int f=1;f<64/POWER2;f++){
        unsigned byte = hash&MASK;
        hash >>= POWER2;
        if(branches[byte]==NULL)
            branches[byte] = createBranch();
        branches = branches[byte]->branches;
    }

    if(branches[hash]==NULL)
        branches[hash] = (Branch*)createLinkedMap();
    insert((LinkedMap*)branches[hash],value);
}

void delete( char* key, char* value){
    unsigned long long hash = getHash(key);

    Branch** branches = mainMap.branches;

    for(int f=1;f<64/POWER2;f++){
        unsigned byte = hash&MASK;
        hash >>= POWER2;
        if(branches[byte]==NULL)
            return;
        branches = branches[byte]->branches;
    }
    if(branches[hash]==NULL)
        return;

    deleteFromLinkedMap((LinkedMap*)branches[hash],value);
}

void deleteall(char* key){
    unsigned long long hash = getHash(key);

    Branch** branches = mainMap.branches;

    for(int f=1;f<64/POWER2;f++){
        unsigned byte = hash&MASK;
        hash >>= POWER2;
        if(branches[byte]==NULL)
            return;
        branches = branches[byte]->branches;
    }
    if(branches[hash]==NULL)
        return;

    destroyLinkedMap((LinkedMap*)branches[hash]);
    branches[hash] = NULL;
}

LinkedMap* get(char* key){
    unsigned long long hash = getHash(key);

    Branch** branches = mainMap.branches;
    for(int f=1;f<64/POWER2;f++){
        unsigned byte = hash&MASK;
        hash >>= POWER2;
        if(branches[byte]==NULL)
            return NULL;
        branches = branches[byte]->branches;
    }
    return (LinkedMap*)branches[hash];
}


int main() {
    FILE* inputFile = fopen("multimap.in","r");
    char command[20];
    char key[30];
    char value[30];

    FILE* outputFile = fopen("multimap.out","w");

    initSet();

    while(fscanf(inputFile, "%s %s",command, key)!=-1){
        LinkedMap* linkedMap;
        switch (command[0]) {
            case 'p':
                fscanf(inputFile, "%s", value);
                put(key, value);
                break;
            case 'd':
                if(command[6]==0){
                    fscanf(inputFile, "%s", value);
                    delete(key, value);
                }else{
                    deleteall(key);
                }
                break;
            case 'g':
                linkedMap = get(key);
                if(linkedMap){
                    fprintf(outputFile, "%d",linkedMap->length);
                    Node* node = linkedMap->lastNode;
                    while(node){
                        fprintf(outputFile, " %s",node->value);
                        node = node->prev;
                    }
                    fprintf(outputFile, "\n");
                }else{
                    fprintf(outputFile, "0\n");
                }
                break;
        }
    }
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}