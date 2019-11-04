#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gasdev.c"

typedef double(*funcPtr)(double, double, int*);
typedef struct distributor{
    funcPtr func;
    char* name;
    double arg1;
    double arg2; 
} distributor;

double uniform_random(double l, double r, int* idum);
double gaussian_random(double  m, double s, int* idum);
void make_distribution(distributor generator, int sections, int samples);

int main(){
    distributor distributors[2] = {
        { uniform_random, "Uniform Distribution", -3.0, 2.0 },
        { gaussian_random, "Gaussian Distribution", -0.5, 1.5 }
    }; 

    for(int samples = 100; samples<=1000000; samples*=100){
        for(int sections=50; sections<=100; sections*=2){
            for(int i=0;i<2;i++){
                make_distribution(distributors[i], sections, samples);            
            }    
        }
    }
   return 0;
}

double uniform_random(double l, double r, int* idum){
    double p = ran1(idum);
    return l + (r-l)*p;
}
double gaussian_random(double  m, double s, int* idum){
    double n = gasdev(idum);
    return n*s + m;
}



void make_distribution(distributor generator, int sections, int samples){
    char* name = generator.name;
    funcPtr func = generator.func;
    double arg1 = generator.arg1;
    double arg2 = generator.arg2;
    double l,r;
    double interval;
    int idum=1;

    // set interval boundary
    if(name=="Uniform Distribution"){
        l=arg1;
        r=arg2;
    }
    else{
        l=arg1-4*arg2;
        r=arg1+4*arg2;
    }
    interval = (r-l)/sections;
    
    // make distribution
    int cnt[201] = { 0 };
    for(int i=0;i<samples;i++){
        double num;
        do{
            num = func(arg1, arg2, &idum);
        }while(num<l || num>r);
        int step = (int)((num-l)/interval);
        if(num-(l+interval*step) > interval/2) step++;
        cnt[step]++;
    }

    // save distribution
    char filename[50];
    sprintf(filename, "%s_%d_%d.csv", name,sections,samples);
    printf("%s\n\n", filename);
    FILE* fp;
    fp=fopen(filename, "w+");
    fprintf(fp,",x,cnt\n");
    for(int i=0;i<=sections;i++){
        fprintf(fp,"%d,%lf,%d\n",i,l+i*interval,cnt[i]);
    }
    fclose(fp);
}