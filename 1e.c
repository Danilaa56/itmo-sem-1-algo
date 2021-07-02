#include <stdio.h>

typedef struct Person{
    int id;
    float income;
} Person;

int main() {
    FILE* inputFile = fopen("sortland.in","r");
    int n;
    fscanf(inputFile,"%d\n",&n);
    Person array[n];
    float income;
    for(int i=0;i<n;i++){
        fscanf(inputFile,"%f",&income);
        array[i].id = i+1;
        array[i].income = income;
    }
    fclose(inputFile);

    int count = 1;
    typeof(array[0]) tmp;
    while(count!=0){
        count = 0;
        for(int i=1;i<n;i++){
            if(array[i-1].income>array[i].income){
                tmp = array[i];
                array[i] = array[i-1];
                array[i-1] = tmp;
                count++;
            }
        }
    }

    FILE* outputFile = fopen("sortland.out","w");
    fprintf(outputFile,"%d %d %d",array[0].id,array[n/2].id,array[n-1].id);
    fclose(outputFile);

    return 0;
}


