<<<<<<< HEAD
#pragma once

class Vector2D
  {
  public:
    
    Vector2D(double i_x = 0, double i_y = 0);
    
    Vector2D Normalize();
    Vector2D GetOrthogonal() const;
    
    double Length() const;
    double Length2() const;
    
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

=======
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

>>>>>>> 5ee04f3a035fe2c3e6c7e7679da55c0ea9c11643
double operator * (const Vector2D&, const Vector2D&);