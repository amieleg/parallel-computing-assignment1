Ivo
Ivo#3986

Ivo — 11/04/2023 14:15
nee
dat is donderdag
amiel — 11/04/2023 14:15
welk vak deed je nu nou
combinatorics toch?
of heb ik dat verkeerd onthouden
Ivo — 11/04/2023 14:16
nee😅
amiel — 11/04/2023 14:16
shit
had ik het fout
Ivo — 11/04/2023 14:16
ik zou geen combinatorics willen SA'en
hahaha
amiel — 11/04/2023 14:16
bro combinatorics leuk
Ivo — 11/04/2023 14:16
srs?
amiel — 11/04/2023 14:16
ik simp voor recurrence relations
nou de eerste helft niet maar ik vond recurrence relations wel heel interessant
Ivo — 11/04/2023 14:16
really?
amiel — 11/04/2023 14:17
ja...
Ivo — 11/04/2023 14:17
naja ik vond het vooral zo moeilijk dat het niet meer leuk was
amiel — 11/04/2023 14:17
ja was ook wel moeilijk
had er ook vgm maar een 6 voor
maar toch
Ivo — 11/04/2023 14:17
aja
ik weet nog dat ik heel boos was dat het examen alleen antwoorden was
amiel — 11/04/2023 14:18
ja dat was wel bs
Ivo — 18/04/2023 15:23
Attachment file type: unknown
exercise4.tar.xz
210.33 KB
Ivo — 20/04/2023 15:59
Come on Jessica, and come on Tory
Let's go to the mall, you won't be sorry
Put on your jelly bracelets
amiel — 20/04/2023 15:59
and your cool graffiti coats
at the mall having fun
is what its all about
Ivo — 20/04/2023 16:00
aboot*
Canadian accent
amiel — 20/04/2023 16:00
mijn favoriete bar is
there's this boy i like
met him at the foooodcourt
Ivo — 20/04/2023 16:02
he smells like frisbey(?) and he does jumps on his skateboard
hope he asks me out
takes me to my favorite spot
amiel — 20/04/2023 16:02
has hair like gretzky!!!
Ivo — 20/04/2023 16:02
oh hahahaha
amiel — 20/04/2023 16:03
bekende ijshockeyer
Ivo — 20/04/2023 16:03
wow
https://discord.gg/4aSv6fCa
Ivo — 21/04/2023 12:09
Zal ik trouwens Pol mailen om te vragen over dat onze assignments niet worden nagekeken?
amiel — 21/04/2023 12:12
oh sure
misschien zijn ze gewoon sloom
Ivo — 21/04/2023 12:42
bij andere mensen was het niet zo
amiel — 21/04/2023 12:45
hmmm raar
amiel — 10/05/2023 16:26
172.21.152.1
172.21.152.44
172.21.152.79
172.21.152.200
172.21.152.255
172.21.153.135
amiel — Today at 16:17


#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <immintrin.h>
#include <sched.h>
#include <mpi.h>
Expand
message.txt
6 KB
﻿
amiel
amiel#1781
just now i roll with them goons (goons)
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

void Stencil(REAL **in, REAL **out, size_t n, int iterations)
{
    (*out)[0] = (*in)[0];
    (*out)[n - 1] = (*in)[n - 1];

    for (int t = 1; t <= iterations; t++) {
        /* Update only the inner values. */
        for (int i = 1; i < n - 1; i++) {
            (*out)[i] = a * (*in)[i - 1] +
                        b * (*in)[i] +
                        c * (*in)[i + 1];
        }

        /* The output of this iteration is the input of the next iteration (if there is one). */
        if (t != iterations) {
            REAL *temp = *in;
            *in = *out;
            *out = temp;
        }
    }
}

void swap(REAL** in, REAL** out)
{
    REAL *temp = *in;
    *in = *out;
    *out = temp;
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

    int my_rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int viewsize = n / size;
    int shift = 0;

    if(my_rank == 0) {
        shift = n % size;
    }

    viewsize+= shift;

    //printf("Hello world from, rank %d/%d running on CPU %d!\n", my_rank, p, 1);


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
    REAL left_val;
    REAL right_val;
    REAL send_left;
    REAL send_right;
    for(int t = 0; t < iterations; t++) 
    {
        //printf("iteration: %d", t);
        send_left = in[0];
        send_right = in[viewsize-1];
        if(my_rank != 0)
        {
            //printf("rank %d sending to the left \n", my_rank);
            MPI_Send(&send_left, 1, MPI_DOUBLE, my_rank-1, 0, MPI_COMM_WORLD);
        }
        if(my_rank != size-1)
        {
            //printf("rank %d sending to the right \n", my_rank);
            MPI_Send(&send_right, 1, MPI_DOUBLE, my_rank+1, 1, MPI_COMM_WORLD);
        }

        for(int i = 0; i < viewsize; i++)
        {
            if(i == 0)
            {
                if(my_rank != 0)
                {
                    //printf("my rank %i", my_rank);
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
                    //printf("my rank %i", my_rank);
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
            //printf("swapping %p and %p\n", in, out);
            swap(&in, &out);
            //printf("swapped %p and %p\n", in, out);
        }
    }

    /*
    if (show == 1)
    {
        printf("my rank %d ", my_rank);
        for(int i = 0; i < viewsize; i++)
        {
             printf("%lf ", out[i]);
        }
        printf("\n");
    }*/
    
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

    MPI_Finalize();

    return EXIT_SUCCESS;
}