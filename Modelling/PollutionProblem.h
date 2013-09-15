#pragma once
#include "SulfurParticle.h"
#include "Model.h"
#include <vector>
class PollutionProblem
  {
  public:
    PollutionProblem(double i_radius = 0.1, double i_average_ttl = 0.0, const Vector2D& i_centre = Vector2D());
    void SetParams(double i_radius, double i_average_ttl, const Vector2D& i_centre);
    const std::vector<SulfurParticle>& GetParticles();
    const Vector2D& GetCentre();
    double GetRadius();
    double GetAvgTTL();
    double GetCurrTime();
    void GenerateParticles();
    void MoveParticles(const Model& i_model);
    void UpdateTime(const Model& i_model);
  private:
    std::vector<SulfurParticle> m_particles;
    double m_radius, m_avg_ttl, m_curr_time;
    Vector2D m_centre;
  };
extern PollutionProblem probl;


