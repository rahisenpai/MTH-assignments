//header files
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>

//struct for shared memory while testing
struct shm_struct{
    bool board[25][25];
};

//function declarations
int testKnightsTour(int x, int y, char **argv);

int main(int argc, char **argv){
    if (argc!=3){
        printf("Usage: %s <Rows> <Columns>\n",argv[0]);
        exit(1);
    }

    //checking command line arguments
    char *ptr;
    int rows = strtol(argv[1], &ptr, 10);
    int cols = strtol(argv[2], &ptr, 10);
    if (strcmp(ptr, "") != 0){
        printf("Arguments are expected to be numbers.\n");
        exit(1);
    }
    if (rows<1 || cols<1){
        printf("Inavlid rows and columns.\n");
        exit(1);
    }
    if (rows>25 || cols>25){
        printf("Upper cap of 25 rows and columns.\n");
        exit(1);
    }

    //memory allocation and initialisation for board
    int shm_fd = shm_open("shm", O_CREAT|O_RDWR, 0666);
    if (shm_fd == -1){
        perror("shm_open");
        exit(1);
    }
    //set desired size for shared memory segment using "ftruncate"
    if (ftruncate(shm_fd, sizeof(struct shm_struct)) == -1){
        perror("ftruncate");
        exit(1);
    }
    //map the shared memory into process's address space using "mmap"
    struct shm_struct *shm_board = mmap(NULL, sizeof(struct shm_struct), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd,0);
    if (shm_board == MAP_FAILED){
        perror("mmap");
        exit(1);
    }

    //initialize the shm board
    for (int i=0; i<25; i++){
        for (int j=0; j<25; j++){
            shm_board->board[i][j] = false;
        }
    }
    //storing children info
    int children = 0;
    int *pid_arr = (int*)malloc(sizeof(int)*rows*cols);

    //testing over entire board
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            pid_arr[children++] == testKnightsTour(i, j, argv);
        }
    }

    //waiting for children to terminate
    for (int i=0; i<children; i++){
        int ret;
        int pid = waitpid(pid_arr[i], &ret, 0);
        if (pid < 0) {
            perror("waitpid");
            exit(1);
        }
        if (!WIFEXITED(ret)) {
            printf("Abnormal termination of %d\n", pid);
        }
    }
    //checking results
    int pass=0,fail=0;
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            if (shm_board->board[i][j] == true){
                pass++;
            }
            else{
                fail++;
            }
        }
    }

    //printing results
    printf("Total possible starting positions for knight's tour: %d\n", rows*cols);
    printf("\nSuccessful starting positions (pass): %d\n", pass);
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            if (shm_board->board[i][j]){
                printf("(%d,%d) ", i, j);
            }
        }
    }
    printf("\n\nUnsuccessful starting positions (fail): %d\n", fail);
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            if (!shm_board->board[i][j]){
                printf("(%d,%d) ", i, j);
            }
        }
    }
    printf("\n");
    return 0;
}

int testKnightsTour(int x, int y, char **argv){
    char xcord[4], ycord[4];
    sprintf(xcord, "%d", x);
    sprintf(ycord, "%d", y);
    //command with arguments to be passed to exec
    char *args[] = {"./tour", argv[1], argv[2], xcord, ycord, "1", NULL};
    //create child process to execute test case
    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("Error forking process");
        exit(1);
    }
    //child process
    if (child_pid == 0) {
        //execute the knightstour program
        if (execvp("./tour", args) == -1) {
            perror("Error executing the program");
            exit(1);
        }
    }
    //parent process
    else{
        return child_pid;
    }
}