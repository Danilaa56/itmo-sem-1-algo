#include <stdio.h>

int* ar[1000000];
int nums[4000000];
int length = 0;

void heapify(int i){
    int left = 2*i;
    int right = 2*i+1;
    int NEED = i;
    if(left<=length && *ar[left-1]<*ar[NEED-1]){
        NEED = left;
    }
    if(right<=length && *ar[right-1]<*ar[NEED-1]){
        NEED = right;
    }
    if(NEED!=i){
        int *tmp = ar[NEED-1];
        ar[NEED-1] = ar[i-1];
        ar[i-1] = tmp;
        heapify(NEED);
    }
}

void buildHeap(){
    for(int i=length/2;i>0;i--)
        heapify(i);
}

void push(int* x){
    length++;
    ar[length-1] = x;
    int i = length;
    while(i>1 && *ar[(i>>1)-1] > *ar[i-1]){
        int* tmp = ar[i-1];
        ar[i-1] = ar[(i>>1)-1];
        ar[(i>>1)-1] = tmp;
        i >>= 1;
    }
}

void extract(FILE* outputFile){
    if(length==0){
        fprintf(outputFile,"*\n");
        return;
    }
    int min = *ar[0];
    ar[0] = ar[length-1];
    length--;
    heapify(1);
    fprintf(outputFile,"%d\n",min);
}

int main() {
    FILE* inputFile = fopen("priorityqueue.in","r");
    char command[40];
    int x, y;

    int i = 0;

    FILE* outputFile = fopen("priorityqueue.out","w");

    while(fscanf(inputFile,"%s",command)!=-1){
        switch (command[0]) {
            case 'e':
                //operations[i++] = 2000000000;
                extract(outputFile);
                i++;
                break;
            case 'p':
                fscanf(inputFile,"%d",&nums[i]);
                push(&nums[i]);
                i++;
                break;
            case 'd':
                fscanf(inputFile,"%d %d",&x,&y);
                nums[x-1] = y;
                buildHeap();
                i++;
                break;
        }
    }
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}