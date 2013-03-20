#include<iostream>

double Derivative(double x, double (* _func)(double))
  {
    double prec = 0.0001;
    return (_func(x + prec) - _func(x - prec)) / (2 * h);
  }

double SecondDerivative(double x, double (* _func)(double))
  {
  double prec = 0.0001;
  return (_func(x + prec) - 2 * _func(x) + _func(x - prec)) / (h * h);
  }


int main()
  {
  return 0;
  }