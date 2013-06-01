#include "Drawer.h"


Drawer::Drawer(double i_virt_left, double i_virt_right, double i_virt_bottom, double i_virt_top,
         double i_targ_left, double i_targ_right, double i_targ_bottom, double i_targ_top):
  m_virt_left(i_virt_left), m_virt_right(i_virt_right),
  m_virt_bottom(i_virt_bottom), m_virt_top(i_virt_top),
  m_targ_left(i_targ_left), m_targ_right(i_targ_right),
  m_targ_bottom(i_targ_bottom), m_targ_top(i_targ_top)
  {
  }

void Drawer::SetViewport(double i_virt_left, double i_virt_right, double i_virt_bottom, double i_virt_top)
  {
    m_virt_left = i_virt_left;
    m_virt_right = i_virt_right;
    m_virt_bottom = i_virt_bottom;
    m_virt_top = i_virt_top;
  }

void Drawer::SetTarget(double i_targ_left, double i_targ_right, double i_targ_bottom, double i_targ_top)
  {
    m_targ_left = i_targ_left;
    m_targ_right = i_targ_right;
    m_targ_bottom = i_targ_bottom;
    m_targ_top = i_targ_top;
  }


void Drawer::DrawVert(double i_x, double i_y) const
  {
    if(i_x < m_virt_left || i_x > m_virt_right || i_y < m_virt_bottom || i_y > m_virt_top)
      return;
    glVertex2d(m_targ_left + (i_x - m_virt_left) * (m_targ_right - m_targ_left) / (m_virt_right - m_virt_left), 
      m_targ_bottom + (i_y - m_virt_bottom) * (m_targ_top - m_targ_bottom) / (m_virt_top - m_virt_bottom));
  }
void Drawer::DrawText(const std::string& i_text, double i_x, double i_y) const
  { 
    double realx = m_targ_left + (i_x - m_virt_left) * (m_targ_right - m_targ_left) / (m_virt_right - m_virt_left), 
      realy = m_targ_bottom + (i_y - m_virt_bottom) * (m_targ_top - m_targ_bottom) / (m_virt_top - m_virt_bottom);
    glRasterPos2f(realx, realy);
    for(std::size_t i = 0; i < i_text.size(); ++i)
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, i_text[i]);
  }

double Drawer::GetMinY() const
  {
    return m_virt_bottom;
  }

double Drawer::GetMinX() const
  {
    return m_virt_left;
  }

double Drawer::GetMaxX() const
  {
    return m_virt_right;
  }

double Drawer::GetMaxY() const
  {
    return m_virt_top;
  }

