#include "StdAfx.h"
#include "Vector3D.h"

const double CVector3D::PI = 3.14159265358979; // �~����

// �R���X�g���N�^�C�f�X�g���N�^
// �f�t�H���g�R���X�g���N�^
CVector3D::CVector3D(double x /* = 0.0 */, double y /* = 0.0 */, double z /* = 0.0 */)
{
	m_Elements[0] = x;
	m_Elements[1] = y;
	m_Elements[2] = z;
}

// �z�񂩂�R���X�g���N�^
CVector3D::CVector3D(const double elements[3])
{
	m_Elements[0] = elements[0];
	m_Elements[1] = elements[1];	
	m_Elements[2] = elements[2];
}

//// �R�s�[�R���X�g���N�^
//CVector3D::CVector3D(const CVector3D &v) 
//{
//	m_Elements[0] = v.X();
//	m_Elements[1] = v.Y();
//	m_Elements[2] = v.Z();
//}

// �f�X�g���N�^
CVector3D::~CVector3D(void)
{

}

// ��]
// axis����theta�x��]�i���Ɛ����̏ꍇ�̂݁C�S�����x���ԃJ�b�g�j
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

// axis����theta�x��]
CVector3D& CVector3D::Rotate2(double theta, const CVector3D& axis)
{
	double rad = PI / 180.0 * theta;
	*this = (*this) * cos(rad) + axis * (*this) * sin(rad);

	return *this;
}

// ��
// �O��
CVector3D CVector3D::OutorProduct(const CVector3D &v) const
{
	return CVector3D(
		m_Elements[1] * v.Z() - m_Elements[2] * v.Y(),
		m_Elements[2] * v.X() - m_Elements[0] * v.Z(),
		m_Elements[0] * v.Y() - m_Elements[1] * v.X()
		);
}

// ����
double CVector3D::InnerProduct(const CVector3D &v) const 
{
	return m_Elements[0] * v.X() + m_Elements[1] * v.Y() + m_Elements[2] * v.Z();
}

// ���̑��̉��Z
// 2�̃x�N�g���̂Ȃ��p�x
double CVector3D::GetAngle(const CVector3D &v) const 
{
	return acos(((*this) % v) / (this->GetNorme() * v.GetNorme())) * 180.0 / PI;
}

// ���Z�q
// += �����Z
CVector3D& CVector3D::operator +=(const CVector3D &v) 
{
	m_Elements[0] += v.X();
	m_Elements[1] += v.Y();
	m_Elements[2] += v.Z();

	return *this;
}

// -= �����Z
CVector3D& CVector3D::operator -=(const CVector3D &v) 
{
	m_Elements[0] -= v.X();
	m_Elements[1] -= v.Y();
	m_Elements[2] -= v.Z();

	return *this;
}

// *= �萔�{
CVector3D& CVector3D::operator *=(const double c) 
{
	m_Elements[0] *= c;
	m_Elements[1] *= c;
	m_Elements[2] *= c;

	return *this;
}

// /= �萔����Z
CVector3D& CVector3D::operator /=(const double c) 
{
	ASSERT(c != 0.0);

	m_Elements[0] /= c;
	m_Elements[1] /= c;
	m_Elements[2] /= c;

	return *this;
}

// =  ���
CVector3D& CVector3D::operator = (const CVector3D &v) 
{
	if(this == &v) return *this;

	m_Elements[0] = v.X();
	m_Elements[1] = v.Y();
	m_Elements[2] = v.Z();

	return *this;	
}

// =  ���
CVector3D& CVector3D::operator = (const double elements[3]) 
{
	m_Elements[0] = elements[0];
	m_Elements[1] = elements[1];
	m_Elements[2] = elements[2];

	return *this;
}

// �o�͉��Z�q
std::ostream& operator <<(std::ostream& lhs, const CVector3D &v)
{
	lhs << "x:" << v.X() << ", y:" << v.Y() << ", z:" << v.Z();
	return lhs;
}

// �O���[�o�����Z�q
// + �����Z
CVector3D operator + (const CVector3D &v1, const CVector3D &v2) 
{
	CVector3D temp = v1;
	return temp += v2;
}

// - �����Z
CVector3D operator - (const CVector3D &v1, const CVector3D &v2) 
{
	CVector3D temp = v1;
	return temp -= v2;
}

// * �萔�{
CVector3D operator * (const CVector3D &v , const double c) 
{
	CVector3D temp = v;
	return temp *= c;
}

// / �萔����Z
CVector3D operator / (const CVector3D &v , const double c) 
{
	CVector3D temp = v;
	return temp /= c;
}

// * �O��
CVector3D operator * (const CVector3D &v1, const CVector3D &v2)
{
	return v2.OutorProduct(v1);
}

// % ����
double operator % (const CVector3D &v1, const CVector3D &v2)
{
	return v1.InnerProduct(v2);
}

