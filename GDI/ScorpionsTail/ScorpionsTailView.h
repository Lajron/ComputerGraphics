
// ScorpionsTailView.h : interface of the CScorpionsTailView class
//

#pragma once


class CScorpionsTailView : public CView
{
protected: // create from serialization only
	CScorpionsTailView() noexcept;
	DECLARE_DYNCREATE(CScorpionsTailView)

// Attributes
public:
	CScorpionsTailDoc* GetDocument() const;

// Operations
public:

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
	virtual ~CScorpionsTailView();
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

#ifndef _DEBUG  // debug version in ScorpionsTailView.cpp
inline CScorpionsTailDoc* CScorpionsTailView::GetDocument() const
   { return reinterpret_cast<CScorpionsTailDoc*>(m_pDocument); }
#endif

