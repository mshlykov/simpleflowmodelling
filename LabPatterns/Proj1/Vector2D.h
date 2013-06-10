#pragma once

class Vector2D
  {
  public:
    
    Vector2D(double i_x = 0, double i_y = 0);
    
    Vector2D Normalize();
    Vector2D GetOrthogonal() const;
    
    double Length() const;
    
    double& X();
    double& Y();

    const double& X() const;
    const double& Y() const;
  
  private:
    
    double m_x, m_y;
  };

Vector2D operator + (const Vector2D&, const Vector2D&);

Vector2D operator - (const Vector2D&, const Vector2D&);

Vector2D operator * (const Vector2D&, double);

Vector2D operator * (double, const Vector2D&);

double operator * (const Vector2D&, const Vector2D&);