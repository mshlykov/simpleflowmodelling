#pragma once
#include <windows.h>
#include <string>
#include "glut.h"
class Drawer
  {
  public:
    Drawer(double i_virt_left = 0, double i_virt_right = 1, double i_virt_bottom = 0, double i_virt_top = 1,
      double i_targ_left = 0, double i_targ_right = 1, double i_targ_bottom = 0, double i_targ_top = 1);
    void SetViewport(double i_virt_left, double i_virt_right, double i_virt_bottom, double i_virt_top);
    void SetTarget(double i_targ_left, double i_targ_right, double i_targ_bottom, double i_targ_top);
    void DrawVert(double i_x, double i_y) const;
    void DrawText(const std::string& i_text, double i_x, double i_y) const;
    double GetMinY() const;
    double GetMinX() const;
    double GetMaxX() const;
    double GetMaxY() const;
  private:
    double m_virt_left, m_virt_right, m_virt_bottom, m_virt_top,
      m_targ_left, m_targ_right, m_targ_bottom, m_targ_top;
  };

