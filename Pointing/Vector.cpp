#include "StdAfx.h"
#include ".\vector.h"

const double CVector::PI = 3.14159265358979; // �~����
// const double CVector::PI = 4 * atan2(1.0, 1.0);

// �R���X�g���N�^�C�f�X�g���N�^
// �f�t�H���g�R���X�g���N�^
CVector::CVector(double xx /* = 0.0 */, double yy /* = 0.0 */)
:x(xx), y(yy)
{

}

// �z�񂩂�R���X�g���N�^
CVector::CVector(const double elements[2])
:x(elements[0]), y(elements[1])
{
	
}

// CPoint����R���X�g���N�^
CVector::CVector(const CPoint &point)
:x(point.x), y(point.y)
{

}

// CSize����R���X�g���N�^
//CVector::CVector(const CSize &size)
//:x(size.cx), y(size.cy)
//{
//
//}

// �R�s�[�R���X�g���N�^
CVector::CVector(const CVector &v)
:x(v.x), y(v.y)
{

}

// �f�X�g���N�^
CVector::~CVector(void)
{

}

// CPoint�ɃL���X�g
CVector::operator CPoint()
{
	CPoint temp;
	temp.x = static_cast<int>(x);
	temp.y = static_cast<int>(y);
	return temp;
}

// ��]
// theta�x��]
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

// ���̑��̉��Z
// 2�̃x�N�g���̂Ȃ��p�x
double CVector::GetAngle(const CVector &v) const 
{
	return acos(((*this) * v) / (GetNorme() * v.GetNorme())) * 180.0 / PI;
}

// 0�`360��Ԃ�
double CVector::GetFullAngle(const CVector &v) const
{
	return (OuterProduct(v) >= 0) ? GetAngle(v) : (360.0 - GetAngle(v));
}

// v�ւ̎ˉe
double CVector::DirectComponent(const CVector &v) const
{
	return (*this * v) / (GetNorme() * v.GetNorme()) * GetNorme();
}

// ���Z�q
// += �����Z
CVector& CVector::operator +=(const CVector &v) 
{
	x += v.x;
	y += v.y;

	return *this;
}

// -= �����Z
CVector& CVector::operator -=(const CVector &v) 
{
	x -= v.x;
	y -= v.y;

	return *this;
}

// *= �萔�{
CVector& CVector::operator *=(const double c) 
{
	x *= c;
	y *= c;

	return *this;
}

// /= �萔����Z
CVector& CVector::operator /=(const double c) 
{
	ASSERT(c != 0.0);

	x /= c;
	y /= c;

	return *this;
}

// =  ���
CVector& CVector::operator = (const CVector &v) 
{
	if(this == &v) return *this;

	x = v.x;
	y = v.y;

	return *this;	
}

// =  ���
CVector& CVector::operator = (const double elements[2]) 
{
	x = elements[0];
	y = elements[1];

	return *this;
}

// �o�͉��Z�q
std::ostream& operator <<(std::ostream& lhs, const CVector &v)
{
	lhs << "x:" << v.x << ", y:" << v.y;
	return lhs;
}

// �O���[�o�����Z�q
// + �����Z
CVector operator + (const CVector &v1, const CVector &v2) 
{
	CVector temp = v1;
	return temp += v2;
}

// - �����Z
CVector operator - (const CVector &v1, const CVector &v2) 
{
	CVector temp = v1;
	return temp -= v2;
}

// * �萔�{
CVector operator * (const CVector &v , const double c) 
{
	CVector temp = v;
	return temp *= c;
}

// / �萔����Z
CVector operator / (const CVector &v , const double c) 
{
	CVector temp = v;
	return temp /= c;
}

// * ����
double operator * (const CVector &v1, const CVector &v2)
{
	return v1.InnerProduct(v2);
}

// % �O��
double operator % (const CVector &v1, const CVector &v2)
{
	return v1.OuterProduct(v2);
}

