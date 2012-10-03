<<<<<<< HEAD
#include "stdafx.h"
#include "Matrix.h"
namespace MatrSpace
{
	Matrix::Matrix(int _rownum,int _colnum)
	{
		row=_rownum;
		col=_colnum;
		els=new double*[row];
		for(int i=0;i<row;i++)
			els[i]=new double[col];
		for(int i=0;i<row;i++)
			for(int j=0;j<col;j++)els[i][j]=0;
	}
	Matrix::Matrix(Matrix&b)
	{
		row=b.row;
		col=b.col;
		els=new double*[row];
		for(int i=0;i<row;i++)
			els[i]=new double[col];
		for(int i=0;i<row;i++)
			for(int j=0;j<col;j++)els[i][j]=b.els[i][j];
	}
	Matrix::~Matrix()
	{
		if(els!=0)
		{
			for(int i=0;i<row;i++)
			{
				if(els[i]!=0)delete[]els[i];
				els[i]=0;
			}
			delete[]els;
			els=0;
		}
	}
	void Matrix::Resize(int _rownum, int _colnum)
	{
		if(els!=0)
		{
			for(int i=0;i<row;i++)
			{
				if(els[i]!=0)delete[]els[i];
				els[i]=0;
			}
			delete[]els;
			els=0;
		}
		row=_rownum;
		col=_colnum;
		els=new double*[row];
		for(int i=0;i<row;i++)
			els[i]=new double[col];
		for(int i=0;i<row;i++)
			for(int j=0;j<col;j++)els[i][j]=0;
	}
	double& Matrix::operator()(int i,int j)
	{
		return els[i][j];
	}
	ostream& operator <<(ostream&os,Matrix& A)
	{
		for(int i=0;i<A.row;i++)
		{
			for(int j=0;j<A.col;j++)os<<A.els[i][j]<<" ";
			os<<endl;
		}
		return os;
	}
	Matrix operator *(Matrix A,Matrix B)
	{

		Matrix C(A.row,B.col);
		for(int i=0;i<C.row;i++)
			for(int j=0;j<C.col;j++)
			{
				for(int k=0;k<A.col;k++)C.els[i][j]+=A.els[i][k]*B.els[k][j];
			}
		return C;
	}
	Matrix operator *(double a,Matrix B)
	{
		Matrix C(B.row,B.col);
		for(int i=0;i<C.row;i++)
			for(int j=0;j<C.col;j++)
			{
				C.els[i][j]=a*B.els[i][j];
			}
		return C;
	}
	Matrix operator +(Matrix A,Matrix B)
	{
		Matrix C(B.row,B.col);
		for(int i=0;i<C.row;i++)
			for(int j=0;j<C.col;j++)
			{
				C.els[i][j]=A.els[i][j]+B.els[i][j];
			}
		return C;
	}
	Matrix operator -(Matrix A,Matrix B)
	{
		Matrix C(B.row,B.col);
		for(int i=0;i<C.row;i++)
			for(int j=0;j<C.col;j++)
			{
				C.els[i][j]=A.els[i][j]-B.els[i][j];
			}
		return C;
	}
	Matrix operator -(Matrix A)
	{
		return (-1)*A;
	}
	Matrix& Matrix::operator =(Matrix& B)
	{
		if(els!=0)
		{
			for(int i=0;i<row;i++)
			{
				if(els[i]!=0)delete[]els[i];
				els[i]=0;
			}
			delete[]els;
			els=0;
		}
		row=B.row;
		col=B.col;
		els=new double*[row];
		for(int i=0;i<row;i++)
			els[i]=new double[col];
		for(int i=0;i<row;i++)
			for(int j=0;j<col;j++)els[i][j]=B.els[i][j];
		return B;
	}
	Matrix Matrix::Transp()
	{
		Matrix A(col,row);
		for(int i=0;i<col;i++)
			for(int j=0;j<row;j++)A.els[i][j]=els[j][i];
		return A;
	}
	void getCoef(Matrix& matr) 
	{
		for (int i=0; i<matr.GetRsize(); ++i)
		{
			for (int j=0; j<matr.GetCsize(); ++j)
			{
				//matr(i,j) = max(i,j) + j;
				matr(i,j) = 1;
			}
		}
	}
	double NormVect(Matrix A)
	{
		double s=fabs(A.els[0][0]);
		for(int i=0;i<A.row;i++)if(s<fabs(A.els[i][0]))s=A.els[i][0];
		return s;
	}
	double NormMatr(Matrix A)
	{
		double s=0,s1=0;
		int i=1;
		for(int j=0;j<A.col;j++)s+=fabs(A.els[0][j]);
		do
		{
			s1=0;
			for(int j=0;j<A.col;j++)s1+=fabs(A.els[i][j]);
			if(s1>s)s=s1;
			i++;
		}
		while(i<A.row);
		return s;
	}
	double Matrix::Norm1(int& _i,int&_j)
	{
		double s=fabs(els[0][1]),summ=0;
		_i=0;_j=1;
		for(int i=0;i<row;i++)
			for(int j=i+1;j<col;j++)
			{
				summ+=2*els[i][j]*els[i][j];
				if(s<fabs(els[i][j])){s=fabs(els[i][j]);_i=i;_j=j;}
			}
		return summ;
	}
	Matrix Matrix::Inverse()
	{
		Matrix  res(row,col),A=*this,L(row,col),E(row,col);
		for(int i=0;i<col;i++)res.els[i][i]=1,E.els[i][i]=1;
		for(int j=0;j<col;j++)
		{
			L=E;
			for(int i=0;i<row;i++)
			{
				if(i!=j)L.els[i][j]=-A.els[i][j]/A.els[j][j];
				else L.els[i][j]=1.0/A.els[j][j];
			}
			A=L*A;
			res=L*res;
		}
		return res;
	}

	double Matrix::SqEuclNorm()
	{
		double summ=0;
		for(int i=0;i<row;i++)summ+=els[i][0]*els[i][0];
		return summ;
	}
	bool operator ==(Matrix&A,Matrix&B)
	{
		if(A.row!=B.row||A.col!=B.col)return false;
		for(int i=0;i<A.row;i++)
			for(int j=0;j<A.col;j++)
				if(A.els[i][j]!=B.els[i][j])return false;
		return true;
	}
	Matrix Matrix::PseudoInverse()
	{
		Matrix Inv(row,1),E(row,row),AA,Inv1(row,1),Col(row,1),beta,NewInv,Pr;
		for(int i=0;i<row;i++)Inv1(i,0)=els[i][0];
		double Inorm=Inv1.SqEuclNorm();
		if(Inorm!=0)Inv=(1/Inorm)*Inv1.Transp();
		else Inv=Inv1.Transp();
		for(int i=1;i<col;i++)
		{
			//cout<<"Inv\n"<<Inv<<"\nInv1\n"<<Inv1<<"\n";
			for(int j=0;j<row;j++)Col(j,0)=els[j][i];
			//cout<<"Col\n"<<Col<<'\n';
			AA=Inv1*Inv*Col;
			//cout<<"AA\n"<<AA<<"\n\n";
			Pr=Col-AA;
			Inorm=Pr.SqEuclNorm();
			if(Inorm>=0.000000001)beta=(1/Inorm)*Pr;
			else beta=(1/(1+(Inv*Col).SqEuclNorm()))*Inv.Transp()*Inv*Col;
			//cout<<"Beta\n"<<beta<<"\n";
			Inv=Inv-Inv*Col*beta.Transp();
			//cout<<"Inv(New)\n"<<Inv<<"\n";
			NewInv.Resize(Inv.row+1,Inv.col);
			for(int j=0;j<NewInv.row;j++)
				for(int k=0;k<NewInv.col;k++)
					if(j!=NewInv.row-1)NewInv.els[j][k]=Inv.els[j][k];
					else NewInv.els[j][k]=beta.els[k][0];
			Inv=NewInv;
			NewInv.Resize(Inv1.row,i+1);
			for(int j=0;j<NewInv.row;j++)
				for(int k=0;k<NewInv.col;k++)
					NewInv.els[j][k]=els[j][k];
			Inv1=NewInv;
		}
		return Inv;
	}
=======
#include "stdafx.h"
#include "Matrix.h"
namespace MatrSpace
{
	Matrix::Matrix(int _rownum,int _colnum)
	{
		row=_rownum;
		col=_colnum;
		els=new double*[row];
		for(int i=0;i<row;i++)
			els[i]=new double[col];
		for(int i=0;i<row;i++)
			for(int j=0;j<col;j++)els[i][j]=0;
	}
	Matrix::Matrix(Matrix&b)
	{
		row=b.row;
		col=b.col;
		els=new double*[row];
		for(int i=0;i<row;i++)
			els[i]=new double[col];
		for(int i=0;i<row;i++)
			for(int j=0;j<col;j++)els[i][j]=b.els[i][j];
	}
	Matrix::~Matrix()
	{
		if(els!=0)
		{
			for(int i=0;i<row;i++)
			{
				if(els[i]!=0)delete[]els[i];
				els[i]=0;
			}
			delete[]els;
			els=0;
		}
	}
	void Matrix::Resize(int _rownum, int _colnum)
	{
		if(els!=0)
		{
			for(int i=0;i<row;i++)
			{
				if(els[i]!=0)delete[]els[i];
				els[i]=0;
			}
			delete[]els;
			els=0;
		}
		row=_rownum;
		col=_colnum;
		els=new double*[row];
		for(int i=0;i<row;i++)
			els[i]=new double[col];
		for(int i=0;i<row;i++)
			for(int j=0;j<col;j++)els[i][j]=0;
	}
	double& Matrix::operator()(int i,int j)
	{
		return els[i][j];
	}
	ostream& operator <<(ostream&os,Matrix& A)
	{
		for(int i=0;i<A.row;i++)
		{
			for(int j=0;j<A.col;j++)os<<A.els[i][j]<<" ";
			os<<endl;
		}
		return os;
	}
	Matrix operator *(Matrix A,Matrix B)
	{

		Matrix C(A.row,B.col);
		for(int i=0;i<C.row;i++)
			for(int j=0;j<C.col;j++)
			{
				for(int k=0;k<A.col;k++)C.els[i][j]+=A.els[i][k]*B.els[k][j];
			}
		return C;
	}
	Matrix operator *(double a,Matrix B)
	{
		Matrix C(B.row,B.col);
		for(int i=0;i<C.row;i++)
			for(int j=0;j<C.col;j++)
			{
				C.els[i][j]=a*B.els[i][j];
			}
		return C;
	}
	Matrix operator +(Matrix A,Matrix B)
	{
		Matrix C(B.row,B.col);
		for(int i=0;i<C.row;i++)
			for(int j=0;j<C.col;j++)
			{
				C.els[i][j]=A.els[i][j]+B.els[i][j];
			}
		return C;
	}
	Matrix operator -(Matrix A,Matrix B)
	{
		Matrix C(B.row,B.col);
		for(int i=0;i<C.row;i++)
			for(int j=0;j<C.col;j++)
			{
				C.els[i][j]=A.els[i][j]-B.els[i][j];
			}
		return C;
	}
	Matrix operator -(Matrix A)
	{
		return (-1)*A;
	}
	Matrix& Matrix::operator =(Matrix& B)
	{
		if(els!=0)
		{
			for(int i=0;i<row;i++)
			{
				if(els[i]!=0)delete[]els[i];
				els[i]=0;
			}
			delete[]els;
			els=0;
		}
		row=B.row;
		col=B.col;
		els=new double*[row];
		for(int i=0;i<row;i++)
			els[i]=new double[col];
		for(int i=0;i<row;i++)
			for(int j=0;j<col;j++)els[i][j]=B.els[i][j];
		return B;
	}
	Matrix Matrix::Transp()
	{
		Matrix A(col,row);
		for(int i=0;i<col;i++)
			for(int j=0;j<row;j++)A.els[i][j]=els[j][i];
		return A;
	}
	void getCoef(Matrix& matr) 
	{
		for (int i=0; i<matr.GetRsize(); ++i)
		{
			for (int j=0; j<matr.GetCsize(); ++j)
			{
				//matr(i,j) = max(i,j) + j;
				matr(i,j) = 1;
			}
		}
	}
	double NormVect(Matrix A)
	{
		double s=fabs(A.els[0][0]);
		for(int i=0;i<A.row;i++)if(s<fabs(A.els[i][0]))s=A.els[i][0];
		return s;
	}
	double NormMatr(Matrix A)
	{
		double s=0,s1=0;
		int i=1;
		for(int j=0;j<A.col;j++)s+=fabs(A.els[0][j]);
		do
		{
			s1=0;
			for(int j=0;j<A.col;j++)s1+=fabs(A.els[i][j]);
			if(s1>s)s=s1;
			i++;
		}
		while(i<A.row);
		return s;
	}
	double Matrix::Norm1(int& _i,int&_j)
	{
		double s=fabs(els[0][1]),summ=0;
		_i=0;_j=1;
		for(int i=0;i<row;i++)
			for(int j=i+1;j<col;j++)
			{
				summ+=2*els[i][j]*els[i][j];
				if(s<fabs(els[i][j])){s=fabs(els[i][j]);_i=i;_j=j;}
			}
		return summ;
	}
	Matrix Matrix::Inverse()
	{
		Matrix  res(row,col),A=*this,L(row,col),E(row,col);
		for(int i=0;i<col;i++)res.els[i][i]=1,E.els[i][i]=1;
		for(int j=0;j<col;j++)
		{
			L=E;
			for(int i=0;i<row;i++)
			{
				if(i!=j)L.els[i][j]=-A.els[i][j]/A.els[j][j];
				else L.els[i][j]=1.0/A.els[j][j];
			}
			A=L*A;
			res=L*res;
		}
		return res;
	}

	double Matrix::SqEuclNorm()
	{
		double summ=0;
		for(int i=0;i<row;i++)summ+=els[i][0]*els[i][0];
		return summ;
	}
	bool operator ==(Matrix&A,Matrix&B)
	{
		if(A.row!=B.row||A.col!=B.col)return false;
		for(int i=0;i<A.row;i++)
			for(int j=0;j<A.col;j++)
				if(A.els[i][j]!=B.els[i][j])return false;
		return true;
	}
	Matrix Matrix::PseudoInverse()
	{
		Matrix Inv(row,1),E(row,row),AA,Inv1(row,1),Col(row,1),beta,NewInv,Pr;
		for(int i=0;i<row;i++)Inv1(i,0)=els[i][0];
		double Inorm=Inv1.SqEuclNorm();
		if(Inorm!=0)Inv=(1/Inorm)*Inv1.Transp();
		else Inv=Inv1.Transp();
		for(int i=1;i<col;i++)
		{
			//cout<<"Inv\n"<<Inv<<"\nInv1\n"<<Inv1<<"\n";
			for(int j=0;j<row;j++)Col(j,0)=els[j][i];
			//cout<<"Col\n"<<Col<<'\n';
			AA=Inv1*Inv*Col;
			//cout<<"AA\n"<<AA<<"\n\n";
			Pr=Col-AA;
			Inorm=Pr.SqEuclNorm();
			if(Inorm>=0.000000001)beta=(1/Inorm)*Pr;
			else beta=(1/(1+(Inv*Col).SqEuclNorm()))*Inv.Transp()*Inv*Col;
			//cout<<"Beta\n"<<beta<<"\n";
			Inv=Inv-Inv*Col*beta.Transp();
			//cout<<"Inv(New)\n"<<Inv<<"\n";
			NewInv.Resize(Inv.row+1,Inv.col);
			for(int j=0;j<NewInv.row;j++)
				for(int k=0;k<NewInv.col;k++)
					if(j!=NewInv.row-1)NewInv.els[j][k]=Inv.els[j][k];
					else NewInv.els[j][k]=beta.els[k][0];
			Inv=NewInv;
			NewInv.Resize(Inv1.row,i+1);
			for(int j=0;j<NewInv.row;j++)
				for(int k=0;k<NewInv.col;k++)
					NewInv.els[j][k]=els[j][k];
			Inv1=NewInv;
		}
		return Inv;
	}
>>>>>>> 5ee04f3a035fe2c3e6c7e7679da55c0ea9c11643
};