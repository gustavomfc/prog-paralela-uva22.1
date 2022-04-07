#include <stdlib.h>
#include <stdio.h>
#include <math.h>

const int nMaxTerms=20;
double factorials[nMaxTerms];

double factorial(int n)
{
    if (n==1)
        return 1;
    else
        return (double)n * factorial(n - 1.0);
}

void precalcFactorials()
{
    for (int i=1; i<nMaxTerms+1; i++)
    {
        factorials[i-1] = factorial(i);
    }
}

/*
    sin(x) = x - (x^3)/3! + (x^5)/5! - (x^7)/7! .......
*/
double taylorSine(double rads)
{
    double result = rads;

    for (int curTerm=1; curTerm<=(nMaxTerms/2)-1; curTerm++)
    {
        double curTermValue = pow(rads, (curTerm*2)+1);
        curTermValue /= factorials[ curTerm*2 ];
        if (curTerm & 0x01)
            result -= curTermValue;
        else
            result += curTermValue;
    }
    return result;
}

/*
    cos(x) = 1 - (x^2)/2! + (x^4)/4! - (x^6)/6! .......
*/
double taylorCos(double rads)
{
    double result = 1.0;
    for (int curTerm=1; curTerm<=(nMaxTerms/2)-1; curTerm++)
    {
        double curTermValue = pow(rads, (curTerm*2) );
        curTermValue /= factorials[ (curTerm*2) - 1 ];
        if (curTerm & 0x01)
            result -= curTermValue;
        else
            result += curTermValue;
    }
    return result;
}

int main()
{
    precalcFactorials();
    double seno_taylor = taylorSine(1.0472);
    double cos_taylor = taylorCos(1.0472);
    double tg_taylor = seno_taylor/cos_taylor;

    printf("Math sin(1.0472) = %f\n", sin(1.0472));
    printf("taylorSin(1.0472) = %f\n", seno_taylor);

    printf("Math cos(1.0472) = %f\n", cos(1.0472));
    printf("taylorCos(1.0472) = %f\n", cos_taylor);

    printf("Math tg(1.0472) = %f\n", tan(1.0472));
    printf("taylorTg(1.0472) = %f\n", tg_taylor);
    return 0;
}
