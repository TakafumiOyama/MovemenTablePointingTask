#include "StdAfx.h"
#include "Parameter.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <stdio.h>


using namespace std;

int CParameter::WindowSizeX;
int CParameter::WindowSizeY;

int CParameter::m_TouchCount;
int CParameter::m_RemoveCount;
int CParameter::m_correct;
int CParameter::m_wrong;

bool CParameter::IsFirstTouch;
bool CParameter::IsTasking;
bool CParameter::IsCorrectTouch;
bool CParameter::IsTaskFinish;
CPoint CParameter::ObjectPos[100];
CPoint CParameter::ObjectPos2[100];
CPoint CParameter::TargetPos[100];
CPoint CParameter::DefObjectPos[4][3][3];
CPoint CParameter::DefTargetPos[4][3][3];
CPoint CParameter::tmpObjectPos[100];
CPoint CParameter::GhostObjectPos[100];
CPoint CParameter::currentTouchPos;
CPoint CParameter::GhostTouchPos;
CPoint CParameter::RemovePos;
int CParameter::m_TableState;
int CParameter::m_PreTableState;
int CParameter::m_ShapePattern;
int CParameter::m_objectPattern;

bool CParameter::LetsGo;
bool CParameter::m_Moving;
bool CParameter::m_MoveStart;
bool CParameter::m_ShapeShift;
bool CParameter::IsLined;
bool CParameter::IsLshape;
bool CParameter::IsStandby;
bool CParameter::m_CreateGoalFlag;
DWORD CParameter::m_ProgramStart;
DWORD CParameter::m_TaskStartTime;
DWORD CParameter::m_TouchTime;
DWORD CParameter::m_TaskFinishTime;
int CParameter::m_presetNum;
int CParameter::m_windowNum;

CParameter::CParameter(void)
{
	//IsTasking = false;
	//IsCorrectTouch = false;
	//IsTaskFinish = true;
	//IsLined = false;
	////m_ShapeShift = false;
	////m_MoveStart = false;
}


CParameter::~CParameter(void)
{
}



int CParameter::RecordInit()
{
	ofstream file;
	file.open("results/pointresult.csv",ios::out | ios::app);
	if(! file.is_open())	
		return EXIT_FAILURE;

	//出力されるファイルのフォーマット
	// タスクのパターン，オブジェクトが出現した時間，タッチが始まった時間，タスクが終わった時間，
	// スタートのオブジェクトの位置xy，タッチした位置xt，指を離した位置xy，正しいタッチ回数，間違えたタッチ回数
	file<< "shape pattern,task pattern,task start time,touch start time,task finish time,";
	file<< "StartObjPos x,StartObjPos y,TargetPos x,TargetPos y,TouchPos x,TouchPos y,RemovePos x,RemovePos y,CorrectTouchCount,WrongTouchCount,RemoveCount,,タッチ時間,ドラッグ時間,オブジェクト位置,ターゲット位置" << endl;
	
	file.close();
	return 0;

}

int CParameter::RecordInformation()
{

	fstream file;
	file.open("results/0401_Ishikawa.csv",ios::out | ios::app);
	if(! file.is_open())
		return EXIT_FAILURE;


	//CTime cTime = CTime::GetCurrentTime();
	//CString str = cTime.Format("%m/%d %H:%M:%S");
	
	//SYSTEMTIME systime;
	//GetLocalTime(&systime);
	//CString time;
	//time.Format(_T("%u/%u/%u/%u/%u/%u"),
	//	systime.wMonth,systime.wDay,
	//	systime.wHour,systime.wMinute,systime.wSecond,
	//	systime.wMilliseconds);
	//preTime = time;

	//TRACE(time);
	//char *s;

	int objectwindow=0,targetwindow=0;
	if(ObjectPos[m_correct].x < 1920)
		objectwindow = 0;
	if(1920 < ObjectPos[m_correct].x && ObjectPos[m_correct].x < 3840)
		objectwindow = 1;
	if(3840 < ObjectPos[m_correct].x && ObjectPos[m_correct].x < 5760)
		objectwindow = 2;

	if(TargetPos[m_correct].x < 1920)
		targetwindow = 0;
	if(1920 < TargetPos[m_correct].x && TargetPos[m_correct].x < 3840)
		targetwindow = 1;
	if(3840 < TargetPos[m_correct].x && TargetPos[m_correct].x < 5760)
		targetwindow = 2;

	TRACE("(%d, %d)",ObjectPos[m_correct].x, ObjectPos[m_correct].y);

	char timing;


	//出力
	file << "1,";
	file << m_ShapePattern << ",";
	file << m_objectPattern << ",";
	if(m_correct >= 15)
		file << "1,";
	if(m_correct < 15)
		file << "0,";
	file << m_TaskStartTime << ",";
	file << m_TouchTime << ",";
	file << m_TaskFinishTime << ",";
	file << ObjectPos[m_correct].x << ",";
	file << ObjectPos[m_correct].y << ",";
	file << TargetPos[m_correct].x << ",";
	file << TargetPos[m_correct].y << ",";
	file << currentTouchPos.x << ",";
	file << currentTouchPos.y << ",";
	file << RemovePos.x << ",";
	file << RemovePos.y << ",";
	file << m_correct << ",";
	file << m_wrong << ",";
	file << m_RemoveCount << ",,";
	file << m_TouchTime - m_TaskStartTime << ",";
	file << m_TaskFinishTime - m_TouchTime << ",";
	file << objectwindow << ",";
	file << targetwindow << ",";
	file << endl;
	
	file.close();
	return 0;
}

int CParameter::PushBackData()
{
	OUTPUTDATA outputData;
//	outputData.time =GetTime();

	outputData.touchCount = 0;
	outputData.correctCount = 0;
	outputData.wrongCount = 0;
	outputData.touchPos_x = 0;
	outputData.touchPox_y = 0;
	m_outputDataBuffer.push_back(outputData);

	return 0;
}

int CParameter::CreateObjectPos()
{
	for(int i=0;i<10;i++)
	{

		//int r = rand() % 15;
		//ObjectPos[i] = CPoint(200*r + 200, 100 * r + 100);
		//tmpObjectPos[i] = ObjectPos[i];

	}
	return 0;
}

int CParameter::CreateTargetPos()
{
	for(int i=0;i<10;i++)
	{
		int r = rand() * rand() % 15;
		TargetPos[i] = CPoint(100*r + 200, 50 * r + 100);
	}
	return 0;
}



CPoint CParameter::GetObjectPos(int i)
{
	return ObjectPos[i];
}

CPoint CParameter::GetTouchPos(CPoint p)
{
	p = currentTouchPos;
	return p;
}

int CParameter::GetCorrectCount()
{
	return m_correct;
}

int CParameter::GetWrongCount()
{
	return m_wrong;
}

void CParameter::GetAllParameter(int i)
{
	//GetObjectPos(i);
	//GetTouchPos();
	//GetCorrectCount();
	//GetWrongCount();
}

int CParameter::AddCorrect()
{
	m_correct++;
	return m_correct;
}

int CParameter::AddWrong()
{
	m_wrong++;
	return m_wrong;
}

CPoint CParameter::RegistTouchPos(CPoint p)
{
	currentTouchPos = p;
	return currentTouchPos;
}


int CParameter::TableState(int p)
{
	switch (p)
	{
		//長方形
		case 0:
			m_TableState = 0;
			break;
		//線形
		case 1:
			m_TableState = 1;
			break;
		//カーブ
		case 2:
			m_TableState = 2;
			break;
		//L字
		case 3:
			m_TableState = 3;
			break;
		default:
			break;
	}
	return 0;
}

int CParameter::CreateDefPos3()
{
	for(int a =0;a<3;a++)
	{
		for(int i=0;i<3;i++)
		{
			for(int j=0;j<3;j++)
			{
				DefObjectPos[a][i][j] = CPoint(480*(i+1) + 1920*a, 270*(j+1));
				DefTargetPos[a][i][j] = CPoint(480*(i+1) + 1920*a, 270*(j+1));
			}
		}
	}
	IsTaskFinish = true;
	return 0;
}

int CParameter::CreateDefPos4()
{
	for(int a =0;a<4;a++)
	{
		for(int i=0;i<3;i++)
		{
			for(int j=0;j<3;j++)
			{
				if(a<2)
				{
					DefObjectPos[a][i][j] = CPoint(480*(i+1) + 1920*a, 270*(j+1));
					DefTargetPos[a][i][j] = CPoint(480*(i+1) + 1920*a, 270*(j+1));
				}else if(2 <= a){
					DefObjectPos[a][i][j] = CPoint(480*(i+1) + 1920*(a-2), 270*(j+1))+1080;
					DefTargetPos[a][i][j] = CPoint(480*(i+1) + 1920*(a-2), 270*(j+1))+1080;
					TRACE("(%d, %d)",DefTargetPos[a][i][j].x, DefTargetPos[a][i][j].y);
				}

			}
		}
	}
	return 0;
}


void CParameter::CreatePreset(int n)
{
	if(this->m_windowNum == 3)
		SetPreset3(n);
	if(this->m_windowNum == 4)
		SetPreset4(n);

}

// 1-1
void CParameter::SetPreset3(int n)
{
	//objectの座標
	int objx,objy;
	//target
	int tarx,tary;
	int time = (int)::timeGetTime();

	int GG1[30] = {2,2,1,0,2,0,1,2,0,1,2,1,0,0,1,1,0,2,1,2,0,2,1,0,0,0,2,1,2,1};
	int GG2[30] = {1,0,2,1,2,0,2,1,0,0,0,2,1,2,1,2,0,2,1,2,0,0,1,1,1,0,1,0,2,2};
	int GM[30] =  {2,0,2,1,2,0,0,1,1,1,0,1,0,2,2,1,1,1,2,1,1,1,1,1,1,1,0,2,1,0};
	int GML[30] = {2,0,2,1,2,0,0,1,1,1,0,1,0,2,2,0,2,0,1,0,0,0,0,0,0,2,0,0,1,0};
	//int GS[30] =  {2,2,1,0,2,0,1,2,0,1,2,1,0,0,1,1,0,2,0,0,2,2,0,0,2,2,1,2,0,1};
	//int GSL[30] = {2,2,1,0,2,0,1,2,0,1,2,1,0,0,1,2,0,1,1,2,2,1,2,1,0,1,2,2,0,1};
	int MM[30] =  {1,1,1,2,1,1,1,1,1,1,1,0,2,1,0,1,0,1,1,0,2,1,1,1,1,1,1,2,1,1};
	int MML[30] = {2,0,1,0,0,0,0,0,2,0,0,0,0,1,0,0,2,0,1,0,0,0,0,0,0,2,0,0,1,0};
	//int SS[30] =  {0,2,1,2,0,2,0,2,2,1,0,1,0,2,0,1,0,2,1,0,2,0,2,1,2,0,0,2,2,0};
	//int SSL[30] = {1,2,1,1,0,1,2,2,2,1,0,2,1,2,0,0,2,2,1,1,2,1,1,1,2,2,1,0,2,0};

	int GG[36] =  {0,1,2,1,1,0,0,2,2,0,0,2,2,1,0,1,1,2,1,0,1,1,2,1,0,0,1,2,2,2,0,2,0,0,2,1};
	int DD[36] =  {2,2,2,0,2,2,2,1,0,2,0,2,2,2,0,2,1,2,2,1,0,0,2,2,1,0,2,2,0,2,2,2,2,2,2,2};
	int DDL[36] = {1,1,1,1,1,1,2,2,1,1,1,0,2,1,2,0,1,1,1,0,1,1,2,1,2,2,2,1,1,1,1,1,1,1,0,1};
	int SS[36] =  {2,0,2,0,0,2,2,2,2,2,1,2,0,1,0,0,0,0,1,0,2,0,0,2,2,2,0,2,0,0,0,0,2,2,2,1};
	int SSL[36] = {0,1,1,0,2,2,2,2,1,1,2,2,1,2,2,1,1,1,0,1,1,0,2,2,2,2,1,1,2,2,1,2,2,1,1,1};

	int GD[30] =  {0,2,2,0,1,1,0,1,0,0,1,2,2,2,1,1,2,0,1,2,2,2,2,2,0,1,2,2,2,0};
	int GDL[30] = {0,2,2,0,1,1,0,1,0,0,1,2,2,2,1,1,2,2,1,0,1,1,1,0,1,2,1,1,1,0};
	int GS[30] =  {1,1,1,0,2,0,2,1,0,0,1,0,2,2,2,0,2,1,0,0,2,1,2,0,0,2,2,2,1,0};
	int GSL[30] = {1,1,1,0,2,0,2,1,0,0,1,0,2,2,2,1,0,1,2,1,1,2,1,2,2,0,0,1,2,2};
	int DG[30] =  {1,2,0,2,1,0,2,2,2,2,2,2,1,0,2,1,1,1,0,2,0,2,1,0,0,1,0,2,2,2};
	int DGL[30] = {2,1,1,1,1,0,1,1,0,1,0,1,2,2,1,1,1,1,0,2,0,2,1,0,0,1,0,2,2,2};
	int DS[30] =  {1,2,0,1,2,2,2,2,2,0,1,2,2,2,0,0,1,0,1,0,2,0,2,2,2,2,1,0,0,2};
	int DSL[30] = {1,2,2,1,0,1,1,1,0,1,2,1,1,1,0,2,1,0,0,1,0,2,1,2,1,2,2,1,2,1};
	int SG[30] =  {0,1,0,1,0,2,0,2,2,2,2,1,0,0,2,0,2,2,0,1,1,0,1,0,0,1,2,2,2,1};
	int SGL[30] = {2,1,0,0,1,0,2,1,2,1,2,2,1,2,1,0,2,2,0,1,1,0,1,0,0,1,2,2,2,1};
	int SD[30] =  {0,2,1,0,0,2,1,2,0,0,2,2,2,1,0,1,2,0,2,1,0,2,2,2,2,2,2,1,0,2};
	int SDL[30] = {1,0,1,2,1,1,2,1,2,2,0,0,1,2,2,2,1,1,1,1,0,1,1,0,1,0,1,2,2,1};

	
	switch(n)
	{
	case 1:
		for(int i=0; i<36;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;
			tarx = (rand() + rand()%2 + time) % 3;
			tary = (rand() + rand()%2 + time) * i % 3;
			if(objx == tarx)
				tarx = (tarx+5) % 3;
			if(objy == tary)
				tary = (tary+5) % 3;
			ObjectPos[i] = DefObjectPos[GG[i]][objx][objy];
			TargetPos[i] = DefTargetPos[GG[i]][tarx][tary];
			m_objectPattern = 1;
		}
		break;
	case 2:
		for(int i=0; i<36;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;
			tarx = (rand() + rand()%2 + time) % 3;
			tary = (rand() + rand()%2 + time) * i % 3;
			if(objx == tarx)
				tarx = (tarx+5) % 3;
			if(objy == tary)
				tary = (tary+5) % 3;
			ObjectPos[i] = DefObjectPos[DD[i]][objx][objy];
			TargetPos[i] = DefTargetPos[DD[i]][tarx][tary];
			m_objectPattern = 2;
		}
		break;
	case 3:
		for(int i=0; i<36;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;
			tarx = (rand() + rand()%2 + time) % 3;
			tary = (rand() + rand()%2 + time) * i % 3;
			if(objx == tarx)
				tarx = (tarx+5) % 3;
			if(objy == tary)
				tary = (tary+5) % 3;
			ObjectPos[i] = DefObjectPos[DDL[i]][objx][objy];
			TargetPos[i] = DefTargetPos[DDL[i]][tarx][tary];
			m_objectPattern = 3;
		}
		break;
	case 4:
		for(int i=0; i<36;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;
			tarx = (rand() + rand()%2 + time) % 3;
			tary = (rand() + rand()%2 + time) * i % 3;
			if(objx == tarx)
				tarx = (tarx+5) % 3;
			if(objy == tary)
				tary = (tary+5) % 3;
			ObjectPos[i] = DefObjectPos[SS[i]][objx][objy];
			TargetPos[i] = DefTargetPos[SS[i]][tarx][tary];
			m_objectPattern = 4;
		}
		break;
	case 5:
		for(int i=0; i<36;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;
			tarx = (rand() + rand()%2 + time) % 3;
			tary = (rand() + rand()%2 + time) * i % 3;
			if(objx == tarx)
				tarx = (tarx+5) % 3;
			if(objy == tary)
				tary = (tary+5) % 3;
			ObjectPos[i] = DefObjectPos[SSL[i]][objx][objy];
			TargetPos[i] = DefTargetPos[SSL[i]][tarx][tary];
			m_objectPattern = 5;
		}
		break;
	case 6:
		for(int i=0; i<36;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;
			tarx = (rand() + rand()%2 + time) % 3;
			tary = (rand() + rand()%2 + time) * i % 3;
			if(objx == tarx)
				tarx = (tarx+5) % 3;
			if(objy == tary)
				tary = (tary+5) % 3;
			ObjectPos[i] = DefObjectPos[GD[i]][objx][objy];
			TargetPos[i] = DefTargetPos[GD[i]][tarx][tary];
			m_objectPattern = 6;
		}
		break;
	case 7:
		for(int i=0; i<36;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;
			tarx = (rand() + rand()%2 + time) % 3;
			tary = (rand() + rand()%2 + time) * i % 3;
			if(objx == tarx)
				tarx = (tarx+5) % 3;
			if(objy == tary)
				tary = (tary+5) % 3;
			ObjectPos[i] = DefObjectPos[GDL[i]][objx][objy];
			TargetPos[i] = DefTargetPos[GDL[i]][tarx][tary];
			m_objectPattern = 7;
		}
		break;
	case 8:
		for(int i=0; i<36;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;
			tarx = (rand() + rand()%2 + time) % 3;
			tary = (rand() + rand()%2 + time) * i % 3;
			if(objx == tarx)
				tarx = (tarx+5) % 3;
			if(objy == tary)
				tary = (tary+5) % 3;
			ObjectPos[i] = DefObjectPos[GS[i]][objx][objy];
			TargetPos[i] = DefTargetPos[GS[i]][tarx][tary];
			m_objectPattern = 8;
		}
		break;
	case 9:
		for(int i=0; i<36;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;
			tarx = (rand() + rand()%2 + time) % 3;
			tary = (rand() + rand()%2 + time) * i % 3;
			if(objx == tarx)
				tarx = (tarx+5) % 3;
			if(objy == tary)
				tary = (tary+5) % 3;
			ObjectPos[i] = DefObjectPos[GSL[i]][objx][objy];
			TargetPos[i] = DefTargetPos[GSL[i]][tarx][tary];
			m_objectPattern = 9;
		}
		break;
		
	case 11:
		for(int i=0; i<30;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;

			//左の列
			if(objx == 0 && i%2 == 0)
			{
				tarx = objx + 1;
				tary = objy;
			}
			if(objx == 0 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//真ん中の列
			if(objx == 1 && i%2 == 0){
				tary = objy;
				if(i%2 == 0)
					tarx = objx + 1;
				if(i%2 == 1)
					tarx = objx - 1;
			}
			if(objx == 1 && i%2 == 1){
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//右の列
			if(objx == 2 && i%2 == 0){
				tarx = objx - 1;
				tary = objy;
			}
			if(objx == 2 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}
			ObjectPos[i] = DefObjectPos[GD[i]][objx][objy];
			TargetPos[i] = DefTargetPos[GD[i]][tarx][tary];
			m_objectPattern = n;
		}
		break;
	case 12:
		for(int i=0; i<30;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;

			//左の列
			if(objx == 0 && i%2 == 0)
			{
				tarx = objx + 1;
				tary = objy;
			}
			if(objx == 0 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//真ん中の列
			if(objx == 1 && i%2 == 0){
				tary = objy;
				if(i%2 == 0)
					tarx = objx + 1;
				if(i%2 == 1)
					tarx = objx - 1;
			}
			if(objx == 1 && i%2 == 1){
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//右の列
			if(objx == 2 && i%2 == 0){
				tarx = objx - 1;
				tary = objy;
			}
			if(objx == 2 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}
			ObjectPos[i] = DefObjectPos[GDL[i]][objx][objy];
			TargetPos[i] = DefTargetPos[GDL[i]][tarx][tary];
			m_objectPattern = n;
		}
		break;
	case 13:
		for(int i=0; i<30;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;

			//左の列
			if(objx == 0 && i%2 == 0)
			{
				tarx = objx + 1;
				tary = objy;
			}
			if(objx == 0 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//真ん中の列
			if(objx == 1 && i%2 == 0){
				tary = objy;
				if(i%2 == 0)
					tarx = objx + 1;
				if(i%2 == 1)
					tarx = objx - 1;
			}
			if(objx == 1 && i%2 == 1){
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//右の列
			if(objx == 2 && i%2 == 0){
				tarx = objx - 1;
				tary = objy;
			}
			if(objx == 2 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}
			ObjectPos[i] = DefObjectPos[GS[i]][objx][objy];
			TargetPos[i] = DefTargetPos[GS[i]][tarx][tary];
			m_objectPattern = n;
		}
		break;
	case 14:
		for(int i=0; i<30;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;

			//左の列
			if(objx == 0 && i%2 == 0)
			{
				tarx = objx + 1;
				tary = objy;
			}
			if(objx == 0 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//真ん中の列
			if(objx == 1 && i%2 == 0){
				tary = objy;
				if(i%2 == 0)
					tarx = objx + 1;
				if(i%2 == 1)
					tarx = objx - 1;
			}
			if(objx == 1 && i%2 == 1){
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//右の列
			if(objx == 2 && i%2 == 0){
				tarx = objx - 1;
				tary = objy;
			}
			if(objx == 2 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}
			ObjectPos[i] = DefObjectPos[GSL[i]][objx][objy];
			TargetPos[i] = DefTargetPos[GSL[i]][tarx][tary];
			m_objectPattern = n;
		}
		break;
	case 15:
		for(int i=0; i<30;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;

			//左の列
			if(objx == 0 && i%2 == 0)
			{
				tarx = objx + 1;
				tary = objy;
			}
			if(objx == 0 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//真ん中の列
			if(objx == 1 && i%2 == 0){
				tary = objy;
				if(i%2 == 0)
					tarx = objx + 1;
				if(i%2 == 1)
					tarx = objx - 1;
			}
			if(objx == 1 && i%2 == 1){
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//右の列
			if(objx == 2 && i%2 == 0){
				tarx = objx - 1;
				tary = objy;
			}
			if(objx == 2 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}
			ObjectPos[i] = DefObjectPos[DG[i]][objx][objy];
			TargetPos[i] = DefTargetPos[DG[i]][tarx][tary];
			m_objectPattern = n;
		}
		break;
	case 16:
		for(int i=0; i<30;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;

			//左の列
			if(objx == 0 && i%2 == 0)
			{
				tarx = objx + 1;
				tary = objy;
			}
			if(objx == 0 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//真ん中の列
			if(objx == 1 && i%2 == 0){
				tary = objy;
				if(i%2 == 0)
					tarx = objx + 1;
				if(i%2 == 1)
					tarx = objx - 1;
			}
			if(objx == 1 && i%2 == 1){
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//右の列
			if(objx == 2 && i%2 == 0){
				tarx = objx - 1;
				tary = objy;
			}
			if(objx == 2 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}
			ObjectPos[i] = DefObjectPos[DGL[i]][objx][objy];
			TargetPos[i] = DefTargetPos[DGL[i]][tarx][tary];
			m_objectPattern = n;
		}
		break;
	case 17:
		for(int i=0; i<30;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;

			//左の列
			if(objx == 0 && i%2 == 0)
			{
				tarx = objx + 1;
				tary = objy;
			}
			if(objx == 0 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//真ん中の列
			if(objx == 1 && i%2 == 0){
				tary = objy;
				if(i%2 == 0)
					tarx = objx + 1;
				if(i%2 == 1)
					tarx = objx - 1;
			}
			if(objx == 1 && i%2 == 1){
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//右の列
			if(objx == 2 && i%2 == 0){
				tarx = objx - 1;
				tary = objy;
			}
			if(objx == 2 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}
			ObjectPos[i] = DefObjectPos[DS[i]][objx][objy];
			TargetPos[i] = DefTargetPos[DS[i]][tarx][tary];
			m_objectPattern = n;
		}
		break;
	case 18:
		for(int i=0; i<30;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;

			//左の列
			if(objx == 0 && i%2 == 0)
			{
				tarx = objx + 1;
				tary = objy;
			}
			if(objx == 0 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//真ん中の列
			if(objx == 1 && i%2 == 0){
				tary = objy;
				if(i%2 == 0)
					tarx = objx + 1;
				if(i%2 == 1)
					tarx = objx - 1;
			}
			if(objx == 1 && i%2 == 1){
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//右の列
			if(objx == 2 && i%2 == 0){
				tarx = objx - 1;
				tary = objy;
			}
			if(objx == 2 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}
			ObjectPos[i] = DefObjectPos[DSL[i]][objx][objy];
			TargetPos[i] = DefTargetPos[DSL[i]][tarx][tary];
			m_objectPattern = n;
		}
		break;
	case 19:
		for(int i=0; i<30;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;

			//左の列
			if(objx == 0 && i%2 == 0)
			{
				tarx = objx + 1;
				tary = objy;
			}
			if(objx == 0 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//真ん中の列
			if(objx == 1 && i%2 == 0){
				tary = objy;
				if(i%2 == 0)
					tarx = objx + 1;
				if(i%2 == 1)
					tarx = objx - 1;
			}
			if(objx == 1 && i%2 == 1){
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//右の列
			if(objx == 2 && i%2 == 0){
				tarx = objx - 1;
				tary = objy;
			}
			if(objx == 2 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}
			ObjectPos[i] = DefObjectPos[SG[i]][objx][objy];
			TargetPos[i] = DefTargetPos[SG[i]][tarx][tary];
			m_objectPattern = n;
		}
		break;
	case 20:
		for(int i=0; i<30;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;

			//左の列
			if(objx == 0 && i%2 == 0)
			{
				tarx = objx + 1;
				tary = objy;
			}
			if(objx == 0 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//真ん中の列
			if(objx == 1 && i%2 == 0){
				tary = objy;
				if(i%2 == 0)
					tarx = objx + 1;
				if(i%2 == 1)
					tarx = objx - 1;
			}
			if(objx == 1 && i%2 == 1){
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//右の列
			if(objx == 2 && i%2 == 0){
				tarx = objx - 1;
				tary = objy;
			}
			if(objx == 2 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}
			ObjectPos[i] = DefObjectPos[SGL[i]][objx][objy];
			TargetPos[i] = DefTargetPos[SGL[i]][tarx][tary];
			m_objectPattern = n;
		}
		break;
	case 21:
		for(int i=0; i<30;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;

			//左の列
			if(objx == 0 && i%2 == 0)
			{
				tarx = objx + 1;
				tary = objy;
			}
			if(objx == 0 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//真ん中の列
			if(objx == 1 && i%2 == 0){
				tary = objy;
				if(i%2 == 0)
					tarx = objx + 1;
				if(i%2 == 1)
					tarx = objx - 1;
			}
			if(objx == 1 && i%2 == 1){
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//右の列
			if(objx == 2 && i%2 == 0){
				tarx = objx - 1;
				tary = objy;
			}
			if(objx == 2 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}
			ObjectPos[i] = DefObjectPos[SD[i]][objx][objy];
			TargetPos[i] = DefTargetPos[SD[i]][tarx][tary];
			m_objectPattern = n;
		}
		break;
	case 22:
		for(int i=0; i<30;i++)
		{

			objx = (rand()+time) % 3;
			objy = (rand()+time) * i % 3;

			//左の列
			if(objx == 0 && i%2 == 0)
			{
				tarx = objx + 1;
				tary = objy;
			}
			if(objx == 0 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//真ん中の列
			if(objx == 1 && i%2 == 0){
				tary = objy;
				if(i%2 == 0)
					tarx = objx + 1;
				if(i%2 == 1)
					tarx = objx - 1;
			}
			if(objx == 1 && i%2 == 1){
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}

			//右の列
			if(objx == 2 && i%2 == 0){
				tarx = objx - 1;
				tary = objy;
			}
			if(objx == 2 && i%2 == 1)
			{
				tarx = objx;
				if(objy == 0 || objy == 2){
					tary = 1;
				}else if(objy == 1 && rand()%2 == 0){
					tary = 0;
				}else{
					tary = 2;
				}
			}
			ObjectPos[i] = DefObjectPos[SDL[i]][objx][objy];
			TargetPos[i] = DefTargetPos[SDL[i]][tarx][tary];
			m_objectPattern = n;
		}
		break;
	default:
		break;
	}


		
}


// 1-1
void CParameter::SetPreset4(int n)
{
	switch(n)
	{
	case 1:
		ObjectPos[0] = DefObjectPos[0][0][0];
		TargetPos[0] = DefTargetPos[1][2][0];

		ObjectPos[1] = DefObjectPos[1][2][1];
		TargetPos[1] = DefTargetPos[1][0][1];

		ObjectPos[2] = DefObjectPos[1][0][2];
		TargetPos[2] = DefTargetPos[1][2][2];

		ObjectPos[3] = DefObjectPos[0][0][0];
		TargetPos[3] = DefTargetPos[0][0][0];

		ObjectPos[4] = DefObjectPos[1][2][2];
		TargetPos[4] = DefTargetPos[0][2][2];

		ObjectPos[5] = DefObjectPos[1][1][0];
		TargetPos[5] = DefTargetPos[1][1][0];

		ObjectPos[6] = DefObjectPos[1][0][2];
		TargetPos[6] = DefTargetPos[0][0][2];

		ObjectPos[7] = DefObjectPos[2][1][1];
		TargetPos[7] = DefTargetPos[1][1][1];

		ObjectPos[8] = DefObjectPos[0][2][1];
		TargetPos[8] = DefTargetPos[0][2][1];
		break;

		
	case 7:
		ObjectPos[0] = DefObjectPos[0][0][0];
		TargetPos[0] = DefTargetPos[0][2][0];

		ObjectPos[1] = DefObjectPos[1][2][1];
		TargetPos[1] = DefTargetPos[1][0][1];

		ObjectPos[2] = DefObjectPos[2][0][2];
		TargetPos[2] = DefTargetPos[2][2][2];

		ObjectPos[3] = DefObjectPos[0][0][0];
		TargetPos[3] = DefTargetPos[1][0][0];

		ObjectPos[4] = DefObjectPos[1][2][2];
		TargetPos[4] = DefTargetPos[0][2][2];

		ObjectPos[5] = DefObjectPos[1][1][0];
		TargetPos[5] = DefTargetPos[2][1][0];

		ObjectPos[6] = DefObjectPos[2][0][2];
		TargetPos[6] = DefTargetPos[1][0][2];

		ObjectPos[7] = DefObjectPos[0][1][1];
		TargetPos[7] = DefTargetPos[2][1][1];

		ObjectPos[8] = DefObjectPos[2][2][1];
		TargetPos[8] = DefTargetPos[0][2][1];
		break;

	default:
		break;
	}
}
