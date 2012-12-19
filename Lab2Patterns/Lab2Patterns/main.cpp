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
#include "Matrix.h"
#include "ellipsoids.hpp"
#include "Algos.h"
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include "RandNums.h"
GLint Width = 512, Height = 512;
int K1;
boost::numeric::ublas::vector<double> centre(2);
boost::numeric::ublas::matrix<double> points, Q(2, 2);
std::vector<std::vector<Vector2D>> border(2);
double xl = -10, xr = 10, yb = -10, yt = 10;
MatrSpace::Matrix centroid_ellipse, centroid;

void CalcBorder()
  {
  double h = 0.01, eps = 0.0001;
  for(double i = xl; i < xr; i += h)
    for(double j = yb; j < yt; j += h)
      {
      if(std::abs( Q(0,0) * (i - centre(0)) * (i - centre(0)) + 
        (Q(0,1) + Q(1,0)) * (i - centre(0)) * (j - centre(1)) + 
        Q(1,1) * (j - centre(1)) * (j - centre(1)) - 1) < eps)
        border[0].push_back(Vector2D(i,j));

      if( std::abs( centroid_ellipse(0,0) * (i - centroid(0, 0)) * (i - centroid(0, 0)) + 
        (centroid_ellipse(0, 1) + centroid_ellipse(1,0)) * (i - centroid(0, 0)) * (j - centroid(1, 0)) + 
        centroid_ellipse(1, 1) * (j - centroid(1, 0)) * (j - centroid(1, 0)) - 1) < eps)
        border[1].push_back(Vector2D(i,j));
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

  ////учебная выборка первого класса
  //glColor3d(1,0,0);
  //glBegin(GL_POINTS);
  //for(std::size_t i = 0; i < K1; ++i)
  //  glVertex2d(points(0, i), points(1, i));
  //glEnd();

  glColor3d(0,1,0);

  double PI = 3.14159;

  glBegin(GL_POINTS);
  for(std::size_t i = 0; i < border[0].size(); ++i)
    glVertex2d(border[0][i].X(), border[0][i].Y());
  glEnd();

  glColor3d(0,0,1);

  glBegin(GL_POINTS);
  for(std::size_t i = 0; i < border[1].size(); ++i)
    glVertex2d(border[1][i].X(), border[1][i].Y());
  glEnd();

  //учебная выборка первого класса
  glColor3d(1,0,0);
  glBegin(GL_POINTS);
  for(std::size_t i = 0; i < K1; ++i)
    glVertex2d(points(0, i), points(1, i));
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
  //std:: cout << Q(0,0) << " " << Q(0,1) << '\n' << Q(1,0) << " " << Q(1,1) << "\n\n";
  //std:: cout << centre(0) << " " << centre(1) << "\n\n";
  MatrSpace::Matrix Q1 = ConvToMatr(Q), vec(2, 1);
  //std::cout << Q1;
  std::vector<MatrSpace::Matrix> points1;
  points1.resize(points.size2(), MatrSpace::Matrix(2,1));
  for(int i = 0; i < points.size2(); ++i)
    {
    points1[i](0, 0) = points(0, i);
    points1[i](1, 0) = points(1, i);
    }
  
  CentroidEllipse(centroid_ellipse, centroid, Q1, ConvToMatr(centre), points1);
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
