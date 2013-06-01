#pragma once

#include "Matrix.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <vector>

MatrSpace::Matrix ConvToMatr(const boost::numeric::ublas::matrix<double>& i_matrix);
MatrSpace::Matrix ConvToMatr(const boost::numeric::ublas::vector<double>& i_vector);
void CentroidEllipse(MatrSpace::Matrix& Q, MatrSpace::Matrix& o_centre, const MatrSpace::Matrix& i_ellipsematr, const MatrSpace::Matrix& centre, std::vector<MatrSpace::Matrix> points);
void AffineEllipse(MatrSpace::Matrix& Q, MatrSpace::Matrix& o_centre, std::vector<MatrSpace::Matrix> points);
void KhachiyanEllipse(MatrSpace::Matrix& Q, MatrSpace::Matrix& o_centre, std::vector<MatrSpace::Matrix> points);
void GrahamScan(std::vector<MatrSpace::Matrix>& o_result, std::vector<MatrSpace::Matrix>& i_points);
double ConvexVol(const std::vector<MatrSpace::Matrix>& i_points);
double EllipseVol(const MatrSpace::Matrix& i_ellipse_matr);