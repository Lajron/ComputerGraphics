
// FlowersView.h : interface of the CFlowersView class
//

#pragma once


class CFlowersView : public CView
{
protected: // create from serialization only
	CFlowersView() noexcept;
	DECLARE_DYNCREATE(CFlowersView)

// Attributes
public:
	CFlowersDoc* GetDocument() const;
	double rotAngle1 = 0.0f;
	double rotAngle2 = 0.0f;

// Operations
public:
	double degreeToRad(double degreeAngle);
	void Translate(CDC* pDC, int dx, int dy, bool rightMultiply = false);
	void Rotate(CDC* pDC, double angle, bool rightMultiply = false);
	void Scale(CDC* pDC, double sx, double sy, bool rightMultiply = false);
	void Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply = false);
	void LoadIdentity(CDC* pDC) { pDC->ModifyWorldTransform(NULL, MWT_IDENTITY); }


	void DrawPetal(CDC* pDC, CRect rect, int nArcs, COLORREF clrFill, COLORREF clrLine);
	void DrawFlower(CDC* pDC, CRect rect, int nArcs, COLORREF clrFill, COLORREF clrLine, float dRot, CString str);
	void DrawFlowers(CDC* pDC, CRect* aRect, int* anArcs, COLORREF* aClrFill, COLORREF* aClrLine, float* aRot, CString* aStr, CPoint* aPts, int nFlower);
	void SaveDC(CDC* pDC, CRect rcDC, CRect rcBmp);

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
	virtual ~CFlowersView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FlowersView.cpp
inline CFlowersDoc* CFlowersView::GetDocument() const
   { return reinterpret_cast<CFlowersDoc*>(m_pDocument); }
#endif

