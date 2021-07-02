#include <stdio.h>
#include <stdlib.h>

typedef struct Stack {
    int* ar;
    int length;
} Stack;

int pull(Stack* stack){
    return stack->ar[--stack->length];
}
void push(Stack* stack, int value){
    stack->ar[stack->length++] = value;
}

int main() {
    FILE* inputFile = fopen("stack.in","r");
    int n;
    fscanf(inputFile,"%d", &n);


    Stack stack;
    stack.length = 0;
    stack.ar = malloc(sizeof(int)*1000000);

    FILE* outputFile = fopen("stack.out","w");

    char c;
    int num;
    for(int i=0;i<n;i++){
        fscanf(inputFile,"\n%c", &c);
        if(c=='-'){
            fprintf(outputFile,"%d\n", pull(&stack));
        }
        else{
            fscanf(inputFile," %d", &num);
            push(&stack, num);
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