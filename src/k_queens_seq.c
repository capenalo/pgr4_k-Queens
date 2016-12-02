#include<stdio.h>
#include<math.h>

int board[20];
int solCount = 1;

//Temporary print function
void printSolution(int n) {
    int i, j;
    printf("\n\nSolution %d:\n\n", solCount++);

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
            if(row == n) //dead end
                printSolution(n); //printing the board configuration
            else
                queen(row + 1, n);
        }
    }
}

//If there are no conflicts returns 1 otherwise returns 0
int tryPlacingQueen(int row, int col) {
    int i;
    for(i = 1; i <= row-1; i++) {
        if(board[i] == col)
            return 0;
        else
            if(abs(board[i] - col) == abs(i - row)) //two queens in same diagonal
                return 0;
    }
    return 1;
}

int main() {
    int K,i,j;
    void queen(int row, int n);

    printf(" - K Queens Problem -");
    printf("\n\nEnter number of Queens:");
    scanf("%d", &K);
    queen(1, K);
    return 0;
}
