#include <stdio.h>
#include <stdlib.h>

/*
Using ChatGPT
*/

// Function to print the matrix
void printMatrix(double* matrix, int rows, int cols) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            printf("%f ", matrix[i*cols + j]);
        }
        printf("\n");
    }
}

// Function to add two matrices
void addMatrix(double* a, double* b, double* result, int rows, int cols) {
    for(int i = 0; i < rows*cols; i++) {
        result[i] = a[i] + b[i];
    }
}

// Function to multiply two matrices
void multiplyMatrix(double* a, int aRows, int aCols, double* b, int bRows, int bCols, double* result) {
    if(aCols != bRows) {
        printf("Incompatible dimensions for multiplication\n");
        return;
    }

    for(int i = 0; i < aRows; i++) {
        for(int j = 0; j < bCols; j++) {
            double sum = 0;
            for(int k = 0; k < aCols; k++) {
                sum += a[i*aCols + k] * b[k*bCols + j];
            }
            result[i*bCols + j] = sum;
        }
    }
}

// Function to transpose a matrix
void transposeMatrix(double* matrix, double* result, int rows, int cols) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            result[j*rows + i] = matrix[i*cols + j];
        }
    }
}

// Function to reshape a matrix (optional)
void reshapeMatrix(double* matrix, int originalRows, int originalCols, double* result, int newRows, int newCols) {
    // Simple copy, assuming originalRows * originalCols == newRows * newCols
    for(int i = 0; i < originalRows * originalCols; i++) {
        result[i] = matrix[i];
    }
}

int main() {
    // Example usage
    int rows = 2, cols = 3;
    double a[] = {1, 2, 3, 4, 5, 6};
    double b[] = {6, 5, 4, 3, 2, 1};
    double resultAdd[rows*cols];
    double resultTranspose[cols*rows];
    double resultMultiply[rows*rows]; // For the sake of example, assuming square result for multiplication

    addMatrix(a, b, resultAdd, rows, cols);
    printf("Added Matrix:\n");
    printMatrix(resultAdd, rows, cols);

    transposeMatrix(a, resultTranspose, rows, cols);
    printf("Transposed Matrix:\n");
    printMatrix(resultTranspose, cols, rows);

    multiplyMatrix(a, rows, cols, b, cols, rows, resultMultiply); // b transposed implicitly for example
    printf("Multiplied Matrix:\n");
    printMatrix(resultMultiply, rows, rows);

    return 0;
}
