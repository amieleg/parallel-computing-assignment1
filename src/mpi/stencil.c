#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <immintrin.h>
#include <sched.h>
#include <mpi.h>

#define REAL double

/* You may need a different method of timing if you are not on Linux. */
#define TIME(duration, fncalls)                                        \
    do {                                                               \
        struct timeval tv1, tv2;                                       \
        gettimeofday(&tv1, NULL);                                      \
        fncalls                                                        \
        gettimeofday(&tv2, NULL);                                      \
        duration = (REAL) (tv2.tv_usec - tv1.tv_usec) / 1000000 +    \
         (REAL) (tv2.tv_sec - tv1.tv_sec);                           \
    } while (0)

const REAL a = 0.1;
const REAL b = 0.2;
const REAL c = 0.3;


void swap(REAL** in, REAL** out)
{
    REAL *temp = *in;
    *in = *out;
    *out = temp;
}

void stencil(size_t n, int iterations, int show)
{
    int my_rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int viewsize = n / size;
    int shift = 0;

    if(my_rank == 0) {
        shift = n % size;
    }

    viewsize+= shift;

    REAL *in = calloc(viewsize, sizeof(REAL));
    REAL *out = malloc(viewsize * sizeof(REAL));

    if (my_rank == 0)
    {
        in[0] = 100;
    }
    if (my_rank == (size-1))
    {
        in[viewsize-1] = 1000;
    }

    REAL left_val, right_val, send_left, send_right;
    
    for(int t = 0; t < iterations; t++) 
    {
        send_left = in[0];
        send_right = in[viewsize-1];
        if(my_rank != 0)
        {
            MPI_Send(&send_left, 1, MPI_DOUBLE, my_rank-1, 0, MPI_COMM_WORLD);
        }
        if(my_rank != size-1)
        {
            MPI_Send(&send_right, 1, MPI_DOUBLE, my_rank+1, 1, MPI_COMM_WORLD);
        }

        for(int i = 0; i < viewsize; i++)
        {
            if(i == 0)
            {
                if(my_rank != 0)
                {
                    MPI_Recv(&left_val, 1, MPI_DOUBLE, my_rank-1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    out[i] = left_val * a + in[i] * b + in[i+1] * c;
                }
                else
                {
                    out[i] = in[i];
                }
            }
            else if(i == (viewsize - 1))
            {
                if(my_rank != size-1)
                {
                    MPI_Recv(&right_val, 1, MPI_DOUBLE, my_rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    out[i] = in[i-1] * a + in[i] * b + right_val * c;
                }
                else
                {
                    out[i] = in[i];
                }
            }
            else
            {
                out[i] = in[i-1] * a + in[i] * b + in[i+1] * c;
            }
        }

        if (t != iterations-1) {
            swap(&in, &out);
        }
    }
    
    REAL *output = NULL;

    if(my_rank == 0)
    {
        viewsize-= shift;
        output = malloc(n * sizeof(REAL));

        for(int i = 0; i < shift; i++) {
            output[i] = out[i];
        }
    }

    MPI_Gather(out + shift, viewsize, MPI_DOUBLE, output + shift, viewsize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (show == 1 && my_rank == 0)
    {
        printf("After gathering:");
        for(int i = 0; i < n; i++)
        {
             printf("%lf ", output[i]);
        }
        printf("\n");
    }

    free(in);
    free(out);
    free(output);
}


int main(int argc, char **argv)
{
    if (argc != 4) {
        printf("Please specify 3 arguments (n, iterations, show).\n");
        return EXIT_FAILURE;
    }

    size_t n = atoll(argv[1]);
    int iterations = atoi(argv[2]);
    int show = atoi(argv[3]);

    double duration;
    MPI_Init(&argc, &argv);
    TIME(duration, stencil(n, iterations, show););
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    if(my_rank == 0) {
        float gflops = ((float) (((float) n*5*iterations)/1000000000))/duration;
        printf("%lfs, %f\n", duration, gflops);
    }
    MPI_Finalize();

    return EXIT_SUCCESS;
}