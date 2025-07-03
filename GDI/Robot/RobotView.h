
// RobotView.h : interface of the CRobotView class
//

#pragma once
#include "DImage.h"


class CRobotView : public CView
{
protected: // create from serialization only
	CRobotView() noexcept;
	DECLARE_DYNCREATE(CRobotView)

// Attributes
public:
	CRobotDoc* GetDocument() const;
	DImage* glava;
	DImage* nKolenica;
	DImage* nLaktica;
	DImage* pKolenica;
	DImage* pLaktica;
	DImage* saka;
	DImage* stopalo;
	DImage* telo;
	DImage* pozadina;

	double rot1 = 0.0;
	double rot2 = 0.0;
	double rot3 = 0.0;
	double scale = 0.0;

// Operations
public:
	void DrawBackground(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawHalf(CDC* pDC);
	void DrawHead(CDC* pDC);
	void DrawRobot(CDC* pDC);

	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Mirror(CDC* pDC, bool mX, bool mY, bool rightMultiply);

	void DrawPart(CDC* pDC, DImage* pImg, int dX, int dY);

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CRobotView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in RobotView.cpp
inline CRobotDoc* CRobotView::GetDocument() const
   { return reinterpret_cast<CRobotDoc*>(m_pDocument); }
#endif

