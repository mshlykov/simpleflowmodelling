#include "mathaux.h"
#include <cmath>
#include <vector>
double a = 0, b = 1, a1 = a - 1, b1 = b + 1;

struct coefs
  {
  double a2, a1, a0, b, t;
  };


double Derivative(double x, double s, double (* _func)(double, double))
  {
  double prec = 0.0001;
  return (_func(x + prec, s) - _func(x - prec, s)) / (2 * prec);
  }

double SecondDerivative(double x, double s, double (* _func)(double, double))
  {
  double prec = 0.0001;
  return (_func(x + prec, s) - 2 * _func(x, s) + _func(x - prec, s)) / (prec * prec);
  }

double G(double x, double s)
  {
  double Pi = 3.14159;
  return 1.0 / (4.0 * Pi * std::abs(x - s));
  }

std::vector<std::vector<double>> Makematrix(const std::vector<coefs>& i_coefs)
  {
  std::size_t N1 = 0, N2 = 20, N3 = 0;
  double eps = 0.00001;
  std::vector<std::vector<double>> result;
  for(std::size_t i = 0; i < i_coefs.size() && (!N1 || !N3); ++i)
    {
    if(std::abs(i_coefs[i].t - a) <  eps)
      N1 = 20;
    if(std::abs(i_coefs[i].t - b) <  eps)
      N3 = 20;
    }

  result.resize(i_coefs.size());
  for(std::size_t i = 0; i < i_coefs.size(); ++i)
    result[i].resize(N1 + N2 + N3 + 1);
  double h1 = N1 > 1? (a - a1) / (N1 - 1) : 0,
  h2 = N2 > 1? (b - a) / (N2 - 1) : 0,
  h3 = N3 > 1? (b1 - b) / (N3 - 1) : 0;
  for(std::size_t i = 0; i < i_coefs.size(); ++i)
    {
    for(std::size_t j = 0; j < N1; ++j)
      {
      result[i][j] = i_coefs[i].a2 * SecondDerivative(i_coefs[i].t, a1 + j * h1, G)
        + i_coefs[i].a1 * Derivative(i_coefs[i].t, a1 + j * h1, G)
        + i_coefs[i].a0 * G(i_coefs[i].t, a1 + j * h1);
      }

    for(std::size_t j = 0; j < N2; ++j)
      {
      result[i][j + N1] = i_coefs[i].a2 * SecondDerivative(i_coefs[i].t, a + j * h2, G)
        + i_coefs[i].a1 * Derivative(i_coefs[i].t, a + j * h2, G)
        + i_coefs[i].a0 * G(i_coefs[i].t, a + j * h2);
      }

    for(std::size_t j = 0; j < N3; ++j)
      {
      result[i][j + N1 + N2] = i_coefs[i].a2 * SecondDerivative(i_coefs[i].t, b + j * h3, G)
        + i_coefs[i].a1 * Derivative(i_coefs[i].t, b + j * h2, G)
        + i_coefs[i].a0 * G(i_coefs[i].t, b + j * h2);
      }

    result[i][N1 + N2 + N3 - 1] = i_coefs[i].b;
    }
  return result;
  }

