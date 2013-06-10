#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

#include <iostream>
#include <cmath>
#include <vector>
#include <windows.h>
#include <fstream>
#include "glut.h"
#include "Vector2D.h" 

GLint Width = 512, Height = 512;
int K1, K2, K3, K4;
std::vector<Vector2D> points1, points2, border;
std::vector<double> coefs;

double xl = -10, xr = 10, yb = -10, yt = 10;

double U(const Vector2D& x, const Vector2D& y)
  {
  return 1.0 / (1.0 + 10 * (x - y) * (x - y));
  }

void Teach()
  {
  double curr_phi;
  for(int i = 0; i < K1 + K2; ++i)
    {
    curr_phi = 0;
    for(int j = 0; j < i; ++j)
      curr_phi += coefs[j] * U(points1[i], points1[j]);
    if(i < K1 && curr_phi > 0 || i >= K1 && curr_phi < 0)
      coefs[i] = 0;
    else if(i < K1 && curr_phi < 0 || i == 0 && K1 > 0)
      coefs[i] = 1;
    else if(i >= K1 && curr_phi > 0 || i == 0 && K1 == 0 && K2 > 0)
      coefs[i] = -1;
    }
  }

double Phi(const Vector2D& x)
  {
  double summ = 0;
  for(int i = 0; i < K1 + K2; ++i)
    if(coefs[i])
      summ += coefs[i] * U(x, points1[i]);
  return summ;
  }

void Test()
  {
  int mist1 = 0, mist2 = 0;
  for(int i = 0; i < K3 + K4; ++i)
    {
    if( i < K3 && Phi(points2[i]) <=0)
      mist1++;
    if( i >= K3 && Phi(points2[i]) >=0)
      mist2++;
    }

  std::cout << "First class representatives:\n\n";
  for(int i = 0; i < K3; ++i)
    std::cout << points2[i].X() << " " << points2[i].Y() << '\n';
  std::cout << "\nMistakes: " << mist1 * 100 / (double) K3 << " percents\n\n\n\n";

  std::cout << "Second class representatives:\n\n";
  for(int i = K3; i < K3 + K4; ++i)
    std::cout << points2[i].X() << " " << points2[i].Y() << '\n';
  std::cout << "\nMistakes: " << mist2 * 100 / (double) K4 << " percents\n";
  }

void CalcBorder()
  {
  double h = 0.01, eps = 0.0001;
  Vector2D vec;
  for(double i = xl; i < xr; i += h)
    for(double j = yb; j < yt; j += h)
      {
      vec.X() = i;
      vec.Y() = j;
      if(std::abs(Phi(vec)) < eps)
        border.push_back(vec);
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
    glVertex2d(points1[i].X(), points1[i].Y());
  glEnd();
  
  //учебная выборка второго класса
  glColor3d(0,1,0);
  glBegin(GL_POINTS);
  for(std::size_t i = K1; i < K1 + K2; ++i)
    glVertex2d(points1[i].X(), points1[i].Y());
  glEnd();

  //тестовая выборка первого класса
  glColor3d(1,1,0);
  glBegin(GL_POINTS);
  for(std::size_t i = 0; i < K3; ++i)
    glVertex2d(points2[i].X(), points2[i].Y());
  glEnd();

  //тестовая выборка второго класса
  glColor3d(0,1,1);
  glBegin(GL_POINTS);
  for(std::size_t i = K3; i < K3 + K4; ++i)
    glVertex2d(points2[i].X(), points2[i].Y());
  glEnd();

  //граница (разделяющая функция)
  glColor3d(1,1,1);
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
  Vector2D vec;
  std::ifstream f("Teaching1.txt"), g("Teaching2.txt"),
    h("Teaching1.txt"), t("Teaching2.txt");
  f >> K1;
  g >> K2;
  h >> K3;
  t >> K4;
  coefs.resize(K1 + K2);
  for(int i = 0; i < K1; ++i)
    {
    f >> vec.X();
    f >> vec.Y();

    points1.push_back(vec);
    }
  f.close();
  
  for(int i = 0; i < K2; ++i)
    {
    g >> vec.X();
    g >> vec.Y();

    points1.push_back(vec);
    }
  g.close();

  for(int i = 0; i < K3; ++i)
    {
    h >> vec.X();
    h >> vec.Y();

    points2.push_back(vec);
    }
  h.close();

  for(int i = 0; i < K4; ++i)
    {
    t >> vec.X();
    t >> vec.Y();

    points2.push_back(vec);
    }
  t.close();

  Teach();
  Test();
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
