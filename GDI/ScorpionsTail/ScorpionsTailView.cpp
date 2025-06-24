
// ScorpionsTailView.cpp : implementation of the CScorpionsTailView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ScorpionsTail.h"
#endif

#include "ScorpionsTailDoc.h"
#include "ScorpionsTailView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScorpionsTailView

IMPLEMENT_DYNCREATE(CScorpionsTailView, CView)

BEGIN_MESSAGE_MAP(CScorpionsTailView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CScorpionsTailView construction/destruction

CScorpionsTailView::CScorpionsTailView() noexcept
{
	// TODO: add construction code here

}

CScorpionsTailView::~CScorpionsTailView()
{
}

BOOL CScorpionsTailView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CScorpionsTailView drawing
float rotAngle1 = 0.0;

static double degreeToRad(double degreeAngle) {
	return degreeAngle * (M_PI / 180);
}
static void Translate(CDC* pDC, float dx, float dy, bool rightMultiply = false) {
	XFORM xForm = {
		1.0, 0.0,
		0.0, 1.0,
		dx, dy
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
static void Rotate(CDC* pDC, double angle, bool rightMultiply = false) {
	double rad = degreeToRad(angle);
	XFORM xForm = {
		cos(rad), sin(rad),
		-sin(rad), cos(rad),
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
static void Scale(CDC* pDC, double sx, double sy, bool rightMultiply = false) {
	XFORM xForm = {
		sx, 0.0,
		0.0, sy,
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
static void Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply = false) {
	XFORM xForm = {
		(mx ? -1.0 : 1.0), 0.0,
		0.0, (my ? -1.0 : 1.0),
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
static void LoadIdentity(CDC* pDC) {
	pDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
}

static void DrawTop(CDC* pDC, int size) {
	CPen blackPen(PS_SOLID, 2, RGB(0, 0, 0));
	CBrush orangeBrush(RGB(255, 128, 0));
	CPen* oldPen = pDC->SelectObject(&blackPen);
	CBrush* oldBrush = pDC->SelectObject(&orangeBrush);

	int sizeHalf = size / 2;
	CPoint points[] = { {sizeHalf, 0}, {0, sizeHalf}, {0, size} };

	pDC->BeginPath();
	{
		int ogAD = pDC->SetArcDirection(AD_COUNTERCLOCKWISE);

		pDC->MoveTo(points[0]);
		pDC->ArcTo(CRect(-sizeHalf, -sizeHalf, sizeHalf, sizeHalf), points[0], points[1]);

		pDC->SetArcDirection(AD_CLOCKWISE);
		pDC->ArcTo(CRect(-sizeHalf/2, sizeHalf, sizeHalf/2, size), points[1], points[2]);
		pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
		pDC->ArcTo(CRect(-sizeHalf, -size, sizeHalf, size), points[2], points[0]);

		pDC->SetArcDirection(ogAD);
	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
}
static void DrawTop2(CDC* pDC, int size) {
	XFORM prevForm; pDC->GetWorldTransform(&prevForm);

	Rotate(pDC, 90);
	Mirror(pDC, false, true);
	DrawTop(pDC, size);

	pDC->SetWorldTransform(&prevForm);

}
static void DrawTail(CDC* pDC, int size, int count, double alpha) {
	CPen blackPen(PS_SOLID, 2, RGB(0, 0, 0));
	CBrush orangeBrush(RGB(255, 128, 0));
	CPen* oldPen = pDC->SelectObject(&blackPen);
	CBrush* oldBrush = pDC->SelectObject(&orangeBrush);

	XFORM prevForm; pDC->GetWorldTransform(&prevForm);

	Translate(pDC, -size/2, 0);
	for (int i = 0; i < count; i++) {
		Rotate(pDC, alpha);
		Translate(pDC, size, 0);
		pDC->Ellipse(-size, -size/2, size, size/2);
		Translate(pDC, size, 0);
	}
	Rotate(pDC, alpha);
	Translate(pDC, size/2, 0);
	DrawTop2(pDC, size);

	pDC->SetWorldTransform(&prevForm);

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
}

void CScorpionsTailView::OnDraw(CDC* pDC)
{
	CScorpionsTailDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect window;
	GetClientRect(&window);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);

	CBitmap* memBitmap = new CBitmap();
	memBitmap->CreateCompatibleBitmap(pDC, window.Width(), window.Height());

	CBitmap* oldMemBitmap = memDC->SelectObject(memBitmap);
	memDC->FillSolidRect(&window, RGB(255, 255, 255));

	int oldGM = memDC->SetGraphicsMode(GM_ADVANCED);
	XFORM ogForm; memDC->GetWorldTransform(&ogForm);

	Translate(memDC, 250, 250);
	DrawTail(memDC, 50, 5, rotAngle1);

	memDC->SetWorldTransform(&ogForm);
	memDC->SetGraphicsMode(oldGM);

	pDC->BitBlt(
		window.left, window.top, window.Width(), window.Height(),
		memDC, window.left, window.top,
		SRCCOPY
	);

	memDC->SelectObject(oldMemBitmap);
	delete memBitmap;
	delete memDC;
}


// CScorpionsTailView printing

BOOL CScorpionsTailView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CScorpionsTailView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CScorpionsTailView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CScorpionsTailView diagnostics

#ifdef _DEBUG
void CScorpionsTailView::AssertValid() const
{
	CView::AssertValid();
}

void CScorpionsTailView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CScorpionsTailDoc* CScorpionsTailView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScorpionsTailDoc)));
	return (CScorpionsTailDoc*)m_pDocument;
}
#endif //_DEBUG


// CScorpionsTailView message handlers

void CScorpionsTailView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 'A': {
		rotAngle1 -= 5;
		break;
	}
	case 'D': {
		rotAngle1 += 5;
		break;
	}
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	Invalidate();
}

BOOL CScorpionsTailView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}
