#include<cmath>
#include<vector>
#include"Matrix.h"
void GetSample(std::vector<MatrSpace::Matrix>& o_points, int sample_size)
  {
  int K1 = sample_size;
  o_points.resize(K1, MatrSpace::Matrix(2, 1));
  double x, y, PI = 3.14159, 
    shift_x1 = 2, shift_y1 = 2 , shift_x2 = -2, shift_y2 = -2, ro = 0.5;
  for(int i = 0; i < K1; ++i)
    {
    x = rand() / (double)RAND_MAX;
    y = rand() / (double)RAND_MAX;

    o_points[i](0, 0) = cos(2 * PI * x) * sqrt(-2 * log(y));
    o_points[i](1, 0) = sin(2 * PI * x) * sqrt(-2 * log(y));

    }
  }