#include "matrix_math.h"
#include <FPT.h>

int main(int argc, const char* argv[])
{
  double mat[3][3], inv[3][3];
  double x[] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 0.0 };
  double y[] = { 0.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0 };
  CreateIdentity(mat);
  PrintMatrix(mat);
  PrintMatrixN2(x, y, 10);
  MatrixMultiplyPoints(x, y, mat, x, y, 10);
  PrintMatrixN2(x, y, 10);
}
