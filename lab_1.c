#include <FPT.h>

int WIDTH = 500;
int HEIGHT = 500;

struct Object {
  int points;
  double x[100], y[100];
  double center[2];
};

struct Object GetPolygon()
{
  double p[2] = { 100, 100 };
  struct Object obj;
  obj.points = 0;
  while (p[0] > 50 || p[1] > 50) {
    G_wait_click(p);
    obj.x[obj.points]
        = p[0];
    obj.y[obj.points] = p[1];
    obj.points++;
    if (obj.points > 1) {
      G_line(obj.x[obj.points - 1], obj.y[obj.points - 1], obj.x[obj.points], obj.y[obj.points]);
    }
  }
  G_line(obj.x[obj.points], obj.y[obj.points], obj.x[0], obj.y[0]);
  return obj;
};

int main(int argc, char* argv[])
{
  G_init_graphics(WIDTH, HEIGHT);
  G_rgb(0, 0, 0);
  G_clear();
  G_rgb(0, 1, 1);
  GetPolygon();
  G_close();
  return 0;
}
