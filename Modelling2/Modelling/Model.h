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
    void ReInit();
    void SetParams(const Vector2D& i_velocity, double i_gamma);
    void CalcGamma();
    double CalcPhi(const Vector2D& i_point) const;
    double CalcPsi(const Vector2D& i_point) const;
    Vector2D CalcSpeed(const Vector2D& i_point) const;

    Vector2D V(const Vector2D& i_vect1, const Vector2D& i_vect2) const;
    Vector2D V(int i_idx, const Vector2D&  i_vect) const;
    Vector2D GetPoint(int i_idx) const;
    double Model::GetGamma(int i_i, int i_j) const;
    void UpdatePoints();
    
    const Contours& GetContours() const;
    const Vector2D& GetVelocity() const;

    const std::vector<std::vector<Vector2D>>& GetOffPoints() const;
    const std::vector<std::vector<double>>& GetOffGamma() const;
    const std::vector<double>& GetCurrGamma() const;

    int GetSize() const;

    static std::vector<std::vector<int>> colors_by_phi;

private:
    
    std::vector<double> m_curr_gamma;
    std::vector<std::vector<double>> m_off_gamma;
    Contours m_contours, m_off_points;
    Points m_colloc, m_normals;
    double m_delta, m_gamma;
    Vector2D m_velocity;
  };

extern Model model;