#include <stdio.h>
#include <stdlib.h>

extern int WIDTH;
extern int HEIGHT;
extern int INVERSE;
extern double HITHER;
extern double YON;
extern double HALF;
extern double light[3];
extern double eye[3], last_eye[3];
extern double coi[3], last_coi[3];
extern double up[3], last_up[3];

#define max(a, b) \
  ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a, b) \
  ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

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
  double red, green, blue;
  double radians;
};
extern struct Object objects[10];
extern int n_obj;

double frand(double fMin, double fMax);

void CalcCenter(struct Object* obj);

struct Object LoadObject(const char* file);

double Dot(double a[3], double b[3]);
double DotP(double a[3], double b[3]);

void Normalize(double in[3], double out[3]);
void Invert(double in[3], double out[3]);
void Scalar(double lhs[3], double rhs, double out[3]);
void Difference(double lhs[3], double rhs[3], double out[3]);

void DrawPoint(double x, double y, double z);

struct Object TranslateObj(struct Object obj, int dir);
struct Object RotateObj(struct Object obj, int dir);
void ScaleObj(struct Object* obj, int dir);
void Clear();
int GetInit(const char* value, int current);
double GetFloat(const char* value, double current);
void SetFOV();
void ReadColor(double* red, double* green, double* blue);
void ReadLight();

double average(int obj, int poly);
int Compare(const void* a, const void* b);
void FindCenter(Polygon poly, double center[3]);
void FindNormal(Polygon poly, double red[3]);
void SetColor(Polygon poly);
void DisplayPolygon(Polygon poly);
int ClipAgainstSurface(double* x, double* y, double* z, int n, double a, double b, double c, double d, double* res_x, double* res_y, double* res_z);
int ClipSave(double* x, double* y, double* z, int n, double a, double b, double c, double d);
int ClipView(double* x, double* y, double* z, int n);
void DisplayCliped(Polygon poly);
void DisplayObjs();
