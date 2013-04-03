#pragma once
#include <vector>
struct coefs
  {
  double a2, a1, a0, b, t;
  };
std::vector<std::vector<double>> Makematrix(const std::vector<coefs>& i_coefs);
//double Derivative(double x, double s, double (* _func)(double, double));
//double SecondDerivative(double x, double s, double (* _func)(double, double));