
// ShurikenView.h : interface of the CShurikenView class
//
#include "DImage.h"
#pragma once


class CShurikenView : public CView
{
protected: // create from serialization only
	CShurikenView() noexcept;
	DECLARE_DYNCREATE(CShurikenView)

// Attributes
public:
	CShurikenDoc* GetDocument() const;

// Operations
public:
	void DrawBlade(CDC* pDC, int size);
	void DrawStar(CDC* pDC, int size);
	void DrawScene(CDC* pDC);

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
	virtual ~CShurikenView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in ShurikenView.cpp
inline CShurikenDoc* CShurikenView::GetDocument() const
   { return reinterpret_cast<CShurikenDoc*>(m_pDocument); }
#endif

