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

  //for(int i = 0; i < m_curr_gamma.size(); ++i)
    //m_curr_gamma[i] = 0;
  }

//-------------------------------------

void Model::ReInit()
  {
    for(std::size_t i = 0; i < m_off_points.size(); ++i)
      {
      model.m_off_points[i].clear();
      model.m_off_gamma[i].clear();
      }

    for(std::size_t i = 0; i < m_prev_gamma.size(); ++i)
      {
        m_curr_gamma[i] = 0;
        m_prev_gamma[i] = 0;
      }
    m_dt = 0;
  }

//-------------------------------------

const Contours& Model::GetContours() const
  {
  return m_contours;
  }

//-------------------------------------

double Model::GetGamma(std::size_t i_i, std::size_t i_j) const
  {
  std::size_t summ = 0;
  for(std::size_t i = 0; i < i_i; ++i)
    summ += m_contours[i].size();
  return m_curr_gamma[summ + i_j];
  }

//-------------------------------------

void Model::CalcGamma()
  {
    /*if(!m_curr_gamma.empty())
      m_curr_gamma.clear();*/

    std::size_t M = m_colloc.size() + 1;  
    MatrSpace::Matrix A(M, M), b(M, 1);
  
    for(std::size_t i = 0; i < M - 1; ++i)
      {
      b(i, 0) = m_velocity * m_normals[i] * (-1);
    
      for(std::size_t j = 0; j < m_off_points.size(); ++j)
        for(std::size_t k = 0; k < m_off_points[j].size(); ++k)
          b(i, 0) -= m_off_gamma[j][k] * V(m_colloc[i], m_off_points[j][k]) * m_normals[i];

      A(M - 1, i) = 1;

      for(std::size_t j = 0; j < M; ++j)
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

double Model::CalcPhiSec(const Vector2D& i_point) const
  {
    double summ = 0, d, delta_star = 0.45 * m_delta;
    Vector2D xstar, diff, orth;
    
    std::vector<double> Q(m_off_gamma.size());
    for(std::size_t i = 0; i < Q.size(); ++i)
      for(std::size_t j = 0; j < m_off_points[i].size(); ++j)
        {
          Q[i] += m_off_gamma[i][j];
          if(j == m_off_points[i].size() - 1)
            {
            switch(i)
              {
            case 0:
                {
                  diff = m_contours[0][0] - m_off_points[i][j];
                  xstar = 0.5 *(m_off_points[i][j] + m_contours[0][0]);
                  break;
                }
            case 1:
                {
                  diff = m_contours[0][30] - m_off_points[i][j];
                  xstar = 0.5 *(m_off_points[i][j] + m_contours[0][30]);
                  break;
                }
            case 2:
                {
                  diff = m_contours[0][60] - m_off_points[i][j];
                  xstar = 0.5 *(m_off_points[i][j] + m_contours[0][60]);
                  break;
                }
            case 3:
                {
                  diff = m_contours[0][90] - m_off_points[i][j];
                  xstar = 0.5 *(m_off_points[i][j] + m_contours[0][90]);
                  break;
                }
            case 4:
                {
                  diff = m_contours[1][0] - m_off_points[i][j];
                  xstar = 0.5 *(m_off_points[i][j] + m_contours[1][0]);
                  break;
                }
              }
            }
          else
            {
              diff = m_off_points[i][j + 1] - m_off_points[i][j];
              xstar = 0.5 * (m_off_points[i][j + 1] + m_off_points[i][j]);
            }
          orth = i_point - xstar;
          d = orth.Length2();
          orth = orth.GetOrthogonal();
          if(d < delta_star * delta_star)
            {
            d = delta_star * delta_star;
            //orth = orth * delta_star;
            }
          summ += Q[i] * (orth * diff) / d;
        }

    for(std::size_t i = 0; i < m_contours[1].size(); ++i)
      {
        Q[4] += m_curr_gamma[91 + i];
        if(i == m_contours[1].size() - 1)
          {
            diff = m_contours[0][45] - m_contours[1][i];
            xstar = 0.5 *(m_contours[0][45] + m_contours[1][i]);
          }
        else
          {
            diff = m_contours[1][i + 1] - m_contours[1][i];
            xstar = 0.5 * (m_contours[1][i + 1] + m_contours[1][i]);
          }
        orth = i_point - xstar;
        d = orth.Length2();
        orth = orth.GetOrthogonal();
        if(d < delta_star * delta_star)
          {
          d = delta_star * delta_star;
          //orth = orth * delta_star;
          }
        summ += Q[4] * (orth * diff) / d;
      }

    //final
    for(std::size_t i = 0; i < m_contours[0].size() - 1; ++i)
      {
      diff = m_contours[0][i + 1] - m_contours[0][i];
      xstar = 0.5 * (m_contours[0][i + 1] + m_contours[0][i]);
      Q[0] += m_curr_gamma[i];
      if(i % 30 == 0 && i != 0)
        Q[0] += Q[i / 30];
      if(i == 45)
        Q[0] += Q[4];
      orth = i_point - xstar;
      d = orth.Length2();
      orth = orth.GetOrthogonal();
      if(d < delta_star * delta_star)
        {
        d = delta_star * delta_star;
        orth = orth * delta_star;
        }
      summ += Q[0] * (orth * diff) / d;
      }

    double tt = m_curr_gamma[90] + Q[3];

    if(m_gamma != 0.0)
      {
        double dx = i_point.X() - m_contours[0][90].X(), 
          dy = i_point.Y() - m_contours[0][90].Y(), pointarg = 0;
        if(! (Math::Abs(dx) < 0.00001))
          {
          if(dx > 0 && dy > 0)
            pointarg = Math::Atan(dy / dx);
          else if(dx < 0)
            pointarg = Math::Atan(dy / dx) + Math::PI;
          else if(dx > 0 && dy < 0)
            pointarg = Math::Atan(dy / dx) + 2 * Math::PI;
          }
        summ += m_gamma * pointarg;
      }

    summ /= 2 * Math::PI;
    return summ;
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
    double dphi = 0, delta_star = 0.5 * m_delta;
    Vector2D xstar, diff, orth;
    if(m_dt != 0)
      {
        double QQ = 0, d;
        double Qsum = m_off_gamma[4].back();

        for(std::size_t i = 0; i < m_contours[1].size(); ++i)
          {
            if(i == m_contours[1].size() - 1)
              {
                diff = m_contours[0][45] - m_contours[1][i];
                xstar = 0.5 * (m_contours[0][45] + m_contours[1][i]);
              }
            else
              {
                diff = m_contours[1][i + 1] - m_contours[1][i];
                xstar = 0.5 * (m_contours[1][i + 1] + m_contours[1][i]);
              }
            orth = i_point - xstar;
            Qsum += m_curr_gamma[91 + i] - m_prev_gamma[91 + i];
            d = orth.Length2();
            orth = orth.GetOrthogonal();
            if(d < delta_star * delta_star)
              {
                d = delta_star * delta_star;
              }
            dphi += Qsum * (orth * diff) / d;
          }

        for(std::size_t i = 0; i < m_contours[0].size() - 1; ++i)
          {
            diff = m_contours[0][i + 1] - m_contours[0][i];
            orth = i_point - m_colloc[i];
            QQ += m_curr_gamma[i] - m_prev_gamma[i];
            if(i == 45)
              QQ += Qsum;
            if(i % 30 == 0)
              QQ += m_off_gamma[i / 30].back();
            d = orth.Length2();
            orth = orth.GetOrthogonal();
            if(d < delta_star * delta_star)
              {
              d = delta_star * delta_star;
              }
            dphi += QQ * (orth * diff) / d;
          }

        for(std::size_t i = 0; i < m_off_points.size(); ++i)
          {
            diff = m_corners[i] - m_off_points[i].back();
            xstar = 0.5 * (m_corners[i] + m_off_points[i].back());
            orth = i_point - xstar;
            d = orth.Length2();
            orth = orth.GetOrthogonal();
            if(d < delta_star * delta_star)
              {
              d = delta_star * delta_star;
              }
            dphi += m_off_gamma[i].back() * (orth * diff) / d;
          }
        dphi /= 2 * Math::PI * m_dt;
        for(std::size_t i = 0; i < m_off_points.size(); ++i)
          for(std::size_t j = 0; j < m_off_points[i].size(); ++j)
            dphi -= m_off_gamma[i][j] * (V(i_point, m_off_points[i][j]) * CalcSpeed(m_off_points[i][j]));
      }
    return  1 - CalcSpeed(i_point).Length2() - 2 * dphi;

  }

//-------------------------------------

Vector2D Model::CalcSpeed(const Vector2D& i_point) const
  {
    Vector2D res = m_velocity;
    std::size_t move = 0;

    for(std::size_t i = 0; i < m_contours.size(); ++i)
      {
        for(std::size_t j = 0; j < m_contours[i].size(); ++j)
          {
            res = res + m_curr_gamma[move + j] * V(i_point, m_contours[i][j]);
          }
        move += m_contours[i].size();
      }

    for(std::size_t i = 0; i < m_off_points.size(); ++i)
      for(std::size_t j = 0; j < m_off_points[i].size(); ++j)
        res = res + m_off_gamma[i][j] * V(i_point, m_off_points[i][j]);
    return res;
  }

//-------------------------------------

Vector2D Model::V(const Vector2D& i_vect1, const Vector2D& i_vect2) const
  {
    Vector2D a = (i_vect1 - i_vect2).GetOrthogonal();
    double len = a.Length2();
    if(len > m_delta* m_delta)
      a = (1.0 / (2 * len * System::Math::PI)) * a;
    else
      a = (1.0 / (2 * m_delta * m_delta * System::Math::PI)) * a;
    return a;
  }


//-------------------------------------

Vector2D Model::V(std::size_t i_idx, const Vector2D& i_vect) const
  {
    std::size_t summ = 0;
    std::size_t i = 0;
    for(; summ < i_idx + 1; ++i)
      summ += m_contours[i].size();
    summ -= m_contours[i - 1].size();

    return V(i_vect, m_contours[i - 1][i_idx - summ]);
  }

//-------------------------------------

Vector2D Model::GetPoint(std::size_t i_idx) const
  {
    std::size_t summ = 0;
    std::size_t i = 0;
    for(; summ < i_idx + 1; ++i)
      summ += m_contours[i].size();
    summ -= m_contours[i - 1].size();

    return m_contours[i - 1][i_idx - summ];
  }

//-------------------------------------

Vector2D Model::MoveFromContour(const Vector2D& i_point, const Vector2D& prev_pos) const
  {
    double D = 2 * m_delta;
    Vector2D res = i_point;
    if(std::abs(i_point.Y() + 0.5) < D && i_point.X() > -0.5 && i_point.X() < 0.5)
      {
      if(prev_pos.Y() < -0.5)
        res.Y() = -0.5 - D;
      else
        res.Y() = -0.5 + D;

      }

    if(std::abs(i_point.X() + 0.5) < D && i_point.Y() > -0.5  && i_point.Y() < 0.5)
      {
      if(prev_pos.X() < -0.5)
        res.X() = -0.5 - D;
      else
        res.X() = -0.5 + D;
      }

    if(std::abs(i_point.X()) < D && i_point.Y() > -0.5 && i_point.Y() < 0.5)
      {
      if(prev_pos.X() < 0)
        res.X() = -D;
      else
        res.X() = D;
      }

    if(std::abs(i_point.X() - 0.5) < D && i_point.Y() > -0.5 && i_point.Y() < 0.5)
      {
      if(prev_pos.X() < 0.5)
        res.X() = 0.5 - D;
      else
        res.X() = 0.5 + D;
      }
    return res;
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
      for(std::size_t j = 0; j < m_off_points[i].size(); ++j)
        new_points[i].push_back(MoveFromContour(m_off_points[i][j] + m_dt * CalcSpeed(m_off_points[i][j]), m_off_points[i][j]));

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

std::size_t Model::GetSize() const
  {
    std::size_t summ = 0;
    for(std::size_t i = 0; i < m_contours.size(); ++i)
      summ += m_contours[i].size();
    return summ;
  }

//-------------------------------------

double Model::GetDt() const
  {
    return m_dt;
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