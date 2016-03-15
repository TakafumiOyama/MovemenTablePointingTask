#pragma once
#include "GLBase.h"
#include "Parameter.h"
#include "Main.h"
#include "JudgePosition.h"

// CWindow2

class CWindow2 : public CWnd, public CParameter
{
	DECLARE_DYNAMIC(CWindow2)

public:
	CWindow2(CWnd* parent);
	virtual ~CWindow2();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnDestroy();

public:
    BOOL RenderScene( void );
	LRESULT OnTouch( WPARAM wParam, LPARAM lParam);
private:
    BOOL SetDCPixelFormat(HDC hd);
    BOOL InitializeOpenGL( void );
   // void ShowError(int e);

    CDC* m_pDC;

	HGLRC m_GLRC;


	CGLBase m_GlBase;
	CGLBase * m_pGLBase;
	CParameter * m_pParameter;
	CJudgePosition * m_pJudge;

	double RotDeg;
	CVector ImagePos;
	CPoint m_pPoint;
	int WindowNum;

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

protected:
	virtual ULONG GetGestureStatus(CPoint ptTouch);
    virtual BOOL OnTouchInput(CPoint pt, int nInputNumber, int nInputsCount, PTOUCHINPUT pInput);
//	virtual BOOL OnTouchInputs(UINT nInputsCount, PTOUCHINPUT pInputs);
	
    // Handlers for different touch input events
    BOOL OnTouchInputDown(CPoint pt, PTOUCHINPUT pInput);
    BOOL OnTouchInputMove(CPoint pt, PTOUCHINPUT pInput);
    BOOL OnTouchInputUp(CPoint pt, PTOUCHINPUT pInput);

};

extern CWindow2* pMyWnd;


