#include "matrix_math.h"
#include <FPT.h>

double jx[5] = { 0, 40, 35, 10, 0 };
double jy[5] = { 0, 0, 8, 8, 15 };

double up[3][3];
double invup[3][3];
double down[3][3];
double invdown[3][3];
int state = -1;

int First()
{
  double inv[3][3];
  CreateIdentity(up);
  CreateIdentity(down);
  Rotate(up, invup, (3.1415 / 4.0));
  Translate(up, invup, 0, 500);
  NegateY(down, invdown);
  Rotate(down, invdown, (3.1415 / 4.0));
  Translate(down, invdown, 0, 500);
}

int Move(int frame)
{
  Translate(up, invup, 1.41421, -1.4121);
  Translate(down, invdown, 1.41421, -1.4121);
}

void Copy(double* a, double* b, int n)
{
  for (int i = 0; i < n; i++) {
    b[i] = a[i];
  }
}

void Frame()
{
  G_rgb(0, 0, 0);
  G_clear();
  G_rgb(0, 0, 1);
  double tjx[5];
  double tjy[5];
  Copy(jx, tjx, 5);
  Copy(jy, tjy, 5);
  if (state == 1) {
    MatrixMultiplyPoints(tjx, tjy, up, jx, jy, 5);
  } else if (state == 0) {
    MatrixMultiplyPoints(tjx, tjy, down, jx, jy, 5);
  } else if (state == -1) {
  }
  G_fill_polygon(tjx, tjy, 5);
  G_rgb(1, 1, 10);
  G_draw_string("Press any key to continue", 250, 10);
  G_rgb(1, 0, 1);
  G_line(0, 500, 500, 0);
}

int main(int argc, char* argv[])
{
  G_init_graphics(600, 600);
  int frame = 0;
  for (frame = 0; frame < 1000; frame++) {
    if (frame == 1) {
      First();
      state = 1;
    } else if (frame > 1) {
      if (frame % 50 == 0) {
        if (state == 0) {
          state = 1;
        } else {
          state = 0;
        }
      }
      Move(frame);
    }
    Frame();
    int key = G_wait_key();
    if (key == 'q') {
      break;
    }
    printf("%i\n", frame + 1);
  }
  return 0;
}
