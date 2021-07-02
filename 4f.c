#include <stdio.h>

int n;
double a;

double getLength(double a2){
    double lastLast = a;
    double last = a2;
    for(int i=3; i<=n; i++){
        double p = last*2+200 - lastLast;
        if(p<0){
            return -1;
        }
        lastLast = last;
        last = p;
    }
    return last;
}

double epsilon = 0.00000001;
double findRighty(double left, double right){
    if(left==right)
        return left;
    if(right-left<epsilon){
        if(getLength(right)!=-1)
            return right;
        else
            return left;
    }
    if(getLength((left+right)/2)==-1){
        return findRighty((left+right)/2, right);
    }else{
        return findRighty(left, (left+right)/2);
    }
}

int main() {
    FILE* inputFile = fopen("garland.in","r");
    FILE* outputFile = fopen("garland.out","w");

    fscanf(inputFile, "%d %lf", &n, &a);
    a *= 100;

    fprintf(outputFile, "%0.2lf\n",getLength(findRighty(0,a*100))/100);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}