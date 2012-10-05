#pragma once
#include "Vector2D.h"
#include <vector>
typedef std::vector<Vector2D> Points;
typedef std::vector<Points> Contours;
class Model
  {
  public:
    
    Model();
    
    void Init();
    void CalcGamma(std::vector<double>& o_gamma, const Vector2D& i_velocity, double i_gamma);
    double CalcPhi(const Vector2D& i_point, const std::vector<double>& i_gamma);
    double CalcPsi(const Vector2D& i_point, const std::vector<double>& i_gamma);

    Vector2D V(int i_idx, const Vector2D&  i_vect) const;
    
    const Contours& GetContours() const;
    const Vector2D& GetVelocity() const;
    int GetSize() const;
    static std::vector<double> curr_gamma;

  private:

    Contours m_contours;
    Points m_colloc, m_normals;
    double m_delta, m_gamma;
    Vector2D m_velocity;
  };

extern Model model;