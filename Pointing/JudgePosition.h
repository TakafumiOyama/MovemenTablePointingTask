#pragma once
#include "parameter.h"
#include "GLBase.h"

class CJudgePosition :
	public CParameter
{
public:
	CJudgePosition(void);
	~CJudgePosition(void);

	CParameter * m_pParameter;
	CGLBase * m_pGL;

	bool JudgeDown(CPoint);
	bool JudgeUp(CPoint);
	CPoint OPos1;
	CPoint OPos2;
	CPoint TPos;

	CPoint DragPos;
	CPoint TarPos;
};

