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
#include "Algos.h"
#include "RandNums.h"
GLint Width = 512, Height = 512;
std::vector<MatrSpace::Matrix> points;
std::vector<std::vector<Vector2D>> border(3);
double xl = -10, xr = 10, yb = -10, yt = 10;
MatrSpace::Matrix khachiyan_ellipse, khachiyan_centre, 
  centroid_ellipse, centroid, 
  affine_ellipse, affine_centroid;
bool draw_points = true, draw_khachiyan_ellipse = true, 
  draw_centroid_ellipse = true, draw_affine_ellipse = true;

void CalcBorders()
  {
    std::cout << "Calculating borders...\n";
    double h = 0.01, eps = 0.0001;
    for(double i = xl; i < xr; i += h)
      for(double j = yb; j < yt; j += h)
        {
        if(std::abs( khachiyan_ellipse(0,0) * (i - khachiyan_centre(0, 0)) * (i - khachiyan_centre(0, 0)) + 
          (khachiyan_ellipse(0,1) + khachiyan_ellipse(1, 0)) * (i - khachiyan_centre(0, 0)) * (j - khachiyan_centre(1, 0)) + 
          khachiyan_ellipse(1,1) * (j - khachiyan_centre(1, 0)) * (j - khachiyan_centre(1, 0)) - 1) < eps)
          border[0].push_back(Vector2D(i,j));

        if( std::abs( centroid_ellipse(0, 0) * (i - centroid(0, 0)) * (i - centroid(0, 0)) + 
          (centroid_ellipse(0, 1) + centroid_ellipse(1 ,0)) * (i - centroid(0, 0)) * (j - centroid(1, 0)) + 
          centroid_ellipse(1, 1) * (j - centroid(1, 0)) * (j - centroid(1, 0)) - 1) < eps)
          border[1].push_back(Vector2D(i,j));

        if( std::abs( affine_ellipse(0, 0) * (i - affine_centroid(0, 0)) * (i - affine_centroid(0, 0)) + 
          (affine_ellipse(0, 1) + affine_ellipse(1, 0)) * (i - affine_centroid(0, 0)) * (j - affine_centroid(1, 0)) + 
          affine_ellipse(1, 1) * (j - affine_centroid(1, 0)) * (j - affine_centroid(1, 0)) - 1) < eps)
          border[2].push_back(Vector2D(i,j));
        }
  }

void Display(void)
  {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(2.0);

    //Îñü ÎÕ
    glColor3d(0, 0, 1);
    glBegin(GL_LINES);
    glVertex2d(xl, 0);
    glVertex2d(xr, 0);
    glEnd();

    //Îñü ÎÓ
    glBegin(GL_LINES);
    glVertex2d(0, yb);
    glVertex2d(0, yt);
    glEnd();

    double PI = 3.14159;

    if(draw_khachiyan_ellipse)
      {
        glColor3d(0,1,0);
        glBegin(GL_POINTS);
        for(std::size_t i = 0; i < border[0].size(); ++i)
          glVertex2d(border[0][i].X(), border[0][i].Y());
        glEnd();
      }

    if(draw_centroid_ellipse)
      {
        glColor3d(0,0,1);

        glBegin(GL_POINTS);
        for(std::size_t i = 0; i < border[1].size(); ++i)
          glVertex2d(border[1][i].X(), border[1][i].Y());
        glEnd();
      }
 
    if(draw_affine_ellipse)
      {
      glColor3d(0,1,1);

      glBegin(GL_POINTS);
      for(std::size_t i = 0; i < border[2].size(); ++i)
        glVertex2d(border[2][i].X(), border[2][i].Y());
      glEnd();
      }

    if(draw_points)
      {
      glColor3d(1,0,0);
      glBegin(GL_POINTS);
      for(std::size_t i = 0; i < points.size(); ++i)
        glVertex2d(points[i](0, 0), points[i](1, 0));
      glEnd();
      }
  


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
    if(key == '1')
      draw_points = !draw_points;
  
    if(key == '2')
      draw_khachiyan_ellipse = !draw_khachiyan_ellipse;
  
    if(key == '3')
      draw_centroid_ellipse = !draw_centroid_ellipse;
  
    if(key == '4')
      draw_affine_ellipse = !draw_affine_ellipse;
  
    glutPostRedisplay();
  }


void Test()
  {
    std::cout << "Testing samples...\nEllipses order:\nKhachiyan  Centroid  Affine\n";
    int N = 100;
    std::vector<MatrSpace::Matrix> sample;
    MatrSpace::Matrix khachiyan_ellipse1, khachiyan_centre1, 
      centroid_ellipse1, centroid1, 
      affine_ellipse1, affine_centroid1;
    double d1, d2, d3;
    std::ofstream f("TestedEllipses.txt"), g("TestedSamples.txt");
    for(int i = 0; i < 100; ++i)
      {
        GetSample(sample, 100);
        KhachiyanEllipse(khachiyan_ellipse1, khachiyan_centre1, sample);
        CentroidEllipse(centroid_ellipse1, centroid1, khachiyan_ellipse1, khachiyan_centre1, sample);
        AffineEllipse(affine_ellipse1, affine_centroid1, sample);
        
        f << i << '\n' 
          << "Khachiyan ellipse\n" << khachiyan_ellipse1 << '\n' << khachiyan_centre1(0, 0) << " " << khachiyan_centre1(1, 0) << "\n\n"
          << "Centroid ellipse\n" << centroid_ellipse1 << '\n' << centroid1(0, 0) << " " << centroid1(1, 0) << "\n\n"
          << "Affine ellipse\n" << affine_ellipse1 << '\n' << affine_centroid1(0, 0) << " " << affine_centroid1(1, 0) << '\n';
        f <<"\n\n";
         
        g << i << '\n';
        for(int j = 0; j < points.size(); ++j)
          g << points[j](0, 0) << "  " << points[j](1, 0) << '\n';
        g << "\n\n";
        d1 = EllipseVol(khachiyan_ellipse1);
        d2 = EllipseVol(centroid_ellipse1);
        d3 = EllipseVol(affine_ellipse1);

        std::cout << d1 << "  " << d2 << "  " << d3 << "  " << "\n";
      }
    std::cout << "Test finished.\n";
  }

void main(int argc, char *argv[])
  {
    srand((int)time(0));
    //GetSample(points, 100);
    int K1;
    std::ifstream f("Teaching.txt");
    f >> K1;
    points.resize(K1, MatrSpace::Matrix(2, 1));
    for(int i = 0; i < K1; ++i)
      {
      f >> points[i](0, 0);
      f >> points[i](1, 0);
      }
    f.close();

    std::cout << "Calculating ellipses for output...\n";

    KhachiyanEllipse(khachiyan_ellipse, khachiyan_centre, points);
    CentroidEllipse(centroid_ellipse, centroid, khachiyan_ellipse, khachiyan_centre, points);
    AffineEllipse(affine_ellipse, affine_centroid, points);
    double d1, d2, d3;
    std::cout << "Output ellipses volumes:\nEllipses order:\nKhachiyan  Centroid  Affine\n";
    d1 = EllipseVol(khachiyan_ellipse);
    d2 = EllipseVol(centroid_ellipse);
    d3 = EllipseVol(affine_ellipse);

    std::cout << d1 << "  " << d2 << "  " << d3 << "  " << "\n";
    CalcBorders();
  
    Test();

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
