#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

#include <iostream>
#include <cmath>
#include <vector>
#include <windows.h>
#include <fstream>
#include<ctime>
#include "glut.h"
#include "Vector2D.h"
#include "ellipsoids.hpp"
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include "RandNums.h"
GLint Width = 512, Height = 512;
int K1;
boost::numeric::ublas::vector<double> centre(2);
boost::numeric::ublas::matrix<double> points, Q(2, 2);
std::vector<Vector2D> border;
double xl = -10, xr = 10, yb = -10, yt = 10;


void CalcBorder()
  {
  double h = 0.01, eps = 0.0001;
  border.resize(2, 0);
  for(double i = xl; i < xr; i += h)
    for(double j = yb; j < yt; j += h)
      {
      if(std::abs( Q(0,0) * (i - centre(0)) * (i - centre(0)) + 
        (Q(0,1) + Q(1,0)) * (i - centre(0)) * (j - centre(1)) + 
        Q(1,1) * (j - centre(1)) * (j - centre(1)) - 1) < eps)
        border.push_back(Vector2D(i,j));
      }
  }

void Display(void)
  {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_POINT_SMOOTH);
  glPointSize(2.0);

  //Ось ОХ
  glColor3d(0, 0, 1);
  glBegin(GL_LINES);
  glVertex2d(xl, 0);
  glVertex2d(xr, 0);
  glEnd();

  //Ось ОУ
  glBegin(GL_LINES);
  glVertex2d(0, yb);
  glVertex2d(0, yt);
  glEnd();

  //учебная выборка первого класса
  glColor3d(1,0,0);
  glBegin(GL_POINTS);
  for(std::size_t i = 0; i < K1; ++i)
    glVertex2d(points(0, i), points(1, i));
  glEnd();

  glColor3d(0,1,0);

  double PI = 3.14159;

  glBegin(GL_POINTS);
  for(std::size_t i = 0; i < border.size(); ++i)
    glVertex2d(border[i].X(), border[i].Y());
  glEnd();

  glutSwapBuffers();
  }




void Reshape(GLint w, GLint h)
  {
  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(xl, xr, yb, yt, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  }


void Keyboard( unsigned char key, int x, int y )
  {
  }

void main(int argc, char *argv[])
  {
  srand((int)time(0));
  
  std::ifstream f("Teaching2.txt");
  f >> K1;
  points.resize(2,K1);
  for(int i = 0; i < K1; ++i)
    {
    f >> points(0,i);
    f >> points(1,i);
    }
  f.close();
  Minim::KhachiyanAlgo(points, 0.000001, 100, Q, centre);
  CalcBorder();
  
  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); 
  glutInitWindowSize(Width, Height); 
  glutInitWindowPosition(0,0); 
  glutCreateWindow("Lab2"); 

  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Keyboard);

  glutMainLoop();
  }
