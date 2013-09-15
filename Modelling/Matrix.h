#pragma once

#include<iostream>
#include<cmath>

using namespace std;

namespace MatrSpace
{
class Matrix
	{
	public:
		
    Matrix():els(0),row(0),col(0){};
		Matrix(std::size_t,std::size_t);
		Matrix(const Matrix&);
		
    ~Matrix();
		
    Matrix Inverse();
		
    void Resize(std::size_t,std::size_t);
		
    double& operator()(std::size_t,std::size_t);
    const double& operator()(std::size_t,std::size_t) const;
		
    std::size_t GetRsize()
		{
			return row;
		}

		std::size_t GetCsize()
		{
			return col;
		}
		
    friend ostream& operator <<(ostream&,Matrix&);
		
    friend Matrix operator *(const Matrix&,const Matrix&);
		friend Matrix operator *(double,const Matrix&);
		
    friend Matrix operator +(const Matrix&,const Matrix&);
		friend Matrix operator -(const Matrix&,const Matrix&);
		
    friend Matrix operator -(const Matrix&);
		friend bool operator ==(const Matrix&,const Matrix&);
		
    friend void getCoef(Matrix&);
		Matrix& operator =(const Matrix&);
		
    Matrix Transp();
		
    friend double NormVect(const Matrix&);
		friend double NormMatr(const Matrix&);
		

    Matrix SolveGauss(Matrix b);
    double Norm1(std::size_t&,std::size_t&);
		double SqEuclNorm();
		Matrix PseudoInverse();
    bool CheckNull() const
      {
      for(std::size_t i = 0; i < row; ++i)
        for(std::size_t j = 0; j < col; ++j)
          if(std::abs(els[i][j]) > 0.00000001)
            return false;

      return true;
      
      }
  private:
    static double Eps;
    double **els;
    std::size_t row,col;
	};
}