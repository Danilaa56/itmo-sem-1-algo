#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POWER2 8
#define MASK 255

unsigned short magic[128] = {0xd76a, 0xa478, 0xe8c7, 0xb756, 0x2420, 0x70db, 0xc1bd, 0xceee,
                             0xf57c, 0x0faf, 0x4787, 0xc62a, 0xa830, 0x4613, 0xfd46, 0x9501,
                             0x6980, 0x98d8, 0x8b44, 0xf7af, 0xffff, 0x5bb1, 0x895c, 0xd7be,
                             0x6b90, 0x1122, 0xfd98, 0x7193, 0xa679, 0x438e, 0x49b4, 0x0821,
                             0xf61e, 0x2562, 0xc040, 0xb340, 0x265e, 0x5a51, 0xe9b6, 0xc7aa,
                             0xd62f, 0x105d, 0x0244, 0x1453, 0xd8a1, 0xe681, 0xe7d3, 0xfbc8,
                             0x21e1, 0xcde6, 0xc337, 0x07d6, 0xf4d5, 0x0d87, 0x455a, 0x14ed,
                             0xa9e3, 0xe905, 0xfcef, 0xa3f8, 0x676f, 0x02d9, 0x8d2a, 0x4c8a,
                             0xfffa, 0x3942, 0x8771, 0xf681, 0x6d9d, 0x6122, 0xfde5, 0x380c,
                             0xa4be, 0xea44, 0x4bde, 0xcfa9, 0xf6bb, 0x4b60, 0xbebf, 0xbc70,
                             0x289b, 0x7ec6, 0xeaa1, 0x27fa, 0xd4ef, 0x3085, 0x0488, 0x1d05,
                             0xd9d4, 0xd039, 0xe6db, 0x99e5, 0x1fa2, 0x7cf8, 0xc4ac, 0x5665,
                             0xf429, 0x2244, 0x432a, 0xff97, 0xab94, 0x23a7, 0xfc93, 0xa039,
                             0x655b, 0x59c3, 0x8f0c, 0xcc92, 0xffef, 0xf47d, 0x8584, 0x5dd1,
                             0x6fa8, 0x7e4f, 0xfe2c, 0xe6e0, 0xa301, 0x4314, 0x4e08, 0x11a1,
                             0xf753, 0x7e82, 0xbd3a, 0xf235, 0x2ad7, 0xd2bb, 0xeb86, 0xd391};

unsigned long long getHash(char* x) {
    unsigned short a0 = 0x8765;
    unsigned short b0 = 0x4321;
    unsigned short c0 = 0x1234;
    unsigned short d0 = 0x5678;

    unsigned short a,b,c,d,f;

    while(*x){
        a = a0;
        b = b0;
        c = c0;
        d = d0;

        f = ((b&c)|((~b)&d)) + a + magic[(*x)%128];
        a = d;
        d = c;
        c = b;
        b = b + ((f<<4)|(f>>12));

        f = (b^c^d) + a + magic[(*x)%128];
        a = d;
        d = c;
        c = b;
        b = b + ((f<<4)|(f>>12));

        a0 += a;
        b0 += b;
        c0 += c;
        d0 += d;
        x++;
    }
    unsigned long long hash = a0;
    hash = (hash<<16) + b0;
    hash = (hash<<16) + c0;
    hash = (hash<<16) + d0;
    //printf("%s   %llu %u %u %u %u\n", start, hash, a0, b0, c0, d0);
    return hash;
}

typedef struct Node{
  void* value;
  struct Node* next;
  struct Node* prev;
} Node;

typedef struct Branch{
    struct Branch** branches;
} Branch;

typedef struct Map{
    Branch** branches;
} Map;

Map map;
Node* lastNode = NULL;

void initSet() {
    map.branches = calloc(sizeof(Branch *), 1<<POWER2);
}

Branch* createBranch(){
    Branch* node = malloc(sizeof(Branch));
    node->branches = calloc(sizeof(Branch*),1<<POWER2);
    return node;
}

void destroyNode(Node* node){
    if(node->prev)
        node->prev->next = node->next;
    if(node->next)
        node->next->prev = node->prev;
    if(node==lastNode)
        lastNode = node->prev;
    free(node->value);
    free(node);
}

void put(char* key, char* value){
    unsigned long long hash = getHash(key);

    Branch** branches = map.branches;

    for(int f=1;f<64/POWER2;f++){
        unsigned byte = hash&MASK;
        hash >>= POWER2;
        if(branches[byte]==NULL)
            branches[byte] = createBranch();
        branches = branches[byte]->branches;
    }

    if(branches[hash]==NULL){
        Node* node = malloc(sizeof(Node));
        node->prev = lastNode;
        if(lastNode)
            lastNode->next = node;
        node->next = NULL;
        node->value = malloc(40);
        strcpy((char*)node->value,value);
        lastNode = node;

        branches[hash] = (Branch*)node;
    } else {
        Node* node = (Node*)branches[hash];
        strcpy((char*)node->value,value);
    }
}

void delete(char* key){
    unsigned long long hash = getHash(key);

    Branch** branches = map.branches;

    for(int f=1;f<64/POWER2;f++){
        unsigned byte = hash&MASK;
        hash >>= POWER2;
        if(branches[byte]==NULL)
            return;
        branches = branches[byte]->branches;
    }
    if(branches[hash]==NULL)
        return;

    destroyNode((Node*)branches[hash]);
    branches[hash] = NULL;
}

Node* getNode(char* key){
    unsigned long long hash = getHash(key);

    Branch** branches = map.branches;
    for(int f=1;f<64/POWER2;f++){
        unsigned byte = hash&MASK;
        hash >>= POWER2;
        if(branches[byte]==NULL)
            return NULL;
        branches = branches[byte]->branches;
    }
    return (Node*)branches[hash];
}

char* get(char* key){
    Node* node = getNode(key);
    if(node)
        return node->value;
    return NULL;
}

char* prev(char* key){
    Node* node = getNode(key);
    if(node && node->prev)
        return node->prev->value;
    return NULL;
}

char* next(char* key){
    Node* node = getNode(key);
    if(node && node->next)
        return node->next->value;
    return NULL;
}


int main() {
    FILE* inputFile = fopen("linkedmap.in","r");
    char command[20];
    char key[40];
    char value[40];

    FILE* outputFile = fopen("linkedmap.out","w");

    initSet();

    while(fscanf(inputFile, "%s %s",command, key)!=-1){
        char* str;
        switch (command[0]) {
            case 'p':
                if(command[1]=='u'){
                    fscanf(inputFile, "%s", value);
                    put(key, value);
                } else {
                    str = prev(key);
                    fprintf(outputFile, "%s\n", str?str:"none");
                }
                break;
            case 'd':
                delete(key);
                break;
            case 'g':
                str = get(key);
                fprintf(outputFile, "%s\n", str?str:"none");
                break;
            case 'n':
                str = next(key);
                fprintf(outputFile, "%s\n", str?str:"none");
                break;
        }
    }
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}