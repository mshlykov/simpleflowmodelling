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
    void SetParams(const Vector2D& i_velocity, double i_gamma)
      {
      m_velocity = i_velocity;
      m_gamma = i_gamma;
      }
    void CalcGamma(std::vector<double>& o_gamma);
    double CalcPhi(const Vector2D& i_point, const std::vector<double>& i_gamma) const;
    double CalcPsi(const Vector2D& i_point, const std::vector<double>& i_gamma) const;
    Vector2D CalcSpeed(const Vector2D& i_point) const;

    Vector2D V(const Vector2D& i_vect1, const Vector2D& i_vect2) const;
    Vector2D V(int i_idx, const Vector2D&  i_vect) const;
    Vector2D GetPoint(int i_idx);
    double Model::GetGamma(int i_i, int i_j) const;
    void UpdatePoints();
    
    const Contours& GetContours() const;
    const Vector2D& GetVelocity() const;
    int GetSize() const;

    static std::vector<double> curr_gamma;
    static std::vector<std::vector<int>> colors_by_phi;
    static std::vector<std::vector<int>> colors_by_psi;
    static std::vector<std::vector<Vector2D>> off_points;
    static std::vector<std::vector<double>> off_gamma;

  private:

    Contours m_contours;
    Points m_colloc, m_normals;
    double m_delta, m_gamma;
    Vector2D m_velocity;
  };

extern Model model;