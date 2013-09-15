#include "StdAfx.h"
#include "SulfurParticle.h"


SulfurParticle::SulfurParticle(double i_time_to_live, const Vector2D& i_location):
  m_location(i_location),
  m_time_to_live(i_time_to_live)
  {
  }

const Vector2D& SulfurParticle::GetLocation() const
  {
    return m_location;
  }

double SulfurParticle::GetTTL() const
  {
    return m_time_to_live;
  }

void SulfurParticle::TranslateTo(const Vector2D& i_vector)
  {
    m_location = i_vector;
  }



