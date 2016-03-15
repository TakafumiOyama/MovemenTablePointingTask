#pragma once

#include <iostream>
#include <cmath>

class CVector
{
public:
	// �R���X�g���N�^�C�f�X�g���N�^
	CVector(double xx = 0.0, double yy = 0.0);
	CVector(const double elements[2]);  // �z�񂩂�R���X�g���N�^
	CVector(const CPoint &point);       // CPoint����R���X�g���N�^
//	CVector(const CSize &size);         // CSize����R���X�g���N�^
	CVector(const CVector &v);          // �R�s�[�R���X�g���N�^
	~CVector(void);                     // �f�X�g���N�^
	operator CPoint();                  // CPoint�ɃL���X�g

	// �m����
	double GetNorme2() const { return x * x +  y * y; };   // �m������2��
	double GetNorme() const { return sqrt(GetNorme2()); }; // �m����
	double Length() const { return GetNorme(); };          // �m�����̕ʖ�
	CVector& Normalize() { return *this /= GetNorme(); };  // ���g�𐳋K��

	// ��]
	CVector& Rotate (double theta); // theta�x��]

	// ��
	double OuterProduct(const CVector &v) const { return x * v.y - y * v.x; }; // �O��
	double InnerProduct(const CVector &v) const { return x * v.x + y * v.y; }; // ����

	// ���̑��̉��Z
	double GetAngle(const CVector &v) const;                     // 2�̃x�N�g���̂Ȃ��p�x
	double Angle(const CVector &v) const { return GetAngle(v); } // GetAngle�̕ʖ�
	double GetFullAngle(const CVector &v) const;                 // 0�`360��Ԃ�
	double DirectComponent(const CVector &v) const;              // v�ւ̎ˉe

	// ���Z�q
	CVector& operator +=(const CVector &v);          // += �����Z
	CVector& operator -=(const CVector &v);          // -= �����Z
	CVector& operator *=(const double c);            // *= �萔�{
	CVector& operator /=(const double c);            // /= �萔����Z
	BOOL     operator ==(const CVector &v) const     // == ��r
	{
		return (x == v.x && y == v.y);
	};
	BOOL     operator !=(const CVector &v) const     // != ��r
	{
		return !(*this == v);
	}; 
	CVector& operator = (const CVector &v);          // =  ���
	CVector& operator = (const double elements[2]);  // =  ���

	operator const double* () { return &x; }

	double x;  // X����
	double y;  // Y����

	static const double PI; // �~����
};

// �o�͉��Z�q
std::ostream& operator <<(std::ostream& lhs, const CVector &v);

// �O���[�o�����Z�q
CVector operator + (const CVector &v1, const CVector &v2); // + �����Z
CVector operator - (const CVector &v1, const CVector &v2); // - �����Z
CVector operator * (const CVector &v , const double c);    // * �萔�{
CVector operator / (const CVector &v , const double c);    // / �萔����Z

double  operator * (const CVector &v1, const CVector &v2); // * ����
double  operator % (const CVector &v1, const CVector &v2); // % �O��

