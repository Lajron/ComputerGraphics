
// BicycleView.h : interface of the CBicycleView class
//

#pragma once


class CBicycleView : public CView
{
protected: // create from serialization only
	CBicycleView() noexcept;
	DECLARE_DYNCREATE(CBicycleView)

// Attributes
public:
	CBicycleDoc* GetDocument() const;
	double rotAngle = 0.0;
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

	void DrawCycle(CDC* pDC, int r, int n, int d);
	void DrawBicycle(CDC* pDC, int r, int n, int d, int alpha, int beta);
	void DrawTextForBicycle(CDC* pDC, int size, CString strText, COLORREF clrText);
	void SaveDC(CDC* pDC, CRect rcDC, CRect rcBmp, CString strBmp);

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
	virtual ~CBicycleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BicycleView.cpp
inline CBicycleDoc* CBicycleView::GetDocument() const
   { return reinterpret_cast<CBicycleDoc*>(m_pDocument); }
#endif

