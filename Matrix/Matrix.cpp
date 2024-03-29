#include "Matrix.h"

namespace MyMathFunction_MatlabNameSpace
{
	// 此内文件不要添加到工程 , 避免函数重复定义 .
#include "MyMathFunction\MyMathFunction.h"

#include "MyMathFunction\MyMathFunction.cpp"
#include "MyMathFunction\rt_nonfinite.cpp"
#include "MyMathFunction\rtGetInf.cpp"
#include "MyMathFunction\rtGetNaN.cpp"
}

using namespace MyMathFunction_MatlabNameSpace;


CMatrix::CMatrix(void)
{
	m_pData = 0 ; m_nRow = 0 ; m_nCol  =0;
	MyMathFunction_initialize();
}

CMatrix::CMatrix( CMatrix& c )
{
	m_nRow = c.m_nRow;
	m_nCol = c.m_nCol;
	int		nDataCount = m_nRow * m_nCol;
	if ( nDataCount )
	{
		m_pData = new double[nDataCount];
// 		TRACE1( "= new : %x\n" , (UINT)m_pData );
		memcpy( m_pData , c.m_pData , nDataCount * sizeof(double) );
	}
	else
		m_pData = 0;
}

CMatrix::CMatrix(int nRow, int nCol, const double * pdata)
{
    m_pData = 0 ; m_nRow = 0 ; m_nCol  =0;
	CreateMatrix(nRow, nCol);
    SetData(pdata);
}

CMatrix::~CMatrix( void )
{
	if ( m_pData )
	{
// 		TRACE1( "delete : %x\n" , (UINT)m_pData );
			delete []  m_pData;
		m_pData = 0;
	}
}

double* CMatrix::operator()( int iCol )
{
	MY_ASSERT( iCol < m_nRow );
	return	m_pData + m_nRow * iCol;
}

double& CMatrix::operator()( int iRow , int iCol )
{
	MY_ASSERT( iRow < m_nRow );
	MY_ASSERT( iCol < m_nCol );
	return	m_pData[ m_nRow * iCol + iRow ];
}

const double& CMatrix::operator()(int iRow, int iCol) const
{
	MY_ASSERT(iRow < m_nRow);
	MY_ASSERT(iCol < m_nCol);
	return	m_pData[m_nRow * iCol + iRow];
}

double* CMatrix::GetMatrixData()
{
	return	m_pData;
}

const double* CMatrix::GetMatrixData() const
{
	return	m_pData;
}

void CMatrix::CreateMatrix( int nRow , int nCol )
{
	int nDataCount = nRow * nCol;
	if(0 != m_pData)
	{
		delete[] m_pData;
	}
	m_pData = new double[nDataCount];
// 	TRACE1( "Create new : %x\n" , (UINT)m_pData );

	m_nCol = nCol ;
	m_nRow = nRow;
}

CMatrix CMatrix::GetInvertMatrix()
{
	MY_ASSERT( m_nCol == m_nRow );
	MY_ASSERT( m_nCol != 0 );
	CMatrix		cM;
	cM.CreateMatrix( m_nRow , m_nCol );
	emxArray_real_T	M , y;
	M.allocatedSize = m_nCol * m_nRow;
	M.canFreeData = 0;
	M.data = m_pData;
	M.numDimensions = 2;
	M.size = new int32_T[2];
	M.size[0] = m_nRow;
	M.size[1] = m_nCol;

	y.allocatedSize = M.allocatedSize;
	y.canFreeData = 0;
	y.data = cM.GetMatrixData();
	y.numDimensions = 2;
	y.size = new int32_T[2];
	y.size[0] = m_nRow;
	y.size[1] = m_nCol;

	my_inv( &M , &y );

	delete	M.size;
	delete	y.size;

	return	cM;
}

CMatrix& CMatrix::operator=( const CMatrix& c )
{
	m_nRow = c.m_nRow;
	m_nCol = c.m_nCol;
	int		nDataCount = m_nRow * m_nCol;
	if ( nDataCount )
	{
		if ( m_pData )
			delete	m_pData;
		m_pData = new double[nDataCount];
// 		TRACE1( "= new : %x\n" , (UINT)m_pData );
		memcpy( m_pData , c.m_pData , nDataCount * sizeof(double) );
	}
	else
	{
		if ( m_pData ){
			delete	m_pData;
			m_pData = 0;
		}
	}
	return *this;
}

CMatrix CMatrix::LeftDiv( CMatrix& cLeftDivM )
{
	// 必须保证 cSourceM 和 cLeftDivM 的行数相等
	MY_ASSERT( m_nRow == cLeftDivM.m_nRow );
	MY_ASSERT( m_nRow != 0 );
	MY_ASSERT( m_nCol !=  0 );
	MY_ASSERT( cLeftDivM.m_nCol !=  0 );
	
	emxArray_real_T	M , divM, y;
	M.allocatedSize = m_nCol * m_nRow;
	M.canFreeData = 0;
	M.data = m_pData;
	M.numDimensions = 2;
	M.size = new int32_T[2];
	M.size[0] = m_nRow;
	M.size[1] = m_nCol;

	divM.allocatedSize = cLeftDivM.m_nCol * cLeftDivM.m_nRow;
	divM.allocatedSize = 0;
	divM.data = cLeftDivM.m_pData;
	divM.numDimensions = 2;
	divM.size = new int32_T[2];
	divM.size[0] = cLeftDivM.m_nRow;
	divM.size[1] = cLeftDivM.m_nCol;

	CMatrix cM;
	cM.CreateMatrix( cLeftDivM.m_nCol , m_nCol );
	y.allocatedSize = cLeftDivM.m_nCol*m_nCol;
	y.canFreeData = 0;
	y.data = cM.GetMatrixData();
	y.numDimensions = 2;
	y.size = new int32_T[2];
	y.size[0] = cLeftDivM.m_nCol;
	y.size[1] = m_nCol;

	my_left_div( &M, &divM , &y );

	delete	M.size;
	delete	y.size;
	delete	divM.size;

	return cM;
}

CMatrix CMatrix::RightDiv( CMatrix& cRightDivM )
{
	MY_ASSERT( m_nCol == cRightDivM.m_nCol );
	MY_ASSERT( m_nRow != 0 );
	MY_ASSERT( m_nCol !=  0 );
	MY_ASSERT( cRightDivM.m_nCol !=  0 );

	CMatrix cM;
	emxArray_real_T	M , divM, y;

	M.allocatedSize = m_nCol * m_nRow;
	M.canFreeData = 0;
	M.data = m_pData;
	M.numDimensions = 2;
	M.size = new int32_T[2];
	M.size[0] = m_nRow;
	M.size[1] = m_nCol;

	divM.allocatedSize = cRightDivM.m_nCol * cRightDivM.m_nRow;
	divM.allocatedSize = 0;
	divM.data = cRightDivM.m_pData;
	divM.numDimensions = 2;
	divM.size = new int32_T[2];
	divM.size[0] = cRightDivM.m_nRow;
	divM.size[1] = cRightDivM.m_nCol;

	cM.CreateMatrix( m_nRow , cRightDivM.m_nRow );
	y.allocatedSize = m_nRow * cRightDivM.m_nRow ;
	y.canFreeData = 0;
	y.data = cM.GetMatrixData();
	y.numDimensions = 2;
	y.size = new int32_T[2];
	y.size[0] = m_nRow;
	y.size[1] = cRightDivM.m_nRow;

	my_right_div( &M , &divM , &y );

	delete	M.size;
	delete	y.size;
	delete	divM.size;
	return	cM;
}

double CMatrix::GetDet()
{
	MY_ASSERT( m_nRow == m_nCol );
	emxArray_real_T	M1;
	M1.allocatedSize = m_nCol * m_nRow;
	M1.canFreeData = 0;
	M1.data = m_pData;
	M1.numDimensions = 2;
	M1.size = new int32_T[2];
	M1.size[0] = m_nRow;
	M1.size[1] = m_nCol;

	double	fres = my_det( &M1 );

	delete	M1.size;

	return fres;
}

CMatrix CMatrix::operator*( const CMatrix& c )const
{
	MY_ASSERT( m_nCol == c.m_nRow );
	emxArray_real_T	M1 , M2, y;
	M1.allocatedSize = m_nCol * m_nRow;
	M1.canFreeData = 0;
	M1.data = m_pData;
	M1.numDimensions = 2;
	M1.size = new int32_T[2];
	M1.size[0] = m_nRow;
	M1.size[1] = m_nCol;

	M2.allocatedSize = c.m_nCol * c.m_nRow;
	M2.allocatedSize = 0;
	M2.data = c.m_pData;
	M2.numDimensions = 2;
	M2.size = new int32_T[2];
	M2.size[0] = c.m_nRow;
	M2.size[1] = c.m_nCol;

	CMatrix	cM;
	cM.CreateMatrix( m_nRow , c.m_nCol );
	y.allocatedSize = m_nRow * c.m_nCol;
	y.canFreeData = 0;
	y.data = cM.GetMatrixData();
	y.numDimensions =2;
	y.size = new int32_T[2];
	y.size[0] = m_nRow;
	y.size[1] = c.m_nCol;

	my_mul( &M1 , &M2 , &y );

	delete	M1.size;
	delete	M2.size;
	delete	y.size;

	return cM;

}

CMatrix CMatrix::operator*(double fVal) const
{
	CMatrix cMatx;

	cMatx.CreateMatrix(m_nRow, m_nCol);
	int		nAllCount = m_nRow * m_nCol;
	for (int i = 0; i < nAllCount; i++)
	{
		cMatx.m_pData[i] = m_pData[i] * fVal;
	}

	return cMatx;
}

double CMatrix::GetMaxAbsVal()
{
	int		nAllCount = m_nRow * m_nCol;
	double fmaxval = -1;
	for( int i = 0 ; i < nAllCount ; i++ )
	{
		double s = fabs( m_pData[i] );
		if ( s > fmaxval )
		{
			fmaxval = s;
		}
	}
	return fmaxval;
}

CMatrix CMatrix::GetPInv()
{
	MY_ASSERT(m_nRow != 0);
	MY_ASSERT(m_nCol != 0);
	CMatrix		cM;
	cM.CreateMatrix(m_nRow, m_nCol);
	emxArray_real_T	M, y;
	M.allocatedSize = m_nCol * m_nRow;
	M.canFreeData = 0;
	M.data = m_pData;
	M.numDimensions = 2;
	M.size = new int32_T[2];
	M.size[0] = m_nRow;
	M.size[1] = m_nCol;

	y.allocatedSize = M.allocatedSize;
	y.canFreeData = 0;
	y.data = cM.GetMatrixData();
	y.numDimensions = 2;
	y.size = new int32_T[2];
	y.size[0] = m_nRow;
	y.size[1] = m_nCol;

	my_pinv(&M, &y);

	delete	M.size;
	delete	y.size;

	return	cM;
}

int CMatrix::operator==(const CMatrix& c) const
{
	if (!(m_nRow == c.m_nRow))
	{
		return 0;
	}
	if (!(m_nCol == c.m_nCol))
	{
		return 0;
	}

	for (int i = 0; i < m_nRow * m_nCol; i++)
	{
		if (!(m_pData[i] == c.m_pData[i]))
		{
			return 0;
		}
	}

	return 1;
}

CMatrix CMatrix::operator-(const CMatrix& c)const
{
	MY_ASSERT(m_nRow == c.m_nRow);
	MY_ASSERT(m_nCol == c.m_nCol);
	int		nAllCount = m_nRow * m_nCol;
	CMatrix	cM;
	cM.CreateMatrix(m_nRow, m_nCol);
	double* pTargetdata = cM.GetMatrixData();
	const double*	pSubdata = c.GetMatrixData();
	for (int i = 0; i < nAllCount; i++)
	{
		pTargetdata[i] = m_pData[i] - pSubdata[i];
	}
	return	cM;
}

void CMatrix::SetZero()
{
	for (int i = 0; i < m_nRow * m_nCol; i++)
	{
		m_pData[i] = 0;
	}
}

void CMatrix::SetOne()
{
	SetZero();
    for (int i = 0; i < std::min(m_nRow, m_nCol); i++)
	{
		m_pData[i * m_nRow + i] = 1;
	}
}

int CMatrix::SetData(const double* pData)
{
	if (m_pData && pData)
	{
		int nDataCount = m_nRow * m_nCol;
		memcpy(m_pData, pData, nDataCount * sizeof(double));
		return 1;
	}
	return 0;
}

int CMatrix::GetMatrixRowCount() const
{
	return	m_nRow;
}

int CMatrix::GetMatrixColCount() const
{
	return	m_nCol;
}

CMatrix CMatrix::GetTransPos() const
{
	CMatrix	cM;
	cM.CreateMatrix(m_nCol, m_nRow);
	for (int i = 0; i < m_nRow; i++)
	{
		for (int j = 0; j < m_nCol; j++)
		{
			cM(j, i) = (*this)(i, j);
		}
	}
	return cM;
}

//CStaticArray<CMatrix, 3> CMatrix::GetSvd() const
//{
//	MY_ASSERT(m_nRow != 0);
//	MY_ASSERT(m_nCol != 0);
//	emxArray_real_T	M;
//	M.allocatedSize = m_nCol * m_nRow;
//	M.canFreeData = 0;
//	M.data = m_pData;
//	M.numDimensions = 2;
//	M.size = new int32_T[2];
//	M.size[0] = m_nRow;
//	M.size[1] = m_nCol;

//	CStaticArray<CMatrix, 3> cU_S_V;
//	cU_S_V[0].CreateMatrix(m_nRow, m_nRow);
//	cU_S_V[1].CreateMatrix(m_nRow, m_nCol);
//	cU_S_V[2].CreateMatrix(m_nCol, m_nCol);
//	CStaticArray<emxArray_real_T, 3> U_S_V;
//	for (int i = 0; i < 3; i++)
//	{
//		U_S_V[i].allocatedSize = cU_S_V[i].m_nRow * cU_S_V[i].m_nCol;
//		U_S_V[i].canFreeData = 0;
//		U_S_V[i].data = cU_S_V[i].GetMatrixData();
//		U_S_V[i].numDimensions = 2;
//		U_S_V[i].size = new int32_T[2];
//		U_S_V[i].size[0] = cU_S_V[i].m_nRow;
//		U_S_V[i].size[1] = cU_S_V[i].m_nCol;

//	}

//	my_svd(&M, &U_S_V[0], &U_S_V[1], &U_S_V[2]);

//	for (int i = 0; i < 3; i++)
//	{
//		delete U_S_V[i].size;
//	}

//	return	cU_S_V;
//}
