#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Person {
    char* country;
    char* lastname;
} Person;

inline int isLess(Person* value1, Person* value2){
    return strcmp(value1->country,value2->country)<0;
};

void merge(Person** array, int center, int length, Person** cache){
    for(int f=0;f<length;f++){
        cache[f] = array[f];
    }
    Person** current = array;
    Person** leftArray = cache;
    Person** rightArray = cache+center;
    for(int f=0;f<length;f++){
        if(isLess(*rightArray,*leftArray)){
            *current = *rightArray;
            rightArray++;
            current++;
            if(rightArray-cache==length){
                for(int i = length-f-1; i>0; i--){
                    *current = *leftArray;
                    leftArray++;
                    current++;
                }
                return;
            }
        }else{
            *current = *leftArray;
            leftArray++;
            current++;
            if(leftArray-cache==center){
                for(int i = length-f-1; i>0; i--){
                    *current = *rightArray;
                    rightArray++;
                    current++;
                }
                return;
            }
        }
    }
}

void merge_sort(Person** array, int length, Person** cache){
    if(cache==NULL){
        cache = malloc(sizeof(void*)*length);
    }
    if(length==1)
        return;
    if(length==2){
        if(isLess(array[1],array[0])) {
            void* tmp = array[1];
            array[1] = array[0];
            array[0] = tmp;
        }
        return;
    }
    merge_sort(array,length/2,cache);
    merge_sort(array+length/2,(length+1)/2,cache);
    merge(array, length/2, length,cache);
}

int main() {
    FILE* inputFile = fopen("race.in","r");

    int n;

    fscanf(inputFile,"%d\n",&n);

    Person* persons[n];

    int personSize = sizeof(Person);

    Person* person;

    char* countryName;
    char* lastname;

    int stringSize = sizeof(char)*10;

    for(int i=0;i<n;i++){
        countryName = malloc(stringSize);
        lastname = malloc(stringSize);
        fscanf(inputFile,"%s %s\n",countryName,lastname);
        person = malloc(personSize);
        person->lastname = lastname;
        person->country = countryName;
        persons[i] = person;
    }

    fclose(inputFile);

    merge_sort(persons,n, NULL);

    FILE* outputFile = fopen("race.out","w");
    countryName = "";

    for(int f=0;f<n;f++){
        if(strcmp(countryName,persons[f]->country)!=0){
            countryName = persons[f]->country;
            fprintf(outputFile,"=== %s ===\n",countryName);
        }
        fprintf(outputFile,"%s\n",persons[f]->lastname);
    }
    fclose(outputFile);
    return 0;
}