#include "stdafx.h"
#include "Model.h"
#include "Matrix.h"
std::vector<double> Model::curr_gamma;

Model model;

Model::Model()
  {
  Init();
  }

//-------------------------------------

void Model::Init()
  {
  int N = 31;
  m_contours.resize(2);
  for(int i = 0; i < N; ++i)
    m_contours[0].push_back(Vector2D(-0.5, 0.5 - double(i) / (N - 1)));
  for(int i = 1; i < N; ++i)
    m_contours[0].push_back(Vector2D(-0.5 + double(i) / (N - 1), -0.5));
  for(int i = 1; i < N; ++i)
    m_contours[0].push_back(Vector2D(0.5, -0.5 + double(i) / (N - 1)));
  for(int i = 0; i < N; ++i)
    m_contours[1].push_back(Vector2D(0, 0.5 - double(i) / (N - 1)));
  m_delta = (m_contours[0][1] - m_contours[0][0]).Length();
  for(std::size_t i = 0; i < m_contours.size(); ++i)
    for(std::size_t j = 0; j < m_contours[i].size() - 1; ++j)
      {
      m_colloc.push_back(0.5 * (m_contours[i][j] + m_contours[i][j + 1]));
      if((m_contours[i][j + 1] - m_contours[i][j]).Length() < m_delta)
        m_delta = (m_contours[i][j + 1] - m_contours[i][j]).Length();
      m_normals.push_back((m_contours[i][j + 1] - m_contours[i][j]).Normalize().GetOrthogonal());
      }
  m_delta *= 0.49;
  m_contours[1].pop_back();
  }

//-------------------------------------

const Contours& Model::GetContours() const
  {
  return m_contours;
  }

//-------------------------------------

void Model::CalcGamma(std::vector<double>& o_gamma, const Vector2D& i_velocity, double i_gamma)
  {
  if(!o_gamma.empty())
    o_gamma.clear();

  m_velocity = i_velocity;
  m_gamma = i_gamma;  
  
  int M = m_colloc.size() + 1;  
  MatrSpace::Matrix A(M, M), b(M, 1);
  
  for(int i = 0; i < M - 1; ++i)
    {
    b(i, 0) = m_velocity * m_normals[i] * (-1);
    
    A(M - 1, i) = 1;

    for(int j = 0; j < M; ++j)
      A(i, j) = V(j, m_colloc[i]) * m_normals[i];
    }
  
  A(M - 1, M - 1) = 1;
  b(M - 1, 0) = m_gamma;

  b = A.Inverse() * b;

  o_gamma.resize(M);
  for(std::size_t i = 0; i < o_gamma.size(); ++i)
    o_gamma[i] = b(i, 0);
  }

//-------------------------------------

Vector2D Model::V(int i_idx, const Vector2D& i_vect) const
  {
  int summ = 0;
  int i = 0;
  for(; summ < i_idx + 1; ++i)
    summ += m_contours[i].size();
  summ -= m_contours[i - 1].size();

  Vector2D a = (i_vect - m_contours[i - 1][i_idx - summ]).GetOrthogonal();
  double len = a.Length();
  if(len > m_delta)
    a = (1.0 / (2 * len * len * System::Math::PI)) * a;
  else
    a = (1.0 / (2 * m_delta * m_delta * System::Math::PI)) * a;
  return a;
  }

//-------------------------------------

const Vector2D& Model::GetVelocity() const
  {
  return m_velocity;
  }

//-------------------------------------

int Model::GetSize() const
  {
  int summ = 0;
  for(std::size_t i = 0; i < m_contours.size(); ++i)
    summ += m_contours[i].size();
  return summ;
  }