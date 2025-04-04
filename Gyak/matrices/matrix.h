#ifndef MATRIX_H
#define MATRIX_H

/**
 * Initializes all elements of the matrix to zero.
 */
void init_zero_matrix(float matrix[3][3]);

/**
 * Print the elements of the matrix.
 */
void print_matrix(const float matrix[3][3]);

/**
 * Add matrices.
 */
void add_matrices(const float a[3][3], const float b[3][3], float c[3][3]);

void init_identity_matrix(float matrix[3][3]);//egység mátrix inicializálás

void scalar_multiplication(float matrix[3][3], float n);// skalárral való szorzás

void multiply_matrices(const float a[3][3], const float b[3][3], float c[3][3]);//mátrixok szorzása

void transform_point(int a, int b, const float matrixIn[3][3]);//pont transzformáció

void scale(float scaleNum, const float matrixIn[3][3], float matrixOut[3][3]);//mátrix skálázás
#endif // MATRIX_H

