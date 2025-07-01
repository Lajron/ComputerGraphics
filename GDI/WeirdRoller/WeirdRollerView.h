
// WeirdRollerView.h : interface of the CWeirdRollerView class
//

#pragma once
#include "DImage.h"


class CWeirdRollerView : public CView
{
protected: // create from serialization only
	CWeirdRollerView() noexcept;
	DECLARE_DYNCREATE(CWeirdRollerView)

// Attributes
public:
	CWeirdRollerDoc* GetDocument() const;
	DImage* bkg;
	DImage* basket;
	DImage* arm;

	double rot1 = 0.0;
	double rot2 = 0.0;
	double rot3 = 0.0;
	int moveX = 0;

// Operations
public:
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);

	void DrawBackground(CDC* pDC);
	void DrawImageTransparent(CDC* pDC, DImage* pImage);
	void DrawArm(CDC* pDC);
	void DrawBasket(CDC* pDC, int r);
	void DrawBasketCouple(CDC* pDC, int l, int r, float angle);
	void DrawPlatform(CDC* pDC, int l, int r, double angle);
	void DrawCarousel(CDC* pDC, int h, int r, double offset, double alpha, double beta, double angle);

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
	virtual ~CWeirdRollerView();
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

#ifndef _DEBUG  // debug version in WeirdRollerView.cpp
inline CWeirdRollerDoc* CWeirdRollerView::GetDocument() const
   { return reinterpret_cast<CWeirdRollerDoc*>(m_pDocument); }
#endif

