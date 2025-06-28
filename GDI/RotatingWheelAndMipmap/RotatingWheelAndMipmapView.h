
// RotatingWheelAndMipmapView.h : interface of the CRotatingWheelAndMipmapView class
//

#pragma once


class CRotatingWheelAndMipmapView : public CView
{
protected: // create from serialization only
	CRotatingWheelAndMipmapView() noexcept;
	DECLARE_DYNCREATE(CRotatingWheelAndMipmapView)

// Attributes
public:
	CRotatingWheelAndMipmapDoc* GetDocument() const;
	double rotAngle1 = 0.0;
	double moveBy = 0.0;

// Operations
public:
	double degreeToRad(double degreeAngle);
	void Translate(CDC* pDC, float dx, float dy, bool rightMultiply = false);
	void Rotate(CDC* pDC, double angle, bool rightMultiply = false);
	void Scale(CDC* pDC, double sx, double sy, bool rightMultiply = false);
	void Mirror(CDC* pDC, bool mx = false, bool my = false, bool rightMultiply = false);
	void Skew(CDC* pDC, float sx, float sy, bool rightMultiply = false);
	void ResetWorldTransformation(CDC* pDC);

	void DrawWheel(CDC* pDC, double r1, double r2, double w);
	void DrawWP(CDC* pDC, double r1, double r2, double w, double L, double alpha, double d);

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
	virtual ~CRotatingWheelAndMipmapView();
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

#ifndef _DEBUG  // debug version in RotatingWheelAndMipmapView.cpp
inline CRotatingWheelAndMipmapDoc* CRotatingWheelAndMipmapView::GetDocument() const
   { return reinterpret_cast<CRotatingWheelAndMipmapDoc*>(m_pDocument); }
#endif

