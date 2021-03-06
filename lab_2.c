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

int WIDTH = 1000;
int HEIGHT = 1000;

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
  double scale_factor = min(0.8 * (double)WIDTH / (obj.max_x - obj.min_x), 0.8 * (double)HEIGHT / (obj.max_y - obj.min_y));
  Scale(trans, inv, scale_factor, scale_factor);
  MatrixMultiplyPoints(obj.x, obj.y, trans, obj.x, obj.y, obj.points_n);
  printf("Loaded \"%s\"\n", file);
  return obj;
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
    G_rgb(obj.red[i], obj.green[i], obj.blue[i]);
    G_fill_polygon(x, y, obj.poly_size[i]);
  }
  MatrixMultiplyPoints(obj.x, obj.y, inv, obj.x, obj.y, obj.points_n);
}

void Clear()
{
  G_rgb(1.0, 1.0, 1.0);
  G_clear();
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
    if (key >= 48 && key <= 57 && key - 47 < argc) {
      key -= 48;
      if (key != current) {
        Clear();
        DisplayObj(objects[key]);
        current = key;
      } else if (key == current) {
        Clear();
        objects[key].radians += 0.157079;
        DisplayObj(objects[key]);
      }
    } else if (key == 'q') {
      break;
    }
  }
  G_close();
}
