
#pragma once
#include "DImage.h"
//* INIT
//* Add to pch.h next 2 lines so we can use M_PI
//* #define _USE_MATH_DEFINES
//* #include <cmath>
//*
//* Add DImage.h in Header Files and DImage.cpp in Source Files
//* On Solution right click
//* 1. Clean Solution & 2. Rebuild Solution

class CDocumentationGDIView : public CView
{
protected: // create from serialization only
	CDocumentationGDIView() noexcept;
	DECLARE_DYNCREATE(CDocumentationGDIView)

// Attributes
public:
	CDocumentationGDIDoc* GetDocument() const;

// Operations : FROM TOP TO BOTTOM EXPLAINING EVERYTHING IN DETAIL
public:
	//* IMPORTANT UTILITY METHODS
	double degreeToRad(double degreeAngle);
	double radToDegree(double radAngle);

	//* WORLD TRANSFORMATION METHODS (RIGHTMULTIPLY = FALSE)
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Mirror(CDC* pDC, bool mX, bool mY, bool rightMultiply);
	void Skew(CDC* pDC, float sx, float sy, bool rightMultiply);
	void ResetWorldTransformation(CDC* pDC);

	//* GRID FOR CHECKING WORLD TRANSFORMATION (OPTIONAL)
	void DrawGrid(CDC* pDC, int gridSize = 500, int sqNum = 20);

	//* DRAWING A SHAPE 
	void DrawPuzzleShape(CDC* pDC, int size);
	void DrawScorpionsNeedle(CDC* pDC, int size);
	void DrawCybinWithText(CDC* pDC, int w, int d, COLORREF clr, CString str);
	void DrawWheelWithEllipses(CDC* pDC, int r, int d, int n, COLORREF clr);
	void DrawWheelWithClipRegion(CDC* pDC, double r1, double r2, double w);

	//* DIMAGE CLASS
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	
	//* MEMORY DC
	void SetupForNoFlickr(CDC* pDC);
	void ScaleWithoutStretchBlt(CDC* pSrcDC, int srcWidth, int srcHeight, int zoom);
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
	virtual ~CDocumentationGDIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	//* CLASS VIEW OVERRIDE BOTH
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in DocumentationGDIView.cpp
inline CDocumentationGDIDoc* CDocumentationGDIView::GetDocument() const
   { return reinterpret_cast<CDocumentationGDIDoc*>(m_pDocument); }
#endif

