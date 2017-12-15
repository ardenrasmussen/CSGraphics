#include "base.h"
#include "matrix_math.h"
#include <FPT.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char* argv[])
{
  srand(time(NULL));
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
    DisplayObjs();
    G_rgb(1.0, 0.0, 1.0);
    DrawPoint(light[0], light[1], light[2]);
    int key = G_wait_key();
    if (key >= 48 && key <= 57 && key - 47 < argc) {
      current = key - 48;
    } else if (key == 't') {
      setting = 1;
    } else if (key == 'r') {
      setting = 2;
    } else if (key == 'f') {
      SetFOV();
    } else if (key == 'c') {
      ReadColor(&objects[current].red, &objects[current].green, &objects[current].blue);
    } else if (key == 'g') {
      setting = 4;
    } else if (key == 'j') {
      if (INVERSE == 1) {
        INVERSE = -1;
      } else if (INVERSE == -1) {
        INVERSE = 1;
      }
    } else if (key == 'l') {
      ReadLight();
    } else if (key == 'w') {
      light[1]++;
    } else if (key == 's') {
      light[1]--;
    } else if (key == 'a') {
      light[0]--;
    } else if (key == 'd') {
      light[0]++;
    } else if (key == 'q') {
      light[2]--;
    } else if (key == 'e') {
      light[2]++;
    } else if (key == 65307) {
      break;
    } else if (key == '-') {
      ScaleObj(&objects[current], -4);
    } else if (key == '+') {
      ScaleObj(&objects[current], 4);
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
