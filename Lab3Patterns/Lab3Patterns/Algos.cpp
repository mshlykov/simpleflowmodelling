#define _USE_MATH_DEFINES
#include "Algos.h"
#include "ellipsoids.hpp"
#include "Matrix.h"
#include <numeric>
#include <cmath>
//#include <math.h>

MatrSpace::Matrix ConvToMatr(const boost::numeric::ublas::matrix<double>& i_matrix)
  {
    MatrSpace::Matrix res(i_matrix.size1(), i_matrix.size2());
    for(std::size_t i = 0; i < res.GetRsize(); ++i)
      for(std::size_t j = 0; j < res.GetCsize(); ++j)
        res(i, j) = i_matrix(i, j);
    return res;
  }

MatrSpace::Matrix ConvToMatr(const boost::numeric::ublas::vector<double>& i_vector)
  {
    MatrSpace::Matrix res(i_vector.size(), 1);
    for(std::size_t i = 0; i < res.GetRsize(); ++i)
      res(i, 0) = i_vector(i);
    return res;
  }

void CentroidEllipse(MatrSpace::Matrix& Q, MatrSpace::Matrix& o_centre, const MatrSpace::Matrix& i_ellipsematr, const MatrSpace::Matrix& centre, std::vector<MatrSpace::Matrix> points)
  {
    double phi = 0.5 * std::atan(2 * i_ellipsematr(0, 1) / (i_ellipsematr(1, 1) - i_ellipsematr(0, 0)));
    
    if(std::abs(i_ellipsematr(1, 1) - i_ellipsematr(0, 0)) < 0.0001)
      {
        if(i_ellipsematr(0, 1) > 0)
          phi = M_PI / 2;
        else
          phi = - M_PI / 2;
      }

    MatrSpace::Matrix rot(2, 2), transprot(2, 2), scale(2, 2), invscale(2, 2), transf(2, 2), invtransf(2, 2);
  
    transprot(0, 0) = transprot(1, 1) = rot(1, 1) = rot(0, 0) = std::cos(phi);
    transprot(1, 0) = rot(0, 1) = std::sin(phi);
    transprot(0, 1) = rot(1, 0) = -rot(0, 1);
  
    Q = transprot * i_ellipsematr * rot;
  
    scale(0, 0) = 1 / std::sqrt(Q(0, 0));
    scale(1, 1) = 1 / std::sqrt(Q(1, 1));
    invscale(0, 0) = 1 / scale(0,0);
    invscale(1, 1) = 1 / scale(1,1);
  
    invtransf = invscale * transprot;
    MatrSpace::Matrix centroid(2,1);
    for(std::size_t i = 0; i < points.size(); ++i)
      {
        points[i] = points[i] - centre;
        points[i] = invtransf * points[i];
        centroid = centroid + points[i];
      }
    centroid = (1.0 / static_cast<double>(points.size())) * centroid;
    double rad = 0;
    rad = (points[0] - centroid).SqEuclNorm();
  
    for(std::size_t i = 1; i < points.size(); ++i)
      {
        double newrad = (points[i] - centroid).SqEuclNorm();
        if(rad < newrad)
          rad = newrad;
      }
  
    Q(0, 0) = Q(1, 1) = 1 / rad;
    Q(0, 1) = Q(1, 0) = 0;  
    Q = rot * invscale * Q * invscale * transprot;
    o_centre = rot * scale * centroid;
    o_centre = o_centre + centre;
  }

void AffineEllipse(MatrSpace::Matrix& Q, MatrSpace::Matrix& o_centre, std::vector<MatrSpace::Matrix> points)
  {
    if(points.size() == 2)
      {
        o_centre = 0.5 * (points[0] + points[1]);
        Q.Resize(2, 2);
        Q(0, 0) = Q(1, 1) = 4.0 / (points[0] - points[1]).SqEuclNorm();
        return;
      }
    std::size_t idx1, idx2;
    double dist = (points[0] - points[1]).SqEuclNorm();
    idx1 = 0;
    idx2 = 1;
    for(std::size_t i = 0; i < points.size(); ++i)
      for(std::size_t j = i + 1; j < points.size(); ++j)
        if(dist < (points[i] - points[j]).SqEuclNorm())
          {
            idx1 = i;
            idx2 = j;
            dist = (points[i] - points[j]).SqEuclNorm();
          }

    dist = std::sqrt(dist);

    MatrSpace::Matrix vect = points[idx1] - points[idx2];

    double dev1 = 0, dev2 = 0, 
      a = - vect(1, 0),
      b = vect(0, 0),
      c = - a * points[idx1](0, 0) - b * points[idx1](1, 0);

    for(std::size_t i = 0; i < points.size(); ++i)
      {
        if(dev1 > a * points[i](0, 0) + b * points[i](1, 0) + c)
          dev1 = a * points[i](0, 0) + b * points[i](1, 0) + c;
        
        if(dev2 < a * points[i](0, 0) + b * points[i](1, 0) + c)
          dev2 = a * points[i](0, 0) + b * points[i](1, 0) + c;
      }

    double dist1 = (std::abs(dev1) + std::abs(dev2)) / std::sqrt(a * a + b * b), phi = atan(vect(1, 0) / vect(0, 0));

    if(vect(0, 0) < 0)
      phi += M_PI;
    else if(vect(1, 0) < 0)
      phi += 2 * M_PI;

    if(std::abs(vect(0, 0)) < 0.0001)
      {
        if(vect(1, 0) > 0)
          phi = M_PI / 2;
        else
          phi = - M_PI / 2;
      }

    MatrSpace::Matrix rot(2, 2), invrot(2, 2), scale(2, 2), invscale(2, 2), invtransf;

    invrot(0, 0) = invrot(1, 1) = rot(1, 1) = rot(0, 0) = std::cos(phi);
    invrot(1, 0) = rot(0, 1) = -std::sin(phi);
    invrot(0, 1) = rot(1, 0) = -rot(0, 1);
    
    scale(0, 0) = dist;
    scale(1, 1) = dist1;
    invscale(0, 0) = 1 / scale(0, 0);
    invscale(1, 1) = 1 / scale(1, 1);
    invtransf = invscale * invrot;
    MatrSpace::Matrix translate_vector = points[idx2], centroid(2, 1); 

    for(std::size_t i = 0; i < points.size(); ++i)
      {
        points[i] = points[i] - translate_vector;
        points[i] = invtransf * points[i];
        centroid = centroid + points[i];
      }
    centroid = (1.0 / static_cast<double>(points.size())) * centroid;

    double rad = 0;
    rad = (points[0] - centroid).SqEuclNorm();

    for(std::size_t i = 1; i < points.size(); ++i)
      {
        double newrad = (points[i] - centroid).SqEuclNorm();
        if(rad < newrad)
          rad = newrad;
      }

    Q.Resize(2, 2);
    Q(0, 0) = Q(1, 1) = 1 / rad;
    Q(0, 1) = Q(1, 0) = 0;  
    
    Q = rot * invscale * Q * invscale * invrot;

    o_centre = rot * scale * centroid;
    o_centre = o_centre + translate_vector;
  }

void KhachiyanEllipse(MatrSpace::Matrix& Q, MatrSpace::Matrix& o_centre, std::vector<MatrSpace::Matrix> points)
  {
    boost::numeric::ublas::vector<double> centre(2);
    boost::numeric::ublas::matrix<double> points1, Q1(2, 2);
    points1.resize(2,points.size());
    for(std::size_t i = 0; i < points1.size2(); ++i)
      {
        points1(0,i) = points[i](0, 0);
        points1(1,i) = points[i](1, 0);
      }

    Minim::KhachiyanAlgo(points1, 0.000001, 100, Q1, centre);
    Q = ConvToMatr(Q1);
    o_centre = ConvToMatr(centre);
  }

double VectProd(const MatrSpace::Matrix& i_matr1, const MatrSpace::Matrix& i_matr2)
  {
    return i_matr1(0, 0) * i_matr2(1, 0) - i_matr2(0, 0) * i_matr1(1, 0);
  }

void GrahamScan(std::vector<MatrSpace::Matrix>& o_result, std::vector<MatrSpace::Matrix>& i_points)
  {
    if(i_points.size() <= 3)
      {
        o_result = i_points;
        return;
      }
    double minx = i_points[0](0, 0);
    std::size_t jmin = 0;
    for(std::size_t i = 0; i < i_points.size(); ++i)
      {
        if(i_points[i](1, 0) < i_points[jmin](1, 0))
          jmin = i;
      }
    std::swap(i_points[0], i_points[jmin]);
    std::sort(i_points.begin() + 1, i_points.end(), [&i_points](const MatrSpace::Matrix& i_matr1, const MatrSpace::Matrix& i_matr2)->bool
      {
        MatrSpace::Matrix m1 = i_matr1 - i_points[0], m2 = i_matr2 - i_points[0];
        double acos1 = acos(m1(0, 0) / sqrt(m1(0, 0) * m1(0, 0) + m1(1, 0) * m1(1, 0))),
          acos2 = acos(m2(0, 0) / sqrt(m2(0, 0) * m2(0, 0) + m2(1, 0) * m2(1, 0)));
        return acos1 < acos2;
      });
    o_result.push_back(i_points[0]);
    o_result.push_back(i_points[1]);
    for(std::size_t i = 2; i < i_points.size(); ++i)
      {
        while(VectProd(o_result[o_result.size() - 1] - o_result[o_result.size() - 2] , i_points[i] - o_result[o_result.size() - 1]) < -0.00001)
          o_result.pop_back();
        o_result.push_back(i_points[i]);
      }
  }

double ConvexVol(const std::vector<MatrSpace::Matrix>& i_points)
  {
    double s = 0;
    for(std::size_t i = 1; i < i_points.size() - 1; ++i)
      s += std::abs(0.5 * VectProd(i_points[i] - i_points[0], i_points[i + 1] - i_points[0]));
    return s;
  }

double EllipseVol(const MatrSpace::Matrix& i_ellipse_matr)
  {
  double vol = 4.0 / 3.0 * M_PI / std::sqrt(i_ellipse_matr(0, 0) * i_ellipse_matr(1, 1) - i_ellipse_matr(1, 0) * i_ellipse_matr(0, 1));
  return vol;
  }