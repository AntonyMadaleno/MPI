// required MPI include file
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc , char *argv [])
{
  int numtasks , rank , len , rc, msg;
  char hostname[MPI_MAX_PROCESSOR_NAME ];

  // initialize MPI
  MPI_Init (&argc ,& argv);
  // get number of tasks
  MPI_Comm_size(MPI_COMM_WORLD ,& numtasks);
  // get my rank
  MPI_Comm_rank(MPI_COMM_WORLD ,& rank);

  long npoint = 10000000000;

  srand(time(NULL) * rank);

  //rank !0
  if (rank != 0)
  {
    double np = 0;
    for (long i = 0; i < npoint; i++)
    {
      double x = (double) rand() / (RAND_MAX);
      double y = (double) rand() / (RAND_MAX);
      double d = x*x + y*y;
      if (d < 1) {np++;}
    }
    //MPI STUFF
    double ret = (double) 4*np/npoint;
    MPI_Send(&ret, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

  }
  else //rank 0
  {
    double buffer;
    double sum;
    for (int i = 1; i < numtasks; i++)
    {
      MPI_Recv(&buffer, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      sum = sum + buffer;
    }

    double pi = sum /(numtasks-1);

    printf("%4.16f\n", pi);
  }

  MPI_Finalize ();
}
