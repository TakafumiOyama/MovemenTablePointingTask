#pragma once

#include <iostream>
#include <cmath>

class CVector3D
{
public:
	// �R���X�g���N�^�C�f�X�g���N�^
	CVector3D(double x = 0.0, double y = 0.0, double z = 0.0);
	CVector3D(const double elements[3]);  // �z�񂩂�R���X�g���N�^
	//CVector3D(const CVector &v);          // �R�s�[�R���X�g���N�^
	~CVector3D(void);

	// �Z�b�^�[
	void X(const double x) { m_Elements[0] = x; };
	void Y(const double y) { m_Elements[1] = y; };
	void Z(const double z) { m_Elements[2] = z; };
	double& X() { return m_Elements[0]; };
	double& Y() { return m_Elements[1]; };
	double& Z() { return m_Elements[2]; };

	// �Q�b�^�[
	const double X() const { return m_Elements[0]; };
	const double Y() const { return m_Elements[1]; };
	const double Z() const { return m_Elements[2]; };
	const double * V() const { return m_Elements; };

	// �m����
	double GetNorme2() const                               // �m������2��
	{
		return m_Elements[0] * m_Elements[0] +  m_Elements[1] * m_Elements[1] + m_Elements[2] * m_Elements[2];
	};
	double GetNorme() const { return sqrt(GetNorme2()); }; // �m����
	CVector3D& Normalize() { return *this /= GetNorme(); };  // ���g�𐳋K��

	// ��]
	CVector3D& Rotate (double theta, const CVector3D& axis); // axis����theta�x��]
	                                                     //�i���Ɛ����̏ꍇ�̂݁C�S�����x���ԃJ�b�g�j
	CVector3D& Rotate2(double theta, const CVector3D& axis); // axis����theta�x��]

	// ��
	CVector3D OutorProduct(const CVector3D &v) const;   // �O��
	double  InnerProduct(const CVector3D &v) const;   // ����

	// ���̑��̉��Z
	double GetAngle(const CVector3D &v) const; // 2�̃x�N�g���̂Ȃ��p�x

	// ���Z�q
	CVector3D& operator +=(const CVector3D &v);          // += �����Z
	CVector3D& operator -=(const CVector3D &v);          // -= �����Z
	CVector3D& operator *=(const double c);            // *= �萔�{
	CVector3D& operator /=(const double c);            // /= �萔����Z
	BOOL     operator ==(const CVector3D &v) const     // == ��r
	{
		return (m_Elements[0] == v.X() && m_Elements[1] == v.Y() && m_Elements[2] == v.Z());
	};
	BOOL     operator !=(const CVector3D &v) const     // != ��r
	{
		return !(*this == v);
	}; 
	CVector3D& operator = (const CVector3D &v);          // =  ���
	CVector3D& operator = (const double elements[3]);  // =  ���

	// Z������̂Ȃ��p�����߂�֐�
	double AngleFromZAxis(){return -180/PI * atan(sqrt((m_Elements[0] * m_Elements[0]) + (m_Elements[1] * m_Elements[1])) / m_Elements[2]);}

	// X������̂Ȃ��p�����߂�֐�
	double AngleFromXAxis(){
		if(m_Elements[1] >= 0)return 180/PI * acos(m_Elements[0] / sqrt((m_Elements[0] * m_Elements[0]) + (m_Elements[1] * m_Elements[1])));
		else			return 360 - 180/PI * acos(m_Elements[0] / sqrt((m_Elements[0] * m_Elements[0]) + (m_Elements[1] * m_Elements[1])));
	}

private:
	double m_Elements[3];   // �x�N�g����x, y, z
	static const double PI; // �~����
public:
};

// �o�͉��Z�q
std::ostream& operator <<(std::ostream& lhs, const CVector3D &v);

// �O���[�o�����Z�q
CVector3D operator + (const CVector3D &v1, const CVector3D &v2); // + �����Z
CVector3D operator - (const CVector3D &v1, const CVector3D &v2); // - �����Z
CVector3D operator * (const CVector3D &v , const double c);    // * �萔�{
CVector3D operator / (const CVector3D &v , const double c);    // / �萔����Z

CVector3D operator * (const CVector3D &v1, const CVector3D &v2); // * �O��
double  operator % (const CVector3D &v1, const CVector3D &v2); // % ����



