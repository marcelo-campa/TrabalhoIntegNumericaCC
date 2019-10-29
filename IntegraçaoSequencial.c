#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <time.h> 

double Integral(double a, double b, double err);
double area (double p1,double p2);
char letter=' ';
double GET_TIME()
{
    clock_t time = clock();
    return (double )time;
}

int main(int argc ,char *argv[])
{
    if(argc!=5)
    {
        printf("Por favor digite os argumentos corretamente :\nLetra da funcao , Valor a, Valor b, Valor do erro");
        return 0;        
    }
    letter = tolower(argv[1][0]);
    if(letter<'a'||letter>'g'){
        printf("O a letra da integral nao e valida\n");
        return 0;
    }
    double a = atof(argv[2]),b=atof(argv[3]),erro= atof(argv[4]);    
    double ini= GET_TIME();

    double integra = Integral(a,b,erro);
    double fim= GET_TIME();
    printf("O valor da integral %lf\n",integra);
    printf("Tempo total de integracao %0.lf milisegundos\n",fim-ini);

   

}
double f(double x)
{
    switch (letter)
    {
    case 'a':
        return 1+x;
        break;
    case 'b':
        if(x<-1 ||x>1)
        {
            printf("A fora do dominio permitido \n");
            exit(-1);
        }
        return sqrt(1-pow(x,2));
        break;
    case 'c':
        return sqrt(1+pow(x,4));
        break;
    case 'd':
        return sin(pow(x,2));
        break;
    case 'e':
        return cos(exp(-x));
        break;
    case 'f':
        return cos(exp(-x))*x;
        break;
    case 'g':
        return cos(exp(-x))*(0.005*pow(x,3)+1);
        break;
    
    
    }

    return pow(x,3);
}
double area (double p1,double p2){
    return (p2-p1)*f(p2);
}
double module(double n)
{
    if(n <0)n*=-1;
    return n;
}
double Integral(double a, double b, double err)
{
    double pmedio= (a+b)/2;
    double areaRetMaior= area(a,b);
    double ret1= area(a,pmedio);
    double ret2=area(pmedio,b);
    double error =  areaRetMaior-  (ret1+ret2) ;
    if( module(error) <err )
    {
        return ret1+ret2;
    }else
     return Integral (a,pmedio,err)+Integral(pmedio,b,err);
    

}