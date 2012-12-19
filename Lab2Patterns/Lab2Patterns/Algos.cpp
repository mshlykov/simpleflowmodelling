#include "Algos.h"
#include <cmath>

MatrSpace::Matrix ConvToMatr(const boost::numeric::ublas::matrix<double>& i_matrix)
  {
  MatrSpace::Matrix res(i_matrix.size1(), i_matrix.size2());
  for(int i = 0; i < res.GetRsize(); ++i)
    for(int j = 0; j < res.GetCsize(); ++j)
      res(i, j) = i_matrix(i, j);
  return res;
  }
MatrSpace::Matrix ConvToMatr(const boost::numeric::ublas::vector<double>& i_vector)
  {
  MatrSpace::Matrix res(i_vector.size(), 1);
  for(int i = 0; i < res.GetRsize(); ++i)
    res(i, 0) = i_vector(i);
  return res;
  }

void CentroidEllipse(MatrSpace::Matrix& Q, MatrSpace::Matrix& o_centre, const MatrSpace::Matrix& i_ellipsematr, const MatrSpace::Matrix& centre, std::vector<MatrSpace::Matrix> points)
  {
  double phi = 0.5 * std::atan(2 * i_ellipsematr(0, 1) / (i_ellipsematr(1, 1) - i_ellipsematr(0, 0)));
  
  MatrSpace::Matrix rot(2, 2), transprot(2, 2), scale(2, 2), invscale(2, 2), transf(2, 2), invtransf(2, 2);
  
  transprot(0, 0) = transprot(1, 1) = rot(1, 1) = rot(0, 0) = std::cos(phi);
  transprot(1, 0) = rot(0, 1) = std::sin(phi);
  transprot(0, 1) = rot(1, 0) = -rot(0, 1);
  
  Q = transprot * i_ellipsematr * rot;
  
  scale(0, 0) = 1 / std::sqrt(Q(0, 0));
  scale(1, 1) = 1 / std::sqrt(Q(1, 1));
  invscale(0, 0) = 1 / scale(0,0);
  invscale(1, 1) = 1 / scale(1,1);
  
  Q = scale * Q * scale;
  std::cout << Q;
  transf = rot * scale;
  invtransf = invscale * transprot;
  
  for(int i = 0; i < points.size(); ++i)
    {
    points[i] = points[i] - centre;
    points[i] = invtransf * points[i];
    }
  
  double rad = 0;
  MatrSpace::Matrix centroid(2,1);
  
  for(int i = 0; i < points.size(); ++i)
    centroid = centroid + points[i];
  std::cout<<centroid;
  centroid = (1.0 / points.size()) * centroid;
  std::cout<<centroid;
  rad = (points[0] - centroid).SqEuclNorm();
  
  for(int i = 1; i < points.size(); ++i)
    {
    double newrad = (points[i] - centroid).SqEuclNorm();
    if(rad < newrad)
      rad = newrad;
    }
  
  Q(0, 0) = Q(1, 1) = 1 / rad;
  Q(0, 1) = Q(1, 0) = 0;  
  Q = rot * invscale * Q * invscale * transprot;
  o_centre = transf * centroid;
  o_centre = o_centre + centre;
  std::cout << o_centre;
  }