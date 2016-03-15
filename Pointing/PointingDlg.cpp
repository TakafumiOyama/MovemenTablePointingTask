
// PointingDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Pointing.h"
#include "PointingDlg.h"
#include "afxdialogex.h"
#include <iostream>
//#include <string>
//#include <sstream>
#include "Window2.h"
#include "Window3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

serial::CSerial serial_;
CControl control;

// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPointingDlg ダイアログ




CPointingDlg::CPointingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPointingDlg::IDD, pParent)
	, m_WinInit_x(_T("0"))
	, m_WinInit_y(_T("0"))
	, m_WinSize_x(_T("100"))
	, m_WinSize_y(_T("100"))
	, m_WindowNum(_T("3"))
	, m_TaskPattern(_T("1"))
	, m_xvLocation_x(_T("0"))
	, m_xvLocation_y(_T("0"))
	, m_xvLocation_z(_T("0"))
	, m_xvDirection_x(_T("0"))
	, m_xvDirection_y(_T("0"))
	, m_xvDirection_z(_T("0"))
	, m_OptiFlag(false)
	, m_currentState1(0)
	, m_currentState2(0)
	, m_RecToLineFlag(false)
	, m_LineToRecFlag(false)
	, m_LineToCurveFlag(false)
	, m_CurveToLineFlag(false)
	, m_LineToLFlag(false)
	, m_LToLineFlag(false)
	, m_xvLocation_x2(_T("0"))
	, m_xvLocation_y2(_T("0"))
	, m_xvLocation_z2(_T("0"))
	, m_xvDirection_x2(_T("0"))
	, m_xvDirection_y2(_T("0"))
	, m_xvDirection_z2(_T("0"))
	, m_xvShiftOnOff(_T("false"))
	, m_xvShapePattern(_T(""))
	, m_xvShapeState(0)
	, m_xvObjectCount(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPointingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_WinInit_x);
	DDX_Text(pDX, IDC_EDIT2, m_WinInit_y);
	DDX_Text(pDX, IDC_EDIT3, m_WinSize_x);
	DDX_Text(pDX, IDC_EDIT4, m_WinSize_y);
	DDX_Text(pDX, IDC_EDIT5, m_WindowNum);
	DDX_Text(pDX, IDC_EDIT6, m_TaskPattern);
	DDX_Text(pDX, IDC_POSITION_X, m_xvLocation_x);
	DDX_Text(pDX, IDC_POSITION_Y, m_xvLocation_y);
	DDX_Text(pDX, IDC_POSITION_Z, m_xvLocation_z);
	DDX_Text(pDX, IDC_DIRECTION_X, m_xvDirection_x);
	DDX_Text(pDX, IDC_DIRECTION_Y, m_xvDirection_y);
	DDX_Text(pDX, IDC_DIRECTION_Z, m_xvDirection_z);
	DDX_Text(pDX, IDC_POSITION_X2, m_xvLocation_x2);
	DDX_Text(pDX, IDC_POSITION_Y2, m_xvLocation_y2);
	DDX_Text(pDX, IDC_POSITION_Z2, m_xvLocation_z2);
	DDX_Text(pDX, IDC_DIRECTION_X2, m_xvDirection_x2);
	DDX_Text(pDX, IDC_DIRECTION_Y3, m_xvDirection_y2);
	DDX_Text(pDX, IDC_DIRECTION_Z2, m_xvDirection_z2);
	DDX_Text(pDX, IDC_ISMOVE, m_xvShiftOnOff);
	DDX_Text(pDX, IDC_PATTERN2, m_xvShapePattern);
	DDX_Radio(pDX, IDC_RADIO1, m_xvShapeState);
	DDX_Text(pDX, IDC_OBJNUMADD, m_xvObjectCount);
}

BEGIN_MESSAGE_MAP(CPointingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPointingDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_TEST, &CPointingDlg::OnBnClickedTest)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ROOMBA1, &CPointingDlg::OnBnClickedRoomba1)
	ON_BN_CLICKED(IDC_ROOMBA2, &CPointingDlg::OnBnClickedRoomba2)
	ON_BN_CLICKED(IDC_ROOMBA3, &CPointingDlg::OnBnClickedRoomba3)
	ON_BN_CLICKED(IDC_ROOMBA4, &CPointingDlg::OnBnClickedRoomba4)
	ON_BN_CLICKED(IDC_ROOMBA_DISCONNECT1, &CPointingDlg::OnBnClickedRoombaDisconnect1)
	ON_BN_CLICKED(IDC_ROOMBA_DISCONNECT2, &CPointingDlg::OnBnClickedRoombaDisconnect2)
	ON_BN_CLICKED(IDC_ROOMBA_DISCONNECT3, &CPointingDlg::OnBnClickedRoombaDisconnect3)
	ON_BN_CLICKED(IDC_ROOMBA_DISCONNECT4, &CPointingDlg::OnBnClickedRoombaDisconnect4)
	ON_BN_CLICKED(IDC_OPTI_CONNECT, &CPointingDlg::OnBnClickedOptiConnect)
	ON_BN_CLICKED(IDC_OPTI_DISCONNECT, &CPointingDlg::OnBnClickedOptiDisconnect)
	ON_BN_CLICKED(IDC_TEST2, &CPointingDlg::OnBnClickedTest2)
	ON_BN_CLICKED(IDC_MOVE_ONOFF, &CPointingDlg::OnBnClickedMoveOnoff)
	ON_BN_CLICKED(IDC_COUNT, &CPointingDlg::OnBnClickedCount)
	ON_BN_CLICKED(IDC_TEST3, &CPointingDlg::OnBnClickedTest3)
	ON_BN_CLICKED(IDC_TEST4, &CPointingDlg::OnBnClickedTest4)
END_MESSAGE_MAP()


// CPointingDlg メッセージ ハンドラー

BOOL CPointingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定
	RegisterTouchWindow();

	// TODO: 初期化をここに追加します。
	m_pParameter = new CParameter();
	m_pMain = new CMain();
	m_pOptiTrack = new COptiTrack();
	m_pTrackable = new CTrackable();
	m_pSerial = new serial::CSerial();
	m_pControl = new roomba::CControl();
	TimerStart = false;

	hWnd = ::FindWindow(NULL, _T("Pointing"));
	::RegisterTouchWindow(hWnd, 0);

	SetTimer(1,10,0);

	m_pParameter->RecordInit();

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CPointingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CPointingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CPointingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPointingDlg::OnBnClickedButton1()
{
	UpdateData();
	//m_pParameter->RecordInformation();
	
	//ダイアログの値を変数に格納
	m_pParameter->m_WindowPos_x = _ttoi(m_WinInit_x);
	m_pParameter->m_WindowPos_y = _ttoi(m_WinInit_y);
	m_pParameter->m_WindowSize_x = _ttoi(m_WinSize_x);
	m_pParameter->m_WindowSize_y = _ttoi(m_WinSize_y);
	m_pParameter->m_windowNum = _ttoi(m_WindowNum);
	m_pParameter->m_presetNum = _ttoi(m_TaskPattern);

	TRACE("(%d, %d)\n", m_pParameter->m_WindowPos_x, m_pParameter->m_WindowPos_y);
	TRACE(m_WinInit_x);


	//if(!pMyWnd2)
	//	new CWindow3(this);
	//HWND h1 = ::FindWindow(NULL, _T("SubWindow1"));
	//::SetWindowPos(h1,HWND_TOP, 1920, -1080, 1920, 1080, 0);

	if(!pMyWnd)
		m_pWindow = new CWindow2(this);

	HWND h = ::FindWindow(NULL, _T("Window"));
	//::SetWindowPos(h,HWND_TOP, m_pParameter->WindowInitPosX, m_pParameter->WindowInitPosY, m_pParameter->WindowSizeX, m_pParameter->WindowSizeY,0);
	if(m_pParameter->m_windowNum == 3)
	{
		m_pParameter->WindowSizeX = 5760;
		m_pParameter->WindowSizeY = 1080;
		m_pParameter->CreateDefPos3();
	}
	::SetWindowPos(h,HWND_TOP, m_pParameter->WindowInitPosX, m_pParameter->WindowInitPosY, m_pParameter->WindowSizeX, m_pParameter->WindowSizeY,0);

	Sleep(100);

	//ShapePattern();
	TimerStart = true;

	m_pParameter->LetsGo = false;
	m_pParameter->IsStandby = true;
	m_pParameter->m_ProgramStart = ::timeGetTime();
	ShapePattern();
	//m_pParameter->CreateDefTargetPos();
	m_pParameter->CreatePreset(m_pParameter->m_presetNum);
	//for(int i=0;i<27;i++){
	//	m_pParameter->tmpObjectPos[i] = m_pParameter->ObjectPos[i];
	//	TRACE("ObjectPos%d = (%d, %d)\n",i,m_pParameter->ObjectPos[i].x, m_pParameter->ObjectPos[i].y);
	//}

	//m_pParameter->m_correct = -1;
}


//色んな移動とかその辺のテストを行いたい関数
void CPointingDlg::OnBnClickedTest()
{
	m_pParameter->m_MoveStart = true;
	m_pParameter->LetsGo = true;
	this->m_currentState1 = 0;
	this->m_currentState2 = 0;
	this->m_LineToCurveFlag = true;
	this->m_CurveToLineFlag = false;
}

void CPointingDlg::OnBnClickedTest2()
{
	m_pParameter->m_MoveStart = true;
	m_pParameter->LetsGo = true;
	this->m_currentState1 = 0;
	this->m_currentState2 = 0;
	this->m_CurveToLineFlag = true;
	this->m_LineToCurveFlag = false;
	
}

void CPointingDlg::OnBnClickedTest3()
{
	m_pParameter->m_MoveStart = true;
	m_pParameter->LetsGo = true;
	this->m_currentState1 = 0;
	this->m_currentState2 = 0;
	this->m_LineToLFlag = true;
	this->m_LToLineFlag = false;
}


void CPointingDlg::OnBnClickedTest4()
{
	m_pParameter->m_MoveStart = true;
	m_pParameter->LetsGo = true;
	this->m_currentState1 = 0;
	this->m_currentState2 = 0;
	this->m_LToLineFlag = true;
	this->m_LineToLFlag = false;
}



LRESULT OnTouch(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    //Insert handler code here to do something with the message or uncomment the following line to test
    //MessageBox(L"touch!", L"touch!", MB_OK);
    return 0;
}

CTrackable CPointingDlg::GetTrackableInfo(int id)
{
	return m_pOptiTrack->GetTrackableInfo(id);
}

//-----------------------------------------------------------------------------------------タイマー---------------------------------------------------------------------------------------
void CPointingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	if(nIDEvent == 1)
	{
		if(m_OptiFlag == true)
		{
			m_pOptiTrack->Update();
			OptiUpdate();
			mt1 = this->GetTrackableInfo(4);
			mt2 = this->GetTrackableInfo(5);
		}
		if(mt1.m_yaw < 0)
			mt1.m_yaw = mt1.m_yaw + 360;
		if(mt2.m_yaw < 0)
			mt2.m_yaw = mt2.m_yaw + 360;
		
		if(TimerStart)
			m_pWindow->OnPaint();

		if(m_pParameter->m_correct >14)
			m_pParameter->LetsGo = true;

		if(m_pParameter->m_ShapeShift == true && m_pParameter->LetsGo)
		{
			m_pParameter->m_MoveStart = true;
			m_pParameter->m_Moving = true;
			if(m_LineToLFlag)
				m_pParameter->IsLshape = true;
			if(m_LToLineFlag)
				m_pParameter->IsLshape = false;
			Sleep(500);
			m_pParameter->m_ShapeShift = false;
			m_pParameter->IsStandby = false;
			m_pWindow->OnPaint();
		}

		if(m_pParameter->m_MoveStart)
		{
			if(m_RecToLineFlag)
				ShiftRecToLine();
			if(m_LineToRecFlag)
				ShiftLineToRec();
			if(m_LineToLFlag)
				ShiftLineToL();
			if(m_LToLineFlag)
				ShiftLToLine();
			if(m_LineToCurveFlag)
				ShiftLineToCurve();
			if(m_CurveToLineFlag)
				ShiftCurveToLine();
		}
		ShapePattern();
		UpdateObjectNum();

		if(m_pParameter->IsStandby == true && m_pParameter->m_Moving == false)
		{
			m_pWindow->OnPaint();
			Sleep(2500);
			m_pParameter->IsStandby = false;
			m_pParameter->m_TaskStartTime = ::timeGetTime() - m_pParameter->m_ProgramStart;
		}

	}
	CDialogEx::OnTimer(nIDEvent);
}

void CPointingDlg::OptiUpdate()
{
	this->m_xvLocation_x.Format(_T("%g"),mt1.m_pos_x);
	this->m_xvLocation_y.Format(_T("%g"),mt1.m_pos_y);
	this->m_xvLocation_z.Format(_T("%g"),mt1.m_pos_z);
	this->m_xvDirection_x.Format(_T("%g"),mt1.m_yaw);
	this->m_xvDirection_y.Format(_T("%g"),mt1.m_pitch);
	this->m_xvDirection_z.Format(_T("%g"),mt1.m_roll);
	this->m_xvLocation_x2.Format(_T("%g"),mt2.m_pos_x);
	this->m_xvLocation_y2.Format(_T("%g"),mt2.m_pos_y);
	this->m_xvLocation_z2.Format(_T("%g"),mt2.m_pos_z);
	this->m_xvDirection_x2.Format(_T("%g"),mt2.m_yaw);
	this->m_xvDirection_y2.Format(_T("%g"),mt2.m_pitch);
	this->m_xvDirection_z2.Format(_T("%g"),mt2.m_roll);

	UpdateData(FALSE);
}

void CPointingDlg::InitFlag()
{
	m_RecToLineFlag = false;
	m_LineToRecFlag = false;
	m_LineToCurveFlag = false;
	m_CurveToLineFlag = false;
	m_LineToLFlag = false;
	m_LToLineFlag = false;
}

void CPointingDlg::ShapePattern()
{
	UpdateData();
	InitFlag();
	switch(m_xvShapeState)
	{
	case 0:
		this->m_xvShapePattern.Format(_T("長方形"));
		break;
	case 1:
		this->m_xvShapePattern.Format(_T("線形(4)"));
		break;
	case 2:
		this->m_xvShapePattern.Format(_T("線形(3)"));
		break;
	case 3:
		this->m_xvShapePattern.Format(_T("曲線形"));
		break;
	case 4:
		this->m_xvShapePattern.Format(_T("線形(3)"));
		break;
	case 5:
		this->m_xvShapePattern.Format(_T("L字型"));
		m_pParameter->IsLshape = true;
		break;
	case 6:
		this->m_xvShapePattern.Format(_T("長方形＞線形"));
		this->m_RecToLineFlag = true;
		break;
	case 7:
		this->m_xvShapePattern.Format(_T("線形＞長方形"));
		this->m_LineToRecFlag = true;
		break;
	case 8:
		this->m_xvShapePattern.Format(_T("線形＞曲線形"));
		this->m_LineToCurveFlag = true;
		break;
	case 9:
		this->m_xvShapePattern.Format(_T("曲線形＞線形"));
		this->m_CurveToLineFlag = true;
		break;
	case 10:
		this->m_xvShapePattern.Format(_T("線形＞L字型"));
		this->m_LineToLFlag = true;
		if(!m_pParameter->m_MoveStart)
			m_pParameter->IsLshape = false;
		break;
	case 11:
		this->m_xvShapePattern.Format(_T("L字型＞線形"));
		this->m_LToLineFlag = true;
		if(!m_pParameter->m_MoveStart)
			m_pParameter->IsLshape = true;
		break;
	default:
		break;
	}
	m_pParameter->m_ShapePattern = m_xvShapeState;
	UpdateData(FALSE);
}

void CPointingDlg::ShiftRecToLine()
{
	//もとが長方形の場合のみ変形を適用
	if(m_pParameter->m_TableState == 0)
	{
		switch(m_currentState1)
		{
		case 0:
			m_pControl->RoombaGo1();
			m_currentState1 = 1;
			break;
		case 1:
			if(2920 < mt1.m_pos_x){
				m_pControl->RoombaRight1();
				m_currentState1 = 2;
			}
			break;
		case 2:
			if(45 < mt1.m_yaw && mt1.m_yaw < 320){
				m_pControl->RoombaGo1();
				m_currentState1 = 3;
			}
			break;
		case 3:
			if(2150 < mt1.m_pos_y){
				m_pControl->RoombaLeft1();
				m_currentState1 = 4;
			}
			break;
		case 4:
			if(mt1.m_yaw < 10){
				m_pControl->RoombaBack1();
				m_currentState1 = 5;
			}
			break;
		case 5:
			if(mt1.m_pos_x < 3230){
				m_pControl->RoombaStop1();
				m_currentState1 = 6;
				m_pParameter->IsLined = true;
			}
			break;
		default:
			break;
		}
		
		switch(m_currentState2)
		{
		case 0:
			m_pControl->RoombaBack2();
			m_currentState2 = 1;
			break;
		case 1:
			if(mt2.m_pos_x < 1270){
				m_pControl->RoombaLeft2();
				m_currentState2 = 2;
			}
			break;
		case 2:
			if(225 < mt2.m_yaw){
				m_pControl->RoombaBack2();
				m_currentState2 = 3;
			}
			break;
		case 3:
			if(2200 < mt2.m_pos_y){
				m_pControl->RoombaRight2();
				m_currentState2 = 4;
			}
			break;
		case 4:
			if(mt2.m_yaw< 185){
				m_pControl->RoombaGo2();
				m_currentState2 = 5;
			}
			break;
		case 5:
			if(910 < mt2.m_pos_x){
				m_pControl->RoombaStop2();
				m_currentState2 = 6;
				m_pParameter->IsLined = true;
			}
			break;
		default:
			break;
		}

	}
}

void CPointingDlg::ShiftLineToRec()
{
	if(m_pParameter->m_TableState == 0)
	{
		switch(m_currentState1)
		{
		case 0:
			m_pControl->RoombaGo1();
			m_currentState1 = 1;
			break;
		case 1:
			if(3300 < mt1.m_pos_x){
				m_pControl->RoombaRight1();
				m_currentState1 = 2;
			}
			break;
		case 2:
			if(40 < mt1.m_yaw && mt1.m_yaw < 320){
				m_pControl->RoombaBack1();
				m_currentState1 = 3;
			}
			break;
		case 3:
			if(mt1.m_pos_y < 1880){
				m_pControl->RoombaLeft1();
				m_currentState1 = 4;
			}
			break;
		case 4:
			if(mt1.m_yaw<10){
				m_pControl->RoombaBack1();
				m_currentState1 = 5;
			}
			break;
		case 5:
			if(mt1.m_pos_x < 2570){
				m_pControl->RoombaStop1();
				m_currentState1 = 6;
				m_pParameter->IsLined = false;
			}
			break;
		default:
			break;
		}
		
		switch(m_currentState2)
		{
		case 0:
			m_pControl->RoombaBack2();
			m_currentState2 = 1;
			break;
		case 1:
			if(mt2.m_pos_x < 870){
				m_pControl->RoombaLeft2();
				m_currentState2 = 2;
			}
			break;
		case 2:
			if(225 < mt2.m_yaw){
				m_pControl->RoombaGo2();
				m_currentState2 = 3;
			}
			break;
		case 3:
			if(mt2.m_pos_y < 1900){
				m_pControl->RoombaRight2();
				m_currentState2 = 4;
			}
			break;
		case 4:
			if(mt2.m_yaw< 182){
				m_pControl->RoombaGo2();
				m_currentState2 = 5;
			}
			break;
		case 5:
			if(1570 < mt2.m_pos_x){
				m_pControl->RoombaStop2();
				m_currentState2 = 6;
				m_pParameter->IsLined = false;
			}
			break;
		default:
			break;
		}
	}

}

void CPointingDlg::ShiftLineToCurve()
{
	if(m_pParameter->m_TableState == 0)
	{
		switch(m_currentState1)
		{
		case 0:
			m_pControl->RoombaGo1();
			m_currentState1 = 1;
			TRACE("curve start %d",::timeGetTime());
			break;
		case 1:
			if(2020 < mt1.m_pos_y){
				m_pControl->RoombaRight1();
				m_currentState1 = 2;
			}
			break;
		case 2:
			if(55 < mt1.m_yaw && mt1.m_yaw < 275){
				m_pControl->RoombaStop1();
				m_currentState1 = 3;
				TRACE("curve finish %d",::timeGetTime());
			}
			break;
		default:
			break;
		}
		
		switch(m_currentState2)
		{
		case 0:
			m_pControl->RoombaGo2();
			m_currentState2 = 1;
			break;
		case 1:
			if(2110 < mt2.m_pos_y){
				m_pControl->RoombaLeft2();
				m_currentState2 = 2;
			}
			break;
		case 2:
			if(235 < mt2.m_yaw && mt2.m_yaw < 325){
				m_pControl->RoombaStop2();
				m_currentState2 = 3;
				TRACE("curve finish %d",::timeGetTime());
			}
			break;
		default:
			break;
		}
		if(m_currentState1 == 3 && m_currentState2 == 3)
		{
			m_pParameter->LetsGo = false;
			m_pParameter->m_Moving = false;
			m_pParameter->IsStandby = true;
			m_currentState1 =4;
			m_currentState2 =4;
		}
	}
}

void CPointingDlg::ShiftCurveToLine()
{
	if(m_pParameter->m_TableState == 0)
	{
		switch(m_currentState1)
		{
		case 0:
			m_pControl->RoombaBack1();
			m_pControl->RoombaBack2();
			Sleep(100);
			m_pControl->RoombaLeft1();
			m_currentState1 = 1;
			break;
		case 1:
			if(mt1.m_yaw < 10){
				m_pControl->RoombaBack1();
				m_currentState1 = 2;
			}
			break;
		case 2:
			if(mt1.m_pos_y < 1720){
				m_pControl->RoombaStop1();
				m_currentState1 = 3;
				m_pParameter->m_Moving = false;
				m_pParameter->LetsGo = false;
			}
			break;
		default:
			break;
		}
		
		switch(m_currentState2)
		{
		case 0:
			m_pControl->RoombaRight2();
			m_currentState2 = 1;
			break;
		case 1:
			if(mt2.m_yaw < 190){
				m_pControl->RoombaBack2();
				m_currentState2 = 2;
			}
			break;
		case 2:
			if(mt2.m_pos_y < 1800){
				m_pControl->RoombaStop2();
				m_currentState2 = 3;
				m_pParameter->m_Moving = false;
				m_pParameter->LetsGo = false;
			}
			break;
		default:
			break;
		}
		if(m_currentState1 == 3 && m_currentState2 == 3)
			m_pParameter->IsStandby = true;
	}
}

void CPointingDlg::ShiftLineToL()
{
	if(m_pParameter->m_TableState == 0)
	{
		switch(m_currentState1)
		{
		case 0:
			m_pControl->RoombaGo1();
			m_currentState1 = 1;
			TRACE("L Start %d",::timeGetTime());
			break;
		case 1:
			if(2180 < mt1.m_pos_y){
				m_pControl->RoombaRight1();
				m_currentState1 = 2;
			}
			break;
		case 2:
			if(75 < mt1.m_yaw && mt1.m_yaw < 320){
				m_pControl->RoombaGo1();
				m_currentState1 = 3;
			}
			break;
		case 3:
			if(mt1.m_pos_x < 2390){
				m_pControl->RoombaStop1();
				m_currentState1 = 4;
				m_pParameter->IsLshape = true;
				m_pParameter->m_Moving = false;
				TRACE("L finish %d",::timeGetTime());
				m_pParameter->LetsGo = false;
				m_pParameter->IsStandby = true;
			}
			break;
		default:
			break;
		}
		
	}
}

void CPointingDlg::ShiftLToLine()
{
	m_pParameter->IsLshape = false;
	if(m_pParameter->m_TableState == 0)
	{
		switch(m_currentState1)
		{
		case 0:
			m_pControl->RoombaBack1();
			m_currentState1 = 1;
			break;
		case 1:
			if(2810 < mt1.m_pos_x){
				m_pControl->RoombaLeft1();
				m_currentState1 = 2;
			}
			break;
		case 2:
			if(mt1.m_yaw < 15){
				m_pControl->RoombaBack1();
				m_currentState1 = 3;
			}
			break;
		case 3:
			if(mt1.m_pos_y < 1710){
				m_pControl->RoombaStop1();
				m_currentState1 = 4;
				m_pParameter->IsLshape = false;
				m_pParameter->m_Moving = false;
				m_pParameter->LetsGo = false;
				m_pParameter->IsStandby = true;
			}
			break;
		default:
			break;
		}
		
	}
}

void CPointingDlg::MiniShiftLineToCurve()
{
	if(m_pParameter->m_TableState == 0)
	{
		switch(m_currentState1)
		{
		case 0:
			TRACE("%d\n",::timeGetTime());
			m_pControl->RoombaGo1();
			m_currentState1 = 1;
			break;
		case 1:
			if(2880 < mt1.m_pos_x){
				m_pControl->RoombaRight1();
				m_currentState1 = 2;
			}
			break;
		case 2:
			if(85<mt1.m_yaw && mt1.m_yaw < 270){
				m_pControl->RoombaGo1();
				m_currentState1 = 3;
			}
			break;
		case 3:
			if(2550 < mt1.m_pos_y){
				m_pControl->RoombaLeft1();
				m_currentState1 = 4;
			}
			break;
		case 4:
			if(mt1.m_yaw < 48){
				m_pControl->RoombaStop1();
				m_currentState1 = 5;
			TRACE("%d\n",::timeGetTime());
			}
			break;
		default:
			break;
		}
		
		switch(m_currentState2)
		{
		case 0:
			m_pControl->RoombaBack2();
			m_currentState2 = 1;
			break;
		case 1:
			if(mt2.m_pos_x < 1250){
				m_pControl->RoombaLeft2();
				m_currentState2 = 2;
			}
			break;
		case 2:
			if(270 < mt2.m_yaw){
				m_pControl->RoombaBack2();
				m_currentState2 = 3;
			}
			break;
		case 3:
			if(2560 < mt2.m_pos_y){
				m_pControl->RoombaRight2();
				m_currentState2 = 4;
			}
			break;
		case 4:
			if(mt2.m_yaw< 230){
				m_pControl->RoombaStop2();
				m_currentState2 = 5;
			}
			break;
		default:
			break;
		}
	}
}

void CPointingDlg::MiniShiftCurveToLine()
{
	if(m_pParameter->m_TableState == 0)
	{
		switch(m_currentState1)
		{
		case 0:
			m_pControl->RoombaRight1();
			m_currentState1 = 1;
			break;
		case 1:
			if(85 < mt1.m_yaw){
				m_pControl->RoombaBack1();
				m_currentState1 = 2;
			}
			break;
		case 2:
			if(mt1.m_pos_y < 2330){
				m_pControl->RoombaLeft1();
				m_currentState1 = 3;
			}
			break;
		case 3:
			if(mt1.m_yaw < 3){
				m_pControl->RoombaBack1();
				m_currentState1 = 4;
			}
			break;
		case 4:
			if(mt1.m_pos_x < 2900){
				m_pControl->RoombaStop1();
				m_currentState1 = 5;
			}
			break;
		default:
			break;
		}
		
		switch(m_currentState2)
		{
		case 0:
			m_pControl->RoombaLeft2();
			m_currentState2 = 1;
			break;
		case 1:
			if(270 < mt2.m_yaw){
				m_pControl->RoombaGo2();
				m_currentState2 = 2;
			}
			break;
		case 2:
			if(mt2.m_pos_y < 2370){
				m_pControl->RoombaRight2();
				m_currentState2 = 3;
			}
			break;
		case 3:
			if(mt2.m_yaw < 185){
				m_pControl->RoombaGo2();
				m_currentState2 = 4;
			}
			break;
		case 4:
			if(1250 < mt2.m_pos_x){
				m_pControl->RoombaStop2();
				m_currentState2 = 5;
			}
			break;
		default:
			break;
		}
	}
}

void CPointingDlg::MiniShiftLineToL()
{
	if(m_pParameter->m_TableState == 0)
	{
		m_pParameter->IsLshape = true;
		switch(m_currentState1)
		{
		case 0:
			m_pControl->RoombaGo1();
			m_currentState1 = 1;
			break;
		case 1:
			if(2980 < mt1.m_pos_x){
				m_pControl->RoombaRight1();
				m_currentState1 = 2;
			}
			break;
		case 2:
			if(120 < mt1.m_yaw && mt1.m_yaw < 300){
				m_pControl->RoombaGo1();
				m_currentState1 = 3;
			}
			break;
		case 3:
			if(mt1.m_pos_x < 2300){
				m_pControl->RoombaLeft1();
				m_currentState1 = 4;
			}
			break;
		case 4:
			if(mt1.m_yaw < 95){
				m_pControl->RoombaBack1();
				m_currentState1 = 5;
			}
			break;
		case 5:
			if(mt1.m_pos_y < 2930){
				m_pControl->RoombaStop1();
				m_currentState1 = 6;
				m_pParameter->IsLshape = true;
			}
			break;
		default:
			break;
		}
		
	}
}

void CPointingDlg::MiniShiftLToLine()
{
	if(m_pParameter->m_TableState == 0)
	{
		switch(m_currentState1)
		{
		case 0:
			m_pControl->RoombaGo1();
			m_currentState1 = 1;
			break;
		case 1:
			if(2980 < mt1.m_pos_y){
				m_pControl->RoombaRight1();
				m_currentState1 = 2;
			}
			break;
		case 2:
			if(135 < mt1.m_yaw){
				m_pControl->RoombaBack1();
				m_currentState1 = 3;
			}
			break;
		case 3:
			if(mt1.m_pos_y < 2330){
				m_pControl->RoombaLeft1();
				m_currentState1 = 4;
			}
			break;
		case 4:
			if(mt1.m_yaw < 3){
				m_pControl->RoombaBack1();
				m_currentState1 = 5;
			}
			break;
		case 5:
			if(mt1.m_pos_x < 2900){
				m_pControl->RoombaStop1();
				m_currentState1 = 6;
				m_pParameter->IsLshape = false;
			}
			break;
		default:
			break;
		}
		
	}
}

void CPointingDlg::StateRecToLine(int state)
{
}

void CPointingDlg::StateLineToRec(int state)
{
	
}

void CPointingDlg::StateLineToCurve(int state)
{
}

void CPointingDlg::StateCurveToLine(int state)
{
}

void CPointingDlg::StateLineToL(int state)
{
}

void CPointingDlg::StateLToLine(int state)
{
}

void CPointingDlg::OnBnClickedRoomba1()
{
	m_pControl->RoombaConnect1();
}


void CPointingDlg::OnBnClickedRoomba2()
{
	m_pControl->RoombaConnect2();
}


void CPointingDlg::OnBnClickedRoomba3()
{
	m_pControl->RoombaConnect3();
}


void CPointingDlg::OnBnClickedRoomba4()
{
	m_pControl->RoombaConnect4();
}


void CPointingDlg::OnBnClickedRoombaDisconnect1()
{
	m_pControl->RoombaDisConnect1();
}


void CPointingDlg::OnBnClickedRoombaDisconnect2()
{
	m_pControl->RoombaDisConnect2();
}


void CPointingDlg::OnBnClickedRoombaDisconnect3()
{
	m_pControl->RoombaDisConnect3();
}


void CPointingDlg::OnBnClickedRoombaDisconnect4()
{
	m_pControl->RoombaDisConnect4();
}


void CPointingDlg::OnBnClickedOptiConnect()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	
	m_OptiFlag = true;
	m_pOptiTrack->Init();

	UpdateData(FALSE);
}


void CPointingDlg::OnBnClickedOptiDisconnect()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}



void CPointingDlg::OnBnClickedMoveOnoff()
{
	if(!m_pParameter->m_ShapeShift)
	{
		this->m_xvShiftOnOff.Format(_T("true"));
		m_pParameter->m_ShapeShift = true;
	}else{
		this->m_xvShiftOnOff.Format(_T("false"));
		m_pParameter->m_ShapeShift = false;
	}
	
	UpdateData(FALSE);
}


void CPointingDlg::OnBnClickedCount()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	this->m_xvObjectCount++;
	m_pParameter->m_correct++;
	

	UpdateData(FALSE);
}

void CPointingDlg::UpdateObjectNum()
{
	m_xvObjectCount = m_pParameter->m_correct;
	UpdateData(FALSE);
}