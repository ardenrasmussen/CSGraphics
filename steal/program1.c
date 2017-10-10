#include <FPT.h>
double max(double* x, int n);
double min(double* x, int n);
void print_arr(double* x, int n);
void sort(double* x, int n);
void project1();
void my_fill_polygon(double* x, double* y, int n);
void my_polygon(double* x, double* y, int n);
void draw_star(double* p1, double* p2);

int HEIGHT;
int WIDTH;

void scale(double* x, double* y, int n, int box)
{
  //printf("\t%lf,%lf",max(x,n),min(x,n));
  //printf("\t%lf,%lf",max(y,n),min(y,n));
  double x_diff = max(x, n) - min(x, n);
  double y_diff = max(y, n) - min(y, n);
  double scale = box / x_diff;
  if (y_diff > x_diff) {
    scale = box / y_diff;
  }
  printf("\tscale: %lf", scale);
  int i;
  for (i = 0; i < n; i++) {
    x[i] = x[i] * scale;
    y[i] = y[i] * scale;
  }
}
void translate(double* x, double* y, int n, int x_center, int y_center)
{
  int i;
  double x_mid = (max(x, n) + min(x, n)) / 2.0;
  double y_mid = (max(y, n) + min(y, n)) / 2.0;

  // printf("\t(x,y) mids: (%lf, %lf)", x_mid,y_mid);
  // printf("\tx_trans: %lf", x_center-x_mid);
  //printf("\ty_trans: %lf", y_center-y_mid);

  for (i = 0; i < n; i++) {
    x[i] += (x_center - x_mid);
    y[i] += (y_center - y_mid);
  }
}
double dist(double x1, double y1, double x2, double y2)
{
  return (sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}
void rotate(double* x, double* y, int n, double delta)
{
  int i;
  // double delta = M_PI/90;
  printf("%lf\n", delta);
  double COS = cos(delta);
  double SIN = sin(delta);
  printf("%lf", COS);
  for (i = 0; i < n; i++) {
    double hold = x[i];
    x[i] = (x[i] - 500) * COS - (y[i] - 500) * SIN + 500;
    y[i] = (y[i] - 500) * COS + (hold - 500) * SIN + 500;
  }
}
void point_info()
{
  double p[2];
  p[0] = 500;
  p[1] = 500;
  printf("Point coordinates (x,y):\n");
  while (p[0] < 0 || p[0] > 20 || p[1] < 0 || p[1] > 20) {
    G_wait_click(p);
    printf("(%lf,%lf)\n", p[0], p[1]);
  }
}
int main(int argc, char** argv)
{
  double p[2];
  int numpoints[10];
  double x[10][1000], y[10][1000];
  int numpolys[10];
  int psize[10][5000];
  int con[10][5000][20];
  double red[10][5000], grn[10][5000], blue[10][5000];

  int i;
  int j;
  FILE* q;

  G_init_graphics(1000, 1000);

  //Reading in info:
  int f;
  for (f = 0; f < argc - 1; f++) {
    char temp[10] = "xy/";
    //printf("%s",temp);
    strcat(temp, argv[f + 1]);
    printf("%s\n", temp);
    q = fopen(temp, "r");
    if (q == NULL) {
      printf("can't open file\n");
      exit(0);
    }
    fscanf(q, "%d", &numpoints[f]);
    for (i = 0; i < numpoints[f]; i++) {
      fscanf(q, "%lf %lf", &x[f][i], &y[f][i]);
    }
    fscanf(q, "%d", &numpolys[f]);
    for (i = 0; i < numpolys[f]; i++) {
      fscanf(q, "%d", &psize[f][i]);
      for (j = 0; j < psize[f][i]; j++) {
        fscanf(q, "%d", &con[f][i][j]);
      }
    }
    for (i = 0; i < numpolys[f]; i++) {
      fscanf(q, "%lf %lf %lf", &red[f][i], &grn[f][i], &blue[f][i]);
    }
    fclose(q);
  }

  //Print test
  //printf("numpoints: %d\tnumpolys: %d\n",numpoints,numpolys);
  //printf("x:\t");
  //print_arr(x,numpoints);
  //printf("y:\t");
  //print_arr(y,numpoints);
  //printf("fsdfsdfa\n");
  //fflush(stdout);

  //draw:
  /*for (f = 0; f < argc-1; f++) {
    for (i = 0; i < numpolys[f]; i++) {
      printf("test\n");
      double xtemp[1000];
      double ytemp[1000];
      for (j = 0; j < psize[f][i]; j++) {
	printf("test1\n");
	xtemp[j] = x[f][con[f][i][j]];
	ytemp[j] = y[f][con[f][i][j]];
      }
      print_arr(xtemp,psize[f][i]);
      //printf("test2\n");
      G_rgb(red[f][i],grn[f][i],blue[f][i]);
      //printf("test3\n");
      my_fill_polygon(xtemp,ytemp,psize[f][i]);
      G_wait_click(p);
    }
    }*/

  for (f = 0; f < argc - 1; f++) {
    printf("-------> %d: ", f);
    printf("\n1:\n");
    print_arr(y[f], numpoints[f]);
    scale(x[f], y[f], numpoints[f], 500);
    printf("\n2:\n");
    print_arr(y[f], numpoints[f]);
    translate(x[f], y[f], numpoints[f], 500, 500);
    printf("\n3:\n");
    print_arr(y[f], numpoints[f]);
    printf("\n");
  }

  int prev = -1;
  while (1) {
    //point_info();
    G_rgb(1, 1, 1);
    int m = G_wait_key();
    int rot = 0;
    if (m == prev) {
      rot = 1;
    }
    prev = m;
    G_clear();
    m -= 48;
    //printf("\n%d\n",m);
    if (rot) {
      //G_wait_key();
      //printf("please\n");
      rotate(x[m], y[m], numpoints[m], M_PI / 90);
    }
    for (i = 0; i < numpolys[m]; i++) {
      //printf(" %d ",i);
      double xtemp[1000];
      double ytemp[1000];
      for (j = 0; j < psize[m][i]; j++) {
        //printf("\ttest1\n");
        xtemp[j] = x[m][con[m][i][j]];
        ytemp[j] = y[m][con[m][i][j]];
      }
      //print_arr(xtemp,psize[m][i]);
      //printf("test2\n");
      G_rgb(red[m][i], grn[m][i], blue[m][i]);
      //printf("test3\n");
      G_fill_polygon(xtemp, ytemp, psize[m][i]);
    }
  }
}
double max(double* x, int n)
{
  if (n <= 0) {
    return -1;
  }
  int max = x[0];
  int i;
  for (i = 0; i < n; i++) {
    if (x[i] > max) {
      max = x[i];
    }
  }
  return max;
}
double min(double* x, int n)
{
  if (n <= 0) {
    return -1;
  }
  int min = x[0];
  int i;
  for (i = 0; i < n; i++) {
    if (x[i] < min) {
      min = x[i];
    }
  }
  return min;
}
void print_arr(double* x, int n)
{
  int i;
  for (i = 0; i < n; i++) {
    printf("%d: %lf\t", i, x[i]);
  }
  printf("\n");
}
void sort(double* x, int n)
{
  int i;
  for (i = 0; i < n; i++) {
    int j;
    for (j = 0; j < n; j++) {
      double hold;
      if (x[j] > x[i]) {
        hold = x[i];
        x[i] = x[j];
        x[j] = hold;
      }
    }
  }
}
void my_fill_polygon(double* x, double* y, int n)
{
  //Loop through each horizontal line
  int r;
  for (r = min(y, n); r < max(y, n) + 1; r++) {
    //printf("%d:\t",r);
    int i;
    double ints[n];
    int count = 0;
    //Locate intersections:
    for (i = 0; i < n; i++) {
      int j = (i + 1) % n;
      if ((r > y[i] && r < y[j]) || (r < y[i] && r > y[j]) && (y[i] != y[j])) {
        ints[count] = (r - y[i]) * ((x[i] - x[j]) / (y[i] - y[j])) + x[i];
        count++;
      }
    }
    //Corners cases:
    for (i = 0; i < n; i++) {
      if (r == y[i]) {
        int h = (i - 1) % n;
        int j = (i + 1) % n;
        //dir1 is last non-horiz
        int rev;
        double dir1 = 0;
        for (rev = 0; rev < n; rev++) {
          dir1 = y[(i - rev + n) % n] - y[(h - rev + n) % n];
          if (dir1 != 0) {
            break;
          }
        }
        double dir2 = y[j] - y[i];
        //printf("%lf %lf ",dir1,dir2);
        if (dir1 * dir2 > 0) {
          //printf("X:%lf ",x[i]);
          ints[count] = x[i];
          count++;
        }
      }
    }
    //Sort intersections
    sort(ints, count);
    //printf("\t");
    //print_arr(ints,count);
    //Fill b/w intersections
    for (i = 0; i < count - 1; i += 2) {
      G_line(ints[i], r, ints[i + 1], r);
    }
  }
}

void my_polygon(double* x, double* y, int n)
{
  int i;
  for (i = 0; i < n; i++) {
    int j = (i + 1) % n;
    G_line(x[i], y[i], x[j], y[j]);
  }
}
void draw_star(double* p1, double* p2)
{
  double r = sqrt(pow((p1[0] - p2[0]), 2) + pow(p1[1] - p2[1], 2));
  int i;
  double x[5];
  double y[5];
  for (i = 0; i < 5; i++) {
    x[i] = r * (cos(4 * M_PI / 5 * i + M_PI / 2)) + p1[0];
    y[i] = r * (sin(4 * M_PI / 5 * i + M_PI / 2)) + p1[1];
  }
  // my_fill_polygon(x,y,5,1000);
  //G_polygon(x,y,5);
}
void project1()
{
  //printf("0 ");
  double p[2];
  double p1[2];
  double p2[2];
  double x[100];
  double y[100];
  G_init_graphics(1000, 1000);
  G_fill_rectangle(0, 0, 20, 20);
  int i = 0;
  while (1) {
    G_wait_click(p);
    x[i] = p[0];
    y[i] = p[1];
    if (p[0] > 0 && p[0] < 20 && p[1] > 0 && p[1] < 20) {
      break;
    }
    if (i > 0) {
      G_line(x[i - 1], y[i - 1], x[i], y[i]);
    }
    i++;
  }
  printf("i:%d\n", i);

  print_arr(x, i);
  print_arr(y, i);

  my_fill_polygon(x, y, i);
  G_wait_click(p);
}
