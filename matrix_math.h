#ifndef MATRIX_MATH_H_
#define MATRIX_MATH_H_

///////////////////////////////////////////////////////////////////////////////
/* 2D Matrix Multiplications */
///////////////////////////////////////////////////////////////////////////////

int PrintMatrix(double mat[3][3]);
int PrintMatrixN2(double* x, double* y, int n);
int PrintMatrixN3(double* x, double* y, int n);

int CreateIdentity(double mat[3][3]);
int CreateEmpty(double mat[3][3]);

int CopyMatrix(double dest[3][3], double source[3][3]);
int CopyMatrixN(double* dest, double* src, int n);

int MatrixMultiply(double res[3][3], double a[3][3], double b[3][3]);
int MatrixMultiplyPoints(double* res_x, double* res_y, double mat[3][3], double* x, double* y, int n);

int NegateX(double a[3][3], double b[3][3]);
int NegateY(double a[3][3], double b[3][3]);

int Translate(double mat[3][3], double inv[3][3], double dx, double dy);
int Scale(double mat[3][3], double inv[3][3], double sx, double sy);
int Rotate(double mat[3][3], double inv[3][3], double rads);

///////////////////////////////////////////////////////////////////////////////
/* 3D Matrix Multiplications */
///////////////////////////////////////////////////////////////////////////////

int PrintMatrix3D(double mat[4][4]);
int printMatrixN33D(double* x, double* y, double* z, int n);
int printMatrixN43D(double* x, double* y, double* z, int n);

int CreateIdentity3D(double mat[4][4]);
int CreateEmpty3D(double mat[4][4]);

int CopyMatrix3D(double dest[4][4], double source[4][4]);
int CopyMatrixN3D(double* dest, double* src, int n);

int MatrixMultiply3D(double res[4][4], double a[4][4], double b[4][4]);
int MatrixMultiplyPoints3D(double* res_x, double* res_y, double* res_z, double mat[4][4], double* x, double* y, double* z, int n);
int MatrixMultiplyPoint3D(double p[3], double m[4][4], double q[3]);

int NegateX3D(double a[4][4], double b[4][4]);
int NegateY3D(double a[4][4], double b[4][4]);
int NegateZ3D(double a[4][4], double b[4][4]);

int Translate3D(double a[4][4], double b[4][4], double dx, double dy, double dz);
int Scale3D(double a[4][4], double b[4][4], double sx, double sy, double sz);
int RotateX3D(double a[4][4], double b[4][4], double radians);
int RotateY3D(double a[4][4], double b[4][4], double radians);
int RotateZ3D(double a[4][4], double b[4][4], double radians);
int RotateCSX3D(double a[4][4], double b[4][4], double cs, double sn);
int RotateCSY3D(double a[4][4], double b[4][4], double cs, double sn);
int RotateCSZ3D(double a[4][4], double b[4][4], double cs, double sn);

int XProduct3D(double res[3], double a[3], double b[3]);
int MakeMovementSequenceMatrix3D(double mat[4][4], double inv[4][4], int num_movements, int* movement_type_list, double* parameter_list);
int View3D(double view[4][4], double view_inverse[4][4], double eye_x, double eye_y, double eye_z, double coi_x, double coi_y, double coi_z, double up_x, double up_y, double up_z);

#endif /* ifndef MATRIX_MATH_H_ */
