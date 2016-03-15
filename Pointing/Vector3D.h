#pragma once

#include <iostream>
#include <cmath>

class CVector3D
{
public:
	// コンストラクタ，デストラクタ
	CVector3D(double x = 0.0, double y = 0.0, double z = 0.0);
	CVector3D(const double elements[3]);  // 配列からコンストラクタ
	//CVector3D(const CVector &v);          // コピーコンストラクタ
	~CVector3D(void);

	// セッター
	void X(const double x) { m_Elements[0] = x; };
	void Y(const double y) { m_Elements[1] = y; };
	void Z(const double z) { m_Elements[2] = z; };
	double& X() { return m_Elements[0]; };
	double& Y() { return m_Elements[1]; };
	double& Z() { return m_Elements[2]; };

	// ゲッター
	const double X() const { return m_Elements[0]; };
	const double Y() const { return m_Elements[1]; };
	const double Z() const { return m_Elements[2]; };
	const double * V() const { return m_Elements; };

	// ノルム
	double GetNorme2() const                               // ノルムの2乗
	{
		return m_Elements[0] * m_Elements[0] +  m_Elements[1] * m_Elements[1] + m_Elements[2] * m_Elements[2];
	};
	double GetNorme() const { return sqrt(GetNorme2()); }; // ノルム
	CVector3D& Normalize() { return *this /= GetNorme(); };  // 自身を正規化

	// 回転
	CVector3D& Rotate (double theta, const CVector3D& axis); // axis軸でtheta度回転
	                                                     //（軸と垂直の場合のみ，４割程度時間カット）
	CVector3D& Rotate2(double theta, const CVector3D& axis); // axis軸でtheta度回転

	// 積
	CVector3D OutorProduct(const CVector3D &v) const;   // 外積
	double  InnerProduct(const CVector3D &v) const;   // 内積

	// その他の演算
	double GetAngle(const CVector3D &v) const; // 2つのベクトルのなす角度

	// 演算子
	CVector3D& operator +=(const CVector3D &v);          // += 足し算
	CVector3D& operator -=(const CVector3D &v);          // -= 引き算
	CVector3D& operator *=(const double c);            // *= 定数倍
	CVector3D& operator /=(const double c);            // /= 定数割り算
	BOOL     operator ==(const CVector3D &v) const     // == 比較
	{
		return (m_Elements[0] == v.X() && m_Elements[1] == v.Y() && m_Elements[2] == v.Z());
	};
	BOOL     operator !=(const CVector3D &v) const     // != 比較
	{
		return !(*this == v);
	}; 
	CVector3D& operator = (const CVector3D &v);          // =  代入
	CVector3D& operator = (const double elements[3]);  // =  代入

	// Z軸からのなす角を求める関数
	double AngleFromZAxis(){return -180/PI * atan(sqrt((m_Elements[0] * m_Elements[0]) + (m_Elements[1] * m_Elements[1])) / m_Elements[2]);}

	// X軸からのなす角を求める関数
	double AngleFromXAxis(){
		if(m_Elements[1] >= 0)return 180/PI * acos(m_Elements[0] / sqrt((m_Elements[0] * m_Elements[0]) + (m_Elements[1] * m_Elements[1])));
		else			return 360 - 180/PI * acos(m_Elements[0] / sqrt((m_Elements[0] * m_Elements[0]) + (m_Elements[1] * m_Elements[1])));
	}

private:
	double m_Elements[3];   // ベクトルのx, y, z
	static const double PI; // 円周率
public:
};

// 出力演算子
std::ostream& operator <<(std::ostream& lhs, const CVector3D &v);

// グローバル演算子
CVector3D operator + (const CVector3D &v1, const CVector3D &v2); // + 足し算
CVector3D operator - (const CVector3D &v1, const CVector3D &v2); // - 引き算
CVector3D operator * (const CVector3D &v , const double c);    // * 定数倍
CVector3D operator / (const CVector3D &v , const double c);    // / 定数割り算

CVector3D operator * (const CVector3D &v1, const CVector3D &v2); // * 外積
double  operator % (const CVector3D &v1, const CVector3D &v2); // % 内積



