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
#include "Drawer.h"
GLint Width = 1002, Height = 333;
std::vector<MatrSpace::Matrix> points, res, khachiyan_ellipses, affine_ellipses, khachiyan_centres, affine_centres;
std::vector<std::vector<Vector2D>> convex_levels, khachiyan_borders, affine_borders;
std::vector<std::vector<double>> areas(3);
std::vector<Vector2D> centres(3), convex_centre;
std::vector<Drawer> drawers(3);
Drawer global_drawer;
int mode = 0;
std::vector<std::vector<Vector2D>> levels;
double minx, maxx, miny, maxy;
void BuildConvexLevels (std::vector<std::vector<Vector2D>>& o_levels, std::vector<double>& o_areas, Vector2D& o_centre, std::vector<MatrSpace::Matrix> i_points)
  {
    std::vector<MatrSpace::Matrix> curr_points;
    while(!i_points.empty())
      {
        o_levels.push_back(std::vector<Vector2D>());
        curr_points.clear();
        GrahamScan(curr_points, i_points);
        for(std::size_t j = 0; j < curr_points.size(); ++j)
          o_levels[o_levels.size() - 1].push_back(Vector2D(curr_points[j](0, 0), curr_points[j](1, 0)));
        o_areas.push_back(ConvexVol(curr_points));
        for(std::size_t j = 0; j < curr_points.size(); ++j)
          for(std::size_t i = 0; i < i_points.size(); ++i)
            if(i_points[i] == curr_points[j])
              {
                i_points.erase(i_points.begin() + i);
                break;
              }
      }
    std::size_t M = o_levels.size(), N = o_levels[M - 1].size();
    for(std::size_t i = 0; i < N; ++i)
      {
        o_centre.X() += o_levels[M - 1][i].X();
        o_centre.Y() += o_levels[M - 1][i].Y();
      }
    o_centre.X() /= N;
    o_centre.Y() /= N;
  }

void BuildEllipses(std::vector<MatrSpace::Matrix>& o_ellipses, std::vector<MatrSpace::Matrix>& o_centres, std::vector<double>& o_areas, Vector2D& o_centre,
  std::vector<MatrSpace::Matrix> i_points, 
  void (*i_ellipse_func)(MatrSpace::Matrix&, MatrSpace::Matrix&, std::vector<MatrSpace::Matrix>))
  {
    MatrSpace::Matrix curr_ellipse, curr_centre;

    while(i_points.size() > 1)
      {
        i_ellipse_func(curr_ellipse, curr_centre, i_points);
        o_ellipses.push_back(curr_ellipse);
        o_areas.push_back(EllipseVol(curr_ellipse));
        o_centres.push_back(curr_centre);
        for(std::vector<MatrSpace::Matrix>::iterator it = i_points.begin(); it != i_points.end();)
          {
          double val = std::abs( ((*it)(0, 0) - curr_centre(0, 0)) * ((*it)(0, 0) - curr_centre(0, 0)) * curr_ellipse(0, 0) + 
            ((*it)(0, 0) - curr_centre(0, 0)) * ((*it)(1, 0) - curr_centre(1, 0)) * (curr_ellipse(0, 1) + curr_ellipse(1, 0)) + 
            ((*it)(1, 0) - curr_centre(1, 0)) * ((*it)(1, 0) - curr_centre(1, 0)) * curr_ellipse(1, 1) - 
            1);
            if(val < 0.0001)
              {
                it = i_points.erase(it);
                continue;
              }
            ++it;
          }
      }
    if(i_points.size() == 1)
      o_centre = Vector2D(i_points[0](0, 0), i_points[0](1, 0));
    else
      o_centre = Vector2D(curr_centre(0, 0), curr_centre(1, 0));
  }

void CalcEllipseBorder(std::vector<Vector2D>& o_vertices, const MatrSpace::Matrix& i_ellipse_matr, const MatrSpace::Matrix& i_ellipse_centre)
  {
    int N = 100;
    double h = (maxx - minx) / 100;
    double D, y1, y2, x;
    double det = i_ellipse_matr(0, 0) * i_ellipse_matr(1, 1) - i_ellipse_matr(1, 0) * i_ellipse_matr(0, 1);
    det = sqrt(det);
    det = 1 / det;
    det *= 8.0 / 3.14159;
    det = sqrt(det);
    h = 2 * det / N;
    for(int i = 0; i < N + 1; ++i)
      {
        x = -det + i * h;
        D = x * x * (i_ellipse_matr(1, 0) + i_ellipse_matr(0, 1)) * (i_ellipse_matr(1, 0) + i_ellipse_matr(0, 1)) - 4 * i_ellipse_matr(1, 1) * ( x * x * i_ellipse_matr(0, 0) - 1);
        if (D < 0)
          continue;
        else
          {
            y1 = - x * (i_ellipse_matr(1, 0) + i_ellipse_matr(0, 1));
            if(D < 0.00001)
              {
                y1 /= 2 * i_ellipse_matr(1, 1);
                o_vertices.push_back(Vector2D(x + i_ellipse_centre(0, 0), y1 + i_ellipse_centre(1, 0)));
              }
            else
              {
                D = sqrt(D);
                y2 = y1;
                y1 += D;
                y2 -= D;
                y1 /= 2 * i_ellipse_matr(1, 1);
                y2 /= 2 * i_ellipse_matr(1, 1);
                o_vertices.push_back(Vector2D(x + i_ellipse_centre(0, 0), y1 + i_ellipse_centre(1, 0)));
                o_vertices.push_back(Vector2D(x + i_ellipse_centre(0, 0), y2 + i_ellipse_centre(1, 0)));
              }
          }
      }
  }

void DrawEllipse(const std::vector<Vector2D>& i_vertices, const Drawer& i_drawer)
  { 
    std::size_t M = i_vertices.size();
    if(M < 2)
      return;
    
    glBegin(GL_LINE_STRIP);
    i_drawer.DrawVert(i_vertices[0].X(), i_vertices[0].Y());
    i_drawer.DrawVert(i_vertices[1].X(), i_vertices[1].Y());
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    i_drawer.DrawVert(i_vertices[M - 2].X(), i_vertices[M - 2].Y());
    i_drawer.DrawVert(i_vertices[M - 1].X(), i_vertices[M - 1].Y());
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    for(std::size_t i = 0; i < M; i += 2)
      i_drawer.DrawVert(i_vertices[i].X(), i_vertices[i].Y());
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    for(std::size_t i = 1; i < M; i += 2)
      i_drawer.DrawVert(i_vertices[i].X(), i_vertices[i].Y());
    glEnd();
  }


void DrawAxes(const Drawer& i_drawer)
  {
    double minx = i_drawer.GetMinX(), miny = i_drawer.GetMinY(),
      maxx = i_drawer.GetMaxX(), maxy = i_drawer.GetMaxY();
    glColor3d(0, 0, 1);
    glBegin(GL_LINE_STRIP);
    i_drawer.DrawVert(0, 0);
    i_drawer.DrawVert(maxx, 0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    i_drawer.DrawVert(0, 0);
    i_drawer.DrawVert(0, maxy);
    glEnd();
  }

void Display(void)
  {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(2.0);
    glColor3d(1, 0, 0);
    global_drawer.DrawText("Convex levels", 0.01, 0.01);
    global_drawer.DrawText("Khachiyan levels", 1.0 / 3.0 + 0.01, 0.01);
    global_drawer.DrawText("Affine levels", 2.0 / 3.0 + 0.01, 0.01);
    glColor3d(1, 1, 1);
    glBegin(GL_LINE_STRIP);
    global_drawer.DrawVert(1 / 3.0, 0);
    global_drawer.DrawVert(1 / 3.0, 1);
    glEnd();


    glBegin(GL_LINE_STRIP);
    global_drawer.DrawVert(2 / 3.0, 0);
    global_drawer.DrawVert(2 / 3.0, 1);
    glEnd();    
    if(mode == 0)
      {
        glColor3d(1, 0, 1);
        std::size_t M = convex_levels.size();
        for(std::size_t i = 0; i < convex_levels.size(); ++i)
          {
            std::size_t N = convex_levels[i].size();
            glBegin(GL_LINE_STRIP);
            for(std::size_t j = 0; j < N + 1; ++j)
              drawers[0].DrawVert(convex_levels[i][j % N].X(), convex_levels[i][j % N].Y());
            glEnd();
          }

        glColor3d(1, 0, 1);
        for(std::size_t i = 0; i < khachiyan_borders.size(); ++i)
          DrawEllipse(khachiyan_borders[i], drawers[1]);
        for(std::size_t i = 0; i < khachiyan_borders.size(); ++i)
          DrawEllipse(affine_borders[i], drawers[2]);


        glColor3d(1, 1, 0);
        glBegin(GL_POINTS);
        for(std::size_t j = 0; j < drawers.size(); ++j)
          for(std::size_t i = 0; i < points.size(); ++i)
            drawers[j].DrawVert(points[i](0, 0), points[i](1, 0));
        glEnd();

        glColor3d(0, 1, 1);
        for(std::size_t i = 0; i < drawers.size(); ++i)
          {
            glBegin(GL_POINTS);
            drawers[i].DrawVert(centres[i].X(), centres[i].Y());
            glEnd();
          }
      }
    else if(mode == 1)
      {
        for(std::size_t i = 0; i < drawers.size(); ++i)
          {
            DrawAxes(drawers[i]);
            glColor3d(1, 1, 1);
            glBegin(GL_LINE_STRIP);
            for(std::size_t j = 0; j < areas[i].size(); ++j)
              drawers[i].DrawVert(j, areas[i][j]);
            glEnd();
            glColor3d(1, 1, 0);
            glBegin(GL_POINTS);
            for(std::size_t j = 0; j < areas[i].size(); ++j)
              drawers[i].DrawVert(j, areas[i][j]);
            glEnd();
            for(std::size_t j = 0; j < areas[i].size(); ++j)
              if( j % (areas[i].size() / 20 + 1) == 0)
                drawers[i].DrawText(std::to_string((long double)areas[i][j]).substr(0, 5), j, areas[i][j] + 0.005 * (drawers[i].GetMaxY() - drawers[i].GetMinY()));
          }
      }
    else if(mode == 2)
     {
        for(std::size_t i = 0; i < drawers.size(); ++i)
          {
            DrawAxes(drawers[i]);
            glColor3d(1, 1, 1);
            glBegin(GL_LINE_STRIP);
            for(std::size_t j = 0; j < areas[i].size() - 1; ++j)
              drawers[i].DrawVert(j, areas[i][j] - areas[i][j + 1]);
            glEnd();
            glColor3d(1, 1, 0);
            glBegin(GL_POINTS);
            for(std::size_t j = 0; j < areas[i].size() - 1; ++j)
              drawers[i].DrawVert(j, areas[i][j] - areas[i][j + 1]);
            glEnd();
            for(std::size_t j = 0; j < areas[i].size() - 1; ++j)
              if(j % (areas[i].size() / 20 + 1) == 0)
                drawers[i].DrawText(std::to_string((long double)areas[i][j] - areas[i][j + 1]).substr(0, 5), j, areas[i][j] - areas[i][j + 1] + 0.005 * (drawers[i].GetMaxY() - drawers[i].GetMinY()));
          }
      }
    glutSwapBuffers();
  }




void Reshape(GLint w, GLint h)
  {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1.0, 1.0);

    drawers[0].SetTarget(0, w / 3.0, 0, h);
    drawers[1].SetTarget(w / 3.0, 2 * w / 3.0, 0, h);
    drawers[2].SetTarget(2 * w / 3.0, w, 0, h);
    global_drawer.SetTarget(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }


void Keyboard( unsigned char key, int x, int y )
  {
    if(key == '1')
      {
        mode = 0;
        for(std::size_t i = 0; i < drawers.size(); ++i)
          drawers[i].SetViewport(minx - 1, maxx + 1, miny - 1, maxy + 1);
      }

    if(key == '2')
      {
        mode = 1;
        for(std::size_t i = 0; i < drawers.size(); ++i)
          drawers[i].SetViewport(-1, areas[i].size() + 1, -1, areas[i][0] + 2);
      }

    if(key == '3')
      {
        mode = 2;
        for(std::size_t i = 0; i < drawers.size(); ++i)
          {
            double mindif = areas[i][0] - areas[i][1], maxdif = areas[i][0] - areas[i][1];
            for(std::size_t j = 0; j < areas[i].size() - 1; ++j)
              {
                if(mindif > areas[i][j] - areas[i][j + 1])
                  mindif = areas[i][j] - areas[i][j + 1];
                if(maxdif < areas[i][j] - areas[i][j + 1])
                  maxdif = areas[i][j] - areas[i][j + 1];
              }
            drawers[i].SetViewport(-1, areas[i].size(), mindif - 1, maxdif + 1);
          }
      }
    glutPostRedisplay();
  }

void main(int argc, char *argv[])
  {
    std::cout << "Enter sample size: ";
    int N;
    std::cin >> N;
    srand((int)time(0));
    if(N <= 20)
      N = 20;
    GetSample(points, N);
    minx = points[0](0, 0);
    maxx =  points[0](0, 0);
    miny = points[0](1, 0);
    maxy =  points[0](1, 0);
    for(std::size_t i = 1; i < points.size(); ++i)
      {
        if(points[i](0, 0) < minx)
          minx = points[i](0, 0);
        if(points[i](0, 0) > maxx)
          maxx = points[i](0, 0);
        if(points[i](1, 0) < miny)
          miny = points[i](1, 0);
        if(points[i](1, 0) > maxy)
          maxy = points[i](1, 0);
      }
    for(std::size_t i = 0; i < drawers.size(); ++i)
      drawers[i].SetViewport(minx - 1, maxx + 1, miny - 1, maxy + 1);
    std::cout << "Calculating convex levels...\n";
    BuildConvexLevels(convex_levels, areas[0], centres[0], points);
    std::cout << "Calculating Khachiyan levels...\n";
    BuildEllipses(khachiyan_ellipses, khachiyan_centres, areas[1], centres[1], points, KhachiyanEllipse);
    std::cout << "Calculating affine levels...\n";
    BuildEllipses(affine_ellipses, affine_centres, areas[2], centres[2], points, AffineEllipse);
    khachiyan_borders.resize(khachiyan_ellipses.size());
    affine_borders.resize(affine_ellipses.size());
    std::cout << "Building borders for ellipses...\n";
    for(std::size_t i = 0; i < khachiyan_borders.size(); ++i)
      CalcEllipseBorder(khachiyan_borders[i], khachiyan_ellipses[i], khachiyan_centres[i]);
    for(std::size_t i = 0; i < affine_borders.size(); ++i)
      CalcEllipseBorder(affine_borders[i], affine_ellipses[i], affine_centres[i]);
    std::cout << "Done.\n";
    std::cout << "Convex centre:\n";
    std::cout << centres[0].X() << " " << centres[0].Y() << '\n';
    std::cout << "Khachiyan centre:\n";
    std::cout << centres[1].X() << " " << centres[1].Y() << '\n';
    std::cout << "Affine centre:\n";
    std::cout << centres[2].X() << " " << centres[2].Y() << '\n';
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
