#include "matrix_math.h"
#include <FPT.h>

#define max(a, b) \
  ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a, b) \
  ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

int WIDTH = 750;
int HEIGHT = 750;

double clip_x[100], clip_y[100];
double clip_center[2];
int clip_n = 0;

struct Object {
  int points_n;
  double x[1000], y[1000];
  int poly_n;
  int poly_size[256];
  int poly[256][20];
  double red[256], green[256], blue[256];
  double max_x, max_y, min_x, min_y;
  double center_x, center_y;
  double radians;
};

struct Object LoadObject(const char* file)
{
  FILE* f = fopen(file, "r");
  if (f == NULL) {
    printf("\"%s\" does not exist!\n", file);
    exit(1);
  }
  struct Object obj;
  fscanf(f, "%i", &obj.points_n);
  for (int i = 0; i < obj.points_n; i++) {
    fscanf(f, "%lf %lf", &obj.x[i], &obj.y[i]);
    if (i == 0) {
      obj.max_x = obj.x[0];
      obj.min_x = obj.x[0];
      obj.max_y = obj.y[0];
      obj.min_y = obj.y[0];
    }
    obj.max_x = max(obj.max_x, obj.x[i]);
    obj.max_y = max(obj.max_y, obj.y[i]);
    obj.min_x = min(obj.min_x, obj.x[i]);
    obj.min_y = min(obj.min_y, obj.y[i]);
  }
  obj.center_x = (obj.max_x + obj.min_x) / 2.0;
  obj.center_y = (obj.max_y + obj.min_y) / 2.0;
  fscanf(f, "%i", &obj.poly_n);
  for (int i = 0; i < obj.poly_n; i++) {
    fscanf(f, "%i", &obj.poly_size[i]);
    for (int j = 0; j < obj.poly_size[i]; j++) {
      fscanf(f, "%i", &obj.poly[i][j]);
    }
  }
  for (int i = 0; i < obj.poly_n; i++) {
    fscanf(f, "%lf %lf %lf", &obj.red[i], &obj.green[i], &obj.blue[i]);
  }
  fclose(f);
  double trans[3][3], inv[3][3];
  CreateIdentity(trans);
  Translate(trans, inv, -obj.center_x, -obj.center_y);
  double scale_factor = min(0.7 * (double)WIDTH / (obj.max_x - obj.min_x), 0.7 * (double)HEIGHT / (obj.max_y - obj.min_y));
  Scale(trans, inv, scale_factor, scale_factor);
  MatrixMultiplyPoints(obj.x, obj.y, trans, obj.x, obj.y, obj.points_n);
  printf("Loaded \"%s\"\n", file);
  return obj;
}

int IsGood(double p1, double p2, int i)
{
  int is_good = 1;
  int j = (i + 1) % clip_n;
  double a = (clip_y[i] - clip_y[j]);
  double b = (clip_x[j] - clip_x[i]);
  double c = (clip_x[i] * clip_y[j] - clip_x[j] * clip_y[i]);
  double val_c = a * clip_center[0] + b * clip_center[1] + c;
  double val_p = a * p1 + b * p2 + c;
  if (val_c > 0 && val_p < 0) {
    return 0;
  } else if (val_c < 0 && val_p > 0) {
    return 0;
  }
  return is_good;
}

void Intersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double* res_x, double* res_y)
{
  *res_x = ((x2 * y1 - x1 * y2) * (x4 - x3) - (x4 * y3 - x3 * y4) * (x2 - x1)) / ((x2 - x1) * (y4 - y3) - (x4 - x3) * (y2 - y1));
  *res_y = ((x2 * y1 - x1 * y2) * (y4 - y3) - (x4 * y3 - x3 * y4) * (y2 - y1)) / ((x2 - x1) * (y4 - y3) - (x4 - x3) * (y2 - y1));
}
int Clip(double* x, double* y, int n, int ci)
{
  int cj = (ci + 1) % clip_n;
  double cx[n];
  double cy[n];
  for (int i = 0; i < n; i++) {
    cx[i] = x[i];
    cy[i] = y[i];
  }
  int cn = 0;
  for (int i = 0; i < n; i++) {
    int j = (i + 1) % n;
    if (IsGood(cx[i], cy[i], ci) == 0 && IsGood(cx[j], cy[j], ci) == 1) {
      double inter_x, inter_y;
      Intersect(cx[i], cy[i], cx[j], cy[j], clip_x[ci], clip_y[ci], clip_x[cj], clip_y[cj], &inter_x, &inter_y);
      x[cn] = inter_x;
      y[cn] = inter_y;
      cn++;
      x[cn] = cx[j];
      y[cn] = cy[j];
      cn++;
    } else if (IsGood(cx[i], cy[i], ci) == 1 && IsGood(cx[j], cy[j], ci) == 1) {
      x[cn] = cx[j];
      y[cn] = cy[j];
      cn++;
    } else if (IsGood(cx[i], cy[i], ci) == 1 && IsGood(cx[j], cy[j], ci) == 0) {
      double inter_x, inter_y;
      Intersect(cx[i], cy[i], cx[j], cy[j], clip_x[ci], clip_y[ci], clip_x[cj], clip_y[cj], &inter_x, &inter_y);
      x[cn] = inter_x;
      y[cn] = inter_y;
      cn++;
    }
  }
  return cn;
}

int ClipPolygon(double* x, double* y, int n)
{
  for (int i = 0; i < clip_n; i++) {
    n = Clip(x, y, n, i);
  }
  return n;
}

void DisplayObj(struct Object obj)
{
  double mat[3][3], inv[3][3];
  CreateIdentity(mat);
  Rotate(mat, inv, obj.radians);
  Translate(mat, inv, (double)WIDTH / 2.0, (double)HEIGHT / 2.0);
  MatrixMultiplyPoints(obj.x, obj.y, mat, obj.x, obj.y, obj.points_n);
  for (int i = 0; i < obj.poly_n; i++) {
    double x[20], y[20];
    for (int j = 0; j < obj.poly_size[i]; j++) {
      x[j] = obj.x[obj.poly[i][j]];
      y[j] = obj.y[obj.poly[i][j]];
    }
    int n = ClipPolygon(x, y, obj.poly_size[i]);
    G_rgb(obj.red[i], obj.green[i], obj.blue[i]);
    G_fill_polygon(x, y, n);
  }
  MatrixMultiplyPoints(obj.x, obj.y, inv, obj.x, obj.y, obj.points_n);
}

void Clear()
{
  G_rgb(1.0, 1.0, 1.0);
  G_clear();
}

void GetClip()
{
  clip_n = 0;
  double p[2] = { 200, 200 };
  G_rgb(0, 0, 0);
  G_fill_rectangle(0, 0, 50, 50);
  while (p[0] > 50 || p[1] > 50) {
    G_wait_click(p);
    if (p[0] < 50 && p[1] < 50) {
      break;
    }
    G_circle(p[0], p[1], 5);
    clip_x[clip_n] = p[0];
    clip_y[clip_n] = p[1];
    if (clip_n > 0) {
      G_line(clip_x[clip_n - 1], clip_y[clip_n - 1], clip_x[clip_n], clip_y[clip_n]);
    }
    clip_n++;
  }
  G_line(clip_x[clip_n - 1], clip_y[clip_n - 1], clip_x[0], clip_y[0]);
  G_rgb(1, 1, 1);
  G_fill_rectangle(0, 0, 50, 50);
  clip_center[0] = 0;
  clip_center[1] = 0;
  for (int i = 0; i < clip_n; i++) {
    clip_center[0] += clip_x[i];
    clip_center[1] += clip_y[i];
  }
  clip_center[0] /= (double)clip_n;
  clip_center[1] /= (double)clip_n;
}

int main(int argc, const char* argv[])
{
  struct Object objects[10];
  for (int i = 1; i < argc; i++) {
    objects[i - 1] = LoadObject(argv[i]);
  }
  G_init_graphics(WIDTH, HEIGHT);
  int current = -1;
  while (1) {
    int key = G_wait_key();
    if (key == 'c') {
      GetClip();
    }
    if (key >= 48 && key <= 57 && key - 47 < argc) {
      key -= 48;
      if (key != current) {
        Clear();
        DisplayObj(objects[key]);
        G_rgb(0, 0, 0);
        G_polygon(clip_x, clip_y, clip_n);
        current = key;
      } else if (key == current) {
        Clear();
        objects[key].radians += 0.157079;
        DisplayObj(objects[key]);
        G_rgb(0, 0, 0);
        G_polygon(clip_x, clip_y, clip_n);
      }
    } else if (key == 'q') {
      break;
    }
  }
  G_close();
}
