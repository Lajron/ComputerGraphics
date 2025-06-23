
// ObservationWheelView.h : interface of the CObservationWheelView class
//

#pragma once


class CObservationWheelView : public CView
{
protected: // create from serialization only
	CObservationWheelView() noexcept;
	DECLARE_DYNCREATE(CObservationWheelView)

// Attributes
public:
	CObservationWheelDoc* GetDocument() const;

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
	virtual ~CObservationWheelView();
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
	void SaveDC(CDC* pDC, CRect rcDC, CRect rcBmp, CString strBmp);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in ObservationWheelView.cpp
inline CObservationWheelDoc* CObservationWheelView::GetDocument() const
   { return reinterpret_cast<CObservationWheelDoc*>(m_pDocument); }
#endif

