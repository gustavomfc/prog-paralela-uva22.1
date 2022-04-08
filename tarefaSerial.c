#include <math.h>
#include <stdio.h>

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
        printf("Fator sen: %d\n", factor);

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
        printf("Fator coss: %d\n", factor);

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
    double angle = 0.5235987755982988;
    double senoValue = seno(angle, 40);
    printf("Seno: %.4f\n", senoValue);
    printf("Prova Real Sin: %.4f\n", sin(angle));
    
    double cosValue = coss(angle, 40);
    printf("Coseno: %.4f\n", cosValue);
    printf("Prova Real Cos: %.4f\n", cos(angle));
    return 0;
}