#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <mpi.h>

#define MASTER  0
#define WORKER  1
#define TAG     1
#define TOTALMSGSIZE 1000000
#define STARTSIZE 100000
#define INCREMENT 100000
#define RUNS 100

#define MAX 25
int main(int argc, char* argv[])
{
    int i, j, my_rank, num_nodes;
    double t1,t2;
    char my_host[MAX];
    char message[TOTALMSGSIZE];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_nodes);
    
    for (i = 0; i < TOTALMSGSIZE; i++){
        message[i] = 'a';
    }
    gethostname (my_host, MAX);
    if (my_rank == MASTER) {
        int nbytes;
        double bandwidth, time, avg_time, avg_bw;
        printf("MASTER host: %s\n",my_host);
        printf ("runs,message size, avg time (s), avg bandwidth(mb)\n");
	for(i = STARTSIZE; i < TOTALMSGSIZE; i+=INCREMENT){
            nbytes =  sizeof(char) * i;
            avg_time = 0.0;
            avg_bw = 0.0;
            for (j = 0; j < RUNS; j++){
                t1 = MPI_Wtime();
                MPI_Send(&message, i, MPI_CHAR, WORKER, TAG, MPI_COMM_WORLD);
                MPI_Recv(&message, i, MPI_CHAR, WORKER, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                t2 = MPI_Wtime();
                time = t2-t1;
                bandwidth = ((double)nbytes * 2) / time;
                avg_time = (avg_time + time) / (j+1);
                avg_bw = (avg_bw + bandwidth) / (j+1); 
            }
            
            printf ("%d,%d,%2.9f,%4.2f\n",RUNS,i,avg_time, avg_bw/1000000.0);
        }

    }
    else {
        printf("WORKER host: %s\n",my_host);
        for(i = STARTSIZE; i < TOTALMSGSIZE; i+=INCREMENT){
            for (j = 0; j < RUNS; j++){
                MPI_Recv(&message, i, MPI_CHAR, MASTER, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(&message, i, MPI_CHAR, MASTER, TAG, MPI_COMM_WORLD);
            }
        }
    } 
    
    MPI_Finalize();
    return 0;
}
