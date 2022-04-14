#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

float time_diff(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec);
}

long long fatorial(int x)
{
    if (x == 0) return 1;
    return x * fatorial(x-1);
}


double seno(double x, int interations)
{
    double sum = 0;
    int aux = 1;
    int incrementor = 1;
    int count = 0;
    int factor = 0;

    for (int i = 0; i < interations; i++) 
    {
        factor = incrementor + i;
        if (factor > interations) break;

        double numerador = pow(x, factor);
        long demoninador = fatorial(factor);
        sum = sum + ((aux * numerador)/demoninador);
        aux = -aux;
        incrementor += 1;
    }
    return sum;
}

double coss(double x, int interations)
{
    double sum = 0;
    int aux = 1;
    int incrementor = 0;
    int count = 0;
    int factor = 0;

    for (int i = 0; i < interations; i++) 
    {
        factor = incrementor + i;
        if (factor > interations) break;

        double numerador = pow(x, factor);
        long demoninador = fatorial(factor);
        sum = sum + ((aux * numerador)/demoninador);
        aux = -aux;
        incrementor += 1;
    }
    return sum;
}

int main (int argc, char** argv)
{
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);
    
    if (argc != 2)
    {
        fprintf(stderr, "Uso: Numero de iterações\n");
        exit(1);
    }

    int iterations = atoi(argv[1]);
    printf("Iterações: %d\n", iterations);

    double angle = 0.5235987755982988;
    double senoValue = seno(angle, iterations);
    printf("-------------------------\n");
    printf("Seno: %.18f\n", senoValue);
    printf("Prova Real Sin: %.18f\n", sin(angle));
    printf("Erro (Real - Taylor): %.18f\n", sin(angle) - senoValue);
    
    double cosValue = coss(angle, iterations);
    printf("-------------------------\n");
    printf("Coseno: %.18f\n", cosValue);
    printf("Prova Real Cos: %.18f\n", cos(angle));
    printf("Erro (Real - Taylor): %.18f\n", cos(angle) - cosValue);

    printf("-------------------------\n");
    printf("Tangente  = %0.18f\n", senoValue/cosValue);
    printf("Prova Real Tan: %.18f\n", tan(angle));
    printf("Erro (Real - Taylor): %.18f\n", tan(angle) - (senoValue/cosValue));
    gettimeofday(&end, NULL);
    printf("Tempo gasto: %0.8f sec\n", time_diff(&start, &end));
    return 0;
}