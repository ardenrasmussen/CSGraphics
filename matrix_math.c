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
  for (int i = 0; i < n; i++) {
    res_x[i] = (mat[0][0] * x[i]) + (mat[0][1] * y[i]) + mat[0][2];
    res_y[i] = (mat[1][0] * x[i]) + (mat[1][1] * y[i]) + mat[1][2];
  }
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
  printf("%lf, %lf\n", cos_value, sin_value);
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
