#include "StdAfx.h"
#include ".\vector.h"

const double CVector::PI = 3.14159265358979; // 円周率
// const double CVector::PI = 4 * atan2(1.0, 1.0);

// コンストラクタ，デストラクタ
// デフォルトコンストラクタ
CVector::CVector(double xx /* = 0.0 */, double yy /* = 0.0 */)
:x(xx), y(yy)
{

}

// 配列からコンストラクタ
CVector::CVector(const double elements[2])
:x(elements[0]), y(elements[1])
{
	
}

// CPointからコンストラクタ
CVector::CVector(const CPoint &point)
:x(point.x), y(point.y)
{

}

// CSizeからコンストラクタ
//CVector::CVector(const CSize &size)
//:x(size.cx), y(size.cy)
//{
//
//}

// コピーコンストラクタ
CVector::CVector(const CVector &v)
:x(v.x), y(v.y)
{

}

// デストラクタ
CVector::~CVector(void)
{

}

// CPointにキャスト
CVector::operator CPoint()
{
	CPoint temp;
	temp.x = static_cast<int>(x);
	temp.y = static_cast<int>(y);
	return temp;
}

// 回転
// theta度回転
CVector& CVector::Rotate (double theta)
{
	double rad = theta / 180.0 * PI;
	double sintheta = sin(rad);
	double costheta = cos(rad);

	double tempX = x;
	double tempY = y;

	x = tempX * costheta + tempY * sintheta;
	y = tempY * costheta - tempX * sintheta;

	return *this;
}

// その他の演算
// 2つのベクトルのなす角度
double CVector::GetAngle(const CVector &v) const 
{
	return acos(((*this) * v) / (GetNorme() * v.GetNorme())) * 180.0 / PI;
}

// 0～360を返す
double CVector::GetFullAngle(const CVector &v) const
{
	return (OuterProduct(v) >= 0) ? GetAngle(v) : (360.0 - GetAngle(v));
}

// vへの射影
double CVector::DirectComponent(const CVector &v) const
{
	return (*this * v) / (GetNorme() * v.GetNorme()) * GetNorme();
}

// 演算子
// += 足し算
CVector& CVector::operator +=(const CVector &v) 
{
	x += v.x;
	y += v.y;

	return *this;
}

// -= 引き算
CVector& CVector::operator -=(const CVector &v) 
{
	x -= v.x;
	y -= v.y;

	return *this;
}

// *= 定数倍
CVector& CVector::operator *=(const double c) 
{
	x *= c;
	y *= c;

	return *this;
}

// /= 定数割り算
CVector& CVector::operator /=(const double c) 
{
	ASSERT(c != 0.0);

	x /= c;
	y /= c;

	return *this;
}

// =  代入
CVector& CVector::operator = (const CVector &v) 
{
	if(this == &v) return *this;

	x = v.x;
	y = v.y;

	return *this;	
}

// =  代入
CVector& CVector::operator = (const double elements[2]) 
{
	x = elements[0];
	y = elements[1];

	return *this;
}

// 出力演算子
std::ostream& operator <<(std::ostream& lhs, const CVector &v)
{
	lhs << "x:" << v.x << ", y:" << v.y;
	return lhs;
}

// グローバル演算子
// + 足し算
CVector operator + (const CVector &v1, const CVector &v2) 
{
	CVector temp = v1;
	return temp += v2;
}

// - 引き算
CVector operator - (const CVector &v1, const CVector &v2) 
{
	CVector temp = v1;
	return temp -= v2;
}

// * 定数倍
CVector operator * (const CVector &v , const double c) 
{
	CVector temp = v;
	return temp *= c;
}

// / 定数割り算
CVector operator / (const CVector &v , const double c) 
{
	CVector temp = v;
	return temp /= c;
}

// * 内積
double operator * (const CVector &v1, const CVector &v2)
{
	return v1.InnerProduct(v2);
}

// % 外積
double operator % (const CVector &v1, const CVector &v2)
{
	return v1.OuterProduct(v2);
}

