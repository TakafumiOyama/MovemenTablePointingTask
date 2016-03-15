#pragma once

#include <iostream>
#include <cmath>

class CVector
{
public:
	// コンストラクタ，デストラクタ
	CVector(double xx = 0.0, double yy = 0.0);
	CVector(const double elements[2]);  // 配列からコンストラクタ
	CVector(const CPoint &point);       // CPointからコンストラクタ
//	CVector(const CSize &size);         // CSizeからコンストラクタ
	CVector(const CVector &v);          // コピーコンストラクタ
	~CVector(void);                     // デストラクタ
	operator CPoint();                  // CPointにキャスト

	// ノルム
	double GetNorme2() const { return x * x +  y * y; };   // ノルムの2乗
	double GetNorme() const { return sqrt(GetNorme2()); }; // ノルム
	double Length() const { return GetNorme(); };          // ノルムの別名
	CVector& Normalize() { return *this /= GetNorme(); };  // 自身を正規化

	// 回転
	CVector& Rotate (double theta); // theta度回転

	// 積
	double OuterProduct(const CVector &v) const { return x * v.y - y * v.x; }; // 外積
	double InnerProduct(const CVector &v) const { return x * v.x + y * v.y; }; // 内積

	// その他の演算
	double GetAngle(const CVector &v) const;                     // 2つのベクトルのなす角度
	double Angle(const CVector &v) const { return GetAngle(v); } // GetAngleの別名
	double GetFullAngle(const CVector &v) const;                 // 0～360を返す
	double DirectComponent(const CVector &v) const;              // vへの射影

	// 演算子
	CVector& operator +=(const CVector &v);          // += 足し算
	CVector& operator -=(const CVector &v);          // -= 引き算
	CVector& operator *=(const double c);            // *= 定数倍
	CVector& operator /=(const double c);            // /= 定数割り算
	BOOL     operator ==(const CVector &v) const     // == 比較
	{
		return (x == v.x && y == v.y);
	};
	BOOL     operator !=(const CVector &v) const     // != 比較
	{
		return !(*this == v);
	}; 
	CVector& operator = (const CVector &v);          // =  代入
	CVector& operator = (const double elements[2]);  // =  代入

	operator const double* () { return &x; }

	double x;  // X成分
	double y;  // Y成分

	static const double PI; // 円周率
};

// 出力演算子
std::ostream& operator <<(std::ostream& lhs, const CVector &v);

// グローバル演算子
CVector operator + (const CVector &v1, const CVector &v2); // + 足し算
CVector operator - (const CVector &v1, const CVector &v2); // - 引き算
CVector operator * (const CVector &v , const double c);    // * 定数倍
CVector operator / (const CVector &v , const double c);    // / 定数割り算

double  operator * (const CVector &v1, const CVector &v2); // * 内積
double  operator % (const CVector &v1, const CVector &v2); // % 外積

