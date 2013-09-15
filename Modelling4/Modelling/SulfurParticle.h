#pragma once
#include "Vector2D.h"

class SulfurParticle
  {
  public:
    SulfurParticle(double i_time_to_live = 0.0, const Vector2D& i_location = Vector2D());
    const Vector2D& GetLocation() const;
    double GetTTL() const;
    void TranslateTo(const Vector2D& i_vector);
  private:
    double m_time_to_live;
    Vector2D m_location;
  };

//void GenerateParticles()

