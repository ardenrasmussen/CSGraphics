#include "base.h"
#include "matrix_math.h"
#include <FPT.h>
#include <stdio.h>
#include <stdlib.h>

struct Object RotateObjView(struct Object obj, int dir){
  double rotate = 0.1;
  double mat[4][4], inv[4][4];
  CreateIdentity3D(mat);
  CreateIdentity3D(inv);
  if(dir < 0){
    dir *= -1;
    rotate *= -1;
  }
  if (dir == 1) {
    RotateCSX3D(mat, inv, cos(rotate), sin(rotate));
  } else if (dir == 2) {
    RotateCSY3D(mat, inv, cos(rotate), sin(rotate));
  } else if (dir == 3) {
    RotateCSZ3D(mat, inv, cos(rotate), sin(rotate));
  }
  MatrixMultiplyPoints3D(obj.x, obj.y, obj.z, mat, obj.x, obj.y, obj.z, obj.points_n);
  return obj;
}

void View(){
  double diff_eye[3], diff_coi[3], diff_up[3];
  for (int i = 0; i < 3;i++){
    diff_eye[i] = eye[i] - last_eye[i];
    diff_coi[i] = coi[i] - last_coi[i];
    diff_up[i] = up[i] - last_up[i];
    last_eye[i] = eye[i];
  }
  printf("<%f, %f, %f>\n", diff_eye[0], diff_eye[1], diff_eye[2]);
  double mat[4][4], inv[4][4];
  CreateIdentity3D(mat);
  CreateIdentity3D(inv);
  Translate3D(mat, inv, -diff_eye[0], -diff_eye[1], -diff_eye[2]);
  for(int i = 0; i < n_obj; i++){
    MatrixMultiplyPoints3D(objects[i].x, objects[i].y, objects[i].z, mat, objects[i].x, objects[i].y, objects[i].z, objects[i].points_n);
  }
  DisplayObjs();
}

void LoadScene(const char* file_path){
  FILE* f = fopen(file_path, "r");
  if (f == NULL){
    printf ("\"%s\" does not exist!\n", file_path);
    exit(1);
  }else{
    printf ("Loaded \"%s\"\n", file_path);
  }
  fscanf(f, "%i", &n_obj);
  printf(">%i\n", n_obj);
  for(int i = 0; i < n_obj; i++){
    char* src;
    fscanf(f, "%s", src);
    printf(">>%s\n", src);
    objects[i] = LoadObject(src);
  }
}

int main(int argc, const char* argv[])
{
  srand(time(NULL));
  n_obj = argc - 1;
  int prev_dir = 1;
  /* if (argc > 1){ */
    /* LoadScene(argv[1]); */
  /* } */
  for (int i = 1; i < argc; i++) {
    objects[i - 1] = LoadObject(argv[i]);
  }
  G_init_graphics(WIDTH, HEIGHT);
  Clear();
  int current = -1;
  int setting = -1;
  while (1) {
    Clear();
    /* DisplayObjs(); */
    View();
    G_rgb(1.0, 0.0, 1.0);
    DrawPoint(light[0], light[1], light[2]);
    int key = G_wait_key();
    if (key >= 48 && key <= 57 && key - 47 < argc) {
      current = key - 48;
    }else if(key == 'o'){
      current = -2;
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
      if (setting == 1 && current != -2) {
        objects[current] = TranslateObj(objects[current], dir);
      }  else if (setting == 2 && current != -2) {
        objects[current] = RotateObj(objects[current], dir);
      } else if (setting == 3 && current != -2) {
        ScaleObj(&objects[current], dir);
      } else if (setting == 4 && current != -2) {
        if (rand() % 10 == 0) {
          prev_dir = (rand() % 3) + 1;
        }
        objects[current] = RotateObj(objects[current], prev_dir);
      }else if(setting == 1){
        if(dir == -1){
          eye[0] -= 0.5;
        }else if(dir == 1){
          eye[0] += 0.5;
        }else if(dir == -2){
          eye[1] -= 0.5;
        }else if(dir == 2){
          eye[1] += 0.5;
        }else if(dir == -3){
          eye[2] -= 0.5;
        }else if(dir == 3){
          eye[2] += 0.5;
        }
      }else if(setting == 2){
        for(int i = 0; i < n_obj; i++){
          objects[i] = RotateObjView(objects[i], -dir);
        }
      }
    }
  }
  G_close();
}
