//header files
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>

//struct for shared memory while testing
struct shm_struct{
    bool board[25][25];
};

//global variables
int **board, rows, cols, xcord, ycord;
//possible knight moves
int xMoves[8] = { 1, 1, -1, -1, 2, 2, -2, -2 };
int yMoves[8] = { 2, -2, 2, -2, 1, -1, 1, -1 };

//function declarations
bool knightsTourSolver(int x, int y, int moveNum);
int countValidMoves(int x, int y);
bool validMove(int x, int y);

int main(int argc, char **argv){
    if (argc!=5 && argc!=6){
        printf("Usage: %s <Rows> <Columns> <X-coord> <Y-coord> <for automated testing>\n",argv[0]);
        exit(1);
    }

    //checking command line arguments
    char *ptr;
    int test=0;
    rows = strtol(argv[1], &ptr, 10);
    cols = strtol(argv[2], &ptr, 10);
    xcord = strtol(argv[3], &ptr, 10);
    ycord = strtol(argv[4], &ptr, 10);
    if (argc == 6){
        test = strtol(argv[5], &ptr, 10);
    }
    if (strcmp(ptr, "") != 0){
        printf("Arguments are expected to be numbers.\n");
        exit(1);
    }
    if (rows<1 || cols<1){
        printf("Inavlid rows and columns.\n");
        exit(1);
    }
    if (xcord<0 || xcord>=rows || ycord<0 || ycord>=cols){
        printf("Invalid initial coordinates.\n");
        exit(1);
    }

    //memory allocation for board
    board = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++){
        board[i] = (int*)malloc(cols * sizeof(int));
    }

    //initialize starting positions
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            board[i][j] = -1;
        }
    }
    board[xcord][ycord] = 0;

    //checking if its test or general tour check
    if (test != 0){
        //its a test so establish shm
        int shm_fd = shm_open("shm", O_RDWR, 0666);
        if (shm_fd == -1){
            perror("shm_open");
            exit(1);
        }
        // map the shared memory into process's address space using "mmap"
        struct shm_struct *shm_board = mmap(NULL, sizeof(struct shm_struct), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd,0);
        if (shm_board == MAP_FAILED){
            perror("mmap");
            exit(1);
        }

        //initiate solver and update main shm array
        shm_board->board[xcord][ycord] = knightsTourSolver(xcord, ycord, 1);
    }
    else{
        //its a general test, initiate solver
        if (knightsTourSolver(xcord, ycord, 1)){
            printf("Solution(s) exist\n");
            for (int i=0; i<rows; i++){
                for (int j=0; j<cols; j++){
                    printf("%-3d ",board[i][j]);
                }
                printf("\n");
            }
        }
        else{
            printf("No solutions exist\n");
        }
    }

    //cleanup
    for (int i = 0; i < rows; i++){
        free(board[i]);
    }
    free(board);
    return 0;
}

//knight's tour recursive solver function
bool knightsTourSolver(int x, int y, int moveNum){
    //base case if entire board is covered
    if (moveNum == rows*cols){
        return true;
    }

    //local variables to strore knight's next position
    int nextRow, nextCol, nextMoves[8][3];
    for (int i=0; i<8; i++){
        nextMoves[i][0] = -1;
    }

    //general backtracking algorithm
    //initiate the search by selecting a valid move from current position
    // for (int i=0; i<8; i++){
    //     nextRow = x + xMoves[i];
    //     nextCol = y + yMoves[i];
    //     if (validMove(nextRow, nextCol)){
    //         board[nextRow][nextCol] = moveNum;
    //         //if move is valid, recursively try next move
    //         if (knightsTourSolver(nextRow, nextCol, moveNum+1)){
    //             return true;
    //         }
    //         //if recursive calls fail, backtrack and try different move combinations
    //         board[nextRow][nextCol] = -1;
    //     }
    // }
    // //if all calls fail, return false
    // return false;

    //warnsdorff's algorithm
    //count the number of valid moves from the next move to find optimal move (least valid moves)
    for (int i=0; i<8; i++){
        nextRow = x + xMoves[i];
        nextCol = y + yMoves[i];
        if (validMove(nextRow, nextCol)){
            nextMoves[i][1] = nextRow;
            nextMoves[i][2] = nextCol;
            nextMoves[i][0] = countValidMoves(nextRow, nextCol);
        }
    }

    //sort the nextMoves array using insertion sorting algorithm based on number of valid moves
    for (int i=1; i<8; i++){
        int temp[3];
        temp[0] = nextMoves[i][0];
        temp[1] = nextMoves[i][1];
        temp[2] = nextMoves[i][2];
        int j=i-1;
        while(nextMoves[j][0]>temp[0] && j>=0){
            nextMoves[j+1][0]=nextMoves[j][0];
            nextMoves[j+1][1]=nextMoves[j][1];
            nextMoves[j+1][2]=nextMoves[j][2];
            j--;
        }
        nextMoves[j+1][0]=temp[0];
        nextMoves[j+1][1]=temp[1];
        nextMoves[j+1][2]=temp[2];
    }

    //initiate the search by selecting the moves in increasing order of valid moves
    for (int i=0; i<8; i++){
        if (nextMoves[i][0]>-1){
            board[nextMoves[i][1]][nextMoves[i][2]] = moveNum;

            //recursively try next move (validity is checked before when counting valid moves)
            if (knightsTourSolver(nextMoves[i][1], nextMoves[i][2], moveNum+1)){
                return true;
            }
            //if recursive calls fail, backtrack and try different move combinations
            board[nextMoves[i][1]][nextMoves[i][2]] = -1;
        }
    }
    //if all calls fail, return false
    return false;
}

//function to count valid moves from a point
int countValidMoves(int x, int y){
    int count=0;
    for (int i=0; i<8; i++){
        int nextRow = x + xMoves[i];
        int nextCol = y + yMoves[i];
        if (validMove(nextRow, nextCol)){
            count++;
        }
    }
    return count;
}

//function to check if the knights move is within bounds(valid)
bool validMove(int x, int y){
    return (x>=0 && x<rows && y>=0 && y<cols && board[x][y]==-1); 
}