#pragma once

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;

ref class Drawer
  {
  public:

    Drawer(int i_target_width, int i_target_height, double i_minx, double i_miny, double i_maxx, double i_maxy)
      : m_minx(i_minx), 
      m_miny(i_miny), 
      m_maxx(i_maxx), 
      m_maxy(i_maxy),
      m_target_width(i_target_width), 
      m_target_height(i_target_height)
      {

      }

    //-------------------------------------

    void SetViewPort(double i_minx, double i_miny, double i_maxx, double i_maxy)
      {
      m_minx = i_minx;
      m_miny = i_miny;
      m_maxx = i_maxx;
      m_maxy = i_maxy;
      }

    //-------------------------------------

    void SetTargetResolution(int i_width, int i_height)
      {
      m_target_width = i_width;
      m_target_height = i_height;
      }

    //-------------------------------------

    void DrawLine(System::Drawing::Pen^ i_pen, System::Drawing::Graphics^ i_graphics, double i_start_x, double i_start_y, double i_end_x, double i_end_y)
      {
      System::Drawing::Point targ_start(ScaleX(i_start_x), ScaleY(i_start_y)), 
        targ_end(ScaleX(i_end_x), ScaleY(i_end_y));
      
      i_graphics->DrawLine(i_pen, targ_start, targ_end);
      }

    //-------------------------------------

    void DrawText(System::Drawing::Graphics^ i_graphics, System::String^ i_str, double i_x, double i_y)
      {
      System::Drawing::Font^ dr_font = gcnew System::Drawing::Font(L"Arial", 8 /*0.035 * System::Math::Min(m_target_height, m_target_width)*/);
      i_graphics->DrawString(i_str, dr_font, System::Drawing::Brushes::Black, static_cast<float>(ScaleX(i_x)), static_cast<float>(ScaleY(i_y)));
      }

    //-------------------------------------

    void DrawPoint(System::Drawing::Graphics^ i_graphics, double i_x, double i_y, int i_color)
      {
      System::Drawing::Brush^ br = gcnew System::Drawing::SolidBrush( Color::FromArgb(i_color));
      i_graphics->FillRectangle(br, ScaleX(i_x) - 1, ScaleY(i_y) - 1, 3, 3);
      }

    //-------------------------------------

    void DrawRect(System::Drawing::Graphics^ i_graphics, double i_x, double i_y, double i_width, double i_height, int i_color)
      {
        System::Drawing::Brush^ br = gcnew System::Drawing::SolidBrush( Color::FromArgb(i_color));
        int x = ScaleX(i_x), y = ScaleY(i_y), x1 = ScaleX(i_x + i_width), y1 = ScaleY(i_y + i_height);
        i_graphics->FillRectangle(br, x, y1, x1 - x, y - y1);
      }

    //-------------------------------------

    double GetMinX()
      {
      return m_minx;
      }

    //-------------------------------------

    double GetMinY()
      {
      return m_miny;
      }

    //-------------------------------------

    double GetMaxX()
      {
      return m_maxx;
      }

    //-------------------------------------

    double GetMaxY()
      {
      return m_maxy;
      }

    //-------------------------------------

  private:

    int ScaleX(double i_value)
      {
      return static_cast<int>((i_value - m_minx) / (m_maxx - m_minx) * m_target_width);
      }

    //-------------------------------------

    int ScaleY(double i_value)
      {
      return static_cast<int>((1.0 - (i_value - m_miny) / (m_maxy - m_miny)) * m_target_height);
      }

    int m_target_width, m_target_height;
    double m_minx, m_miny, m_maxx, m_maxy;
  };