#include "matrix_math.h"

#include <math.h>
#include <stdio.h>

int PrintMatrix(double mat[3][3])
{
  for (int i = 0; i < 3; i++) {
    if (i == 0) {
      printf(" \u23A1");
    } else if (i == 1) {
      printf(" \u23A2");
    } else if (i == 2) {
      printf(" \u23A3");
    }
    for (int j = 0; j < 3; j++) {
      printf(" %8.2f", mat[i][j]);
    }
    if (i == 0) {
      printf("\u23A4\n");
    } else if (i == 1) {
      printf("\u23A5\n");
    } else if (i == 2) {
      printf("\u23A6\n");
    }
  }
  return 1;
}

int PrintMatrixN2(double* x, double* y, int n)
{
  printf(" \u23A1");
  for (int i = 0; i < n; i++) {
    printf(" %8.2f", x[i]);
  }
  printf("\u23A4\n");
  printf(" \u23A3");
  for (int i = 0; i < n; i++) {
    printf(" %8.2f", y[i]);
  }
  printf("\u23A6\n");
  return 1;
}

int PrintMatrixN3(double* x, double* y, int n)
{
  printf(" \u23A1");
  for (int i = 0; i < n; i++) {
    printf(" %8.2f", x[i]);
  }
  printf("\u23A4\n");
  printf(" \u23A2");
  for (int i = 0; i < n; i++) {
    printf(" %8.2f", y[i]);
  }
  printf("\u23A5\n");
  printf(" \u23A3");
  for (int i = 0; i < n; i++) {
    printf(" %8.2f", 1.0);
  }
  printf("\u23A6\n");
  return 1;
}

int CreateIdentity(double mat[3][3])
{
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (i == j) {
        mat[i][j] = 1;
      } else {
        mat[i][j] = 0;
      }
    }
  }
  return 1;
}

int CreateEmpty(double mat[3][3])
{
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      mat[i][j] = 0;
    }
  }
  return 1;
}

int CopyMatrix(double dest[3][3], double source[3][3])
{
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      dest[i][j] = source[i][j];
    }
  }
  return 1;
}

int CopyMatrixN(double* dest, double* src, int n)
{
  for (int i = 0; i < n; i++) {
    dest[i] = src[i];
  }
  return 1;
}

int MatrixMultiply(double res[3][3], double a[3][3], double b[3][3])
{
  double mat[3][3];
  CreateEmpty(mat);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      double sum = 0;
      for (int k = 0; k < 3; k++) {
        sum += a[i][k] * b[k][j];
      }
      mat[i][j] = sum;
    }
  }
  CopyMatrix(res, mat);
  return 1;
}

int MatrixMultiplyPoints(double* res_x, double* res_y, double mat[3][3], double* x, double* y, int n)
{
  double src_x[n], src_y[n];
  for (int i = 0; i < n; i++) {
    src_x[i] = (mat[0][0] * x[i]) + (mat[0][1] * y[i]) + mat[0][2];
    src_y[i] = (mat[1][0] * x[i]) + (mat[1][1] * y[i]) + mat[1][2];
  }
  CopyMatrixN(res_x, src_x, n);
  CopyMatrixN(res_y, src_y, n);
  return 1;
}

int NegateX(double a[3][3], double b[3][3])
{
  double reflect[3][3];
  CreateIdentity(reflect);
  reflect[0][0] = -1;
  MatrixMultiply(a, reflect, a);
  MatrixMultiply(b, b, reflect);
  return 1;
}

int NegateY(double a[3][3], double b[3][3])
{
  double reflect[3][3];
  CreateIdentity(reflect);
  reflect[1][1] = -1;
  MatrixMultiply(a, reflect, a);
  MatrixMultiply(b, b, reflect);
  return 1;
}

int Translate(double mat[3][3], double inv[3][3], double dx, double dy)
{
  double trans[3][3];
  CreateIdentity(trans);

  trans[0][2] = dx;
  trans[1][2] = dy;
  MatrixMultiply(mat, trans, mat);

  trans[0][2] = -dx;
  trans[1][2] = -dy;
  MatrixMultiply(inv, trans, inv);

  return 1;
}

int Scale(double mat[3][3], double inv[3][3], double sx, double sy)
{
  double scale[3][3];
  CreateIdentity(scale);

  scale[0][0] = sx;
  scale[1][1] = sy;
  MatrixMultiply(mat, scale, mat);

  scale[0][0] = 1.0 / sx;
  scale[1][1] = 1.0 / sy;
  MatrixMultiply(inv, scale, inv);

  return 1;
}

int Rotate(double mat[3][3], double inv[3][3], double rads)
{
  double cos_value = cos(rads);
  double sin_value = sin(rads);
  double rotate[3][3];
  CreateIdentity(rotate);
  rotate[0][0] = cos_value;
  rotate[0][1] = sin_value;
  rotate[1][0] = -sin_value;
  rotate[1][1] = cos_value;
  MatrixMultiply(mat, rotate, mat);

  rotate[0][0] = cos_value;
  rotate[0][1] = -sin_value;
  rotate[1][0] = sin_value;
  rotate[1][1] = cos_value;
  MatrixMultiply(inv, rotate, inv);

  return 1;
}

int PrintMatrix3D(double mat[4][4])
{
  for (int i = 0; i < 4; i++) {
    if (i == 0) {
      printf(" \u23A1");
    } else if (i == 1 || i == 2) {
      printf(" \u23A2");
    } else if (i == 3) {
      printf(" \u23A3");
    }
    for (int j = 0; j < 4; j++) {
      printf(" %8.2f", mat[i][j]);
    }
    if (i == 0) {
      printf("\u23A4\n");
    } else if (i == 1 || i == 2) {
      printf("\u23A5\n");
    } else if (i == 3) {
      printf("\u23A6\n");
    }
  }
  return 1;
}

int PrintMatrixN33D(double* x, double* y, double* z, int n)
{
  printf(" \u23A1");
  for (int i = 0; i < n; i++) {
    printf(" %8.2f", x[i]);
  }
  printf("\u23A4\n");
  printf(" \u23A2");
  for (int i = 0; i < n; i++) {
    printf(" %8.2f", y[i]);
  }
  printf("\u23A5\n");
  printf(" \u23A3");
  for (int i = 0; i < n; i++) {
    printf(" %8.2f", z[i]);
  }
  printf("\u23A6\n");
  return 1;
}

int PrintMatrixN43D(double* x, double* y, double* z, int n)
{
  printf(" \u23A1");
  for (int i = 0; i < n; i++) {
    printf(" %8.2f", x[i]);
  }
  printf("\u23A4\n");
  printf(" \u23A2");
  for (int i = 0; i < n; i++) {
    printf(" %8.2f", y[i]);
  }
  printf("\u23A5\n");
  printf(" \u23A2");
  for (int i = 0; i < n; i++) {
    printf(" %8.2f", z[i]);
  }
  printf("\u23A5\n");
  printf(" \u23A3");
  for (int i = 0; i < n; i++) {
    printf(" %8.2f", 1.0);
  }
  printf("\u23A6\n");
  return 1;
}

int CreateIdentity3D(double mat[4][4])
{
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (i == j) {
        mat[i][j] = 1;
      } else {
        mat[i][j] = 0;
      }
    }
  }
  return 1;
}

int CreateEmpty3D(double mat[4][4])
{
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      mat[i][j] = 0;
    }
  }
  return 1;
}

int CopyMatrix3D(double dest[4][4], double source[4][4])
{
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dest[i][j] = source[i][j];
    }
  }
  return 1;
}

int CopyMatrixN3D(double* dest, double* src, int n)
{
  for (int i = 0; i < n; i++) {
    dest[i] = src[i];
  }
  return 1;
}

int MatrixMultiply3D(double res[4][4], double a[4][4], double b[4][4])
{
  double mat[4][4];
  CreateEmpty3D(mat);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      double sum = 0;
      for (int k = 0; k < 4; k++) {
        sum += a[i][k] * b[k][j];
      }
      mat[i][j] = sum;
    }
  }
  CopyMatrix3D(res, mat);
  return 1;
}

int MatrixMultiplyPoints3D(double* res_x, double* res_y, double* res_z, double mat[4][4], double* x, double* y, double* z, int n)
{
  double src_x[n], src_y[n], src_z[n];
  for (int i = 0; i < n; i++) {
    src_x[i] = (mat[0][0] * x[i]) + (mat[0][1] * y[i]) + (mat[0][2] * z[i]) + mat[0][3];
    src_y[i] = (mat[1][0] * x[i]) + (mat[1][1] * y[i]) + (mat[1][2] * z[i]) + mat[1][3];
    src_z[i] = (mat[2][0] * x[i]) + (mat[2][1] * y[i]) + (mat[2][2] * z[i]) + mat[2][3];
  }
  CopyMatrixN3D(res_x, src_x, n);
  CopyMatrixN3D(res_y, src_y, n);
  CopyMatrixN3D(res_z, src_z, n);
  return 1;
}

int NegateX3D(double a[4][4], double b[4][4])
{
  double reflect[4][4];
  CreateIdentity3D(reflect);
  reflect[0][0] = -1;
  MatrixMultiply3D(a, reflect, a);
  MatrixMultiply3D(b, b, reflect);
  return 1;
}

int NegateY3D(double a[4][4], double b[4][4])
{
  double reflect[4][4];
  CreateIdentity3D(reflect);
  reflect[1][1] = -1;
  MatrixMultiply3D(a, reflect, a);
  MatrixMultiply3D(b, b, reflect);
  return 1;
}

int NegateZ3D(double a[4][4], double b[4][4])
{
  double reflect[4][4];
  CreateIdentity3D(reflect);
  reflect[2][2] = -1;
  MatrixMultiply3D(a, reflect, a);
  MatrixMultiply3D(b, b, reflect);
  return 1;
}

int Translate3D(double a[4][4], double b[4][4], double dx, double dy, double dz)
{
  double trans[4][4];
  CreateIdentity3D(trans);

  trans[0][3] = dx;
  trans[1][3] = dy;
  trans[2][3] = dz;
  MatrixMultiply3D(a, trans, a);

  trans[0][3] = -dx;
  trans[1][3] = -dy;
  trans[2][3] = -dz;
  MatrixMultiply3D(b, trans, b);

  return 1;
}

int Scale3D(double a[4][4], double b[4][4], double sx, double sy, double sz)
{
  double scale[4][4];
  CreateIdentity3D(scale);

  scale[0][0] = sx;
  scale[1][1] = sy;
  scale[2][2] = sz;
  MatrixMultiply3D(a, scale, a);

  scale[0][0] = 1.0 / sx;
  scale[1][1] = 1.0 / sy;
  scale[2][2] = 1.0 / sz;
  MatrixMultiply3D(b, scale, b);

  return 1;
}

int RotateX3D(double a[4][4], double b[4][4], double radians)
{
  double cos_value = cos(radians);
  double sin_value = sin(radians);
  double rotate[4][4];
  CreateIdentity3D(rotate);
  rotate[1][1] = cos_value;
  rotate[2][1] = sin_value;
  rotate[1][2] = -sin_value;
  rotate[2][2] = cos_value;
  MatrixMultiply3D(a, rotate, a);

  rotate[1][1] = cos_value;
  rotate[2][1] = -sin_value;
  rotate[1][2] = sin_value;
  rotate[2][2] = cos_value;
  MatrixMultiply3D(b, rotate, b);

  return 1;
}

int RotateY3D(double a[4][4], double b[4][4], double radians)
{
  double cos_value = cos(radians);
  double sin_value = sin(radians);
  double rotate[4][4];
  CreateIdentity3D(rotate);
  rotate[0][0] = cos_value;
  rotate[0][2] = sin_value;
  rotate[2][0] = -sin_value;
  rotate[2][2] = cos_value;
  MatrixMultiply3D(a, rotate, a);

  rotate[0][0] = cos_value;
  rotate[0][2] = -sin_value;
  rotate[2][0] = sin_value;
  rotate[2][2] = cos_value;
  MatrixMultiply3D(b, rotate, b);

  return 1;
}

int RotateZ3D(double a[4][4], double b[4][4], double radians)
{
  double cos_value = cos(radians);
  double sin_value = sin(radians);
  double rotate[4][4];
  CreateIdentity3D(rotate);
  rotate[0][0] = cos_value;
  rotate[0][1] = sin_value;
  rotate[1][0] = -sin_value;
  rotate[1][1] = cos_value;
  MatrixMultiply3D(a, rotate, a);

  rotate[0][0] = cos_value;
  rotate[0][1] = -sin_value;
  rotate[1][0] = sin_value;
  rotate[1][1] = cos_value;
  MatrixMultiply3D(b, rotate, b);

  return 1;
}

int RotateCSX3D(double a[4][4], double b[4][4], double cs, double sn)
{
  double rotate[4][4];
  CreateIdentity3D(rotate);
  rotate[1][1] = cs;
  rotate[2][1] = sn;
  rotate[1][2] = -sn;
  rotate[2][2] = cs;
  MatrixMultiply3D(a, rotate, a);

  rotate[1][1] = cs;
  rotate[2][1] = -sn;
  rotate[1][2] = sn;
  rotate[2][2] = cs;
  MatrixMultiply3D(b, rotate, b);
  return 1;
}

int RotateCSY3D(double a[4][4], double b[4][4], double cs, double sn)
{
  double rotate[4][4];
  CreateIdentity3D(rotate);
  rotate[0][0] = cs;
  rotate[0][2] = sn;
  rotate[2][0] = -sn;
  rotate[2][2] = cs;
  MatrixMultiply3D(a, rotate, a);

  rotate[0][0] = cs;
  rotate[0][2] = -sn;
  rotate[2][0] = sn;
  rotate[2][2] = cs;
  MatrixMultiply3D(b, rotate, b);
  return 1;
}
int RotateCSZ3D(double a[4][4], double b[4][4], double cs, double sn)
{
  double rotate[4][4];
  CreateIdentity3D(rotate);
  rotate[0][0] = cs;
  rotate[0][1] = sn;
  rotate[1][0] = -sn;
  rotate[1][1] = cs;
  MatrixMultiply3D(a, rotate, a);

  rotate[0][0] = cs;
  rotate[0][1] = -sn;
  rotate[1][0] = sn;
  rotate[1][1] = cs;
  MatrixMultiply3D(b, rotate, b);
  return 1;
}
