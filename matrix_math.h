#ifndef MATRIX_MATH_H_
#define MATRIX_MATH_H_

int PrintMatrix(double mat[3][3]);
int PrintMatrixN2(double* x, double* y, int n);
int PrintMatrixN3(double* x, double* y, int n);

int CreateIdentity(double mat[3][3]);
int CreateEmpty(double mat[3][3]);

int CopyMatrix(double dest[3][3], double source[3][3]);

int MatrixMultiply(double res[3][3], double a[3][3], double b[3][3]);
int MatrixMultiplyPoints(double* res_x, double* res_y, double mat[3][3], double* x, double* y, int n);

int Translate(double mat[3][3], double inv[3][3], double dx, double dy);
int Scale(double mat[3][3], double inv[3][3], double sx, double sy);
int Rotate(double mat[3][3], double inv[3][3], double rads);

#endif /* ifndef MATRIX_MATH_H_ */
