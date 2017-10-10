#include <FPT.h>

#define max(a, b) \
  ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a, b) \
  ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

int swidth, sheight;

int click_and_save(double* x, double* y)
{
  int n;
  double P[2];

  G_rgb(0, 1, 0.5);
  G_fill_rectangle(0, 0, swidth, 20);

  G_rgb(1, 0, 0);
  G_wait_click(P);

  n = 0;
  while (P[1] > 20) {
    x[n] = P[0];
    y[n] = P[1];
    G_circle(x[n], y[n], 2);
    if (n > 0) {
      G_line(x[n - 1], y[n - 1], x[n], y[n]);
    }
    n++;
    G_wait_click(P);
  }

  return n;
}

int in_out(double* x, double* y, int n, double P[2])
// return 1 if point P is inside the convex polygon
// else return 0
{
  double center[2] = { 0, 0 };
  int is_good = 1;
  for (int i = 0; i < n; i++) {
    center[0] += x[i];
    center[1] += y[i];
  }
  center[0] /= (double)n;
  center[1] /= (double)n;
  for (int i = 0; i < n; i++) {
    int j = i + 1 % n;
    double a = (y[i] - y[j]), b = (x[j] - x[i]), c = (x[i] * y[j] - x[j] * y[i]);
    double val_c = a * center[0] + b * center[1] + c;
    double val_p = a * P[0] + b * P[1] + c;
    if (val_c > 0 && val_p < 0) {
      is_good = 0;
    } else if (val_c < 0 && val_p > 0) {
      is_good = 0;
    }
  }
  return is_good;
}

int main()
{
  double xp[1000], yp[1000];
  int n, q;
  double P[2];

  swidth = 700;
  sheight = 700;
  G_init_graphics(swidth, sheight);
  G_rgb(0, 0, 0);
  G_clear();

  G_rgb(1, 0, 0);
  n = click_and_save(xp, yp);
  G_rgb(0, 1, 0);
  G_fill_polygon(xp, yp, n);

  while (1) {
    G_wait_click(P);
    if (in_out(xp, yp, n, P) == 1) {
      G_rgb(0, 0, 1);
    } else {
      G_rgb(0, 1, 0);
    }
    G_fill_circle(P[0], P[1], 2);
  }
  return 0;
}
