#include "base.h"
#include "matrix_math.h"
#include <FPT.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int WIDTH = 500;
int HEIGHT = 500;
int INVERSE = -1;
double HITHER = 5;
double YON = 25;
double HALF = 0.785398;
double light[3] = { 0, 0, 0 };
double eye[3], last_eye[3] = {0,0,0};
double coi[3], last_coi[3] = {0,0,0};
double up[3], last_up[3] = {0,0,0};
int n_obj = 0;
struct Object objects[10];

double frand(double fMin, double fMax)
{
  double f = (double)rand() / RAND_MAX;
  return fMin + f * (fMax - fMin);
}

void CalcCenter(struct Object* obj)
{
  for (int i = 0; i < obj->points_n; i++) {
    obj->x[i] -= obj->center_x;
    obj->y[i] -= obj->center_y;
    obj->z[i] -= obj->center_z;
  }
}

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
  CalcCenter(&obj);
  fscanf(f, "%i", &obj.poly_n);
  for (int i = 0; i < obj.poly_n; i++) {
    fscanf(f, "%i", &obj.poly_size[i]);
    for (int j = 0; j < obj.poly_size[i]; j++) {
      fscanf(f, "%i", &obj.poly[i][j]);
    }
  }
  fclose(f);
  printf("Loaded \"%s\"\n", file);
  obj.red = 0.8;
  obj.green = 0.8;
  obj.blue = 0.8;
  return obj;
}

double Dot(double a[3], double b[3])
{
  double val = (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
  return val;
}

double DotP(double a[3], double b[3])
{
  double val = (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
  if (val > 0) {
    return val;
  } else {
    return 0;
  }
}

void Normalize(double in[3], double out[3])
{
  out[0] = in[0];
  out[1] = in[1];
  out[2] = in[2];
  double size = sqrt(pow(out[0], 2) + pow(out[1], 2) + pow(out[2], 2));
  out[0] /= size;
  out[1] /= size;
  out[2] /= size;
}

void Invert(double in[3], double out[3])
{
  out[0] = in[0] * -1;
  out[1] = in[1] * -1;
  out[2] = in[2] * -1;
}

void Scalar(double lhs[3], double rhs, double out[3])
{
  out[0] = lhs[0] * rhs;
  out[1] = lhs[1] * rhs;
  out[2] = lhs[2] * rhs;
}

void Difference(double lhs[3], double rhs[3], double out[3])
{
  out[0] = lhs[0] - rhs[0];
  out[1] = lhs[1] - rhs[1];
  out[2] = lhs[2] - rhs[2];
}

void DrawPoint(double x, double y, double z)
{
  double px = ((((double)WIDTH / 2.0) / tan(HALF)) * (x / z)) + ((double)WIDTH / 2.0);
  double py = ((((double)HEIGHT / 2.0) / tan(HALF)) * (y / z)) + ((double)HEIGHT / 2.0);
  G_fill_circle(px, py, 5);
}

struct Object TranslateObj(struct Object obj, int dir)
{
  double move = 1.0;
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
double GetFloat(const char* value, double current)
{
  char v_str[256];
  sprintf(v_str, "%lf", current);
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
    } else if ((key >= 48 && key <= 57) || key == 46 || key == 45) {
      v_str[strlen(v_str)] = key;
      v_str[strlen(v_str) + 1] = 0;
    }
  }
  if (strlen(v_str) == 0) {
    return current;
  }
  sscanf(v_str, "%lf", &current);
  return current;
}

void SetFOV()
{
  int fov = HALF * 57.2958;
  fov = GetInt("FOV", fov);
  HALF = (double)fov * 0.0174533;
}

void ReadColor(double* red, double* green, double* blue)
{
  int r = 256 * (*red);
  int g = 256 * (*green);
  int b = 256 * (*blue);
  r = GetInt("Red", r);
  g = GetInt("Green", g);
  b = GetInt("Blue", b);
  (*red) = (double)r / 256;
  (*green) = (double)g / 256;
  (*blue) = (double)b / 256;
}
void ReadLight()
{
  light[0] = GetFloat("Light X", light[0]);
  light[1] = GetFloat("Light Y", light[1]);
  light[2] = GetFloat("Light Z", light[2]);
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

void FindCenter(Polygon poly, double center[3])
{
  double x = 0, y = 0, z = 0;
  for (int i = 0; i < objects[poly.obj].poly_size[poly.poly]; i++) {
    x += objects[poly.obj].x[objects[poly.obj].poly[poly.poly][i]];
    y += objects[poly.obj].y[objects[poly.obj].poly[poly.poly][i]];
    z += objects[poly.obj].z[objects[poly.obj].poly[poly.poly][i]];
  }
  x /= (double)objects[poly.obj].poly_size[poly.poly];
  y /= (double)objects[poly.obj].poly_size[poly.poly];
  z /= (double)objects[poly.obj].poly_size[poly.poly];
  center[0] = x;
  center[1] = y;
  center[2] = z;
}

void FindNormal(Polygon poly, double res[3])
{
  double x[3], y[3], z[3];
  for (int i = 0; i < 3; i++) {
    x[i] = objects[poly.obj].x[objects[poly.obj].poly[poly.poly][i]];
    y[i] = objects[poly.obj].y[objects[poly.obj].poly[poly.poly][i]];
    z[i] = objects[poly.obj].z[objects[poly.obj].poly[poly.poly][i]];
  }
  double a[3] = { x[1] - x[0], y[1] - y[0], z[1] - z[0] };
  double b[3] = { x[2] - x[0], y[2] - y[0], z[2] - z[0] };
  res[0] = (a[1] * b[2]) - (a[2] * b[1]);
  res[1] = (a[2] * b[0]) - (a[0] * b[2]);
  res[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

void SetColor(Polygon poly)
{
  double intensity = 0.2;
  double c[3] = { 1, 0, 0 };
  double center[3] = { 0, 0, 0 };
  double nu[3] = { 0, 0, 0 };
  double lu[3] = { 0, 0, 0 };
  double ru[3] = { 0, 0, 0 };
  double vu[3] = { 0, 0, 0 };
  double tmp[3] = { 0, 0, 0 };
  FindCenter(poly, center);
  Normalize(center, vu);
  Invert(vu, vu);
  lu[0] = light[0] - center[0];
  lu[1] = light[1] - center[1];
  lu[2] = light[2] - center[2];
  Normalize(lu, lu);
  FindNormal(poly, nu);
  if (INVERSE == 1) {
    Invert(nu, nu);
  }
  Normalize(nu, nu);
  if (Dot(nu, lu) < 0 && Dot(nu, vu) < 0) {
    Invert(nu, nu);
  }
  Scalar(nu, 2 * Dot(nu, lu), tmp);
  Difference(tmp, lu, ru);
  Invert(ru, ru);
  intensity += (0.5 * DotP(nu, lu));
  intensity += (0.3 * pow(Dot(vu, ru), 30));
  if ((Dot(nu, lu) < 0 && Dot(nu, vu) > 0) || (Dot(nu, lu) > 0 && Dot(nu, vu) < 0)) {
    intensity = 0.2;
  }
  double red = objects[poly.obj].red;
  double green = objects[poly.obj].green;
  double blue = objects[poly.obj].blue;
  double r, g, b;
  if (intensity < 0.7) {
    r = (intensity - 0) / (0.7) * (red);
    g = (intensity - 0) / (0.7) * (green);
    b = (intensity - 0) / (0.7) * (blue);
  } else {
    r = (intensity - 0.7) / (0.3) * (1 - red) + red;
    g = (intensity - 0.7) / (0.3) * (1 - green) + green;
    b = (intensity - 0.7) / (0.3) * (1 - blue) + blue;
  }
  G_rgb(r, g, b);
}

void DisplayPolygon(Polygon poly)
{
  double x[20], y[20];
  for (int j = 0; j < objects[poly.obj].poly_size[poly.poly]; j++) {
    x[j] = (((double)WIDTH / 2.0) / tan(HALF) * (objects[poly.obj].x[objects[poly.obj].poly[poly.poly][j]] / objects[poly.obj].z[objects[poly.obj].poly[poly.poly][j]])) + ((double)WIDTH / 2.0);
    y[j] = (((double)WIDTH / 2.0) / tan(HALF) * (objects[poly.obj].y[objects[poly.obj].poly[poly.poly][j]] / objects[poly.obj].z[objects[poly.obj].poly[poly.poly][j]])) + ((double)WIDTH / 2.0);
  }
  SetColor(poly);
  G_fill_polygon(x, y, objects[poly.obj].poly_size[poly.poly]);
}

int ClipAgainstSurface(double* x, double* y, double* z, int n, double a, double b, double c, double d, double* res_x, double* res_y, double* res_z)
{
  int num = 0;
  for (int i = 0; i < n; i++) {
    int j = (i + 1) % n;
    double x1 = x[i];
    double x2 = x[j];
    double y1 = y[i];
    double y2 = y[j];
    double z1 = z[i];
    double z2 = z[j];
    double s1 = (a * x1 + b * y1 + c * z1 + d);
    double s2 = (a * x2 + b * y2 + c * z2 + d);
    if ((s1 >= 0) && (s2 >= 0)) {

    } else if ((s1 < 0) && (s2 < 0)) {
      res_x[num] = x2;
      res_y[num] = y2;
      res_z[num] = z2;
      num++;
    } else {
      double x21 = x2 - x1;
      double y21 = y2 - y1;
      double z21 = z2 - z1;
      double den = a * x21 + b * y21 + c * z21;
      if (den == 0) {
        continue;
      }
      double t = -(a * x1 + b * y1 + c * z1 + d) / den;
      double xint = x1 + t * x21;
      double yint = y1 + t * y21;
      double zint = z1 + t * z21;
      if (s1 < 0) {
        res_x[num] = xint;
        res_y[num] = yint;
        res_z[num] = zint;
        num++;
      } else {
        res_x[num] = xint;
        res_y[num] = yint;
        res_z[num] = zint;
        num++;
        res_x[num] = x2;
        res_y[num] = y2;
        res_z[num] = z2;
        num++;
      }
    }
  }
  return num;
}

int ClipSave(double* x, double* y, double* z, int n, double a, double b, double c, double d)
{
  double resx[20], resy[20], resz[20];
  n = ClipAgainstSurface(x, y, z, n, a, b, c, d, resx, resy, resz);
  for (int i = 0; i < n; i++) {
    x[i] = resx[i];
    y[i] = resy[i];
    z[i] = resz[i];
  }
  return n;
}

int ClipView(double* x, double* y, double* z, int n)
{
  double resx[20], resy[20], resz[20];
  double pt1[3] = { -HITHER * tan(HALF), HITHER * tan(HALF), HITHER };
  double pt2[3] = { HITHER * tan(HALF), HITHER * tan(HALF), HITHER };
  double pt3[3] = { HITHER * tan(HALF), -HITHER * tan(HALF), HITHER };
  double v1[3], v2[3], res[4] = { 0, 0, -1, HITHER };
  n = ClipSave(x, y, z, n, 0, 0, -1, HITHER);
  n = ClipSave(x, y, z, n, 0, 0, 1, -YON);
  n = ClipSave(x, y, z, n, 0, 1, -tan(HALF), 0);
  /* n = ClipSave(x, y, z, n, 0, -1, -tan(HALF), 0); */
  n = ClipSave(x, y, z, n, 1, 0, -tan(HALF), 0);
  /* n = ClipSave(x, y, z, n, -1, 0, tan(HALF), 0); */
  return n;
}

void DisplayCliped(Polygon poly)
{
  double x[20], y[20], z[20];
  int n = objects[poly.obj].poly_size[poly.poly];
  for (int i = 0; i < n; i++) {
    x[i] = objects[poly.obj].x[objects[poly.obj].poly[poly.poly][i]];
    y[i] = objects[poly.obj].y[objects[poly.obj].poly[poly.poly][i]];
    z[i] = objects[poly.obj].z[objects[poly.obj].poly[poly.poly][i]];
  }
  n = ClipView(x, y, z, n);
  /* n = ClipAgainstSurface(x, y, z, n); */
  double xpt[20], ypt[20];
  for (int j = 0; j < n; j++) {
    xpt[j] = (((double)WIDTH / 2.0) / tan(HALF) * (x[j] / z[j])) + ((double)WIDTH / 2.0);
    ypt[j] = (((double)WIDTH / 2.0) / tan(HALF) * (y[j] / z[j])) + ((double)WIDTH / 2.0);
  }
  SetColor(poly);
  G_fill_polygon(xpt, ypt, n);
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
    DisplayCliped(polygons[i]);
  }
}
