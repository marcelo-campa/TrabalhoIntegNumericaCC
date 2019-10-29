#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <time.h> 
#define N 5000
double area (double p1,double p2);
char letter=' ';


typedef  struct 
{
    double a,b,err;
}integradata ;
struct Stack { 
    int top; 
    unsigned capacity; 
    integradata* array; 
}; 
integradata Buffer[N];
pthread_mutex_t mutex;
double integralTotal=0;
struct Stack *pilha;


double GET_TIME()
{
    clock_t time = clock();
    return (double )time;
}
double module(double n)
{
    if(n <0)n*=-1;
    return n;
}

  
//COMEÇÇA PILHA
struct Stack* createStack(unsigned capacity) 
{ 
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack)); 
    stack->capacity = capacity; 
    stack->top = -1; 
    stack->array = (integradata*)malloc(stack->capacity * sizeof(integradata)); 
    return stack; 
} 
  
int isFull(struct Stack* stack) 
{ 
    return stack->top == stack->capacity - 1; 
} 
  
int isEmpty(struct Stack* stack) 
{ 
    return stack->top == -1; 
} 
  
void push(struct Stack* stack, integradata item) 
{ 
    if (isFull(stack)) 
        {
            printf("Estourou a pilha \n");exit(-1);
        }
    stack->array[++stack->top] = item; 
    //printf("%d pushed to stack\n", item); 
} 
  
integradata* pop(struct Stack* stack) 
{ 
    if (isEmpty(stack)) 
        return NULL; 
    return &stack->array[stack->top--]; 
} 
  
integradata* peek(struct Stack* stack) 
{ 
    if (isEmpty(stack)) 
        return NULL; 
    return &stack->array[stack->top]; 
} 
//TERMINA PILHA



void Integra(double a, double b, double err)
{
    double pmedio= (a+b)/2;
    double areaRetMaior= area(a,b);
    double ret1= area(a,pmedio);
    double ret2=area(pmedio,b);
    double error =  areaRetMaior-  (ret1+ret2) ;
    if( module(error) <err )
    {
        pthread_mutex_lock(&mutex);
        integralTotal+= ret1+ret2;         
    }else
    {
        integradata partea;
        partea.a=a;
        partea.b=pmedio;
        partea.err=err;
        integradata parteb;
        parteb.a= pmedio;
        parteb.b=b;
        parteb.err=err;     
        pthread_mutex_lock(&mutex);   
        push(pilha,partea);     
        push(pilha,parteb);
    }
    pthread_mutex_unlock(&mutex);


}

void* Integral(void* data)
{
    integradata* integraldt = ( integradata*) data;
   
    pthread_mutex_lock(&mutex);
    push(pilha,*integraldt);
    pthread_mutex_unlock(&mutex);

   while(1){
       pthread_mutex_lock(&mutex);
       if(peek(pilha)!=NULL){    
        integradata dt = *pop(pilha);
       // printf("unpopou &lf\n",dt.a);
        pthread_mutex_unlock(&mutex);
        Integra(dt.a,dt.b,dt.err);
        }else{
            pthread_mutex_unlock(&mutex);
            break;
        }


    }
    



   pthread_exit(NULL);

}



double IntegraConcorrente(double a, double b, double err, int nthreads)
{
    pthread_t threads[nthreads];
    pthread_mutex_init(&mutex,NULL);    

    double blocksize = (b-a)/nthreads;

    for(int i=0;i<nthreads;i++)
    {
         integradata* data= (integradata*) malloc(sizeof(integradata));
         data->a=a+(i*blocksize);
         data->b=a+((i+1)*blocksize);
         data->err=err;
         
        // printf("integra de %lf ate %lf\n", data->a,data->b);

         if (pthread_create(&threads[i], NULL, Integral,(void*) data)) 
         {
             printf("--ERRO: pthread_create()\n"); exit(-1);
        
        }
    
    }
    for(int i=0;i<nthreads;i++)
    {
        if (pthread_join(threads[i],NULL)) {
            printf("--ERRO: pthread_join()\n"); exit(-1);
        }
    }
    return integralTotal;
    

}

int main(int argc ,char *argv[])
{
    if(argc!=6)
    {
        printf("Por favor digite os argumentos corretamente :\nLetra da funcao , Valor a, Valor b, Valor do erro,Numero de Threads");
        return 0;        
    }
    letter = tolower(argv[1][0]);
    if(letter<'a'||letter>'g'){
        printf("O a letra da integral nao e valida\n");
        return 0;
    }
    pilha = createStack(1000); 
    
    double a = atof(argv[2]),b=atof(argv[3]),erro= atof(argv[4]);    
    int nThreads=atoi(argv[5]);
    double inicio = GET_TIME();
    //double integra = Integral(a,b,erro);
    double integra=IntegraConcorrente(a,b,erro,nThreads);
    double fim = GET_TIME();
    printf("O valor da integral %lf\n",integra);
    printf("Levando tempo de %0.lf milisegundos ",fim-inicio);
    pthread_exit(NULL);
    return 0;
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
