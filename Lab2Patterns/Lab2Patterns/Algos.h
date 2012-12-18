#pragma once

#include "Matrix.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <vector>

MatrSpace::Matrix ConvToMatr(const boost::numeric::ublas::matrix<double>& i_matrix);
MatrSpace::Matrix ConvToMatr(const boost::numeric::ublas::vector<double>& i_vector);
void CentroidEllipse(MatrSpace::Matrix& Q, MatrSpace::Matrix& o_centre, const MatrSpace::Matrix& i_ellipsematr, const MatrSpace::Matrix& centre, std::vector<MatrSpace::Matrix> points);