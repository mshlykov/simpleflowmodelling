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
		Matrix(int,int);
		Matrix(Matrix&);
		
    ~Matrix();
		
    Matrix Inverse();
		
    void Resize(int,int);
		
    double& operator()(int,int);
		
    int GetRsize()
		{
			return row;
		}

		int GetCsize()
		{
			return col;
		}
		
    friend ostream& operator <<(ostream&,Matrix&);
		
    friend Matrix operator *(Matrix,Matrix);
		friend Matrix operator *(double,Matrix);
		
    friend Matrix operator +(Matrix,Matrix);
		friend Matrix operator -(Matrix,Matrix);
		
    friend Matrix operator -(Matrix);
		friend bool operator ==(Matrix&,Matrix&);
		
    friend void getCoef(Matrix&);
		Matrix& operator =(Matrix&);
		
    Matrix Transp();
		
    friend double NormVect(Matrix);
		friend double NormMatr(Matrix);
		

    Matrix SolveGauss(Matrix b);
    double Norm1(int&,int&);
		double SqEuclNorm();
		Matrix PseudoInverse();
  
  private:

    double **els;
    int row,col;
	};
}