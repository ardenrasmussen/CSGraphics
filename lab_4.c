#include "matrix_math.h"
#include <FPT.h>
#include <stdio.h>
#include <stdlib.h>

#define max(a, b) \
  ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a, b) \
  ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

double frand(double fMin, double fMax)
{
  double f = (double)rand() / RAND_MAX;
  return fMin + f * (fMax - fMin);
}

int WIDTH = 750;
int HEIGHT = 750;
int INVERSE = -1;
double HALF = 0.785398;
double red = 1.0, blue = 1.0, green = 1.0;

typedef struct {
  int obj;
  int poly;
} Polygon;

struct Object {
  int points_n;
  double x[2500], y[2500], z[2500];
  int poly_n;
  int poly_size[5000];
  int poly[5000][20];
  double max_x, max_y, max_z, min_x, min_y, min_z;
  double center_x, center_y, center_z;
  double radians;
};

struct Object objects[10];
int n_obj;

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
    fscanf(f, "%lf %lf %lf", &obj.x[i], &obj.y[i], &obj.z[i]);
    if (i == 0) {
      obj.max_x = obj.x[0];
      obj.min_x = obj.x[0];
      obj.max_y = obj.y[0];
      obj.min_y = obj.y[0];
      obj.max_z = obj.z[0];
      obj.min_z = obj.z[0];
    }
    obj.max_x = max(obj.max_x, obj.x[i]);
    obj.max_y = max(obj.max_y, obj.y[i]);
    obj.max_z = max(obj.max_z, obj.z[i]);
    obj.min_x = min(obj.min_x, obj.x[i]);
    obj.min_y = min(obj.min_y, obj.y[i]);
    obj.min_z = min(obj.min_z, obj.z[i]);
  }
  obj.center_x = (obj.max_x + obj.min_x) / 2.0;
  obj.center_y = (obj.max_y + obj.min_y) / 2.0;
  obj.center_z = (obj.max_z + obj.min_z) / 2.0;
  fscanf(f, "%i", &obj.poly_n);
  for (int i = 0; i < obj.poly_n; i++) {
    fscanf(f, "%i", &obj.poly_size[i]);
    for (int j = 0; j < obj.poly_size[i]; j++) {
      fscanf(f, "%i", &obj.poly[i][j]);
    }
  }
  fclose(f);
  printf("Loaded \"%s\"\n", file);
  return obj;
}

void CalcCenter(struct Object* obj)
{
  double maxv[3] = { obj->x[0], obj->y[0], obj->z[0] };
  double minv[3] = { obj->x[0], obj->y[0], obj->z[0] };
  for (int i = 1; i < obj->points_n; i++) {
    maxv[0] = max(maxv[0], obj->x[i]);
    maxv[1] = max(maxv[1], obj->y[i]);
    maxv[2] = max(maxv[2], obj->z[i]);
    minv[0] = min(minv[0], obj->x[i]);
    minv[1] = min(minv[1], obj->y[i]);
    minv[2] = min(minv[2], obj->z[i]);
  }
  obj->center_x = ((maxv[0] - minv[0]) / 2.0) + minv[0];
  obj->center_y = ((maxv[1] - minv[1]) / 2.0) + minv[1];
  obj->center_z = ((maxv[2] - minv[2]) / 2.0) + minv[2];
}

void DrawPolygon(double* x, double* y, int n)
{
  for (int i = 0; i < n; i++) {
    int j = (i + 1) % n;
    double r, g;
    r = ((double)x[i] / (double)WIDTH);
    g = ((double)y[i] / (double)WIDTH);
    G_rgb(r, g, 1.0);
    G_line(x[i], y[i], x[j], y[j]);
  }
}

int ShouldDraw(double* x, double* y, double* z, double cx, double cy, double cz)
{
  double a[3] = { x[0] - x[1], y[0] - y[1], z[0] - z[1] };
  double b[3] = { x[0] - x[2], y[0] - y[2], z[0] - z[2] };
  double normal[3] = { (a[1] * b[2] - a[2] * b[1]), -(a[0] * b[2] - a[2] * b[0]), (a[0] * b[1] - a[1] * b[0]) };
  double v[3] = { cx - x[0], cy - y[0], cz - z[0] };
  double out = (normal[0] * v[0]) + (normal[1] * v[1]) + (normal[2] * v[2]);
  /* if (out < 0) { */
  /* normal[0] *= -1; */
  /* normal[1] *= -1; */
  /* normal[2] *= -1; */
  /* } */
  double dir[3] = { x[0], y[0], z[0] };
  double dot = (normal[0] * dir[0]) + (normal[1] * dir[1]) + (normal[2] * dir[2]);
  if (dot > 0) {
    return 1;
  }
  return -1;
}

void DisplayPoint(double pt[3])
{
  double x = (((double)WIDTH / 2.0) / tan(HALF) * (pt[0] / pt[2])) + ((double)WIDTH / 2.0);
  double y = (((double)HEIGHT / 2.0) / tan(HALF) * (pt[1] / pt[2])) + ((double)HEIGHT / 2.0);
  G_fill_circle(x, y, 2);
}

double average(int obj, int poly)
{
  double avg = 0.0;
  for (int i = 0; i < objects[obj].poly_size[poly]; i++) {
    avg += objects[obj].z[objects[obj].poly[poly][i]];
  }
  avg /= objects[obj].poly_size[poly];
  return avg;
}

int Compare(const void* a, const void* b)
{
  Polygon *lhs, *rhs;
  lhs = (Polygon*)a;
  rhs = (Polygon*)b;
  double dist_lhs = average((*lhs).obj, (*lhs).poly);
  double dist_rhs = average((*rhs).obj, (*rhs).poly);
  if (dist_lhs < dist_rhs) {
    return 1;
  } else if (dist_lhs > dist_rhs) {
    return -1;
  } else {
    return 0;
  }
}

void DisplayPolygon(Polygon poly)
{
  double x[20], y[20];
  for (int j = 0; j < objects[poly.obj].poly_size[poly.poly]; j++) {
    x[j] = (((double)WIDTH / 2.0) / tan(HALF) * (objects[poly.obj].x[objects[poly.obj].poly[poly.poly][j]] / objects[poly.obj].z[objects[poly.obj].poly[poly.poly][j]])) + ((double)WIDTH / 2.0);
    y[j] = (((double)WIDTH / 2.0) / tan(HALF) * (objects[poly.obj].y[objects[poly.obj].poly[poly.poly][j]] / objects[poly.obj].z[objects[poly.obj].poly[poly.poly][j]])) + ((double)WIDTH / 2.0);
  }
  if (poly.obj == 0) {
    G_rgb(1, 0, 0);
  } else if (poly.obj == 1) {
    G_rgb(0, 1, 0);
  } else if (poly.obj == 2) {
    G_rgb(0, 0, 1);
  }
  G_rgb(frand(0.0, 1.0), frand(0.0, 1.0), frand(0.0, 1.0));
  G_fill_polygon(x, y, objects[poly.obj].poly_size[poly.poly]);
  G_rgb(0, 0, 0);
  G_polygon(x, y, objects[poly.obj].poly_size[poly.poly]);
}

void DisplayObjs()
{
  Polygon polygons[10000];
  int index = 0;
  for (int i = 0; i < n_obj; i++) {
    for (int j = 0; j < objects[i].poly_n; j++) {
      polygons[index].obj = i;
      polygons[index].poly = j;
      index++;
    }
  }
  qsort(polygons, index, sizeof(Polygon), Compare);
  for (int i = 0; i < index; i++) {
    DisplayPolygon(polygons[i]);
  }
}

void DisplayObj(struct Object obj)
{
  for (int i = 0; i < obj.poly_n; i++) {
    double cx[3], cy[3], cz[3];
    for (int j = 0; j < 3; j++) {
      cx[j] = obj.x[obj.poly[i][j]];
      cy[j] = obj.y[obj.poly[i][j]];
      cz[j] = obj.z[obj.poly[i][j]];
    }
    int good = ShouldDraw(cx, cy, cz, obj.center_x, obj.center_y, obj.center_z);
    if (good == 1 ^ INVERSE == 1) {
      double x[20], y[20];
      for (int j = 0; j < obj.poly_size[i]; j++) {
        x[j] = (((double)WIDTH / 2.0) / tan(HALF) * (obj.x[obj.poly[i][j]] / obj.z[obj.poly[i][j]])) + ((double)WIDTH / 2.0);
        y[j] = (((double)WIDTH / 2.0) / tan(HALF) * (obj.y[obj.poly[i][j]] / obj.z[obj.poly[i][j]])) + ((double)WIDTH / 2.0);
      }
      G_rgb(1, 0, 0);
      G_fill_polygon(x, y, obj.poly_size[i]);
      G_rgb(0, 0, 0);
      G_polygon(x, y, obj.poly_size[i]);
    }
    /* DrawPolygon(x, y, obj.poly_size[i]); */
  }
}

struct Object TranslateObj(struct Object obj, int dir)
{
  double move = 1.0;
  /* move = 0.01 * (fabs(max(obj.max_x, obj.min_x)) - fabs(min(obj.min_x, obj.max_x))); */
  double mat[4][4], inv[4][4];
  CreateIdentity3D(mat);
  CreateIdentity3D(inv);
  if (dir < 0) {
    move *= -1;
    dir *= -1;
  }
  if (dir == 1) {
    Translate3D(mat, inv, move, 0.0, 0.0);
    obj.center_x += move;
    obj.max_x += move;
    obj.max_y += move;
    obj.max_z += move;
    obj.min_z += move;
    obj.min_y += move;
    obj.min_z += move;
  } else if (dir == 2) {
    Translate3D(mat, inv, 0.0, move, 0.0);
    obj.center_y += move;
    obj.max_x += move;
    obj.max_y += move;
    obj.max_z += move;
    obj.min_z += move;
    obj.min_y += move;
    obj.min_z += move;
  } else if (dir == 3) {
    Translate3D(mat, inv, 0.0, 0.0, move);
    obj.center_z += move;
    obj.max_x += move;
    obj.max_y += move;
    obj.max_z += move;
    obj.min_z += move;
    obj.min_y += move;
    obj.min_z += move;
  }
  MatrixMultiplyPoints3D(obj.x, obj.y, obj.z, mat, obj.x, obj.y, obj.z, obj.points_n);
  return obj;
}

struct Object RotateObj(struct Object obj, int dir)
{
  double rotate = 0.1;
  double mat[4][4], inv[4][4];
  CreateIdentity3D(mat);
  CreateIdentity3D(inv);
  if (dir < 0) {
    dir *= -1;
    rotate *= -1;
  }
  Translate3D(mat, inv, -obj.center_x, -obj.center_y, -obj.center_z);
  if (dir == 1) {
    RotateCSX3D(mat, inv, cos(rotate), sin(rotate));
  } else if (dir == 2) {
    RotateCSY3D(mat, inv, cos(rotate), sin(rotate));
  } else if (dir == 3) {
    RotateCSZ3D(mat, inv, cos(rotate), sin(rotate));
  }
  Translate3D(mat, inv, obj.center_x, obj.center_y, obj.center_z);
  MatrixMultiplyPoints3D(obj.x, obj.y, obj.z, mat, obj.x, obj.y, obj.z, obj.points_n);
  return obj;
}

void ScaleObj(struct Object* obj, int dir)
{
  double scale = 0.1;
  double mat[4][4], inv[4][4];
  CreateIdentity3D(mat);
  CreateIdentity3D(inv);
  if (dir < 0) {
    dir *= -1;
    scale *= -1;
  }
  Translate3D(mat, inv, -obj->center_x, -obj->center_y, -obj->center_z);
  if (dir == 1) {
    Scale3D(mat, inv, 1.0 + scale, 1.0, 1.0);
  } else if (dir == 2) {
    Scale3D(mat, inv, 1.0, 1.0 + scale, 1.0);
  } else if (dir == 3) {
    Scale3D(mat, inv, 1.0, 1.0, 1.0 + scale);
  } else if (dir == 4) {
    Scale3D(mat, inv, 1.0 + scale, 1.0 + scale, 1.0 + scale);
  }
  Translate3D(mat, inv, obj->center_x, obj->center_y, obj->center_z);
  MatrixMultiplyPoints3D(obj->x, obj->y, obj->z, mat, obj->x, obj->y, obj->z, obj->points_n);
}

void Clear()
{
  G_rgb(0.0, 0.0, 0.0);
  G_clear();
}

int GetInt(const char* value, int current)
{
  char v_str[256];
  sprintf(v_str, "%i", current);
  while (1) {
    Clear();
    G_rgb(1.0, 1.0, 1.0);
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

void SetFOV()
{
  int fov = HALF * 57.2958;
  fov = GetInt("FOV", fov);
  HALF = (double)fov * 0.0174533;
}

void SetColor()
{
  int r = 256 * red;
  int g = 256 * green;
  int b = 256 * blue;
  r = GetInt("Red", r);
  g = GetInt("Green", g);
  b = GetInt("Blue", b);
  red = (double)r / 256;
  green = (double)g / 256;
  blue = (double)b / 256;
}

int main(int argc, const char* argv[])
{
  srand(time(NULL));
  /* struct Object objects[10]; */
  n_obj = argc - 1;
  int prev_dir = 1;
  for (int i = 1; i < argc; i++) {
    objects[i - 1] = LoadObject(argv[i]);
  }
  G_init_graphics(WIDTH, HEIGHT);
  Clear();
  int current = -1;
  int setting = -1;
  while (1) {
    Clear();
    G_rgb(red, green, blue);
    DisplayObjs();
    /* DisplayObjs(objects, n_obj); */
    /* for (int i = 0; i < n_obj; i++) { */
    /* DisplayObj(objects[i]); */
    /* } */
    int key = G_wait_key();
    if (key >= 48 && key <= 57 && key - 47 < argc) {
      current = key - 48;
    } else if (key == 't') {
      setting = 1;
    } else if (key == 'r') {
      setting = 2;
    } else if (key == 's') {
      setting = 3;
    } else if (key == 'f') {
      SetFOV();
    } else if (key == 'c') {
      SetColor();
    } else if (key == 'g') {
      setting = 4;
    } else if (key == 'j') {
      if (INVERSE == 1) {
        INVERSE = -1;
      } else if (INVERSE == -1) {
        INVERSE = 1;
      }
    } else if (key == 'q') {
      break;
    }
    if (current != -1) {
      int dir = 0;
      if (key == 'x') {
        dir = 1;
      } else if (key == 'X') {
        dir = -1;
      } else if (key == 'y') {
        dir = 2;
      } else if (key == 'Y') {
        dir = -2;
      } else if (key == 'z') {
        dir = 3;
      } else if (key == 'Z') {
        dir = -3;
      } else if (key == '+') {
        dir = 4;
      } else if (key == '-') {
        dir = -4;
      }
      if (setting == 1) {
        objects[current] = TranslateObj(objects[current], dir);
      } else if (setting == 2) {
        objects[current] = RotateObj(objects[current], dir);
      } else if (setting == 3) {
        ScaleObj(&objects[current], dir);
      } else if (setting == 4) {
        if (rand() % 10 == 0) {
          prev_dir = (rand() % 3) + 1;
        }
        objects[current] = RotateObj(objects[current], prev_dir);
      }
    }
  }
  G_close();
}
