#pragma once
#include <vector>
struct coefs
  {
    double a2, a1, a0, b, t;
  };

class Analyzer
  {
  public:
    Analyzer(double i_a = 0, double i_b = 1);
    void SetG(double (*i_G)(double x, double s));
    void Makematrix(const std::vector<coefs>& i_coefs);
    const std::vector<std::vector<double>>& GetMatrix();
  
  private:
    double m_a, m_b, 
      m_a1, m_b1;
    std::size_t m_N1, m_N2, m_N3;
    std::vector<std::vector<double>> m_linsys;
    double (*m_G)(double x, double s);
  };