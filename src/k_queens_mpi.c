// MPI-based computation of PI
// demonstrates how to use rank to determine workload
// gw

#include <stdio.h>
#include <stdlib.h>
#define __USE_C99_MATH
#include <math.h>
#include <stdbool.h>
#include <sys/time.h>
#include <mpi.h>

#define MASTER  0
#define TAG     0

int* board;
long solCount = 0;

struct timeval  tp1, tp2;

bool tryPlacingQueen(int row, int col);
void queen(int row, int n);
void incrementSolCount();
void printSolution(int n);

void incrementSolCount(){
    solCount++;
}

//Temporary print function
void printSolution(int n) {
    int i, j;
    printf("\n\nSolution %lld:\n\n", solCount);

    for(i = 1; i <= n; i++)
        printf("\t%d", i);

    for(i = 1; i <= n; i++) {
        printf("\n\n%d", i);

        for(j = 1; j <= n; j++) {
            if(board[i] == j)
                printf("\tQ"); // print Q at i,j position
            else
                printf("\t-"); //print empty slot
        }
    }
    printf("\n");
}

//If there are no conflicts returns 1 otherwise returns 0
bool tryPlacingQueen(int row, int col) {
    int i;
    for(i = 1; i <= row-1; i++) {
        if(board[i] == col){
            return false;
        }
        else {
            if(abs(board[i] - col) == abs(i - row)) {//two queens in same diagonal
                return false;
            }
        }
    }
    return true;
}

//function to check positioning of queen
void queen(int row, int n) {
    int col;
    for(col = 1; col <= n; col++) {
        if(tryPlacingQueen(row, col)){
            board[row] = col; //no conflicts so place queen
            if(row == n){ //dead end
                incrementSolCount();
                //printSolution(n); //printing the board configuration
            }
            else {
                queen(row + 1, n);
            }
        }
    }
}

int kqueen_seq(int K) {
    int i,j;
    void queen(int row, int n);
    board = (int*)malloc((K+1)*sizeof(int));
    gettimeofday(&tp1, NULL);
    queen(1, K);
    gettimeofday(&tp2, NULL);
    double seq_time_result = (double) (tp2.tv_usec - tp1.tv_usec) / 1000000 + (double) (tp2.tv_sec - tp1.tv_sec);
    printf("\n\nSEQ: Total number of solutions: %lld\n", solCount );
    printf("ts,%.9f\n",seq_time_result);
    free(board);
    return 0;
}

int kqueen_par(int K, int queen_col) {
    void queen(int row, int n);
    board = (int*)malloc((K+1)*sizeof(int));
    board[1] = queen_col;
    queen(2, K);
    free(board);
    return 0;
}

int main(int argc, char* argv[])
{
    int my_rank, num_nodes, source;  //MPI related
    int board_size, pos_buff;        //problem related
    long partial_sol, total_sol;

    if (argc != 2) {
        fprintf (stderr, "usage: k_queens_mpi board_size\n");
        exit(-1);
    }
    // number of rectangles to use per process
    board_size = atol (argv[1]);
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_nodes);
    
    int queen_pos = my_rank + 1;
    kqueen_par(board_size, queen_pos); 
    
    
    if (my_rank == MASTER) {
        
        printf("\n\nStarting Parallel! #Nodes: %d \n\n",num_nodes);
        //1. check if num_nodes < board_size
        bool board_completed = true;
        if(num_nodes < board_size){
            board_completed = false;
        }
        int current_pos, processed_cols = 0;
        while(!board_completed){
            current_pos = num_nodes + processed_cols;
            if(board_size == current_pos){
                board_completed = true;
            }
            for (source = 1; source < num_nodes; source++) {
                pos_buff = current_pos + source;
                if(pos_buff <= board_size){
                    printf("\tCalled Source:%d pos_buff:%d\n",source, pos_buff);
                    MPI_Send(&pos_buff, 1, MPI_INT, source, TAG, MPI_COMM_WORLD);
                    processed_cols++;
                }
            }
        }
	for (source = 1; source < num_nodes; source++) {
            pos_buff = -1;
            MPI_Send(&pos_buff, 1, MPI_INT, source, TAG, MPI_COMM_WORLD);
        }
        //2. receive partial results 
        total_sol = solCount;
        for (source = 1; source < num_nodes; source++) {
            MPI_Recv(&partial_sol, 1, MPI_LONG, source, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_sol+=partial_sol;
        }
        printf("\n\nTotal sol count:%d!\n\n",total_sol);
    }
    else {
        bool waiting_for_work = true;
        while(waiting_for_work){
            MPI_Recv(&pos_buff, 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if(pos_buff != -1){
                kqueen_par(board_size, pos_buff);
                //printf("\n\nRANK: %d Partial number of solutions: %lld\n", my_rank, solCount);
            }else{
                //No more cols to be processed;
                waiting_for_work = false;
            }
        }
        //time to send the results
        partial_sol = solCount;
        MPI_Send(&partial_sol, 1, MPI_LONG, MASTER, TAG, MPI_COMM_WORLD);
        printf("\n\nRANK: %d Partial number of solutions: %lld\n", my_rank, partial_sol);
    }

    MPI_Finalize();

    return 0;
}
