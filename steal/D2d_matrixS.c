
#include <D2d_matrix.h>



/*

 ( x')          (x)
 ( y')  =   M * (y)  
 ( 1 )          (1)

instead of (x',y',1) = (x,y,1) * M  

*/

int D2d_mat_mult_points (double *X, double *Y, double m[3][3], double *x, double *y, int numpoints) ;
// |X0 X1 X2 ...|       |x0 x1 x2 ...|
// |Y0 Y1 Y2 ...| = m * |y0 y1 y2 ...|
// | 1  1  1 ...|       | 1  1  1 ...|

// SAFE, user may make a call like D2d_mat_mult_points (x,y, m, x,y, n) ;

int D2d_print_mat (double a[3][3])
{
  int r,c ;
  for (r = 0 ; r < 3 ; r++ ) {
      for (c = 0 ; c < 3 ; c++ ) {
           printf(" %12.4lf ",a[r][c]) ;
      }
      printf("\n") ;
  }

  return 1;
} 

int D2d_copy_mat (double a[3][3], double b[3][3])
// a = b
{
  int r,c ;
  for (r = 0; r < 3; r++) {
      for (c = 0; c < 3; c++ ) {
           a[r][c] = b[r][c];
      }
  }
  return 1 ;
} 

int D2d_mat_mult (double res[3][3], double a[3][3], double b[3][3]) {
  int r,c,i,sum;
  double a_copy[3][3];
  double b_copy[3][3];
  D2d_copy_mat(a_copy,a);
  D2d_copy_mat(b_copy,b);
  for (r = 0; r < 3; r++) {
    for (c = 0; c < 3; c++) {
      sum = 0;
      for (i = 0; i < 3; i++) {
	sum += a_copy[r][i]*b_copy[i][c];
	//printf("%lf,%lf\n",a_copy[r][i],b_copy[i][c]);
      }
      res[r][c] = sum;
      //printf("%lf\n",sum);
    }
  }
}

int D2d_make_identity (double a[3][3])
// a = I
{
  int r,c ;
  for (r = 0 ; r < 3 ; r++ ) {
      for (c = 0 ; c < 3 ; c++ ) {
           if (r == c) a[r][c] = 1.0;
               else    a[r][c] = 0.0;
      }
  }

  return 1;
} 

int D2d_translate (double a[3][3], double b[3][3], double dx, double dy)
// a = translation*a  
// b = b*translation_inverse  
{
  double t[3][3] ;

  D2d_make_identity(t) ;

  t[0][2] = dx; t[1][2] = dy;  
  D2d_mat_mult(a,t,a);

  t[0][2] = -dx; t[1][2] = -dy;
  D2d_mat_mult(b,b,t);

  return 1 ;
}

//int D2d_scale(double a[3][3], double b[3][3], double sx, double sy) {return 1;}

int D2d_scale(double a[3][3], double b[3][3], double sx, double sy){
  double a_scale[3][3];
  printf("test\n");
  D2d_make_identity(a_scale);
  double b_scale[3][3];
  D2d_make_identity(b_scale);
  a_scale[0][0] = sx;
  b_scale[0][0] = 1/sx;
  a_scale[1][1] = sy;
  b_scale[1][1] = 1/sy;
  D2d_mat_mult(a,a_scale,a);
  D2d_mat_mult(b,b_scale,b);
  return 1;
}

int D2d_rotate(double a[3][3], double b[3][3], double radians) {
  double rotate[3][3];
  double rotate_inv[3][3];
  D2d_make_identity(rotate);
  
  D2d_make_identity(rotate_inv);
  
  return 1;
} 

int D2d_mat_mult_points(double *X, double *Y, double m[3][3], double *x, double *y, int numpoints) {
  int i;
  double x_copy[numpoints];
  double y_copy[numpoints];
  for (i = 0; i < numpoints; i++) {
    x_copy[i] = x[i];
    y_copy[i] = y[i];
  }
  for (i = 0; i < numpoints; i++) {
    X[i] = m[0][0]*x[i] + m[0][1]*y[i] + m[0][2]; 
    Y[i] = m[1][0]*x[i] + m[1][1]*y[i] + m[1][2];
  }
  return 1;
}

int D2d_negate_x (double a[3][3], double b[3][3]) {
  double reflect[3][3];
  D2d_make_identity(reflect);
  reflect[0][0] = -1;
  D2d_mat_mult(a,reflect,a);
  D2d_mat_mult(b,b,reflect);
}
int D2d_negate_y (double a[3][3], double b[3][3]) {
  double reflect[3][3];
  D2d_make_identity(reflect);
  reflect[1][1] = -1;
  D2d_mat_mult(a,reflect,a);
  D2d_mat_mult(b,b,reflect);  
}


/*int D2d_print_mat (double a[3][3])
{
  int r,c ;
  for (r = 0 ; r < 3 ; r++ ) {
      for (c = 0 ; c < 3 ; c++ ) {
           printf(" %12.4lf ",a[r][c]) ;
      }
      printf("\n") ;
  }

  return 1 ;
} 




int D2d_copy_mat (double a[3][3], double b[3][3])
// a = b
{
  int r,c ;
  for (r = 0 ; r < 3 ; r++ ) {
      for (c = 0 ; c < 3 ; c++ ) {
           a[r][c] = b[r][c] ;
      }
  }

  return 1 ;
} 








int D2d_make_identity (double a[3][3])
// a = I
{
  int r,c ;
  for (r = 0 ; r < 3 ; r++ ) {
      for (c = 0 ; c < 3 ; c++ ) {
           if (r == c) a[r][c] = 1.0 ;
               else    a[r][c] = 0.0 ;
      }
  }

  return 1 ;
} 


/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////


int D2d_translate (double a[3][3], double b[3][3], double dx, double dy)
// a = translation*a  
// b = b*translation_inverse  
{
  double t[3][3] ;

  D2d_make_identity(t) ;

  t[0][2] =  dx ;  t[1][2] = dy ;  
  D2d_mat_mult(a,  t,a) ;

  t[0][2] = -dx ;  t[1][2] = -dy ;
  D2d_mat_mult(b,  b,t) ;

  return 1 ;
}
*/
