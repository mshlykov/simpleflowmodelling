#define _USE_MATH_DEFINES

#include"Matrix.h"
#include"RandNums.h"
#include<cmath>
#include<vector>
void GetSample(std::vector<MatrSpace::Matrix>& o_points, std::size_t sample_size)
  {
  std::size_t K1 = sample_size;
  o_points.resize(K1, MatrSpace::Matrix(2, 1));
  double x, y, 
    shift_x1 = 2, shift_y1 = 2 , shift_x2 = -2, shift_y2 = -2, ro = 0.5;
  for(std::size_t i = 0; i < K1; ++i)
    {
    x = rand() / (double)RAND_MAX;
    y = rand() / (double)RAND_MAX;

    o_points[i](0, 0) = cos(2 * M_PI * x) * sqrt(-2 * log(y));
    o_points[i](1, 0) = sin(2 * M_PI * x) * sqrt(-2 * log(y));

    }
  }