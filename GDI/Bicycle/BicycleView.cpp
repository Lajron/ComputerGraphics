
// BicycleView.cpp : implementation of the CBicycleView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Bicycle.h"
#endif

#include "BicycleDoc.h"
#include "BicycleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBicycleView

IMPLEMENT_DYNCREATE(CBicycleView, CView)

BEGIN_MESSAGE_MAP(CBicycleView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CBicycleView construction/destruction

CBicycleView::CBicycleView() noexcept
{
	// TODO: add construction code here

}

CBicycleView::~CBicycleView()
{
}

BOOL CBicycleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CBicycleView drawing
void CBicycleView::DrawCycle(CDC* pDC, int r, int n, int d)
{
	CPen outline(PS_SOLID, d, RGB(0, 0, 0));
	CBrush insideBrush(RGB(0, 0, 0));

	CPen* oldPen = pDC->SelectObject(&outline);
	CBrush* oldBrush = pDC->SelectObject(&insideBrush);

	//Inner circle
	pDC->Ellipse(-d, -d, d, d);
	
	//Outer circle
	pDC->SelectStockObject(HOLLOW_BRUSH);
	pDC->Ellipse(-r, -r, r, r);

	CPen bars(PS_SOLID, 2, RGB(0, 0, 0));
	pDC->SelectObject(&bars);
	double rad = degreeToRad(360 / n);
	for (int i = 0; i < n; i++) {
		float x = r * cos(rad * i);
		float y = r * sin(rad * i);

		pDC->MoveTo(0, 0);
		pDC->LineTo(x, y);
	}

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CBicycleView::DrawBicycle(CDC* pDC, int r, int n, int d, int alpha, int beta)
{
}

void CBicycleView::DrawTextForBicycle(CDC* pDC, int size, CString strText, COLORREF clrText)
{
}

void CBicycleView::SaveDC(CDC* pDC, CRect rcDC, CRect rcBmp, CString strBmp)
{
}

void CBicycleView::OnDraw(CDC* pDC)
{
	CBicycleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int oldGM = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM ogForm; pDC->GetWorldTransform(&ogForm);

	Translate(pDC, 250, 250);
	DrawCycle(pDC, 200, 20, 15);

	pDC->SetWorldTransform(&ogForm);
	pDC->SetGraphicsMode(oldGM);
}


// CBicycleView printing

BOOL CBicycleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBicycleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBicycleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CBicycleView diagnostics

#ifdef _DEBUG
void CBicycleView::AssertValid() const
{
	CView::AssertValid();
}

void CBicycleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBicycleDoc* CBicycleView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBicycleDoc)));
	return (CBicycleDoc*)m_pDocument;
}
#endif //_DEBUG


// CBicycleView message handlers

double CBicycleView::degreeToRad(double degreeAngle)
{
	return degreeAngle * (M_PI / 180);
}

void CBicycleView::Translate(CDC* pDC, float dx, float dy, bool rightMultiply)
{
	XFORM xForm = {
		1.0, 0.0,
		0.0, 1.0,
		dx, dy
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CBicycleView::Rotate(CDC* pDC, double angle, bool rightMultiply)
{
	double rad = degreeToRad(angle);
	XFORM xForm = {
		cos(rad), sin(rad),
		-sin(rad), cos(rad),
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CBicycleView::Scale(CDC* pDC, double sx, double sy, bool rightMultiply)
{
	XFORM xForm = {
		sx, 0.0,
		0.0, sy,
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CBicycleView::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	XFORM xForm = {
		(mx ? -1.0 : 1.0), 0.0,
		0.0, (my ? -1.0 : 1.0),
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CBicycleView::Skew(CDC* pDC, float sx, float sy, bool rightMultiply)
{
	XFORM xForm = {
		1.0, sy,
		sx, 1.0,
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CBicycleView::ResetWorldTransformation(CDC* pDC)
{
	pDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
}


