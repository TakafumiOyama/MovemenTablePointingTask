#include "StdAfx.h"
#include "Vector3D.h"

const double CVector3D::PI = 3.14159265358979; // 円周率

// コンストラクタ，デストラクタ
// デフォルトコンストラクタ
CVector3D::CVector3D(double x /* = 0.0 */, double y /* = 0.0 */, double z /* = 0.0 */)
{
	m_Elements[0] = x;
	m_Elements[1] = y;
	m_Elements[2] = z;
}

// 配列からコンストラクタ
CVector3D::CVector3D(const double elements[3])
{
	m_Elements[0] = elements[0];
	m_Elements[1] = elements[1];	
	m_Elements[2] = elements[2];
}

//// コピーコンストラクタ
//CVector3D::CVector3D(const CVector3D &v) 
//{
//	m_Elements[0] = v.X();
//	m_Elements[1] = v.Y();
//	m_Elements[2] = v.Z();
//}

// デストラクタ
CVector3D::~CVector3D(void)
{

}

// 回転
// axis軸でtheta度回転（軸と垂直の場合のみ，４割程度時間カット）
CVector3D& CVector3D::Rotate (double theta, const CVector3D& axis)
{
	double rad = PI / 180.0 * theta;
	double sintheta   = sin(rad / 2);
	double sintheta2  = sintheta * sintheta;
	double costheta   = cos(rad / 2);
	double costheta2  = costheta * costheta;
	double axisNorme2 = axis.GetNorme2();

	*this = axis * (2 * sintheta2 * ((*this) % axis))
		  + (*this) * (costheta2 - sintheta2 * axisNorme2)
		  - (*this) * axis * (2 * sintheta * costheta);

	return *this;
}

// axis軸でtheta度回転
CVector3D& CVector3D::Rotate2(double theta, const CVector3D& axis)
{
	double rad = PI / 180.0 * theta;
	*this = (*this) * cos(rad) + axis * (*this) * sin(rad);

	return *this;
}

// 積
// 外積
CVector3D CVector3D::OutorProduct(const CVector3D &v) const
{
	return CVector3D(
		m_Elements[1] * v.Z() - m_Elements[2] * v.Y(),
		m_Elements[2] * v.X() - m_Elements[0] * v.Z(),
		m_Elements[0] * v.Y() - m_Elements[1] * v.X()
		);
}

// 内積
double CVector3D::InnerProduct(const CVector3D &v) const 
{
	return m_Elements[0] * v.X() + m_Elements[1] * v.Y() + m_Elements[2] * v.Z();
}

// その他の演算
// 2つのベクトルのなす角度
double CVector3D::GetAngle(const CVector3D &v) const 
{
	return acos(((*this) % v) / (this->GetNorme() * v.GetNorme())) * 180.0 / PI;
}

// 演算子
// += 足し算
CVector3D& CVector3D::operator +=(const CVector3D &v) 
{
	m_Elements[0] += v.X();
	m_Elements[1] += v.Y();
	m_Elements[2] += v.Z();

	return *this;
}

// -= 引き算
CVector3D& CVector3D::operator -=(const CVector3D &v) 
{
	m_Elements[0] -= v.X();
	m_Elements[1] -= v.Y();
	m_Elements[2] -= v.Z();

	return *this;
}

// *= 定数倍
CVector3D& CVector3D::operator *=(const double c) 
{
	m_Elements[0] *= c;
	m_Elements[1] *= c;
	m_Elements[2] *= c;

	return *this;
}

// /= 定数割り算
CVector3D& CVector3D::operator /=(const double c) 
{
	ASSERT(c != 0.0);

	m_Elements[0] /= c;
	m_Elements[1] /= c;
	m_Elements[2] /= c;

	return *this;
}

// =  代入
CVector3D& CVector3D::operator = (const CVector3D &v) 
{
	if(this == &v) return *this;

	m_Elements[0] = v.X();
	m_Elements[1] = v.Y();
	m_Elements[2] = v.Z();

	return *this;	
}

// =  代入
CVector3D& CVector3D::operator = (const double elements[3]) 
{
	m_Elements[0] = elements[0];
	m_Elements[1] = elements[1];
	m_Elements[2] = elements[2];

	return *this;
}

// 出力演算子
std::ostream& operator <<(std::ostream& lhs, const CVector3D &v)
{
	lhs << "x:" << v.X() << ", y:" << v.Y() << ", z:" << v.Z();
	return lhs;
}

// グローバル演算子
// + 足し算
CVector3D operator + (const CVector3D &v1, const CVector3D &v2) 
{
	CVector3D temp = v1;
	return temp += v2;
}

// - 引き算
CVector3D operator - (const CVector3D &v1, const CVector3D &v2) 
{
	CVector3D temp = v1;
	return temp -= v2;
}

// * 定数倍
CVector3D operator * (const CVector3D &v , const double c) 
{
	CVector3D temp = v;
	return temp *= c;
}

// / 定数割り算
CVector3D operator / (const CVector3D &v , const double c) 
{
	CVector3D temp = v;
	return temp /= c;
}

// * 外積
CVector3D operator * (const CVector3D &v1, const CVector3D &v2)
{
	return v2.OutorProduct(v1);
}

// % 内積
double operator % (const CVector3D &v1, const CVector3D &v2)
{
	return v1.InnerProduct(v2);
}

