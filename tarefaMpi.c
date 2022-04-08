#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <math.h>
#include <time.h>

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
    double summSeno = seno(0.5235987755982988, iterations, startIter, endIter);

    double summCoss = coss(0.5235987755982988, iterations, startIter, endIter);

    // Print the random numbers on each process
    printf("[Process %d] Start Iter: %d End Iter: %d Seno Soma: %f Cos Soma: %f\n",
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
        printf("Total Sen = %f\n", sen_global_sum);
        printf("Total Cos = %f\n", cos_global_sum);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}