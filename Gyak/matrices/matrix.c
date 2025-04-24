#include "matrix.h"
#include <math.h>
#include <stdio.h>

#define M_PI 3.14159265358979323846

void init_zero_matrix(float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = 0.0;
        }
    }
}

void print_matrix(const float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            printf("%4.4f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const float a[3][3], const float b[3][3], float c[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

void init_identity_matrix(float matrix[3][3]){
    for (int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            matrix[i][j] =  i == j ? 1:0;
        }
    }

}

void scalar_multiplication(float matrix[3][3], float n){
    for (int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            matrix[i][j] =  matrix[i][j] * n;
        }
    }
}

void multiply_matrices(const float a[3][3], const float b[3][3], float c[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            int sum = 0;
            for(int k = 0; k < 3; k++){
                sum += a[i][k] * b[k][j];
            }
            c[i][j] = sum;
        }
    }
}

void transform_point(int a, int b, const float matrixIn[3][3]){
    int point[] ={a, b, 1};
    int i;
    int j;
    float newPoint[] = {0, 0, 0}; 

    for (i = 0; i < 3; ++i) {
        float sum = 0;
        for (j = 0; j < 3; ++j) {
            sum += point[j] * matrixIn[i][j]; 
        }
        newPoint[i] = sum;
            printf("%f ", newPoint[i]);
    }
    printf("\n");

}

void scale(float scaleNum, const float matrixIn[3][3], float matrixOut[3][3]){
    float scaleMatrix[3][3] = {
    {scaleNum, 0,   0},
    {0,   scaleNum, 0},
    {0,      0,     1}
};
    multiply_matrices(scaleMatrix, matrixIn, matrixOut);
}

void shift(float shiftX, float shiftY, const float matrixIn[3][3], float matrixOut[3][3]){
    float shiftMatrix[3][3] = {
    {1,   0,     shiftX},
    {0,   1,     shiftY},
    {0,      0,     1}
};
    multiply_matrices(shiftMatrix, matrixIn, matrixOut);
}

void rotate(float angleDegree, const float matrixIn[3][3], float matrixOut[3][3]){
    double rad =((angleDegree) * M_PI / 180.0) ;
    float rotateMatrix[3][3] = {
    {cos(rad),   -sin(rad),     0},
    {sin(rad),   cos(rad),      0},
    {0,               0,        1}
};
    multiply_matrices(rotateMatrix, matrixIn, matrixOut);
}