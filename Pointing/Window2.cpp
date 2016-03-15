// Window2.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "Pointing.h"
#include "Window2.h"
#include <Windows.h>


// CWindow2

IMPLEMENT_DYNAMIC(CWindow2, CWnd)

CWindow2*		pMyWnd		= NULL;

//const char* ErrorStrings[] = {
//    {"No Error"},                   // 0
//    {"Unable to get a CD"},         // 1
//    {"ChoosePixelFormat failed"},   // 2
//    {"SelectPixelFormat failed"},   // 3
//    {"wglCreateContext failed"},    // 4
//    {"wglMakeCurrect failed"},      // 5
//    {"wglDeleteCotext failed"},     // 6
//    {"SwapBuffer failed"},          // 7
//};
//
////void CWindow2::ShowError(int e){
//// //   MessageBox(ErrorStrings[e]);
////}


CWindow2::CWindow2(CWnd* parent) : m_GLRC(0),m_pDC(0),RotDeg(0),WindowNum(0)
{

	LPCTSTR		lpszClassName;
	CRect		rect( 0, 0, 640, 480);
	HCURSOR		hCursor		= 0;
	DWORD		dwStyle, dwExStyle;

	lpszClassName = AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW, hCursor);
	dwExStyle = WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE;
	dwStyle = WS_POPUP/* | WS_CAPTION*/ | WS_VISIBLE/* | WS_SYSMENU*/ | WS_BORDER /*| WS_THICKFRAME*/;

	m_pGLBase = new CGLBase();
	m_pParameter = new CParameter();
	m_pJudge = new CJudgePosition();
	//char str[100] = {0};
	//sprintf(str, "window%d",WindowNum);
	CreateEx( dwExStyle, lpszClassName, _T("Window"), dwStyle, rect, parent, NULL, NULL);

	pMyWnd = this;

	RegisterTouchWindow();
}

CWindow2::~CWindow2()
{

	pMyWnd = NULL;

}


BEGIN_MESSAGE_MAP(CWindow2, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CWindow2 ���b�Z�[�W �n���h���[




void CWindow2::OnPaint()
{
	 //int w = CParameter::WindowSizeX;
	 //int h = CParameter::WindowSizeY;
	 //glViewport(0, 0, w, h);
	 //glLoadIdentity();
	 //glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);

	CPaintDC dc(this); // device context for painting
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	// �`�惁�b�Z�[�W�� CWnd::OnPaint() ���Ăяo���Ȃ��ł��������B
		CRect		cr;
	GetClientRect( &cr);
	dc.FillSolidRect( cr, RGB( 200, 200, 200));
	
//	dc.DrawText( "This is simple window", cr, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	// �`��p���b�Z�[�W�Ƃ��� CWnd::OnPaint() ���Ăяo���Ă͂����܂���

	//m_GlBase.TestDraw();
	//m_GlBase.DrawRotate(RotDeg);
	//m_GlBase.DrawMove(ImagePos.x, ImagePos.y,0);
	
	//m_pGLBase->Draw(RotDeg, m_pPoint.x, m_pPoint.y, m_pGLBase->m_ChooseWall);
	m_pGLBase->Draw(RotDeg, ObjectPos[m_correct].x, ObjectPos[m_correct].y, 0);
	//m_pGLBase->TestDraw();
	glFinish();
	SwapBuffers(m_pDC->m_hDC) ;    // Double buffer

}


void CWindow2::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
		CWnd::OnNcDestroy();

	delete this;

}



BOOL CWindow2::SetDCPixelFormat(HDC hdc)
{
    PIXELFORMATDESCRIPTOR pfd = 
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_STEREO,
        PFD_TYPE_RGBA,//PFD_STEREO�̃p�����[�^�͗��̎����邽�߂ɂ߂��Ⴍ����d�v
        24,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0,  0, 0, 0,
        16,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    int pixelformat;
    if( 0 == (pixelformat = ::ChoosePixelFormat( m_pDC->GetSafeHdc(), &pfd))){
       // ShowError(2);
        return FALSE;
    }
    if( FALSE == ::SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd)){
        //ShowError(3);
        return FALSE;
    }
    return TRUE;
}



BOOL CWindow2::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

    // TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p��...

    //return CView::OnEraseBkgnd(pDC);
    return TRUE;

}

BOOL CWindow2::InitializeOpenGL()
{
	return true;

}



void CWindow2::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	
	//COpenGLDoc* pDoc = GetDocument();
	m_GlBase.ViewSetting(cx,cy);
}


int CWindow2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �����ɓ���ȍ쐬�R�[�h��ǉ����Ă��������B

	// TODO:  �����ɓ���ȍ쐬�R�[�h��ǉ����Ă��������B
	m_pDC = new CClientDC(this) ;             // DC�̐���
	SetDCPixelFormat(m_pDC->m_hDC) ;          // OpenGL�p��Pixel Format���w��
	m_GLRC = wglCreateContext (m_pDC->m_hDC); // Rendering context�̐���
	wglMakeCurrent (m_pDC->m_hDC, m_GLRC);    // ���݂�context�ݒ�

	
	//ASSERT_VALID(pDoc);
	

	m_pGLBase->Init();
	
	//MakeGLObject();	//3D�I�u�W�F�N�g�𐶐�����


	return 0;
}




void CWindow2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	switch(nChar)
	{
	case VK_RETURN:
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
		break;
	case VK_ESCAPE:
		SendMessage(WM_SYSCOMMAND, SC_RESTORE);
		this->DestroyWindow();
		break;
	//���L�[
	case VK_LEFT:
		ImagePos.x = 0.0;
		ImagePos.y = 0.0;
		RotDeg = - 2.0;
		OnPaint();
		break;
	//�E�L�[
	case VK_RIGHT:
		m_correct++;
		m_CreateGoalFlag = false;
		IsStandby = false;
		IsFirstTouch = true;
		IsTasking = false;
		GhostObjectPos[m_correct] = ObjectPos[m_correct];
		m_pParameter->m_TaskStartTime = ::timeGetTime() - m_pParameter->m_ProgramStart;
		TRACE("-------%d------------\n",m_correct);
		OnPaint();
		break;
	//��L�[
	case VK_UP:
		LetsGo = true;
		break;
	//���L�[
	case VK_DOWN:
		break;
	//�L�[�{�[�h�P
	case 49:
		TRACE("1\n");
		m_pGLBase->m_ChooseWall = 1;
		break;
	//�L�[�{�[�h�Q
	case 50:
		TRACE("2\n");
		m_pGLBase->m_ChooseWall = 2;
		break;
	//�L�[�{�[�h�R
	case 51:
		TRACE("3\n");
		m_pGLBase->m_ChooseWall = 3;
		break;
	//�L�[�{�[�hW
	case 87:
		//TRACE("W\n");
		ImagePos.x = 0.0;
		ImagePos.y = 0.5;
		RotDeg = 0.0;
		OnPaint();
		break;
	//�L�[�{�[�hS
	case 83:
		//TRACE("S\n");
		ImagePos.x = 0.0;
		ImagePos.y = - 0.5;
		RotDeg = 0.0;
		OnPaint();
		break;
	//�L�[�{�[�hA
	case 65:
		//TRACE("A\n");
		ImagePos.x = - 0.5;
		ImagePos.y = 0.0;
		RotDeg = 0.0;
		OnPaint();
		break;
	//�L�[�{�[�hD
	case 68:
		//TRACE("D\n");
		ImagePos.x = 0.5;
		ImagePos.y = 0.0;
		RotDeg = 0.0;
		OnPaint();
		break;

	default:
		break;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CWindow2::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CWnd::OnLButtonDown(nFlags, point);
}

ULONG CWindow2::GetGestureStatus(CPoint ptTouch)
{
	////�l�Ɋւ��Ă̏������s��
	////�p�����[�^�Ƀ^�b�`�������W��ۑ�
	////currentTouchPos = ptTouch;
	//RegistTouchPos(ptTouch);
	//m_pPoint = ptTouch;

	////�^�b�`�����ꏊ�̔�����s��
	//m_pJudge->Judge(ptTouch);

	////�擾�������W�l�C�������s��������ۑ�����
	//m_pParameter->RecordInformation();
	//
	////�l�̌��ʂɉ����ĕ`����s��
	//OnPaint();
	return 0;
}


//BOOL CWindow2::OnTouchInputs(UINT nInputsCount, PTOUCHINPUT pInputs)
//{
//	TRACE("touch count %d\n",nInputsCount);
//	if(nInputsCount == 1)
//		TRACE("");
//	return 0;
//}


BOOL CWindow2::OnTouchInput(CPoint pt, int nInputNumber, int nInputsCount, PTOUCHINPUT pInput)
{    
	if(nInputsCount == 1)
	{
		if ((pInput->dwFlags & TOUCHEVENTF_DOWN) == TOUCHEVENTF_DOWN) // Touch Down event
		{
			//TRACE("�^�b�`����܂���\n");
			//TRACE("(%d, %d)\n",pt.x, pt.y);
			RegistTouchPos(pt);
			m_pPoint = pt;

			//if(2580 < pt.x && pt.x < 3180 && 440 < pt.y && pt.y < 640)
			//{
			//	m_pParameter->IsStandby = false;
			//	m_pParameter->IsFirstTouch = true;
			//	m_pParameter->m_TaskStartTime = ::timeGetTime() - m_pParameter->m_ProgramStart;
			//}

			//m_pParameter->m_TouchTime = ::timeGetTime() - m_pParameter->m_ProgramStart;
			m_pJudge->JudgeDown(pt);
			//m_pParameter->RecordInformation();
//			OnPaint();
			return OnTouchInputDown(pt, pInput);
		}
		else if ((pInput->dwFlags & TOUCHEVENTF_MOVE) == TOUCHEVENTF_MOVE) // Touch Move event
		{
			if(IsTasking == true)
			{
				RegistTouchPos(pt);
				currentTouchPos = pt;
				m_pPoint = pt;
			}
//			OnPaint();
			return OnTouchInputMove(pt, pInput);
		}
		else if ((pInput->dwFlags & TOUCHEVENTF_UP) == TOUCHEVENTF_UP) // Touch Up event
		{
			m_pJudge->JudgeUp(pt);
//			OnPaint();
			return OnTouchInputUp(pt, pInput);
		}
	}
    return FALSE;
}


BOOL CWindow2::OnTouchInputDown(CPoint pt, PTOUCHINPUT pInput)
{

	
	return TRUE;
}

BOOL CWindow2::OnTouchInputMove(CPoint pt, PTOUCHINPUT pInput)
{

	return TRUE;
}

BOOL CWindow2::OnTouchInputUp(CPoint pt, PTOUCHINPUT pInput)
{

	return TRUE;
}
