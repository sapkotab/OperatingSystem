
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#define numOfThread 64

// Define the Matrix data type that will also contain pointer to the heap for created matrix
// and address for each beginning of rows.
typedef struct Matrix {
    int r, c;
    int *content;
    int **rows;                 // double pointer is required to create the array of pointer.
}Matrix;

int rowsPerProc;
Matrix A, B, C;
pthread_t tid[numOfThread];


Matrix builtMatrix(int rows, int cols){
    int i;
    Matrix myMatrix;
    myMatrix.r = rows;
    myMatrix.c = cols;

// Allocate memory for matrix
    myMatrix.content = (int *) malloc(sizeof(int) * rows * cols);
    myMatrix.rows = (int **) malloc(sizeof(int *) * rows);

// find and save starting memory address of each rows of the matrix
    for (i = 0; i < rows; i++){
        myMatrix.rows[i] = myMatrix.content + i*cols;
    }

    return myMatrix;
}

void *multiply(void *tid){
    int i, j, k, total;
    int id = (int)tid;                      // casting to int
    int rowStart = id*rowsPerProc;       // deciding which row need to be started

    if(id < numOfThread - 1){
        for (i = rowStart; i < rowStart + rowsPerProc; i++){
            for (j = 0; j < B.c; j++){
                total = 0;
                for (k = 0; k < A.c; k++){
                    total += A.rows[i][k] * B.rows[k][j];
                }
                C.rows[i][j] = total;
            }
        }
    }
    else{                          //last thread gets all leftover rows
        for (i = rowStart; i < rowStart + (A.r - ((numOfThread-1)*rowsPerProc)); i++){
            for (j = 0; j < B.c; j++){
                total = 0;
                for (k = 0; k < A.c; k++){
                    total += A.rows[i][k] * B.rows[k][j];
                }
                C.rows[i][j] = total;
            }
        }
    }
}

void print(Matrix someMatrix){
    int i, j;
    for (i = 0; i < someMatrix.r; i++){
        for (j = 0; j < someMatrix.c; j++){
            printf("%4d ", someMatrix.rows[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {

    int errCheck;
    int i, j;
    int rowsA, columnA, rowsB, columnB;


    FILE *file = fopen("input.txt", "r"); // opening the file inputfile in reading mode.
    if (file == NULL) {
        perror("could not open the file");
        exit(1);
    }

    errCheck = fscanf(file, "%d", &rowsA);         // if file has better no. it errCheck will be 1 else something else
    if (errCheck != 1) {
        printf("invalid integer in input file");
        exit(1);
    }
    errCheck = fscanf(file, "%d", &columnA);
    if (errCheck != 1) {
        printf("invalid integer in input file");
        exit(1);
    }
    A = builtMatrix(rowsA, columnA);

    for (i = 0; i < rowsA; i++) {            // storing matrix A from file
        for (j = 0; j < columnA; j++) {
            errCheck = fscanf(file, "%d", &A.rows[i][j]);
            if (errCheck != 1) {
                printf("invalid integer in input file");
                exit(1);
            }
        }
    }

    errCheck = fscanf(file, "%d", &rowsB);
    if (errCheck != 1) {
        printf("invalid integer in input file");
        exit(1);
    }
    errCheck = fscanf(file, "%d", &columnB);
    if (errCheck != 1) {
        printf("invalid integer in input file");
        exit(1);
    }
    B = builtMatrix(rowsB, columnB);

    for (i = 0; i < rowsB; i++) {            // storing matrix A from file
        for (j = 0; j < columnB; j++) {
            errCheck = fscanf(file, "%d", &B.rows[i][j]);
            if (errCheck != 1) {
                printf("Invalid integer in input file");
                exit(1);
            }
        }
    }

    // Print Matrix A
    printf("Matrix A:\n");
    print(A);

    // Print Matrix B
    printf("\nMatrix B:\n");
    print(B);
    printf("\n");
    // creating placeholder matrix for result.
    C = builtMatrix(rowsA, columnB);

    fclose(file);

    rowsPerProc = rowsA / numOfThread;

// Initialize and start pthreads
    clock_t startT = clock();              // start counting execution time.
    for (i = 0; i < numOfThread; i++) {
        pthread_create(&tid[i], NULL, multiply, (void *) i);
        printf("Created worker thread %d\n", (int) tid[i]);
    }

    for (i = 0; i < numOfThread; i++) {
        pthread_join(tid[i], NULL);
    }
    clock_t endT = clock();// end counting execution time.
    // Print Matrix c
    printf("\nMatrix C = A x B:\n");
    print(C);
    printf("\nTotal execution time using %d threads is %f ms\n", numOfThread, (double) ((endT - startT) * 1000) / CLOCKS_PER_SEC);
    return 0;
}
