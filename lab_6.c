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
  double x[2500], y[2500], z[2500];
  int poly_n;
  int poly_size[5000];
  int poly[5000][20];
};

void Clear()
{
  G_rgb(0, 0, 0);
  G_clear();
}

int GetInt(const char* value, int current)
{
  char v_str[256];
  sprintf(v_str, "%i", current);
  while (1) {
    Clear();
    G_rgb(1, 1, 1);
    char str[256];
    sprintf(str, "%s: %s", value, v_str);
    G_draw_string(str, 10, HEIGHT);
    int key = G_wait_key();
    if (key == 113 || key == 65293) {
      break;

    } else if (key == 65288 && strlen(v_str) > 0) {
      v_str[strlen(v_str) - 1] = 0;
    } else if (key >= 48 && key <= 57) {
      v_str[strlen(v_str)] = key;
      v_str[strlen(v_str) + 1] = 0;
    }
  }
  if (strlen(v_str) == 0) {
    return current;
  }
  sscanf(v_str, "%d", &current);
  return current;
}

struct Object NormalizeObject(struct Object obj)
{
  double max_x = obj.x[0], max_y = obj.y[0], max_z = obj.z[0], min_x = obj.x[0], min_y = obj.y[0], min_z = obj.z[0];
  for (int i = 0; i < obj.points_n; i++) {
    max_x = max(max_x, obj.x[i]);
    max_y = max(max_y, obj.y[i]);
    max_z = max(max_z, obj.z[i]);
    min_x = min(min_x, obj.x[i]);
    min_y = min(min_y, obj.y[i]);
    min_z = min(min_z, obj.z[i]);
  }
  double center_x = (max_x + min_x) / 2.0;
  double center_y = (max_y + min_y) / 2.0;
  double center_z = (max_z + min_z) / 2.0;
  double scale = max_x - min_x;
  scale = max(scale, max_y - min_y);
  scale = max(scale, max_z - min_z);
  scale = 1.0 / scale;
  for (int i = 0; i < obj.points_n; i++) {
    obj.x[i] -= center_x;
    obj.y[i] -= center_y;
    obj.z[i] -= center_z;
    obj.x[i] *= scale;
    obj.y[i] *= scale;
    obj.z[i] *= scale;
  }
  return obj;
}

struct Object GenObject(double x[100], double y[100], int n, int splits)
{
  struct Object obj;
  double step = 6.283 / (double)splits;
  int i = 0;
  for (int j = 0; j < n; j++) {
    x[j] -= (WIDTH / 2);
  }
  for (double theta = 0; theta < 6.283; theta += step) {
    printf("THETA: %lf\n", theta);
    for (int j = 0; j < n; j++) {
      obj.x[i] = cos(theta) * x[j];
      obj.z[i] = sin(theta) * x[j];
      obj.y[i] = y[j];
      printf("%lf, %lf, %lf\n", obj.x[i], obj.y[i], obj.z[i]);
      i++;
    }
  }
  obj.points_n = i;
  obj.poly_n = (n - 1) * splits;
  i = 0;
  for (int j = 0; j < splits; j++) {
    for (int k = 0; k < n - 1; k++) {
      obj.poly_size[i] = 4;
      obj.poly[i][0] = (j * n) + k;
      obj.poly[i][1] = (j * n) + k + 1;
      if (j + 1 < splits) {
        obj.poly[i][2] = ((j + 1) * n) + k + 1;
        obj.poly[i][3] = ((j + 1) * n) + k;
      } else {
        obj.poly[i][2] = k + 1;
        obj.poly[i][3] = k;
      }
      i++;
    }
  }
  return obj;
}

void SaveObject(const char* file, struct Object obj)
{
  FILE* f = fopen(file, "w");
  if (f == NULL) {
    printf("\"%s\" failed to open!\n", file);
    exit(1);
  }
  fprintf(f, "%i\n", obj.points_n);
  for (int i = 0; i < obj.points_n; i++) {
    fprintf(f, "%lf %lf %lf\n", obj.x[i], obj.y[i], obj.z[i]);
  }
  fprintf(f, "%i\n", obj.poly_n);
  for (int i = 0; i < obj.poly_n; i++) {
    fprintf(f, "%i ", obj.poly_size[i]);
    for (int j = 0; j < obj.poly_size[i]; j++) {
      fprintf(f, "%i ", obj.poly[i][j]);
    }
    fprintf(f, "\n");
  }
}

int main(int argc, const char* argv[])
{
  G_init_graphics(WIDTH, HEIGHT);
  G_rgb(0, 0, 0);
  G_clear();
  G_rgb(1, 1, 1);
  G_fill_rectangle(0, 0, 50, 50);
  G_line(WIDTH / 2.0, HEIGHT, WIDTH / 2.0, 0);
  double x[100], y[100];
  double p[2] = { 100, 100 };
  int index = 0;
  while (p[0] > 50 && p[1] > 50) {
    G_wait_click(p);
    if (p[0] <= 50 && p[1] <= 50) {
      break;
    }
    G_rgb(1, 0, 0);
    G_fill_circle(p[0], p[1], 2);
    x[index] = p[0];
    y[index] = p[1];
    if (index != 0) {
      G_line(x[index - 1], y[index - 1], x[index], y[index]);
    }
    index++;
  }
  int split = GetInt("N", 8);
  struct Object obj = GenObject(x, y, index, split);
  obj = NormalizeObject(obj);
  SaveObject("tmp.xyz", obj);
  G_close();
}
