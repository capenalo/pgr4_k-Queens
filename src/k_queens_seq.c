#include<stdio.h>
#include <stdlib.h>
#define __USE_C99_MATH
#include <stdbool.h>
#include <sys/time.h>

#include<math.h>

int* board;
//int board[20];
long long solCount = 0;

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

//function to check positioning of queen
void queen(int row, int n) {
    int col;
    for(col = 1; col <= n; col++) {
        if(tryPlacingQueen(row, col)) {
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

int main() {
    int K,i,j;
    void queen(int row, int n);

    printf(" - K Queens Problem -");
    printf("\n\nEnter number of Queens:");
    scanf("%d", &K);
    board = (int*)malloc((K+1)*sizeof(int));
    gettimeofday(&tp1, NULL);
    queen(1, K);
    gettimeofday(&tp2, NULL);
    double seq_time_result = (double) (tp2.tv_usec - tp1.tv_usec) / 1000000 + (double) (tp2.tv_sec - tp1.tv_sec);
    printf("\n\nTotal number of solutions: %lld\n", solCount );
    printf("ts,%.9f\n",seq_time_result);
    free(board);
    return 0;
}
