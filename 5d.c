#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Queue {
    unsigned short* ar;
    int length;
    int left;
} Queue;

Queue structure;

unsigned short pull(){
    return structure.ar[structure.left++];
}
void push(unsigned short value){
    structure.ar[structure.length++] = value;
}
void initQueue(){
    structure.length = 0;
    structure.left = 0;
    structure.ar = malloc(sizeof(int)*400000);
}



int getLabelNum(int labelsCount, char labels[10000][40], int* labelsNums, char* label){
    for(int i = 0; i < labelsCount; i++){
        if(strcmp(labels[i], label) == 0)
            return labelsNums[i];
    }
    printf("Sad\n");
    exit(1);
}

int main() {
    FILE* inputFile = fopen("quack.in","r");
    FILE* outputFile = fopen("quack.out","w");
    initQueue();

    int count = 0;
    char command[40];

    char labels[10000][40];
    int labelsNums[10000];
    int labelsCount = 0;

    while(fscanf(inputFile, "%s", command)!=-1){
        if(command[0]==':') {
            labelsNums[labelsCount] = count;
            strcpy(labels[labelsCount++], command + 1);
            continue;
        }
        count++;
    }
    fseek(inputFile, 0, SEEK_SET);

    char lines[count][40];
    int specialNums[count];

    int index = 0;
    while(fscanf(inputFile, "%s", command)!=-1){
        if(command[0]==':'){
            continue;
        }else if(command[0]=='J'){
            specialNums[index] = getLabelNum(labelsCount, labels, labelsNums, command+1);
        }else if(command[0]=='Z'){
            specialNums[index] = getLabelNum(labelsCount, labels, labelsNums, command+2);
        }else if(command[0]=='E' || command[0]=='G'){
            specialNums[index] = getLabelNum(labelsCount, labels, labelsNums, command+3);
        }
        strcpy(lines[index++], command);
    }



    int pointer = 0;

    int x, y;

    unsigned short reg[123];
    for(int i = 'a'; i <= 'z'; i++)
        reg[i] = 0;

    while(pointer<count){
        switch (lines[pointer][0]) {
            case '+':
                push(pull()+pull());
                break;
            case '-':
                x = pull();
                push(x-pull());
                break;
            case '*':
                push(pull()*pull());
                break;
            case '/':
                x = pull();
                y = pull();
                if(y==0)
                    push(0);
                else
                    push(x/y);
                break;
            case '%':
                x = pull();
                y = pull();
                if(y==0)
                    push(0);
                else
                    push(x%y);
                break;
            case '>':
                x = pull();
                reg[lines[pointer][1]] = x;
                break;
            case '<':
                push(reg[lines[pointer][1]]);
                break;
            case 'P':
                if(lines[pointer][1]==0){
                    x = pull();
                }else{
                    x = reg[lines[pointer][1]];
                }
                fprintf(outputFile,"%d\n", x);
                break;
            case 'C':
                if(lines[pointer][1]==0){
                    x = pull();
                }else{
                    x = reg[lines[pointer][1]];
                }
                fprintf(outputFile,"%c", (char)x);
                break;
            case 'J':
                pointer = specialNums[pointer];
                continue;
            case 'Z':
                if(reg[lines[pointer][1]]==0){
                    pointer = specialNums[pointer];
                    continue;
                }
                break;
            case 'E':
                if(reg[lines[pointer][1]]==reg[lines[pointer][2]]){
                    pointer = specialNums[pointer];
                    continue;
                }
                break;
            case 'G':
                if(reg[lines[pointer][1]]>reg[lines[pointer][2]]){
                    pointer = specialNums[pointer];
                    continue;
                }
                break;
            case 'Q':
                pointer = count;
                break;
            default:
                x = 0;
                y = 0;
                while(lines[pointer][y]){
                    x = x*10 + lines[pointer][y]-48;
                    y++;
                }
                push(x);
                break;
        }
        pointer++;
    }
    fclose(inputFile);
    fclose(outputFile);



    return 0;
}
/*
 *
 * HDD   ~80GB - ~100TB
 * SSD   ~64Gb - ~100Gb
 * ОЗУ   ~2Gb  - ~100Gb
 *
 * Кеш процессора
 * L3    8Mb
 * L2    < 1Mb
 * L1    ~64Kb
 * Регистры процессора ~bytes
 *
 *
 *
 *
 * Указтель 0 - 2^32-1   4Gb
 *
 */