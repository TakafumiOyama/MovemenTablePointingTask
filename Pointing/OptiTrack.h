#pragma once
#include <vector>
#include "NPTrackingTools.h"
#include "Trackable.h"
#include "Sensor.h"
#include "Vector3D.h"
#include "Parameter.h"


class COptiTrack:
	public CSensor
{
public:
	COptiTrack(void);
	~COptiTrack(void);

	//オフセット
	static const int PEOPLE_NUM = 6;
	
	// 初期化関数
	int Init(void);
	// 計測開始ボタン以後の処理
	int StartLogging(void);
	// アップデート
	int Update(void);
	
	//Update計算用
	int CalcTrackableMetrics(void);			// 位置・方向など
	int CalcDistanceBetweenPeople(void);	// 対人距離
	int CalcHeadRotation(void);				// 頭部回転回数
	int CalcPersonOrientation(void);		// 人の向き
	int CalcPersonIsActive();			//人がアクティブかどうか
	int CalcSpeed();					//人の速度
	int CalcFformation();              //F-formation
	int CalcTable();                   //テーブルの位置や角度などを計算
	int CalcWallDisplay();             //壁ディスプレイに対する位置や角度などを計算
	int CalcPeopleCenterofGravity();

	//データ出力
	int PushBackData(void);
	int CSVOutInit(void);
	int CSVOut(void);

	//値を取得する関数
	int GetTrackableNum(void){return m_trackableNum;};
	int GetHeadRotateNum(int trackableId){return m_HeadRotationNum[trackableId];};
	int GetNoddingNum(int trackableId){return m_NoddingNum[trackableId];};
	int CheckSensor(void){return m_SensorCheckFlag;};
	int GetSpeed(int personId){return m_Speed[personId];};
	BOOL IsActive(int personId){return m_IsActive[personId];};
	double DistanceBetweenTwoPeople(int personId1, int personId2){return m_DistanceBetweenPeople[personId1][personId2];};
	bool GetFformationOk(int personId1, int personId2){return m_FformationFlag[personId1][personId2];};
	bool GetPersonToTableFlag(int personId1){return m_PersonSeeTableFlag[personId1];};
	bool IsPeopleActive(void){return m_IsPeopleActive;};
	bool GetIsPeopleUseTable(){return m_IsPeopleUseTable;};
	int GetActiveNum(){return m_ActiveNum;};
	int GetUseNum(){return m_UseNum;};
	int GetApproachNum(){return m_ApproachNum;};
	bool GetPersonApproachTable(int personId){ return m_IsPersonApproach[personId];};
	bool GetPersonGazeTableFlag(int personId){ return m_PersonGazeTableFlag[personId];};
	int GetTableToPersonAngle(int personId){ return m_TableToPersonAngle[personId];}; 
	double GetPersonSeeWallValueX(int personId){ return m_PersonSeeWallValue_x[personId];};
	double GetPersonSeeWallValueY(int personId){ return m_PersonSeeWallValue_y[personId];};	
	double GetDistancePersonToWall(int personId){ return m_DistancePersonToWall[personId];};
	int GetPersonLocationToTable(int personId){ return m_PersonLocationToTable[personId];};
	//CVector GetCenterVec(void){ return m_CenterVec;};

	// その他関数
	double GetDistanceBetweenTwoPeople(CTrackable a, CTrackable b);	// ２人の対人距離を計算する関数
	void SetAnswering(int personId, BOOL isAnswering){m_IsAnswering[personId] = isAnswering;}

	// トラッキングエリアの大きさ(mm)
	static const int TRACKING_AREA_X = 4170;
	static const int TRACKING_AREA_Y = 2830;

	static const int OPTITRACK_REFRESH_INTERVAL = 100;	// 値を取得する周期

		//== CheckResult function will display errors and ---
    //== exit application after a key is pressed =====---	
	void CheckResult(NPRESULT result);

	
	// データを取ってくる
	CTrackable GetTrackableInfo(int i)
	{
		//trackableの範囲外にアクセスをチェック
//		ASSERT(0 <= i && (unsigned int)i < m_trackable.size() - 1);

		return m_trackables[i];
	};

protected:

	// はき出すデータ型
	typedef struct _OUTPUTDATA {
		COleDateTimeSpan time;		// 経過時間
		int personId;				// 人のID
		CTrackable metricData;		// 位置や方向などの幾何学情報
		int headRotateNum;			// 頭部回転回数
		int noddingNum;
		double distanceBetweenPeople[PEOPLE_NUM];	// 対人距離
		int bodyOrientation[PEOPLE_NUM];
		BOOL isAnswering;// アンケート回答中かどうか
		int TaskNumber;
		int ObjectNumber;
	} OUTPUTDATA;

	int m_frameCounter;		// フレームカウンター
	BOOL m_flag_SimulationMode;		// 人の動きをシミュレーションで自動生成するかどうか
	std::vector<OUTPUTDATA> m_outputDataBuffer;	// はき出しデータのバッファ
	int m_trackableNum;							// trackableの数 //坂本変換中
	int m_InactiveNum[PEOPLE_NUM];		// 人が動いていない回数
	BOOL m_IsActive[PEOPLE_NUM];			// 人がアクティブかどうか
	bool m_IsPeopleActive;                      //全員がアクティブかどうか
	int m_ActiveNum;                            //動いている人数
	

	// Update関数内で更新されるパラメータ
	std::vector<CTrackable> m_trackables;			// trackableのvector
	std::vector<CTrackable> m_previousTrackables;	// 1つ前のtrackable
	double m_PreDistanceBetweenPeople[PEOPLE_NUM][PEOPLE_NUM];   // 前の対人距離
	double m_DistanceBetweenPeople[PEOPLE_NUM][PEOPLE_NUM];	// 対人距離
	int m_PersonAngle[PEOPLE_NUM][PEOPLE_NUM];        //それぞれの人の頭の向きの角度
	//頭部回転関連
	int m_HeadRotationNum[PEOPLE_NUM];							// 頭部回転回数
	double m_RotateDegree[PEOPLE_NUM];
	double m_PreviousRotateDegree[PEOPLE_NUM];				// 前回単位時間の頭部回転量
	int m_PersonOrientation[PEOPLE_NUM];
	//うなずき関連
	int m_NoddingNum[PEOPLE_NUM];						// うなずき回数
	double m_PreviousPitchDegree[PEOPLE_NUM];
	double m_PitchDegree[PEOPLE_NUM];
	unsigned int m_NoddingStartTime[PEOPLE_NUM];
	double m_NoddingStartYaw[PEOPLE_NUM];
	//歩く速度
	double m_Speed[PEOPLE_NUM];
	//F-formationのフラグ
	bool m_FformationFlag[PEOPLE_NUM][PEOPLE_NUM];
	//テーブル関連
	bool m_PersonSeeTableFlag[PEOPLE_NUM];    //テーブルを見ているかどうかのフラグ
	int m_PersonGazeNum[PEOPLE_NUM];         //テーブルを見ている秒数
	bool m_PersonGazeTableFlag[PEOPLE_NUM];          //テーブルを見つめているかどうかのフラグ
	int m_TableToPersonAngle[PEOPLE_NUM];    //テーブルの人に対する角度
	bool m_IsTableActive;                           //テーブルを動かすかどうか
	bool m_IsPersonUseTable[PEOPLE_NUM];     //それぞれのユーザがテーブルを使っているかどうか
	bool m_IsPeopleUseTable;                        //ユーザのうちだれかがテーブルを使っているかどうか
	int m_UseNum;                               //テーブルを使っている人数
	bool m_IsPersonApproach[PEOPLE_NUM]; //テーブルに近づいているかどうか
	int m_ApproachNum;                          //テーブルに近づいている人数
	int m_PersonLocationToTable[PEOPLE_NUM]; //ユーザのテーブルに対する場所
	//壁ディスプレイ関連
	double m_PersonSeeWallValue_x[PEOPLE_NUM]; //壁ディスプレイに対するユーザの向き
	double m_PersonSeeWallValue_y[PEOPLE_NUM];
	double m_DistancePersonToWall[PEOPLE_NUM]; //人の壁ディスプレイに対する距離

	BOOL m_CalcFirstTime;// yaw,pitchの初期時計算用
	BOOL m_SensorCheckFlag;	//センサが起動したかどうか
	BOOL m_IsLogging;		// ログを取るかどうか
	BOOL m_IsAnswering[PEOPLE_NUM];		// アンケート回答中かどうか

	CParameter * m_pParameter;
};

