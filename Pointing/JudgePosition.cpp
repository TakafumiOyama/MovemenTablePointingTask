#include "StdAfx.h"
#include "JudgePosition.h"


CJudgePosition::CJudgePosition(void)
{
	m_pParameter = new CParameter();
	m_pGL = new CGLBase();
}


CJudgePosition::~CJudgePosition(void)
{
}


bool CJudgePosition::JudgeDown(CPoint touch)
{
	OPos1 = tmpObjectPos[m_correct];
	OPos2 = GhostObjectPos[m_correct];
	TPos = touch;

	if(IsTasking == false)
	{
		//x座標がオブジェクト内部の場合
		if(OPos1.x - 60 < TPos.x && TPos.x < OPos1.x + 60)
		{
			if(OPos1.y - 60 < TPos.y && TPos.y < OPos1.y + 60)
			{
				//TRACE("タッチは成功しました，オブジェクト１\n");
				IsTaskFinish = false;

				IsCorrectTouch = true;
				IsTasking = true;
				if(IsFirstTouch){
					m_TouchTime = ::timeGetTime() - m_ProgramStart;
					IsFirstTouch = false;
					//GhostTouchPos = touch;
				}
				m_CreateGoalFlag = true;
			}
		}else if(OPos2.x - 60 < TPos.x && TPos.x < OPos2.x + 60)
		{
			if(OPos2.y - 60 < TPos.y && TPos.y < OPos2.y + 60)
			{
				//TRACE("タッチは成功しました，オブジェクト２\n");
				IsTaskFinish = false;

				IsCorrectTouch = true;
				IsTasking = true;
				if(IsFirstTouch){
					m_TouchTime = ::timeGetTime() - m_ProgramStart;
					IsFirstTouch = false;
					//GhostTouchPos = touch;
				}
				m_CreateGoalFlag = true;
			}
		}else{
			AddWrong();
			IsCorrectTouch = false;
		}
	}

	return false;
}

bool CJudgePosition::JudgeUp(CPoint up)
{
	//GetCorrectCount();

	DragPos = up;
	TarPos = TargetPos[m_correct];

	//if(IsCorrectTouch)
	if(TarPos.x - 80 < DragPos.x && DragPos.x < TarPos.x + 80 && IsCorrectTouch == true)
	{
		if(TarPos.y - 80 < DragPos.y && DragPos.y < TarPos.y + 80)
		{
			tmpObjectPos[m_correct+1] = up;
			RemovePos = up;
			IsTaskFinish = true;
			IsStandby = true;
			m_TaskFinishTime = ::timeGetTime() - m_ProgramStart;
			RecordInformation();
			m_correct++;
			if(m_correct == 15 && m_ShapeShift == true)
			{
				IsStandby = false;
				m_CreateGoalFlag = false;
				IsFirstTouch = true;
				IsTasking = false;
				GhostObjectPos[m_correct] = ObjectPos[m_correct];
			}
		}
	}else{
		if(IsCorrectTouch)
		{
			tmpObjectPos[m_correct] = up;
			RemovePos = up;
			if(IsLshape)
			{
				if(2760 < DragPos.x && DragPos.x < 3840 && 1010 < DragPos.y)
				{
					GhostObjectPos[m_correct].x = up.y+ 2760;
					GhostObjectPos[m_correct].y = 3840 - up.x;
				}else if(3770 < DragPos.x && 0 < DragPos.y && DragPos.y < 1080)
				{
					GhostObjectPos[m_correct].x = 3840 - up.y;
					GhostObjectPos[m_correct].y = up.x - 2760;

				}
				RemovePos = GhostObjectPos[m_correct];

			}
		}
		if(!IsStandby)
			m_RemoveCount++;
		IsTasking = false;
	}
	IsCorrectTouch = false;
	TRACE("correct = %d, wrong = %d\n", m_correct, m_wrong);

	return false;
}
