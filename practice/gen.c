#include "../matrix_math.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void RotatePoly(double theta, double* x, double* y, double* z, double c_x, double c_y)
{
  double mat[4][4], inv[4][4];
  CreateIdentity3D(mat);
  CreateIdentity3D(inv);
  /* Translate3D(mat, inv, -c_x, -c_y, 0); */
  /* RotateZ3D(mat, inv, theta); */
  /* RotateY3D(mat, inv, sin(n) * theta); */
  Translate3D(mat, inv, 0, 0, 100);
  /* RotateZ3D(mat, inv, theta); */
  RotateY3D(mat, inv, theta);
  /* Translate3D(mat, inv, -c_x, -c_y, 0); */
  MatrixMultiplyPoints3D(x, y, z, mat, x, y, z, 4);
}

void GenPoly(int n, int tot, int depth, double* x, double* y, double* z)
{
  double cer = 2.0 * 3.1415 * 100.0;
  cer /= (double)tot;
  /* cer /= 2.0; */
  double theta = (2.0 * 3.1415) / (double)tot;
  double rotate = (3.1415) / (double)tot;
  double c_x = cos(theta * n) * 100.0;
  double c_y = sin(theta * n) * 100.0;
  x[0] = -cer;
  x[1] = cer;
  x[2] = cer;
  x[3] = -cer;
  y[0] = 0;
  y[1] = 0;
  y[2] = 0;
  y[3] = 0;
  y[0] = cos(rotate * n) * ((double)depth / 2.0);
  y[1] = cos(rotate * n) * ((double)depth / 2.0);
  y[2] = cos(rotate * n) * (-(double)depth / 2.0);
  y[3] = cos(rotate * n) * (-(double)depth / 2.0);
  /* z[0] = depth / 2.0; */
  /* z[1] = depth / 2.0; */
  /* z[2] = -depth / 2.0; */
  /* z[3] = -depth / 2.0; */
  z[0] = sin(rotate * n) * ((double)depth / 2.0);
  z[1] = sin(rotate * n) * ((double)depth / 2.0);
  z[2] = sin(rotate * n) * (-(double)depth / 2.0);
  z[3] = sin(rotate * n) * (-(double)depth / 2.0);
  RotatePoly(theta * n, x, y, z, c_x, c_y);
}

void GenerateFile(int depth, int num)
{
  FILE* out = fopen("tmp.xyz", "w");
  if (out == NULL) {
    printf("Failed to open file\n");
    exit(1);
  }
  double theta = (2.0 * 3.1415) / (double)num;
  fprintf(out, "%d\n", num * 4);
  for (int i = 0; i < num; i++) {
    double x[4], y[4], z[4];
    GenPoly(i, num, depth, x, y, z);
    for (int j = 0; j < 4; j++) {
      fprintf(out, "%lf %lf %lf\n", x[j], y[j], z[j]);
    }
  }
  fprintf(out, "%d\n", num);
  for (int i = 0; i < num * 4; i += 4) {
    fprintf(out, "4 %d %d %d %d\n", i, i + 1, i + 2, i + 3);
  }
  fclose(out);
}

int main(int argc, char* argv[])
{
  int depth, num;
  printf("Depth: ");
  scanf("%d", &depth);
  printf("Number of Polygons: ");
  scanf("%d", &num);
  GenerateFile(depth, num);
  return 0;
}
