#include <stdio.h>
#include <stdlib.h>

typedef struct Queue {
    int* ar;
    int length;
    int left;
} Queue;

int pull(Queue* structure){
    return structure->ar[structure->left++];
}
void push(Queue* stack, int value){
    stack->ar[stack->length++] = value;
}

int main() {
    FILE* inputFile = fopen("queue.in","r");
    int m;
    fscanf(inputFile,"%d", &m);

    Queue structure;
    structure.length = 0;
    structure.left = 0;
    structure.ar = malloc(sizeof(int)*1000000);

    FILE* outputFile = fopen("queue.out","w");

    char c;
    int num;
    for(int i=0;i<m;i++){
        fscanf(inputFile,"\n%c", &c);
        if(c=='-'){
            fprintf(outputFile,"%d\n", pull(&structure));
        }
        else{
            fscanf(inputFile," %d", &num);
            push(&structure, num);
        }
    }
    fclose(outputFile);
    /*

    fprintf(outputFile,"%s",array[0]);
    for(int i=1;i<n;i++){
        fprintf(inputFile,"\n%s",array[i]);
    }
    fclose(outputFile);*/
    return 0;
}