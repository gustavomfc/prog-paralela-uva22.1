#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

float time_diff(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec);
}

signed long long fatorial(long x)
{
    if (x == 0)
        return 1;
    return x * fatorial(x - 1);
}

double seno(double x, int iterations, int startIteration, int endIteration)
{
    double sum = 0;
    int aux = 1;
    int incrementor = 1;
    int count = 0;
    int factor = 0;

    for (int i = 0; i < iterations; i++)
    {
        factor = incrementor + i;
        if (factor > iterations || i > endIteration)
            break;

        if (i >= startIteration)
        {
            double numerador = pow(x, factor);
            signed long long demoninador = fatorial(factor);
            sum = sum + ((aux * numerador) / demoninador);
        }

        aux = -aux;
        incrementor += 1;
    }
    
    return sum;
}

double coss(double x, int iterations, int startIteration, int endIteration)
{
    double sum = 0;
    int aux = 1;
    int incrementor = 0;
    int count = 0;
    int factor = 0;

    for (int i = 0; i < iterations; i++)
    {
        factor = incrementor + i;
        if (factor > iterations || i > endIteration)
            break;

        if (i >= startIteration)
        {
            double numerador = pow(x, factor);
            signed long long  demoninador = fatorial(factor);
            sum = sum + ((aux * numerador) / demoninador);
        }
        aux = -aux;
        incrementor += 1;
    }
    return sum;
}

int main(int argc, char **argv)
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

    MPI_Init(NULL, NULL);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int iterationsPerProc = iterations / world_size;
    int startIter = world_rank * iterationsPerProc;
    int endIter = (world_rank + 1) * iterationsPerProc;
    double angle = 0.5235987755982988;

    double summSeno = seno(angle, iterations, startIter, endIter);

    double summCoss = coss(angle, iterations, startIter, endIter);

    // Print the random numbers on each process
    printf("[Process %d] Start Iter: %d End Iter: %d Seno Soma: %.10f Cos Soma: %.10f\n",
            world_rank, startIter, endIter, summSeno, summCoss);

    // Reduce all of the local sums into the global sum
    double sen_global_sum;
    MPI_Reduce(&summSeno, &sen_global_sum, 1, MPI_DOUBLE, MPI_SUM, 0,
               MPI_COMM_WORLD);
    
    double cos_global_sum;
    MPI_Reduce(&summCoss, &cos_global_sum, 1, MPI_DOUBLE, MPI_SUM, 0,
               MPI_COMM_WORLD);

    // Print the result
    if (world_rank == 0)
    {   
        gettimeofday(&end, NULL);
        printf("Tempo gasto: %0.8f sec\n", time_diff(&start, &end));
        
        printf("-------------------------\n");
        printf("Prova Real Sin: %.18f\n", sin(angle));
        printf("Total Sen = %0.18f\n", sen_global_sum);
        printf("Erro (Real - Taylor): %.18f\n", sin(angle) - sen_global_sum);
        
        printf("-------------------------\n");
        printf("Prova Real Cos: %.18f\n", cos(angle));
        printf("Total Cos = %0.18f\n", cos_global_sum);
        printf("Erro (Real - Taylor): %.18f\n", cos(angle) - cos_global_sum);

        printf("-------------------------\n");
        printf("Tangente  = %0.18f\n", sen_global_sum/cos_global_sum);
        printf("Prova Real Tan: %.18f\n", tan(angle));
        printf("Erro (Real - Taylor): %.18f\n", tan(angle) - (sen_global_sum/cos_global_sum));
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}