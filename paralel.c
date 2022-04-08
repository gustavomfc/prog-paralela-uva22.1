/**********************************
 * MPI cosine(x) code.            *
 *                                *
 * Taylor series representation   *
 * of the trigonometric cosine(x).*
 *                                *
 * Author: Carlos R. Morrison     *
 *                                *
 * Date: 1/10/2017                *
 **********************************/
#include<mpi.h>// (Open)MPI library
#include<math.h>// math library
#include<stdio.h>// Standard Input/Output library

int main(int argc, char*argv[])
{
  long long int total_iter,B,D;
  int n = 17,rank,length,numprocs,i,j;
  unsigned long int k;
  double sum,sum0,rank_integral,A,C,E;
  float angle,ang_rad;
  char hostname[MPI_MAX_PROCESSOR_NAME];

  MPI_Init(&argc, &argv);                    // initiates MPI
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);  // acquire number of processes
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);      // acquire current process id
  MPI_Get_processor_name(hostname, &length); // acquire hostname

  if (rank == 0)
  {
    printf("#######################################################\n");
    printf("*** Number of processes: %d\n",numprocs);

    printf("Master node name: %s\n", hostname);
    printf("Enter angle(deg.):\n");
    scanf("%f",&angle_deg);
    if(angle_deg <= 180.0)
    {
      angle_rad = angle_deg*(M_PI/180.0);
    }
    else
    ang_rad = -(360.0-angle_deg)*(M_PI/180.0);
  }

// broadcast to all processes, the number of segments you want
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&ang_rad, 1, MPI_INT, 0, MPI_COMM_WORLD);

// this loop increments the maximum number of iterations, thus providing
// additional work for testing computational speed of the processors
//for(total_iter = 1; total_iter < n; total_iter++)
  {
    sum0 = 0.0;
//  for(i = rank + 1; i <= total_iter; i += numprocs)
    for(i = rank + 1; i <= n; i += numprocs)
    {
      k = (i-1);

      A = (double)pow(-1,k);// (-1^k)
      B = 2*k;
      C = (double)pow(x,B);// x^2k

      D = 1;
      for(j=1; j <= B; j++)// (2k!)
      {
        D *= j;
      }
      E = (A*C)/(double)D;

      sum0 += E;
    }

    rank_integral = sum0;// Partial sum for a given rank

//  collect and add the partial sum0 values from all processes
    MPI_Reduce(&rank_integral, &sum, 1, MPI_DOUBLE,MPI_SUM, 0,
    MPI_COMM_WORLD);

  }// End of for(total_iter = 1; total_iter < n; total_iter++)

  if(rank == 0)
  {
    printf("%.1f deg. = %.3f rads\n", ang_deg, ang_rad);
    printf("Cosine(%.3f) = %.3f\n", ang_deg, sum);
  }
// clean up, done with MPI
  MPI_Finalize();

  return 0;
}// End of int main(int argc, char*argv[])