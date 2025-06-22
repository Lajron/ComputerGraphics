
// SimplePuzzleView.h : interface of the CSimplePuzzleView class
//

#pragma once


class CSimplePuzzleView : public CView
{
protected: // create from serialization only
	CSimplePuzzleView() noexcept;
	DECLARE_DYNCREATE(CSimplePuzzleView)

// Attributes
public:
	CSimplePuzzleDoc* GetDocument() const;

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
	virtual ~CSimplePuzzleView();
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
};

#ifndef _DEBUG  // debug version in SimplePuzzleView.cpp
inline CSimplePuzzleDoc* CSimplePuzzleView::GetDocument() const
   { return reinterpret_cast<CSimplePuzzleDoc*>(m_pDocument); }
#endif

