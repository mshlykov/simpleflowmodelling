#include<iostream>
#include<cmath>
#include<ctime>
#include<fstream>
#include<vector>
#include"Vector2D.h"
void GetSample(std::vector<Vector2D> o_points)
  {
  o_points.clear();
  int K1 = 40;
  double x, y, nx, ny, PI = 3.14159, 
    shift_x1 = 2, shift_y1 = 2 , shift_x2 = -2, shift_y2 = -2, ro = 0.5;
  for(int i = 0; i < K1; ++i)
    {
    x = rand() / (double)RAND_MAX;
    y = rand() / (double)RAND_MAX;

    nx = cos(2 * PI * x) * sqrt(-2 * log(y)) + shift_x1;
    ny = sin(2 * PI * x) * sqrt(-2 * log(y)) + shift_y1;

    o_points.push_back(Vector2D(nx, ny));
    }
  }