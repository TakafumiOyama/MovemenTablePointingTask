// Window3.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "Pointing.h"
#include "Window3.h"


// CWindow3

IMPLEMENT_DYNAMIC(CWindow3, CWnd)

CWindow3*		pMyWnd2		= NULL;

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
////void CWindow3::ShowError(int e){
//// //   MessageBox(ErrorStrings[e]);
////}


CWindow3::CWindow3(CWnd* parent) : m_GLRC(0),m_pDC(0),RotDeg(0),WindowNum(0)
{

	LPCTSTR		lpszClassName;
	CRect		rect( 0, 0, 640, 480);
	HCURSOR		hCursor		= 0;
	DWORD		dwStyle, dwExStyle;

	lpszClassName = AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW, hCursor);
	dwExStyle = WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE;
	dwStyle = WS_POPUP/* | WS_CAPTION*/ | WS_VISIBLE/* | WS_SYSMENU*/ | WS_BORDER /*| WS_THICKFRAME*/;

	m_pGLBase = new CGLBaseSub1();
	m_pParameter = new CParameter();
	m_pJudge = new CJudgePosition();
	//char str[100] = {0};
	//sprintf(str, "window%d",WindowNum);
	CreateEx( dwExStyle, lpszClassName, _T("SubWindow1"), dwStyle, rect, parent, NULL, NULL);

	pMyWnd2 = this;

	RegisterTouchWindow();
}

CWindow3::~CWindow3()
{

	pMyWnd = NULL;

}


BEGIN_MESSAGE_MAP(CWindow3, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CWindow3 ���b�Z�[�W �n���h���[




void CWindow3::OnPaint()
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
	glFinish();
	SwapBuffers(m_pDC->m_hDC) ;    // Double buffer

}


void CWindow3::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
		CWnd::OnNcDestroy();

	delete this;

}



BOOL CWindow3::SetDCPixelFormat(HDC hdc)
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



BOOL CWindow3::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

    // TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p��...

    //return CView::OnEraseBkgnd(pDC);
    return TRUE;

}

BOOL CWindow3::InitializeOpenGL()
{
	return true;

}



void CWindow3::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	
	//COpenGLDoc* pDoc = GetDocument();
	m_GlBase.ViewSetting(cx,cy);
}


int CWindow3::OnCreate(LPCREATESTRUCT lpCreateStruct)
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




void CWindow3::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	switch(nChar)
	{
	case VK_RETURN:
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
		break;
	case VK_ESCAPE:
		SendMessage(WM_SYSCOMMAND, SC_RESTORE);
		break;
	default:
		break;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CWindow3::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CWnd::OnLButtonDown(nFlags, point);
}

ULONG CWindow3::GetGestureStatus(CPoint ptTouch)
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


//BOOL CWindow3::OnTouchInputs(UINT nInputsCount, PTOUCHINPUT pInputs)
//{
//	TRACE("touch count %d\n",nInputsCount);
//	if(nInputsCount == 1)
//		TRACE("");
//	return 0;
//}


BOOL CWindow3::OnTouchInput(CPoint pt, int nInputNumber, int nInputsCount, PTOUCHINPUT pInput)
{     
	if ((pInput->dwFlags & TOUCHEVENTF_DOWN) == TOUCHEVENTF_DOWN) // Touch Down event
	{
		TRACE("�^�b�`����܂���\n");
		TRACE("(%d, %d)\n",pt.x, pt.y);
		RegistTouchPos(pt);
		m_pPoint = pt;

		m_pJudge->JudgeDown(pt);
		m_pParameter->RecordInformation();
		OnPaint();
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
		OnPaint();

		return OnTouchInputMove(pt, pInput);
	}
	else if ((pInput->dwFlags & TOUCHEVENTF_UP) == TOUCHEVENTF_UP) // Touch Up event
	{
		m_pJudge->JudgeUp(pt);
		OnPaint();
		return OnTouchInputUp(pt, pInput);
	}
    return FALSE;
}


BOOL CWindow3::OnTouchInputDown(CPoint pt, PTOUCHINPUT pInput)
{
    // Create new stroke and add point to it.
	//COLORREF strokeColor = GetTouchColor((pInput->dwFlags & TOUCHEVENTF_PRIMARY) != 0);

	//CStroke* pStrkNew = new CStroke(pInput->dwID, strokeColor);
	//pStrkNew->Add(pt);

    // Add new stroke to the collection of strokes in drawing.
    //m_StrkColDrawing.Add(pStrkNew);	
	
	return TRUE;
}

BOOL CWindow3::OnTouchInputMove(CPoint pt, PTOUCHINPUT pInput)
{
    // Find the stroke in the collection of the strokes in drawing.
 //   int strokeIndex = m_StrkColDrawing.FindStrokeById(pInput->dwID);

	//if (strokeIndex >= 0)
	//{
	//	CStroke* pStrk =  m_StrkColDrawing[strokeIndex];

 //       // Add contact point to the stroke
 //       pStrk->Add(pt);

 //       // Draw the last stroke
 //   	pStrk->Draw(GetDC());
	//}

	return TRUE;
}

BOOL CWindow3::OnTouchInputUp(CPoint pt, PTOUCHINPUT pInput)
{
    // Find the stroke in the collection of the strokes in drawing.
 //   int strokeIndex = m_StrkColDrawing.FindStrokeById(pInput->dwID);

	//if (strokeIndex >= 0)
	//{
	//	CStroke* pStrkCopy = m_StrkColDrawing[strokeIndex];

 //   	// Remove this stroke from the collection of strokes in drawing.
	//	m_StrkColDrawing.RemoveAt(strokeIndex);

	//	// Add this stroke to the collection of finished strokes.
 //   	m_StrkColFinished.Add(pStrkCopy);
	//}

	return TRUE;
}
