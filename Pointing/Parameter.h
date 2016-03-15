#pragma once
#include "Vector.h"
#include <MMSystem.h>
#include <vector>

using namespace std;
class CParameter
{
public:
	CParameter(void);
	~CParameter(void);

	int m_WindowPos_x;
	int m_WindowPos_y;
	int m_WindowSize_x;
	int m_WindowSize_y;
	int m_objNum;

	
	//生成されるウィンドウの初期位置
	static const int WindowInitPosX = 0;
	static const int WindowInitPosY = 0;
	static const int VirtualWindowInitPosX = 0;
	static const int VirtualWindowInitPosY = 0;
	//生成されるウィンドウの初期サイズ
	static int WindowSizeX;
	static int WindowSizeY;
	static const int VirtualWindowSizeX = 3840;
	static const int VirtualWindowSizeY = 2160;
	static const int DefWindowSizeX = 1920;
	static const int DefWindowSizeY = 1080;

	//碁盤目状にデフォルトの位置を設定：1枚のディスプレイにつき3x3の9点配置、ディスプレイが3枚
	static CPoint DefObjectPos[4][3][3];
	static CPoint DefTargetPos[4][3][3];
	static CPoint ObjectPos[100];
	static CPoint ObjectPos2[100];
	static CPoint TargetPos[100];
	static CPoint tmpObjectPos[100];
	static CPoint GhostObjectPos[100];
	//それぞれの値を取得するための関数
	int CreateObjectPos();
	int CreateTargetPos();
	int CreateDefPos3();
	int CreateDefPos4();
	CPoint GetObjectPos(int i);
	CPoint GetTouchPos(CPoint);
	void GetAllParameter(int);
	//値を登録するための関数
	CPoint RegistTouchPos(CPoint);
	
	//ウィンドウハンドル
	HWND hWnd;



	//-------------------------タッチに関する変数・関数-----------------------------------
	//オブジェクト配置のパターン
	static int m_objectPattern;
	//現在のタッチ数
	static int m_TouchCount;
	//ディスプレイから指を離した回数
	static int m_RemoveCount;
	//正しくタッチした回数(基本的には試行の番号とこの値が一致)
	static int m_correct;
	//間違えた回数
	static int m_wrong;
	//カウントをする関数
	int AddCorrect();
	int AddWrong();

	//タッチのカウントを取得する関数
	//正解
	int GetCorrectCount();
	//誤答
	int GetWrongCount();

	//最初のタッチの判定
	static bool IsFirstTouch;
	//試行中かどうかの判定
	static bool IsTasking;
	//正しくタッチしたかどうかの判定
	static bool IsCorrectTouch;
	// タスクが終わったかどうかの判定
	static bool IsTaskFinish;
	// スタートボタンが押されたかどうかの判定
	static bool IsStandby;
	// ゴールを作っていいかどうか
	static bool m_CreateGoalFlag;
	// 

	//現在のオブジェクトの位置
	CPoint currentObjPos;
	//現在タッチした位置
	static CPoint currentTouchPos;
	static CPoint GhostTouchPos;
	static CPoint RemovePos;




	//-------------------------------ファイル出力関係------------------------------------------
	//初期のラベル付け
	int RecordInit();
	//更新するたびにパラメータをファイルに書き込む
	int RecordInformation();
	//データを貯める．現状不要2015/12/16
	int PushBackData();
	//実験を開始する時間を保存しておく．いずれ実装2015/12/16
	static DWORD m_ProgramStart;

	//オブジェクトが出現した時間
	static DWORD m_TaskStartTime;
	//タッチを開始した時間
	static DWORD m_TouchTime;
	//ドラッグが終了した時間
	static DWORD m_TaskFinishTime;
	//----------------------------------------------------------------------------------------------------


	//--------------------------テーブルの形状管理--------------------------------------------
	//変形先の形状を指定する関数
	int TableState(int);
	//次に変形する形の指示
	static int m_TableState;
	//変形前の形状を指定する関数
	int PreTableState(int);
	//今までの形状
	static int m_PreTableState;
	//タスクの種別
	static int m_ShapePattern;

	//移動に関する指示
	static bool LetsGo;
	// 移動を開始する
	static bool m_MoveStart;
	// 移動中である
	static bool m_Moving;
	//変形をするタスクかどうか
	static bool m_ShapeShift;
	//長方形から線形に並んでいるか
	static bool IsLined;
	//L字型に並んでいるかどうか
	static bool IsLshape;


	//--------------------------オブジェクトのプリセット----------------------------------------
	//ウィンドウの枚数
	static int m_windowNum;
	//プリセットの番号
	static int m_presetNum;
	struct ObjectSet{
		int targetCount;
		CPoint Object;
		CPoint Target;
	}OBJ;
	//プリセットのパラメータを作成するための関数
	void CreatePreset(int);
	void SetPreset3(int);
	void SetPreset4(int);


private:
	//前のタッチが行われた時間，次のタッチまでの時間を取得するのに利用
	CString preTime;
	
protected:
	typedef struct _OUTPUTDATA{
		COleDateTimeSpan time;
		int touchCount;
		int correctCount;
		int wrongCount;
		int touchPos_x;
		int touchPox_y;
	}OUTPUTDATA;	
	std::vector<OUTPUTDATA> m_outputDataBuffer;


};
