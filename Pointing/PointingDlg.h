
// PointingDlg.h : ヘッダー ファイル
//
#include "Parameter.h"
#include "Main.h"
#include "Control.h"
#include "Serial.h"
#include "OptiTrack.h"
#include "Trackable.h"
#include "Window2.h"

#pragma once

using namespace roomba;
using namespace serial;

// CPointingDlg ダイアログ
class CPointingDlg : public CDialogEx
{
// コンストラクション
public:
	CPointingDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_POINTING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

	//----------------------------ポインタの管理--------------------------------
	CParameter * m_pParameter;
	CMain * m_pMain;
	COptiTrack * m_pOptiTrack;
	CTrackable * m_pTrackable;
	serial::CSerial * m_pSerial;
	roomba::CControl* m_pControl;
	CWindow2 * m_pWindow;

	void InitTest(void);
	void UpdateObjectNum();
	DWORD m_StartTime;
	DWORD m_ElapsedTime;
	DWORD m_FinishTime;
	bool TimerStart;

	HWND hWnd;
	LRESULT OnTouch(WPARAM wParam, LPARAM lParam);

// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedTest();
	CString m_WinInit_x;
	CString m_WinInit_y;
	CString m_WinSize_x;
	CString m_WinSize_y;
	CString m_WindowNum;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	//-------------------変形を管理する関数群---------------------
	// 変形の種類をダイアログに表示
	void ShapePattern();
	//それぞれの変形の移動を管理する関数
	void ShiftRecToLine();
	void ShiftLineToRec();
	void ShiftLineToCurve();
	void ShiftCurveToLine();
	void ShiftLineToL();
	void ShiftLToLine();
	void MiniShiftLineToCurve();
	void MiniShiftCurveToLine();
	void MiniShiftLineToL();
	void MiniShiftLToLine();
	void StateRecToLine(int);
	void StateLineToRec(int);
	void StateLineToCurve(int);
	void StateCurveToLine(int);
	void StateLineToL(int);
	void StateLToLine(int);
	// 移動に関して何番目のコマンドか
	int m_currentState1;
	int m_previousState1;
	int m_currentState2;
	int m_previousState2;
	// それぞれの変形を始めるためのフラグ
	void InitFlag();
	bool m_RecToLineFlag;
	bool m_LineToRecFlag;
	bool m_LineToCurveFlag;
	bool m_CurveToLineFlag;
	bool m_LineToLFlag;
	bool m_LToLineFlag;
	//------------------------------------------------------------------------
	
	//-------------------Optitrackのデータに関して---------------------------
	//OptitrackのOn/Off
	bool m_OptiFlag;
	//Trackable の情報を取得する関数
	CTrackable GetTrackableInfo(int id);
	//それぞれのテーブルに対するIDの振り分け
	CTrackable mt1;
	CTrackable mt2;
	CTrackable mt3;
	CTrackable mt4;
	CTrackable person;

	//ダイアログ上のOptitrackの数値をアップデート
	void OptiUpdate();
	//--------------------------------------------------------------------------------------
	afx_msg void OnBnClickedRoomba1();
	afx_msg void OnBnClickedRoomba2();
	afx_msg void OnBnClickedRoomba3();
	afx_msg void OnBnClickedRoomba4();
	afx_msg void OnBnClickedRoombaDisconnect1();
	afx_msg void OnBnClickedRoombaDisconnect2();
	afx_msg void OnBnClickedRoombaDisconnect3();
	afx_msg void OnBnClickedRoombaDisconnect4();
	CString m_TaskPattern;
	CString m_xvLocation_x;
	CString m_xvLocation_y;
	CString m_xvLocation_z;
	CString m_xvDirection_x;
	CString m_xvDirection_y;
	CString m_xvDirection_z;
	afx_msg void OnBnClickedOptiConnect();
	afx_msg void OnBnClickedOptiDisconnect();
	afx_msg void OnBnClickedTest2();
	CString m_xvLocation_x2;
	CString m_xvLocation_y2;
	CString m_xvLocation_z2;
	CString m_xvDirection_x2;
	CString m_xvDirection_y2;
	CString m_xvDirection_z2;
	afx_msg void OnBnClickedMoveOnoff();
	CString m_xvShiftOnOff;
	CString m_xvShapePattern;
private:
	int m_xvShapeState;
public:
	int m_xvObjectCount;
	afx_msg void OnBnClickedCount();
	afx_msg void OnBnClickedTest3();
	afx_msg void OnBnClickedTest4();
};
