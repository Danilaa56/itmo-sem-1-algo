#include <stdio.h>
#include <stdlib.h>

typedef struct Stack {
    int ar[120];
    int length;
} Stack;

int pull(Stack* stack){
    return stack->ar[--stack->length];
}
void push(Stack* stack, int value){
    stack->ar[stack->length++] = value;
}

Stack stack;

int main() {
    FILE* inputFile = fopen("postfix.in","r");
    FILE* outputFile = fopen("postfix.out","w");

    stack.length = 0;

    char element[20];
    while(fscanf(inputFile,"%s", element)!=-1){
        if(47<element[0] && element[0] < 58){
            int num = 0;
            int index = 0;
            while(element[index]){
                num = num*10 + element[index]-48;
                index++;
            }
            push(&stack, num);
        }else{
            switch(element[0]){
                case '+':
                    stack.length--;
                    stack.ar[stack.length-1] += stack.ar[stack.length];
                    break;
                case '-':
                    stack.length--;
                    stack.ar[stack.length-1] -= stack.ar[stack.length];
                    break;
                case '*':
                    stack.length--;
                    stack.ar[stack.length-1] *= stack.ar[stack.length];
                    break;
            }
        }
    }
    fprintf(outputFile,"%d\n", pull(&stack));

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}