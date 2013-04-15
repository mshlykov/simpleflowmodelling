#include "mathaux.h"
#include <cmath>

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

double G1(double x, double s)
  {
  double Pi = 3.14159;
  return 1.0 / (4.0 * Pi * std::abs(x - s));
  }