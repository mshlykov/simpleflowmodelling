#include "StdAfx.h"
#include "PollutionProblem.h"
#include <algorithm>

PollutionProblem probl;
std::vector<std::vector<int>> colors_for_conc;

PollutionProblem::PollutionProblem(double i_radius, double i_average_ttl, const Vector2D& i_centre):
  m_centre(i_centre),
  m_avg_ttl(i_average_ttl),
  m_radius(i_radius),
  m_curr_time(0.0)
  {
  }

void PollutionProblem::SetParams(double i_radius, double i_average_ttl, std::size_t i_number_of_particles, const Vector2D& i_centre)
  {
    m_radius = i_radius;
    m_avg_ttl = i_average_ttl;
    m_centre = i_centre;
    GenerateParticles(i_number_of_particles);
  }

const std::vector<SulfurParticle>& PollutionProblem::GetParticles()
  {
    return m_particles;
  }

const Vector2D& PollutionProblem::GetCentre()
  {
    return m_centre;
  }

double PollutionProblem::GetRadius()
  {
    return m_radius;
  }

double PollutionProblem::GetAvgTTL()
  {
    return m_avg_ttl;
  }

double PollutionProblem::GetCurrTime()
  {
    return m_curr_time;
  }

void PollutionProblem::MoveParticles(const Model& i_model)
  {
    std::for_each(m_particles.begin(), m_particles.end(),
      [&i_model, this](SulfurParticle& i_particle)
      {
        Vector2D new_pos = i_particle.GetLocation();
        new_pos = new_pos + i_model.GetDt() * i_model.CalcSpeed(new_pos);
        if(i_particle.GetTTL() > m_curr_time)
          i_particle.TranslateTo(i_model.MoveFromContour(new_pos, i_particle.GetLocation()));
      });
  }

void PollutionProblem::UpdateTime(const Model& i_model)
  {
    m_curr_time += i_model.GetDt();
  }

void PollutionProblem::GenerateParticles(std::size_t i_number_of_particles)
  {
    if(!m_particles.empty())
      {
        m_particles.clear();
        m_curr_time = 0.0;
      }
    double x = 0, y = 0;
    for(; m_particles.size() < i_number_of_particles;)
      {
        x = (static_cast<double>(rand()) / RAND_MAX - 0.5) * 2.0 * m_radius;
        y = (static_cast<double>(rand()) / RAND_MAX - 0.5) * 2.0 * m_radius;
        if(x * x + y * y < m_radius * m_radius)
          {
            double ttl = m_avg_ttl - 0.25 * m_avg_ttl + (static_cast<double>(rand()) / RAND_MAX - 0.5) * m_avg_ttl * 0.5;
            SulfurParticle new_particle(ttl, Vector2D(m_centre.X() + x, m_centre.Y() + y));
            m_particles.push_back(new_particle);
          }
      }
    std::sort(m_particles.begin(), m_particles.end(), 
      [](const SulfurParticle& i_part1, const SulfurParticle& i_part2) -> bool
      {
        return i_part1.GetTTL() < i_part2.GetTTL();
      });
  }
