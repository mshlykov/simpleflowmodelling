<<<<<<< HEAD
#include "stdafx.h"
#include "Vector2D.h"

Vector2D::Vector2D(double i_x, double i_y)
  :m_x(i_x), 
  m_y(i_y)
  {
  }

//-------------------------------------

double Vector2D::Length2() const
  {
  return (*this)*(*this);
  }

Vector2D Vector2D::Normalize()
  {
  double norm = Length();
  m_x /= norm;
  m_y /= norm;

  return *this;
  }

//-------------------------------------

Vector2D Vector2D::GetOrthogonal() const
  {
  return Vector2D(-m_y, m_x);
  }

//-------------------------------------

double Vector2D::Length() const
  {
  return sqrt((*this) * (*this));
  }

//-------------------------------------

double& Vector2D::X()
  {
  return m_x;
  }

//-------------------------------------

double& Vector2D::Y()
  {
  return m_y;
  }

//-------------------------------------

const double& Vector2D::X() const
  {
  return m_x;
  }

//-------------------------------------

const double& Vector2D::Y() const
  {
  return m_y;
  }

//-------------------------------------

Vector2D operator + (const Vector2D& i_vect1, const Vector2D& i_vect2)
  {
  return Vector2D(i_vect1.X() + i_vect2.X(), i_vect1.Y() + i_vect2.Y());
  }

//-------------------------------------

Vector2D operator - (const Vector2D& i_vect1, const Vector2D& i_vect2)
  {
  return Vector2D(i_vect1.X() - i_vect2.X(), i_vect1.Y() - i_vect2.Y());
  }

//-------------------------------------

Vector2D operator * (const Vector2D& i_vect, double i_multer)
  {
  return Vector2D(i_vect.X() * i_multer , i_vect.Y() * i_multer);
  }

//-------------------------------------

Vector2D operator * (double i_multer, const Vector2D& i_vect)
  {
  return operator * (i_vect, i_multer);
  }

//-------------------------------------

double operator * (const Vector2D& i_vect1, const Vector2D& i_vect2)
  {
  return i_vect1.X() * i_vect2.X() + i_vect1.Y() * i_vect2.Y();
=======
#include "stdafx.h"
#include "Vector2D.h"

Vector2D::Vector2D(double i_x, double i_y)
  :m_x(i_x), 
  m_y(i_y)
  {
  }

//-------------------------------------

Vector2D Vector2D::Normalize()
  {
  double norm = Length();
  m_x /= norm;
  m_y /= norm;

  return *this;
  }

//-------------------------------------

Vector2D Vector2D::GetOrthogonal() const
  {
  return Vector2D(-m_y, m_x);
  }

//-------------------------------------

double Vector2D::Length() const
  {
  return sqrt((*this) * (*this));
  }

//-------------------------------------

double& Vector2D::X()
  {
  return m_x;
  }

//-------------------------------------

double& Vector2D::Y()
  {
  return m_y;
  }

//-------------------------------------

const double& Vector2D::X() const
  {
  return m_x;
  }

//-------------------------------------

const double& Vector2D::Y() const
  {
  return m_y;
  }

//-------------------------------------

Vector2D operator + (const Vector2D& i_vect1, const Vector2D& i_vect2)
  {
  return Vector2D(i_vect1.X() + i_vect2.X(), i_vect1.Y() + i_vect2.Y());
  }

//-------------------------------------

Vector2D operator - (const Vector2D& i_vect1, const Vector2D& i_vect2)
  {
  return Vector2D(i_vect1.X() - i_vect2.X(), i_vect1.Y() - i_vect2.Y());
  }

//-------------------------------------

Vector2D operator * (const Vector2D& i_vect, double i_multer)
  {
  return Vector2D(i_vect.X() * i_multer , i_vect.Y() * i_multer);
  }

//-------------------------------------

Vector2D operator * (double i_multer, const Vector2D& i_vect)
  {
  return operator * (i_vect, i_multer);
  }

//-------------------------------------

double operator * (const Vector2D& i_vect1, const Vector2D& i_vect2)
  {
  return i_vect1.X() * i_vect2.X() + i_vect1.Y() * i_vect2.Y();
>>>>>>> 5ee04f3a035fe2c3e6c7e7679da55c0ea9c11643
  }