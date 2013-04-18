#include "Analyzer.h"
#include "mathaux.h"
#include <cmath>
#include <vector>


Analyzer::Analyzer(double i_a, double i_b):m_a(i_a), m_b(i_b)
  {
    m_G = G1;
    m_a1 = m_a - 1;
    m_b1 = m_b + 1;
  }

void Analyzer::SetG(double (*i_G)(double x, double s))
  {
    m_G = i_G;
  }


void Analyzer::Makematrix(const std::vector<coefs>& i_coefs)
  {
    double eps = 0.00001;

    for(std::size_t i = 0; i < i_coefs.size() && (!m_N1 || !m_N3); ++i)
      {
        if(std::abs(i_coefs[i].t - m_a) <  eps)
          m_N1 = 20;
        if(std::abs(i_coefs[i].t - m_b) <  eps)
          m_N3 = 20;
      }

    m_linsys.resize(i_coefs.size());
    for(std::size_t i = 0; i < i_coefs.size(); ++i)
      m_linsys[i].resize(m_N1 + m_N2 + m_N3 + 1);
    double h1 = m_N1 > 1? (m_a - m_a1) / (m_N1 - 1) : 0,
    h2 = m_N2 > 1? (m_b - m_a) / (m_N2 - 1) : 0,
    h3 = m_N3 > 1? (m_b1 - m_b) / (m_N3 - 1) : 0;
    for(std::size_t i = 0; i < i_coefs.size(); ++i)
      {
      for(std::size_t j = 0; j < m_N1; ++j)
        {
          m_linsys[i][j] = i_coefs[i].a2 * SecondDerivative(i_coefs[i].t, m_a1 + j * h1, m_G)
            + i_coefs[i].a1 * Derivative(i_coefs[i].t, m_a1 + j * h1, m_G)
            + i_coefs[i].a0 * m_G(i_coefs[i].t, m_a1 + j * h1);
        }

      for(std::size_t j = 0; j < m_N2; ++j)
        {
          m_linsys[i][j + m_N1] = i_coefs[i].a2 * SecondDerivative(i_coefs[i].t, m_a + j * h2, m_G)
            + i_coefs[i].a1 * Derivative(i_coefs[i].t, m_a + j * h2, m_G)
            + i_coefs[i].a0 * m_G(i_coefs[i].t, m_a + j * h2);
        }

      for(std::size_t j = 0; j < m_N3; ++j)
        {
          m_linsys[i][j + m_N1 + m_N2] = i_coefs[i].a2 * SecondDerivative(i_coefs[i].t, m_b + j * h3, m_G)
            + i_coefs[i].a1 * Derivative(i_coefs[i].t, m_b + j * h2, m_G)
            + i_coefs[i].a0 * m_G(i_coefs[i].t, m_b + j * h2);
        }

      m_linsys[i][m_N1 + m_N2 + m_N3 - 1] = i_coefs[i].b;
    }
  }

const std::vector<std::vector<double>>& Analyzer::GetMatrix() const
  {
    return m_linsys;
  }

