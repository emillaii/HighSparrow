#ifndef MATRIX_H
#define MATRIX_H
#include <QString>
#include <QException>
//#pragma once
//#include "..\..\WaitForSingleObject调试\MY_ASSERT_DEF.h"
//#include "..\..\dll导入导出\DllUse.h"
//#include "..\..\缓冲型数据结构\数组模板\定长数组\StaticArray.h"

// 目前 CMatrix 只能为 double 数据类型



#define MY_ASSERT(c)\
{\
    if( ( c ) == 0 )\
            {\
        QString AssertText;\
        AssertText.asprintf("%s %s\nfunc: %s\nline: %d\n\n--------------Error\n\n", (__DATE__), (__TIME__), (__FUNCSIG__), __LINE__);\
       throw new QException();\
}\
}

class CMatrix
{
	//////////////////////
	// 内部状态
    double *	m_pData;
	int		m_nRow;
	int		m_nCol;

public:
	CMatrix(void);
	CMatrix( CMatrix& c );
	~CMatrix(void);
	CMatrix(int nRow, int nCol, const double * pdata);

	////////////////////////////
	// 运算符重载
	double*	operator() ( int iCol );
	double&	operator() (int iRow, int iCol);
	const double&	operator() (int iRow, int iCol)const;
	CMatrix&	operator = ( const CMatrix& c );
	int operator==(const CMatrix& c)const;
	
	CMatrix operator*(double fVal)const;
	///////////////////////////
	// 用户函数
	// 获取矩阵数据指针
	double*	GetMatrixData();
	const double* GetMatrixData()const;
	// 创建矩阵
	void CreateMatrix( int nRow , int nCol );
	void SetZero();
	void SetOne();
	int SetData(const double* pData);
	// 求矩阵的逆矩阵
	CMatrix	GetInvertMatrix();
	CMatrix GetPInv();
	// 矩阵左除
	CMatrix	LeftDiv(  CMatrix& cLeftDivM );
	// 矩阵右除
	CMatrix	RightDiv( CMatrix& cRightDivM );
	// 获得行列式的值( det )
	double	GetDet();
	// 奇异值分解 .
//	CStaticArray<CMatrix, 3> GetSvd()const;
	// 矩阵乘
	CMatrix	operator* ( const CMatrix& c )const;
	CMatrix	operator-(const CMatrix& c)const;
	// 获取矩阵中绝对值最大的元素的值
    double	GetMaxAbsVal();
	// 获取矩阵个的行数
	int	GetMatrixRowCount()const;
	// 获取矩阵个的列数
	int	GetMatrixColCount()const;
	// 转置矩阵 .
	CMatrix GetTransPos()const;
};
#endif
