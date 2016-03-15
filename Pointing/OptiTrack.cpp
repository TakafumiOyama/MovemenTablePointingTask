#include "StdAfx.h"
#include "OptiTrack.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include "Vector.h"
#include <MMSystem.h>

using namespace std;

COptiTrack::COptiTrack(void)
:m_trackableNum(6)
,m_flag_SimulationMode(false) //シュミレーションモード
,m_CalcFirstTime(TRUE)
,m_SensorCheckFlag(FALSE)
,m_IsLogging(FALSE)
,m_ActiveNum(0)
,m_ApproachNum(0)
{
}


COptiTrack::~COptiTrack(void)
{
	TRACE("Shutting down Natural Point Tracking Tools\n");
	CheckResult(TT_Shutdown());

	TRACE("Complete\n");
	Sleep(200);
	TT_FinalCleanup();
}

// OptiTrackの初期化
int COptiTrack::Init()
{
	// OptiTrackを起動
    TRACE("== NaturalPoint Tracking Tools API Camera, Marker, Trackables Sample =======---\n");
    TRACE("== (C) NaturalPoint, Inc.\n\n");
    TRACE("Initializing NaturalPoint Devices\n");
    int a = TT_Initialize();
	DWORD er = GetLastError();

	// プロジェクトファイルを読み込み
    TRACE("Loading Project: project.ttp\n\n");
    CheckResult(TT_LoadProject("project.ttp"));
	er = GetLastError();

    TRACE("Cameras:\n");
    for(int i=0; i<TT_CameraCount(); i++)
    {
        TRACE("Camera #%d: %s\n", i, TT_CameraName(i));
    }
    TRACE("\n");

    TRACE("Trackables:\n");
	// trackableの数を数える
	m_trackableNum = TT_TrackableCount();
	TRACE("Trackableの数 = %d\n", m_trackableNum);
	for(int i=0; i < m_trackableNum; i++)
    {
		// trackableの数だけpush_back
		CTrackable tmpTrackable;
		tmpTrackable.m_id = i;
		tmpTrackable.m_pos_x = 0;
		tmpTrackable.m_pos_y = 0;
		tmpTrackable.m_pos_z = 0;
		tmpTrackable.m_dir_x = 0;
		tmpTrackable.m_dir_y = 0;
		tmpTrackable.m_dir_z = 0;
		tmpTrackable.m_dir_w = 0;
		tmpTrackable.m_yaw = 0;
		tmpTrackable.m_pitch = 0;
		tmpTrackable.m_roll = 0;
		tmpTrackable.m_offset_x = 4170/2;		// キャリブレーション・プロジェクタの位置に依存
		tmpTrackable.m_offset_y = 2530/2;

		m_trackables.push_back(tmpTrackable);
		m_previousTrackables.push_back(tmpTrackable);
        TRACE("Trackable #%d: %s\n", i, TT_TrackableName(i));
	}
	TRACE("\n");

	m_pParameter = new CParameter();
	CSVOutInit();
	m_SensorCheckFlag = TRUE;
	//m_pParameter->m_ProgramStart = ::timeGetTime();
	m_IsLogging = TRUE;
	return 0;
}

int COptiTrack::StartLogging()
{
	m_IsLogging = TRUE;
	for(int i=0; i < m_trackableNum; i++)
    {
		// 推定量の初期化
		// 頭部回転回数
		m_HeadRotationNum[i] = 0;
		m_NoddingNum[i] = 0;
		m_NoddingStartTime[i] = 0;
		m_IsActive[i] = false;
		//それぞれのユーザがテーブルを使っているかどうか
		this->m_IsPersonUseTable[i] = false;
		// 動く速度
		this->m_Speed[i];
		//テーブルに近付いているかどうか
		this->m_IsPersonApproach[i] = false;
		this->m_PersonSeeTableFlag[i] = false;    //テーブルを見ているかどうかのフラグ
		this->m_PersonGazeNum[i] = 0;         //テーブルを見ている秒数
		this->m_PersonLocationToTable[i] = -1; //テーブルに対するユーザの距離
		//壁ディスプレイに対するユーザの向き
		this->m_PersonSeeWallValue_x[i] = 0;
		this->m_PersonSeeWallValue_y[i] = 0;
		//壁ディスプレイに対するユーザの距離
		this->m_DistancePersonToWall[i] = 0;
		// 身体の向き 
		for(int j = 0; j < m_trackableNum; j++)
		{
			this->m_PreDistanceBetweenPeople[i][j] = 0;
			m_DistanceBetweenPeople[i][j] = 0;
			m_PersonOrientation[j] = 0;
			m_FformationFlag[i][j] = 0;
		}
	}
		this->m_IsPeopleActive = false;
		this->m_IsPeopleUseTable = false;
	return 0;
}


int COptiTrack::Update()
{
	// previousに現在のtrackableを格納
	for(int i = 0; i < m_trackableNum; i++)
    {
		m_previousTrackables[i] = m_trackables[i];
	}
	// 位置や方向を更新（キモ部分）
	CalcTrackableMetrics();

	//今は必要なし 20150707
	//// 対人距離を更新
	//CalcDistanceBetweenPeople();
	//// 頭部回転回数を更新
	//CalcHeadRotation();
	//// 身体の向きを更新
	//CalcPersonOrientation();
	//this->CalcPersonIsActive();
	////ユーザの重心の位置を更新
	//this->CalcPeopleCenterofGravity();
	//// 動くスピード
	//this->CalcSpeed();
	//// F-formation
	//this->CalcFformation();
	//// テーブル関連
	//this->CalcTable();
	//// 壁ディスプレイ関連
	//this->CalcWallDisplay();

	if(m_IsLogging)
	{
		// 現在の生データをプッシュ
		PushBackData();
	}

	this->CSVOut();
	return 0;
}


int COptiTrack::CalcTrackableMetrics()
{
	srand( (unsigned)time( NULL ) );

	NPRESULT result;
	result = TT_Update();
	
	// シミュレーション用
	if(m_flag_SimulationMode && m_trackableNum > 0)
	{
		for(int j = 0; j< m_trackableNum; j++)
		{
			int randWidth1 = 150;
			int randWidth2 = 5;
			m_trackables[j].m_pos_x += rand() % randWidth1 - randWidth1 / 2;
			m_trackables[j].m_pos_y += rand() % randWidth1 - randWidth1 / 2;
			m_trackables[j].m_yaw   += rand() % randWidth2 - randWidth2 / 2;
			
			if(m_trackables[j].m_pos_x < 0 || m_trackables[j].m_pos_x > TRACKING_AREA_X) m_trackables[j].m_pos_x = 1000;
			if(m_trackables[j].m_pos_y < 0 || m_trackables[j].m_pos_y > TRACKING_AREA_Y) m_trackables[j].m_pos_y = 1000;
						
		}
	}
	//リアルタイムのデータ取得時
	else if(result == NPRESULT_SUCCESS)
	{
		if(m_trackableNum > 0)
		{
			CTrackable * tmpTrackable;
			tmpTrackable = new CTrackable();

			// n個のマーカに対応
			for(int j = 0; j< m_trackableNum; j++)
			{
				// OptiTrackから値を取ってくる(yとz座標を入れ替え)
				TT_TrackableLocation(j, &(m_trackables[j].m_pos_x), &(m_trackables[j].m_pos_z), &(m_trackables[j].m_pos_y),
										&(m_trackables[j].m_dir_x), &(m_trackables[j].m_dir_z), &(m_trackables[j].m_dir_y), &(m_trackables[j].m_dir_w),
										&(m_trackables[j].m_yaw),   &(m_trackables[j].m_pitch), &(m_trackables[j].m_roll));
				// パラメータを1000倍
				m_trackables[j].m_pos_x = m_trackables[j].m_pos_x * 1000 + m_trackables[j].m_offset_x;
				m_trackables[j].m_pos_y = -m_trackables[j].m_pos_y * 1000 + m_trackables[j].m_offset_y;
				m_trackables[j].m_pos_z *= 1000;
				m_trackables[j].m_dir_x *= 1000;
				m_trackables[j].m_dir_y *= -1000;
				m_trackables[j].m_dir_z *= 1000;
				m_trackables[j].m_dir_w *= 1000;

				// yaw, pitchの正負を逆にする
				//m_trackables[j].m_yaw = -m_trackables[j].m_yaw;
				//m_trackables[j].m_pitch = m_trackables[j].m_pitch;

				// pos_zのオフセット（床面を0にする）
				//m_trackables[j].m_pos_z += 1050; 
			 }
			
			//テーブルのオフセット
			/*CVector TableVec = CVector(0, 0);
			CVector OffsetVec = CVector(160, -160);
			OffsetVec.Rotate(m_trackables[5].m_yaw);
			TableVec.x = m_trackables[5].m_pos_x;
			TableVec.y = m_trackables[5].m_pos_y;
			TableVec += OffsetVec;
			m_trackables[5].m_pos_x = TableVec.x;
			m_trackables[5].m_pos_y = TableVec.y;*/
			//TRACE("テーブルの位置 = %d, %d\n", (int)m_trackables[5].m_pos_x, (int)m_trackables[5].m_pos_y);
		}
		else
		TRACE("\n");
	}
	return 0;
}


int COptiTrack::CalcPersonIsActive()
{
	// TRHE_TIME ミリ秒以上動いていないとinactiveとみなす
	const int THRE_TIME = 5000;
	int ActiveNum = 0;
	for(int i = 0; i < m_trackableNum-1/*テーブルは抜いている*/; i++)
	{
		if(m_previousTrackables[i].m_pos_x == m_trackables[i].m_pos_x
		&& m_previousTrackables[i].m_pos_y == m_trackables[i].m_pos_y)
		{
			m_InactiveNum[i]++;
			if(m_InactiveNum[i] > THRE_TIME / OPTITRACK_REFRESH_INTERVAL)
			{
				m_IsActive[i] = FALSE;	
			}
		}
		else
		{
			m_InactiveNum[i] = 0;
			m_IsActive[i] = TRUE;
			if(i != 0) ActiveNum++;
		}
	}
	m_ActiveNum = ActiveNum;

	if(m_ActiveNum == 0)
	{
		this->m_IsPeopleActive = false;
	}else
	{
		this->m_IsPeopleActive = true;
	}
	return 0;
}


int COptiTrack::CalcPeopleCenterofGravity()
{
	CVector CenterVec;

	for(int i = 0; i < m_trackableNum-1; i++)
	{
		if(m_IsActive[i] == true)
		{
			CenterVec.x += (double)m_trackables[i].m_pos_x;
			CenterVec.y += (double)m_trackables[i].m_pos_y;
		}
	}
	if(this->m_ActiveNum !=0){
		CenterVec = CenterVec/m_ActiveNum;
	}
	return 0;
}

int COptiTrack::CalcSpeed()
{
	for(int i = 0; i < m_trackableNum; i++)
	{
		m_Speed[i] = m_previousTrackables[i].m_pos_x - m_trackables[i].m_pos_x;
		//TRACE("動くスピード=%d\n", m_Speed[i]);
	}
	return 0;
}


int COptiTrack::CalcDistanceBetweenPeople()
{
	for(int i = 0; i < m_trackableNum; i++)
	{
		for(int j = 0; j < m_trackableNum; j++)
		{
			// 前の対人距離を格納
			m_PreDistanceBetweenPeople[i][j] = m_DistanceBetweenPeople[i][j];
			// 対人距離を格納
			m_DistanceBetweenPeople[i][j] = GetDistanceBetweenTwoPeople(m_trackables[i], m_trackables[j]);
		}
	}
	return 0;
}

//F-formationを計算する関数
int COptiTrack::CalcFformation()
{
	//int slope[6][6];

	for(int i = 1; i < m_trackableNum; i++)
	{	 
		if(this->m_IsActive[i] == true)
		{
			for(int j = i + 1; j < m_trackableNum; j++)
			{
				if(this->m_IsActive[j] == true)
				{
					//slope[i][j] = 90;//(- 0.036 * m_DistanceBetweenPeople[i][j]) + 90;

					if(0 < m_DistanceBetweenPeople[i][j] && m_DistanceBetweenPeople[i][j] < 1500)
					{
						m_FformationFlag[i][j] = true;
						//if(m_PersonAngle[i][j] < 90 && m_PersonAngle[i][j] > -90)
						//{
						//	if( m_PersonAngle[j][i] > -180 && m_PersonAngle[j][i] < -90)
						//	{
						//		if( m_PersonAngle[i][j] - slope[i][j] < m_trackables[i].m_yaw && m_PersonAngle[i][j] > m_trackables[i].m_yaw)
						//		{
						//			if(  m_PersonAngle[j][i] + slope[i][j] > m_trackables[j].m_yaw && m_PersonAngle[j][i] < m_trackables[j].m_yaw)
						//			{
						//				m_FformationFlag[i][j] = true;
						//			}
						//			else
						//			{
						//				m_FformationFlag[i][j] = false;
						//			}
						//		}
						//		if(m_PersonAngle[i][j] + slope[i][j] > m_trackables[i].m_yaw && m_PersonAngle[i][j] < m_trackables[i].m_yaw)
						//		{
						//			if(m_trackables[j].m_yaw > -180)
						//			{
						//				if(m_PersonAngle[j][i] - slope[i][j] < m_trackables[j].m_yaw && m_PersonAngle[j][i] > m_trackables[i].m_yaw)
						//				{
						//					m_FformationFlag[i][j] = true;
						//				}
						//				else
						//				{
						//					m_FformationFlag[i][j] = false;
						//				}
						//			}
						//			else
						//			{
						//				if(-(m_PersonAngle[j][i] - slope[i][j]) < m_trackables[j].m_yaw)
						//				{
						//					m_FformationFlag[i][j] = true;
						//				}
						//				else
						//				{
						//					m_FformationFlag[i][j] = false;
						//				}
						//			}
						//		}
							//}
							//else
							//{
							//	m_FformationFlag[i][j] = false;
							//}
						//}
						//else
						//{
						//	m_FformationFlag[i][j] = false;
						//}
					}
					else
					{
						m_FformationFlag[i][j] = false;
					}
				}
			}
		}
	}
	return 0;
}

// ２人の対人距離を計算する関数
double COptiTrack::GetDistanceBetweenTwoPeople(CTrackable a, CTrackable b)
{
	double x = (double)(a.m_pos_x - b.m_pos_x);
	double y = (double)(a.m_pos_y - b.m_pos_y);
	return sqrt(x*x + y*y);
}

int COptiTrack::CalcHeadRotation()
{
	// 初回だけスルー
	if(m_CalcFirstTime)
	{
		m_CalcFirstTime = FALSE;
		for(int i = 0; i < m_trackableNum/*CRoom::PEOPLE_NUM*/; i++)
		{
			m_PreviousRotateDegree[i] = 0;
			m_PreviousPitchDegree[i] = 0;
			m_NoddingStartYaw[i] = m_trackables[i].m_yaw;
		}
		return 0;
	}

	// 頭部回転があったとみなす閾値（度）
	double threYawDegree = 10;
	// うなずきがあったとみなす閾値（度）
	double threPitchDegree = 5;
	// うなずきのときに許すYawの移動範囲
	double threYawMaxDegree = 7.5;
	// うなずきにかかる時間の閾値
	unsigned int threTime = 1000;// ミリ秒
	unsigned int currentTime = ::timeGetTime();

	for(int i = 0; i < m_trackableNum/*CRoom::PEOPLE_NUM*/; i++)
	{
		 // 単位時間あたりの頭部回転量を計算
		double deltaYawDegree = m_trackables[i].m_yaw - m_previousTrackables[i].m_yaw;
		double deltaPitchDegree = m_trackables[i].m_pitch - m_previousTrackables[i].m_pitch;

		// OptiTrackの誤作動で場所が飛んだとき
		double errorDist = 1000; // ミリメートル
		double errorYawDegree = 90; // 度
		if(GetDistanceBetweenTwoPeople(m_trackables[i], m_previousTrackables[i]) > errorDist || deltaYawDegree > errorYawDegree)
		{
			// 頭部回転角度をリセット
			deltaYawDegree = 0;
			m_RotateDegree[i] = 0;
		}
		else
		{
			// 頭部回転を計算する処理
			//if(i == 0)TRACE(" y1: %lf y2: %lf d; %lf ",m_trackables[i].m_yaw, m_previousTrackables[i].m_yaw, deltaDegree);
			if(deltaYawDegree > 180) deltaYawDegree = 360 - deltaYawDegree;
			else if(deltaYawDegree < -180) deltaYawDegree = 360 + deltaYawDegree;

			// previousと比較
			// 回転方向が一瞬前と同じとき
			if(deltaYawDegree * m_PreviousRotateDegree[i] > 0)
			{
				// 回転量を加算
				m_RotateDegree[i] += deltaYawDegree;
			}
			// 回転方向が変わったとき
			else if(deltaYawDegree * m_PreviousRotateDegree[i] < 0)
			{
				// 閾値を超えているかチェック
				if(m_RotateDegree[i] > threYawDegree)
				{
					//TRACE("%dさんが頭部回転したぞ！\n", i+1);
					// 頭部回転回数をインクリメント
					m_HeadRotationNum[i]++;
				}
				// 回転量をリセット
				m_RotateDegree[i] = deltaYawDegree;
			}

			//うなずきを計算する処理
			if(deltaPitchDegree > 180) deltaPitchDegree = 360 - deltaPitchDegree;
			else if(deltaPitchDegree < -180) deltaPitchDegree = 360 + deltaPitchDegree;

			// 回転方向がうなずく方向のとき
			if(deltaPitchDegree < 0)
			{
				// 回転量を加算
				m_PitchDegree[i] += deltaPitchDegree;
			}
			// 回転方向が元に戻る方向のとき
			else if(deltaPitchDegree > 0)
			{
				// 閾値を超えているかつ，時間がthreTime以内のときかつ，yawの変化が小さいとき
				if(abs(m_PitchDegree[i]) > threPitchDegree &&
				   currentTime - m_NoddingStartTime[i] < threTime &&
				   abs(m_trackables[i].m_yaw - m_NoddingStartYaw[i]) < threYawMaxDegree)
				{
					//TRACE("%dさんがうなずいた！\n", i+1);
					// 頭部回転回数をインクリメント
					m_NoddingNum[i]++;
				}
				// 回転量をリセット
				m_PitchDegree[i] = 0;
				m_NoddingStartYaw[i] = m_trackables[i].m_yaw;
				m_NoddingStartTime[i] = currentTime;
			}
		}
		m_PreviousRotateDegree[i] = deltaYawDegree;
		m_PreviousPitchDegree[i] = deltaPitchDegree;
	}
	return 0;
}

int COptiTrack::CalcPersonOrientation()
{
	for(int i = 1; i < m_trackableNum/*CRoom::PEOPLE_NUM*/; i++)
	{
		int person = -1;
		double miniArg = 360;
		for(int j = 1; j < m_trackableNum/*CRoom::PEOPLE_NUM*/; j++)
		{
			if(i == j) continue;

			CVector3D twoPersonVec = CVector3D(m_trackables[j].m_pos_x - m_trackables[i].m_pos_x, 
											   m_trackables[j].m_pos_y - m_trackables[i].m_pos_y,
											   m_trackables[j].m_pos_z - m_trackables[i].m_pos_z);

			m_PersonAngle[i][j] = atan2(twoPersonVec.Y(), twoPersonVec.X()) * 180 / 3.14159;

//----------------------最も視線に近い人-----------------------------//
			double argGap = abs(m_trackables[i].m_yaw - m_PersonAngle[i][j]/*arg*/);
			if(argGap > 360)argGap -= 360;
			else if(argGap > 180)argGap -= 180;
			
			//if(i == 3 && j == 2)
			//	TRACE("atan: %lf, yaw: %lf\n", arg, m_trackables[i].m_yaw);
			//最も角度の差が小さい人を「向いている人」認定
			if(miniArg > argGap)
			{
				miniArg = argGap;
				person = j;
			}
		if(i == 0)
		m_PersonOrientation[i] = person;
		}

	}
	return 0;
}

int COptiTrack::CalcTable()
{	
	float TablePersonSlope[6];
	double TablePersonDistance[6];
	float TableSlope;
	int UseNum = 0;
	int ApproachNum = 0;
	double PersonApproachToTable[6];
	int PersonToTableAngle[6];
	const int THRE_TIME = 2000;

	for(int i = 1; i < m_trackableNum; i++)
	{	
		CVector3D PersonTableVec = CVector3D(m_trackables[0].m_pos_x - m_trackables[i].m_pos_x,
											 m_trackables[0].m_pos_y - m_trackables[i].m_pos_y,
											 m_trackables[0].m_pos_z - m_trackables[i].m_pos_z);

		PersonToTableAngle[i] = atan2(PersonTableVec.Y(), PersonTableVec.X()) * 180 / 3.14159;
		//ユーザがテーブルを見ているかどうか
		if(this->m_IsActive[i])
		{
			if(PersonToTableAngle[i] - 30 < m_trackables[i].m_yaw && PersonToTableAngle[i] + 30 > m_trackables[i].m_yaw) 
			{
				this->m_PersonSeeTableFlag[i] = true;
			}else{
				this->m_PersonSeeTableFlag[i] = false;
			}
		}
		else this->m_PersonSeeTableFlag[i] = false;

		CVector3D TablePersonVec = CVector3D(m_trackables[i].m_pos_x - m_trackables[0].m_pos_x,
											 m_trackables[i].m_pos_y - m_trackables[0].m_pos_y,
											 m_trackables[i].m_pos_z - m_trackables[0].m_pos_z);

		//ユーザがテーブルを見つめているかどうか
		if(this->m_PersonSeeTableFlag[i] == true)
		{
			m_PersonGazeNum[i]++;
			if(m_PersonGazeNum[i] > THRE_TIME / OPTITRACK_REFRESH_INTERVAL)
			{
				m_PersonGazeTableFlag[i] = true;
			}
		}
		else
		{
			m_PersonGazeNum[i] = 0;
			m_PersonGazeTableFlag[i] = false;
		}


		
		//ユーザに対するテーブルの角度
		this->m_TableToPersonAngle[i] = atan2(TablePersonVec.Y(), TablePersonVec.X()) * 180 / 3.14159;
		//TRACE("ユーザiに対するテーブルの角度 = %d\n", this->m_TableToPersonAngle[i]);
		//人とテーブルの角度のtanθの値
		TablePersonSlope[i] = TablePersonVec.Y()/TablePersonVec.X();
		//ユーザがテーブルに近づいているかどうか
		PersonApproachToTable[i] = this->m_DistanceBetweenPeople[0][i] - this->m_PreDistanceBetweenPeople[0][i];
		if(PersonApproachToTable[i] < -100)
		{
			m_IsPersonApproach[i] = true;
			ApproachNum++;
		}else
		{
			m_IsPersonApproach[i] = false;
		}

		//人とテーブルの間の距離
		TablePersonDistance[i] = this->m_DistanceBetweenPeople[0][i];
		//TRACE("人とテーブルの距離 = %d\n", (int)m_DistanceBetweenPeople[0][i]);
		//ユーザがテーブルを使っているかどうか
		if(this->m_IsActive[i] == true)
		{
			if(TablePersonDistance[i] < 1000)
			{
				this->m_IsPersonUseTable[i] = true;
				UseNum++;
			}else
			{
				this->m_IsPersonUseTable[i] = false;
			}
		}else
		{
			this->m_IsPersonUseTable[i] = false;
		}

	}
	this->m_UseNum = UseNum;
	this->m_ApproachNum = ApproachNum;

	//ユーザのだれかがテーブルを使っているかどうか
		if(m_UseNum > 0)
		{
			m_IsPeopleUseTable = true;
		}else
		{ 
			m_IsPeopleUseTable = false;
		}
	//テーブルの向き
	TableSlope = tan((3.14159 * m_trackables[0].m_yaw) / 180);
	//TRACE("テーブルの傾き = %d\n", (int)TableSlope);

	//テーブルの向き９０°
	double TableSlope90;
	TableSlope90 = tan((3.14159 * m_trackables[0].m_yaw + 90) / 180);

	//テーブルの向き＋４５°
	double TableSlopePlus45;
	TableSlopePlus45 = tan((3.14159 * (m_trackables[0].m_yaw + 45)) / 180);

	//テーブルの向きー４５°
	double TableSlopeMinus45;
	TableSlopeMinus45 = tan((3.14159 * (m_trackables[0].m_yaw - 45)) / 180);

	double Table_y[6];
	double Table90_y[6];
	double TablePlus45_y[6];
	double TableMinus45_y[6];

	CVector tableVector;
	tableVector = CVector(-100, -100);
	tableVector.Rotate(m_trackables[0].m_yaw);
	tableVector += CVector(m_trackables[0].m_pos_x, m_trackables[0].m_pos_y);

	//テーブルの位置に対するユーザの位置
	//int location[6][3];
	for(int i=1; i<6; i++)
	{
		if(i == 0) this->m_PersonLocationToTable[i] = -1;
		else
		{
		TablePlus45_y[i] = TableSlopePlus45 * (m_trackables[i].m_pos_x - tableVector.x) + tableVector.y;


		TableMinus45_y[i] = TableSlopeMinus45 * (m_trackables[i].m_pos_x - tableVector.x) + tableVector.y;


		Table_y[i] = TableSlope * (m_trackables[i].m_pos_x - tableVector.x) + tableVector.y;


		Table90_y[i] = TableSlope90 * (m_trackables[i].m_pos_x - tableVector.x) + tableVector.y;

	
		if(-45 < m_trackables[0].m_yaw && m_trackables[0].m_yaw < 45)
		{
			if(TablePlus45_y[i] > m_trackables[i].m_pos_y)
			{
				if(TableMinus45_y[i] > m_trackables[i].m_pos_y)
				{
					if(-45 < m_trackables[0].m_yaw && m_trackables[0].m_yaw < 0)
					{
						if(Table90_y[i] > m_trackables[i].m_pos_y)
						{
							this->m_PersonLocationToTable[i] = 7;
						}
						else
						{
							this->m_PersonLocationToTable[i] = 6;
						}
					}
					if(0 < m_trackables[0].m_yaw && m_trackables[0].m_yaw < 45)
					{
						if(Table90_y[i] > m_trackables[i].m_pos_y)
						{
							this->m_PersonLocationToTable[i] = 6;
						}
						else
						{
							this->m_PersonLocationToTable[i] = 7;
						}
					}
				}
				else
				{
					if(Table_y[i] > m_trackables[i].m_pos_y)
					{
						this->m_PersonLocationToTable[i] = 0;
					}
					else
					{
						this->m_PersonLocationToTable[i] = 1;
					}
				}
			}
			else
			{
				if(TableMinus45_y[i] > m_trackables[i].m_pos_y)
				{
					if(Table_y[i] > m_trackables[i].m_pos_y)
					{
						this->m_PersonLocationToTable[i] = 5;
					}
					else
					{
						this->m_PersonLocationToTable[i] = 4;
					}
				}
				else
				{
					if(-45 < m_trackables[0].m_yaw && m_trackables[0].m_yaw < 0)
					{
						if(Table90_y[i] > m_trackables[i].m_pos_y)
						{
							this->m_PersonLocationToTable[i] = 2;
						}
						else
						{
							this->m_PersonLocationToTable[i] = 3;
						}
					}
					if(0 < m_trackables[0].m_yaw && m_trackables[0].m_yaw < 45)
					{
						if(Table90_y[i] > m_trackables[i].m_pos_y)
						{
							this->m_PersonLocationToTable[i] = 3;
						}
						else
						{
							this->m_PersonLocationToTable[i] = 2;
						}
					}
				}
			}
		}	
		if(45 < m_trackables[0].m_yaw && m_trackables[0].m_yaw < 135)
		{
			if(TablePlus45_y[i] > m_trackables[i].m_pos_y)
			{
				if(TableMinus45_y[i] > m_trackables[i].m_pos_y)
				{
					if(45 < m_trackables[0].m_yaw && m_trackables[0].m_yaw < 90)
					{
						if(Table_y[i] > m_trackables[i].m_pos_y)
						{
							this->m_PersonLocationToTable[i] = 5;
						}
						else
						{
							this->m_PersonLocationToTable[i] = 4;
						}
					}
					if(90 < m_trackables[0].m_yaw && m_trackables[0].m_yaw < 135)
					{
						if(Table_y[i] > m_trackables[i].m_pos_y)
						{
							this->m_PersonLocationToTable[i] = 4;
						}
						else
						{
							this->m_PersonLocationToTable[i] = 5;
						}
					}
				}
				else
				{
					if(Table90_y[i] > m_trackables[i].m_pos_y)
					{
						this->m_PersonLocationToTable[i] = 3;
					}
					else
					{
						this->m_PersonLocationToTable[i] = 2;
					}
				}
			}
			else
			{
				if(TableMinus45_y[i] > m_trackables[i].m_pos_y)
				{
					if(Table90_y[i] > m_trackables[i].m_pos_y)
					{
						this->m_PersonLocationToTable[i] = 6;
					}
					else
					{
						this->m_PersonLocationToTable[i] = 7;
					}
				}
				else
				{
					if(45 < m_trackables[0].m_yaw && m_trackables[0].m_yaw < 90)
					{
						if(Table_y[i] > m_trackables[i].m_pos_y)
						{
							this->m_PersonLocationToTable[i] = 0;
						}
						else
						{
							this->m_PersonLocationToTable[i] = 1;
						}
					}
					if(90 < m_trackables[0].m_yaw && m_trackables[0].m_yaw < 135)
					{
						if(Table_y[i] > m_trackables[i].m_pos_y)
						{
							this->m_PersonLocationToTable[i] = 1;
						}
						else
						{
							this->m_PersonLocationToTable[i] = 0;
						}
					}
				}
			}
		}
		if(135 < m_trackables[0].m_yaw && m_trackables[0].m_yaw < 180)
		{
			if(TablePlus45_y[i] > m_trackables[i].m_pos_y)
			{
				if(TableMinus45_y[i] > m_trackables[i].m_pos_y)
				{
					if(Table90_y[i] > m_trackables[i].m_pos_y)
					{
						this->m_PersonLocationToTable[i] = 3;
					}
					else
					{
						this->m_PersonLocationToTable[i] = 2;
					}
				}
				else
				{
					if(Table_y[i] > m_trackables[i].m_pos_y)
					{
						this->m_PersonLocationToTable[i] = 1;
					}
					else
					{
						this->m_PersonLocationToTable[i] = 0;
					}
				}
			}
			else
			{
				if(TableMinus45_y[i] > m_trackables[i].m_pos_y)
				{
					if(Table_y[i] > m_trackables[i].m_pos_y)
					{
						this->m_PersonLocationToTable[i] = 4;
					}
					else
					{
						this->m_PersonLocationToTable[i] = 5;
					}
				}
				else
				{
					if(Table90_y[i] > m_trackables[i].m_pos_y)
					{
						this->m_PersonLocationToTable[i] = 6;
					}
					else
					{
						this->m_PersonLocationToTable[i] = 7;
					}
				}
			}
		}
		if(-180 < m_trackables[0].m_yaw && m_trackables[0].m_yaw < -135)
		{
			if(TablePlus45_y[i] > m_trackables[i].m_pos_y)
			{
				if(TableMinus45_y[i] > m_trackables[i].m_pos_y)
				{
					if(Table90_y[i] > m_trackables[i].m_pos_y)
					{
						this->m_PersonLocationToTable[i] = 2;
					}
					else
					{
						this->m_PersonLocationToTable[i] = 3;
					}
				}
				else
				{
					if(Table_y[i] > m_trackables[i].m_pos_y)
					{
						this->m_PersonLocationToTable[i] = 1;
					}
					else
					{
						this->m_PersonLocationToTable[i] = 0;
					}
				}
			}
			else
			{
				if(TableMinus45_y[i] > m_trackables[i].m_pos_y)
				{
					if(Table_y[i] > m_trackables[i].m_pos_y)
					{
						this->m_PersonLocationToTable[i] = 4;
					}
					else
					{
						this->m_PersonLocationToTable[i] = 5;
					}
				}
				else
				{
					if(Table90_y[i] > m_trackables[i].m_pos_y)
					{
						this->m_PersonLocationToTable[i] = 7;
					}
					else
					{
						this->m_PersonLocationToTable[i] = 6;
					}
				}
			}
		}
		if(-135 < m_trackables[0].m_yaw && m_trackables[0].m_yaw < -45)
		{
			if(TablePlus45_y[i] > m_trackables[i].m_pos_y)
			{
				if(TableMinus45_y[i] > m_trackables[i].m_pos_y)
				{
					if(-135 < m_trackables[0].m_yaw && m_trackables[0].m_yaw < -90)
					{
						if(Table_y[i] > m_trackables[i].m_pos_y)
						{
							this->m_PersonLocationToTable[i] = 1;
						}
						else
						{
							this->m_PersonLocationToTable[i] = 0;
						}
					}
					if(-90 < m_trackables[0].m_yaw && m_trackables[0].m_yaw < -45)
					{
						if(Table_y[i] > m_trackables[i].m_pos_y)
						{
							this->m_PersonLocationToTable[i] = 0;
						}
						else
						{
							this->m_PersonLocationToTable[i] = 1;
						}
					}
				}
				else
				{
					if(Table90_y[i] > m_trackables[i].m_pos_y)
					{
						this->m_PersonLocationToTable[i] = 2;
					}
					else
					{
						this->m_PersonLocationToTable[i] = 3;
					}
				}
			}
			else
			{
				if(TableMinus45_y[i] > m_trackables[i].m_pos_y)
				{
					if(Table90_y[i] > m_trackables[i].m_pos_y)
					{
						this->m_PersonLocationToTable[i] = 7;
					}
					else
					{
						this->m_PersonLocationToTable[i] = 6;
					}
				}
				else
				{
					if(-135 < m_trackables[0].m_yaw && m_trackables[0].m_yaw < -90)
					{
						if(Table_y[i] > m_trackables[i].m_pos_y)
						{
							this->m_PersonLocationToTable[i] = 4;
						}
						else
						{
							this->m_PersonLocationToTable[i] = 5;
						}
					}
					if(-90 < m_trackables[0].m_yaw && m_trackables[0].m_yaw < -45)
					{
						if(Table_y[i] > m_trackables[i].m_pos_y)
						{
							this->m_PersonLocationToTable[i] = 5;
						}
						else
						{
							this->m_PersonLocationToTable[i] = 4;
						}
					}
				}
			}
		}
		}
	}

	//TRACE("現在位置 = %d\\", this->m_PersonLocationToTable[1]); 




	return 0;
}

int COptiTrack::CalcWallDisplay()
{

	for(int i = 1; i < m_trackableNum; i++)
	{
		double slope_x = 0;
		double slope_y = 0;

		this->m_DistancePersonToWall[i] = TRACKING_AREA_X - m_trackables[i].m_pos_x; 
		slope_x = tan(m_trackables[i].m_yaw * 3.14159 / 180.0);
		slope_y = tan(m_trackables[i].m_pitch * 3.14159 / 180.0);

		this->m_PersonSeeWallValue_x[i] = m_DistancePersonToWall[i] * slope_x + m_trackables[i].m_pos_y;
		this->m_PersonSeeWallValue_y[i] = m_DistancePersonToWall[i] * slope_y + m_trackables[1].m_pos_z;
	}

	return 0;
}


void COptiTrack::CheckResult(NPRESULT result)   //== CheckResult function will display errors and ---
                                    //== exit application after a key is pressed =====---
{
    if(result==NPRESULT_SUCCESS)    //== If there is no error, do nothing ============---
        return;

    TRACE("Error: %s\n\n(Press any key to continue)\n",TT_GetResultString(result));

    Sleep(200);

    exit(1);
}

int COptiTrack::PushBackData(void)
{
	OUTPUTDATA outPutData;

	outPutData.time = ::timeGetTime() - m_pParameter->m_ProgramStart;
	outPutData.metricData = m_trackables[0];
	outPutData.TaskNumber = m_pParameter->m_ShapePattern;
	outPutData.ObjectNumber = m_pParameter->m_correct;

	//for(int i = 0; i < m_trackableNum/*CRoom::PEOPLE_NUM*/; i++)
	//{
	//	OUTPUTDATA outputData;
	//	
	//	outputData.personId = i;
	//	outputData.time = ::timeGetTime();
	//	outputData.metricData = m_trackables[i];
	//	outputData.headRotateNum = m_HeadRotationNum[i];
	//	outputData.noddingNum = m_NoddingNum[i];
	//	for(int j = 0; j < m_trackableNum/*CRoom::PEOPLE_NUM*/; j++)
	//	{
	//		outputData.distanceBetweenPeople[j] = m_DistanceBetweenPeople[i][j];
	//		outputData.bodyOrientation[j] = m_PersonOrientation[j];
	//	}
	//	outputData.isAnswering = m_IsAnswering[i];
	//	// バッファにため込む
	//	m_outputDataBuffer.push_back(outputData);
	//}
	m_outputDataBuffer.push_back(outPutData);
	return 0;
}

int COptiTrack::CSVOutInit(void)
{
	ofstream file;
	
	file.open("results/optitrack.csv", ios::out | ios::app);
	if(! file.is_open())
	{
		return EXIT_FAILURE;
	}
	// 出力
	file << "Task Number,Time,Pos_X,Pos_Y,Yaw,Object Number" << endl;
	file.close();
	return 0;
}

int COptiTrack::CSVOut(void)
{
	if(!m_IsLogging)
		return 0;

	fstream file;
	
	file.open("results/optitrack.csv", ios::out | ios::app);
	if(! file.is_open())
	{
		return EXIT_FAILURE;
	}

	int size = (int)m_outputDataBuffer.size();

	file << m_outputDataBuffer[0].TaskNumber << ",";
	file << m_outputDataBuffer[0].time << ",";
	file << m_outputDataBuffer[0].metricData.m_pos_x << ",";
	file << m_outputDataBuffer[0].metricData.m_pos_y << ",";
	file << m_outputDataBuffer[0].metricData.m_yaw << ",";
	file << m_outputDataBuffer[0].ObjectNumber << ",";
	file << endl;


	//for(int i = 0; i < size; i++)
	//{
	//	// 出力
	//	// 人のID
	//	file << m_outputDataBuffer[i].personId << ", ";
	//	// 経過時間
	//	file << m_outputDataBuffer[i].time << ", ";
	//	// 幾何学情報
	//	file << m_outputDataBuffer[i].metricData.m_pos_x << ", ";
	//	file << m_outputDataBuffer[i].metricData.m_pos_y << ", ";
	//	file << m_outputDataBuffer[i].metricData.m_pos_z << ", ";
	//	file << m_outputDataBuffer[i].metricData.m_yaw << ", ";
	//	file << m_outputDataBuffer[i].metricData.m_pitch << ", ";
	//	file << m_outputDataBuffer[i].metricData.m_roll << ", ";
	//	// 頭部回転回数・うなずき
	//	file << m_outputDataBuffer[i].headRotateNum << ", ";
	//	file << m_outputDataBuffer[i].noddingNum << ", ";
	//	// 対人距離
	//	for(int j = 0; j < m_trackableNum; j++)
	//	{
	//		// ターゲット方向
	//		file << m_outputDataBuffer[i].distanceBetweenPeople[j] << ", ";
	//	}
	//	//見ている方向
	//	for(int j = 0; j < m_trackableNum; j++)
	//	{
	//		//ターゲット方向
	//		file << m_outputDataBuffer[i].bodyOrientation[j] << ", ";
	//	}
	//	// アンケート回答中かどうか
	//	file << m_outputDataBuffer[i].isAnswering << ", ";
	//	file << endl;
	//}
	file.close();
	
	// バッファにたまってるデータを消す
	m_outputDataBuffer.clear();
	return 0;
}
