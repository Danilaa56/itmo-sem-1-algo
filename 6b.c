#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POWER2 8
#define MASK 255

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

typedef struct Map{
    Branch** branches;
} Map;

Map map;

void initSet() {
    map.branches = calloc(sizeof(Branch *), 1<<POWER2);
}

Branch* createBranch(){
    Branch* node = malloc(sizeof(Branch));
    node->branches = calloc(sizeof(Branch*),1<<POWER2);
    return node;
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

    if(branches[hash]!=NULL)
        free(branches[hash]);

    branches[hash] = malloc(30);
    strcpy((char*)branches[hash],value);
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

    free(branches[hash]);
    branches[hash] = NULL;
}

char* get(char* key){
    unsigned long long hash = getHash(key);

    Branch** branches = map.branches;

    for(int f=1;f<64/POWER2;f++){
        unsigned byte = hash&MASK;
        hash >>= POWER2;
        if(branches[byte]==NULL)
            return "none";
        branches = branches[byte]->branches;
    }
    if(branches[hash]==NULL)
        return "none";
    return (char*)branches[hash];
}



unsigned long long ar[456976];
int count = 0;
int collisions = 0;
void check(char* str, int level){
    if(level==0){
        ar[count] = getHash(str);
        for(int f=0;f<count;f++){
            if(ar[f]==ar[count]){
                printf("Collision %d    %s\n", ++collisions, str);
            }
        }
        return;
    }
    for(char f='a';f<='z';f++){
        str[level] = f;
        check(str, level-1);
    }
}


int main() {
    FILE* inputFile = fopen("map.in","r");
    char command[10];
    char key[30];
    char value[30];

    FILE* outputFile = fopen("map.out","w");

    initSet();

    while(fscanf(inputFile, "%s %s",command, key)!=-1){
        switch (command[0]) {
            case 'p':
                fscanf(inputFile, "%s", value);
                put(key, value);
                break;
            case 'd':
                delete(key);
                break;
            case 'g':
                fprintf(outputFile, "%s\n", get(key));
                break;
        }
    }
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}