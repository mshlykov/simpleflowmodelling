#include "stdafx.h"
#include "Model.h"
#include "Matrix.h"

std::vector<std::vector<int>> Model::colors_by_phi;

using System::Math;
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
  m_dt = 0;
  m_contours[1].pop_back();

  m_corners.push_back(m_contours[0][0]);
  m_corners.push_back(m_contours[0][30]);
  m_corners.push_back(m_contours[0][60]);
  m_corners.push_back(m_contours[0][90]);
  m_corners.push_back(m_contours[1][0]);
  m_prev_gamma.resize(m_colloc.size() + 1);
  m_curr_gamma.resize(m_colloc.size() + 1);
  m_off_gamma.resize(5);
  m_off_points.resize(5);
  }

//-------------------------------------

void Model::ReInit()
  {
    for(std::size_t i = 0; i < model.m_off_points.size(); ++i)
      {
      model.m_off_points[i].clear();
      model.m_off_gamma[i].clear();
      }

    for(std::size_t i = 0; i < model.m_prev_gamma.size(); ++i)
      m_prev_gamma[i] = 0;
    m_dt = 0;
  }

//-------------------------------------

const Contours& Model::GetContours() const
  {
  return m_contours;
  }

//-------------------------------------

double Model::GetGamma(int i_i, int i_j) const
  {
  int summ = 0;
  for(int i = 0; i < i_i; ++i)
    summ += m_contours[i].size();
  return m_curr_gamma[summ + i_j];
  }

//-------------------------------------

void Model::CalcGamma()
  {
    /*if(!m_curr_gamma.empty())
      m_curr_gamma.clear();*/

    int M = m_colloc.size() + 1;  
    MatrSpace::Matrix A(M, M), b(M, 1);
  
    for(int i = 0; i < M - 1; ++i)
      {
      b(i, 0) = m_velocity * m_normals[i] * (-1);
    
      for(std::size_t j = 0; j < m_off_points.size(); ++j)
        for(std::size_t k = 0; k < m_off_points[j].size(); ++k)
          b(i, 0) -= m_off_gamma[j][k] * V(m_colloc[i], m_off_points[j][k]) * m_normals[i];

      A(M - 1, i) = 1;

      for(int j = 0; j < M; ++j)
        A(i, j) = V(j, m_colloc[i]) * m_normals[i];
      }
  
    A(M - 1, M - 1) = 1;
    b(M - 1 , 0) = m_gamma;
    for(std::size_t j = 0; j < m_off_points.size(); ++j)
      for(std::size_t k = 0; k < m_off_points[j].size(); ++k)
        b(M - 1, 0) -= m_off_gamma[j][k];
    b = A.SolveGauss(b);
    //m_curr_gamma.resize(M);
    for(std::size_t i = 0; i < m_curr_gamma.size(); ++i)
      {
      m_prev_gamma[i] = m_curr_gamma[i];
      m_curr_gamma[i] = b(i, 0);
      }
  }

//-------------------------------------

double Model::CalcPhi(const Vector2D& i_point) const
  {
    double summ = 0;
    for(std::size_t i = 0, curr_idx = 0; i < m_contours.size(); ++i)
      {

      for(std::size_t j = 0; j < m_contours[i].size() - 1; ++j)
        {
        double pointarg = 0, dx = i_point.X() - m_contours[i][j].X(), 
          dy = i_point.Y() - m_contours[i][j].Y();
        if(! (Math::Abs(dx) < 0.00001))
          {
          if(dx > 0 && dy > 0)
            pointarg = Math::Atan(dy / dx);
          else if(dx < 0)
            pointarg = Math::Atan(dy / dx) + Math::PI;
          else if(dx > 0 && dy < 0)
            pointarg = Math::Atan(dy / dx) + 2 * Math::PI;
          }
        summ += pointarg * m_curr_gamma[curr_idx + j];
        }
      curr_idx += m_contours[i].size();
      }

    for(std::size_t i = 0; i < m_off_points.size(); ++i)
      for(std::size_t j = 0; j < m_off_points[i].size(); ++j)
        {
        double pointarg = 0, dx = i_point.X() - m_off_points[i][j].X(), 
          dy = i_point.Y() - m_off_points[i][j].Y();
        if(! (Math::Abs(dx) < 0.00001))
          {
          if(dx > 0 && dy > 0)
            pointarg = Math::Atan(dy / dx);
          else if(dx < 0)
            pointarg = Math::Atan(dy / dx) + Math::PI;
          else if(dx > 0 && dy < 0)
            pointarg = Math::Atan(dy / dx) + 2 * Math::PI;
          }
        summ += pointarg * m_off_gamma[i][j];
        }

    summ /= (2 * Math::PI);
    summ += i_point.X() * m_velocity.X() + i_point.Y() * m_velocity.Y();
    return summ;
  }

//-------------------------------------

double Model::CalcPsi(const Vector2D& i_point) const
  {
    double summ = 0, delta_star = 0.1 * m_delta;
    for(std::size_t i = 0, curr_idx = 0; i < m_contours.size(); ++i)
      {

      for(std::size_t j = 0; j < m_contours[i].size() - 1; ++j)
        {
        double len = (i_point - m_contours[i][j]).Length2();
        if(len < delta_star * delta_star)
          len = delta_star * delta_star;
        summ -= Math::Log(len) * m_curr_gamma[curr_idx + j];
        }
    
      curr_idx += m_contours[i].size();
      }

    for(std::size_t i = 0; i < m_off_points.size(); ++i)
      for(std::size_t j = 0; j < m_off_points[i].size(); ++j)
        {
        double len = (i_point - m_off_points[i][j]).Length2();
        if(len < delta_star * delta_star)
          len = delta_star * delta_star;
        summ -= Math::Log(len) * m_off_gamma[i][j];
        }

    summ /= (4 * Math::PI);
    summ += i_point.Y() * m_velocity.X() - i_point.X() * m_velocity.Y();
    return summ;
  }

//-------------------------------------

double Model::CalcCp(const Vector2D& i_point) const
  {
    double dphi = 0, Pi = 3.14159;
    Vector2D xstar, diff, orth;
    int s = 0;
    for(std::size_t i = 0; i < m_contours.size(); s += m_contours[i].size(), ++i)
      for(std::size_t j = 0; j < m_contours[i].size() - 1; ++j)
        {
          diff = m_contours[i][j] + m_contours[i][j + 1];
          xstar = 0.5 * diff;
          orth = i_point - diff;
          dphi += (m_curr_gamma[s + j] - m_prev_gamma[s + j]) / (2 * Pi * m_dt) * (orth.GetOrthogonal()* diff) / (orth.Length2());
        }
    for(std::size_t i = 0; i < m_off_points.size(); ++i)
      {
        diff = m_corners[i] - m_off_points[i][m_off_points[i].size() - 1];
        xstar = 0.5 * diff;
        orth = i_point - diff;
        dphi += m_off_gamma[i][m_off_gamma[i].size() - 1] / (2 * Pi * m_dt) * (orth.GetOrthogonal()* diff) / (orth.Length2());
      }
    for(std::size_t i = 0; i < m_off_points.size(); ++i)
      for(std::size_t j = 0; j < m_off_points[i].size(); ++j)
        dphi += m_off_gamma[i][j] * (V(i_point, m_off_points[i][j]) * CalcSpeed(m_off_points[i][j]));
    return  1 - CalcSpeed(i_point).Length2() / 2 - 2 * dphi;

  }

//-------------------------------------

Vector2D Model::CalcSpeed(const Vector2D& i_point) const
  {
    Vector2D res = m_velocity;
    int M = GetSize();
    for(int i = 0; i < M; ++i)
      res = res + m_curr_gamma[i] * V(i, i_point);
    for(std::size_t i = 0; i < m_off_points.size(); ++i)
      for(std::size_t j = 0; j < m_off_points[i].size(); ++j)
        res = res + m_off_gamma[i][j] * V(i_point, m_off_points[i][j]);
    return res;
  }

//-------------------------------------

Vector2D Model::V(const Vector2D& i_vect1, const Vector2D& i_vect2) const
  {
    Vector2D a = (i_vect1 - i_vect2).GetOrthogonal();
    double len = a.Length();
    if(len > m_delta)
      a = (1.0 / (2 * len * len * System::Math::PI)) * a;
    else
      a = (1.0 / (2 * m_delta * m_delta * System::Math::PI)) * a;
    return a;
  }


//-------------------------------------

Vector2D Model::V(int i_idx, const Vector2D& i_vect) const
  {
    int summ = 0;
    int i = 0;
    for(; summ < i_idx + 1; ++i)
      summ += m_contours[i].size();
    summ -= m_contours[i - 1].size();

    return V(i_vect, m_contours[i - 1][i_idx - summ]);
  }

//-------------------------------------

Vector2D Model::GetPoint(int i_idx) const
  {
    int summ = 0;
    int i = 0;
    for(; summ < i_idx + 1; ++i)
      summ += m_contours[i].size();
    summ -= m_contours[i - 1].size();

    return m_contours[i - 1][i_idx - summ];
  }

//-------------------------------------

void Model::UpdatePoints()
  {
    m_dt  = 1. / CalcSpeed(m_contours[0][0]).Length2();

    if(m_dt > 1. / CalcSpeed(m_contours[0][30]).Length2())
      m_dt = 1. / CalcSpeed(m_contours[0][30]).Length2();

    if(m_dt > 1. / CalcSpeed(m_contours[0][60]).Length2())
      m_dt = 1. / CalcSpeed(m_contours[0][60]).Length2();
  
    if(m_dt > 1. / CalcSpeed(m_contours[0][90]).Length2())
      m_dt = 1. / CalcSpeed(m_contours[0][90]).Length2();
  
    if(m_dt > 1. / CalcSpeed(m_contours[1][0]).Length2())
      m_dt = 1. / CalcSpeed(m_contours[1][0]).Length2();

    double D = 2 * m_delta;
    m_dt = std::sqrt(m_dt) * D;
    std::vector<std::vector<Vector2D>> new_points(m_off_points.size());
    for(std::size_t i = 0; i < m_off_points.size(); ++i)
      {
        for(std::size_t j = 0; j < m_off_points[i].size(); ++j)
          {
          new_points[i].push_back(m_off_points[i][j] + m_dt * CalcSpeed(m_off_points[i][j]));
      
          if(std::abs(new_points[i][j].Y() + 0.5) < D && new_points[i][j].X() > -0.5 && new_points[i][j].X() < 0.5)
            {
            if(m_off_points[i][j].Y() < -0.5)
              new_points[i][j].Y() = -0.5 - D;
            else
              new_points[i][j].Y() = -0.5 + D;

            }
      
          if(std::abs(new_points[i][j].X() + 0.5) < D && new_points[i][j].Y() > -0.5  && new_points[i][j].Y() < 0.5)
            {
            if(m_off_points[i][j].X() < -0.5)
              new_points[i][j].X() = -0.5 - D;
            else
              new_points[i][j].X() = -0.5 + D;
            }

          if(std::abs(new_points[i][j].X()) < D && new_points[i][j].Y() > -0.5 && new_points[i][j].Y() < 0.5)
            {
            if(m_off_points[i][j].X() < 0)
              new_points[i][j].X() = -D;
            else
              new_points[i][j].X() = D;
            }

          if(std::abs(new_points[i][j].X() - 0.5) < D && new_points[i][j].Y() > -0.5 && new_points[i][j].Y() < 0.5)
            {
            if(m_off_points[i][j].X() < 0.5)
              new_points[i][j].X() = 0.5 - D;
            else
              new_points[i][j].X() = 0.5 + D;
            }
          }
      }

    new_points[0].push_back(m_contours[0][0] + m_dt * CalcSpeed(m_contours[0][0]));
    new_points[1].push_back(m_contours[0][30] + m_dt * CalcSpeed(m_contours[0][30]));
    new_points[2].push_back(m_contours[0][60] + m_dt * CalcSpeed(m_contours[0][60]));
    new_points[3].push_back(m_contours[0][90] + m_dt * CalcSpeed(m_contours[0][90]));
    new_points[4].push_back(m_contours[1][0] + m_dt * CalcSpeed(m_contours[1][0]));
    m_off_gamma[0].push_back(m_curr_gamma[0]);
    m_off_gamma[1].push_back(m_curr_gamma[30]);
    m_off_gamma[2].push_back(m_curr_gamma[60]);
    m_off_gamma[3].push_back(m_curr_gamma[90]);
    m_off_gamma[4].push_back(m_curr_gamma[91]);

    std::swap(m_off_points, new_points);
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

//-------------------------------------

const std::vector<std::vector<Vector2D>>& Model::GetOffPoints() const
  {
    return m_off_points;
  }

//-------------------------------------

const std::vector<std::vector<double>>& Model::GetOffGamma() const
  {
    return m_off_gamma;
  }

//-------------------------------------

const std::vector<double>& Model::GetCurrGamma() const
  {
    return m_curr_gamma;
  }

//-------------------------------------

void Model::SetParams(const Vector2D& i_velocity, double i_gamma)
  {
    m_velocity = i_velocity;
    m_gamma = i_gamma;
  }