#include <stdio.h>
#include <stdlib.h>

typedef struct Stack {
    char ar[10240];
    int length;
} Stack;

char pull(Stack* stack){
    return stack->ar[--stack->length];
}
char getLast(Stack* stack) {
    if(stack->length==0)
        return '0';
    return stack->ar[stack->length-1];
}
void push(Stack* stack, char value){
    stack->ar[stack->length++] = value;
}

Stack stack;

int check(char* brackets){
    stack.length = 0;
    char last = '0';
    while(*brackets){
        switch(*brackets){
            case '[':
            case '(':
                push(&stack,*brackets);

                break;
            case ')':
                if(getLast(&stack)=='('){
                    pull(&stack);
                }else{
                    return 0;
                }
                break;
            case ']':
                if(getLast(&stack)=='['){
                    pull(&stack);
                }else{
                    return 0;
                }
                break;
        }
        brackets++;
    }
    return stack.length==0;
}

int main() {
    FILE* inputFile = fopen("brackets.in","r");
    FILE* outputFile = fopen("brackets.out","w");

    char brackets[10240];

    while(fscanf(inputFile,"%s\n", brackets)!=-1){
        fprintf(outputFile,"%s\n", check(brackets) ? "YES" : "NO");
    }

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}