
// SimplePuzzleView.cpp : implementation of the CSimplePuzzleView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SimplePuzzle.h"
#endif

#include "SimplePuzzleDoc.h"
#include "SimplePuzzleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSimplePuzzleView

IMPLEMENT_DYNCREATE(CSimplePuzzleView, CView)

BEGIN_MESSAGE_MAP(CSimplePuzzleView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CSimplePuzzleView construction/destruction

CSimplePuzzleView::CSimplePuzzleView() noexcept
{
	// TODO: add construction code here

}

CSimplePuzzleView::~CSimplePuzzleView()
{
}

BOOL CSimplePuzzleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSimplePuzzleView drawing
double rotAngle1 = 0.0;
double rotAngle2 = 0.0;

static double degreeToRad(double degreeAngle) {
	return degreeAngle * (M_PI / 180);
}
static double radToDegree(double radAngle) {
	return radAngle * (180 / M_PI);
}
static void drawGrid(CDC* pDC, int gridSize = 500, int sqNum = 20) {
	CPen gridPen(PS_SOLID, 1, RGB(0, 0, 0));
	int a = gridSize / sqNum;
	CPen* oldPen = pDC->SelectObject(&gridPen);

	for (int i = 0; i <= gridSize; i += a) {
		pDC->MoveTo(i, 0);
		pDC->LineTo(i, gridSize);

		pDC->MoveTo(0, i);
		pDC->LineTo(gridSize, i);
	}
	pDC->SelectObject(oldPen);
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
static void ResetWorldTransformation(CDC* pDC) {
	pDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
}

#pragma 2022_Januar
static void Part1(CDC* pDC, int size) {
	CBrush yellowBrush(RGB(255, 255, 0));
	CPen newPen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(&yellowBrush);
	CPen* oldPen = pDC->SelectObject(&newPen);

	CPoint points[] = { {size, -size}, {size, size}, {-size, size}, {-size, -size} };
	pDC->BeginPath();
	{
		int oldArcDirection = pDC->SetArcDirection(AD_CLOCKWISE);
		pDC->MoveTo(points[0]);
		pDC->PolylineTo(points, 4);
		pDC->ArcTo(CRect(-size, -size * 2, size, 0), points[3], points[0]);
		pDC->SetArcDirection(oldArcDirection);
	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
}
static void Part2(CDC* pDC, int size) {
	CBrush greenBrush(RGB(0, 255, 0));
	CPen newPen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(&greenBrush);
	CPen* oldPen = pDC->SelectObject(&newPen);

	CPoint points[] = { {size, -size}, {-size, -size}, {-size, size}, {size, size} };
	pDC->BeginPath();
	{
		pDC->MoveTo(points[0]);
		pDC->PolylineTo(points, 4);
		int oldArcDirection = pDC->SetArcDirection(AD_CLOCKWISE);
		pDC->ArcTo(CRect(0, -size, 2 * size, size), points[3], points[0]);
		pDC->SetArcDirection(oldArcDirection);
	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
}
static void Part3(CDC* pDC, int size) {
	CBrush redBrush(RGB(255, 0, 0));
	CPen newPen(PS_SOLID, 1, RGB(0, 0, 255));

	CBrush* oldBrush = pDC->SelectObject(&redBrush);
	CPen* oldPen = pDC->SelectObject(&newPen);

	CPoint points[] = { {size,size}, {size, -size}, {-size, -size}, {-size, size}, {0,0}, {size,size} };
	pDC->BeginPath();
	{
		pDC->MoveTo(points[0]);
		pDC->PolylineTo(points, 6);
	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
}
static void Part4(CDC* pDC, int size) {
	CBrush blueBrush(RGB(0, 0, 255));
	CPen newPen(PS_SOLID, 1, RGB(0, 0, 255));
	CBrush* oldBrush = pDC->SelectObject(&blueBrush);
	CPen* oldPen = pDC->SelectObject(&newPen);

	CPoint points[] = { {size,size}, {size, -size}, {-size,-size}, {-size * 2, 0},{-size, size} };
	pDC->Polygon(points, 5);

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
}
static void DrawPuzzle(CDC* pDC, int size, float angle, int x, int y) {
	XFORM ogForm;
	pDC->GetWorldTransform(&ogForm);

	XFORM prevForm;

	Translate(pDC, x, y);
	Rotate(pDC, angle);
	pDC->GetWorldTransform(&prevForm);
	Part1(pDC, size);

	Translate(pDC, 0, -size * 4);
	Part1(pDC, size);

	Translate(pDC, 0, 2 * size);
	Rotate(pDC, 90);
	Part2(pDC, size);

	pDC->SetWorldTransform(&prevForm);

	Translate(pDC, -size * 2, 0);
	Rotate(pDC, 90);
	Part3(pDC, size);

	pDC->SetWorldTransform(&prevForm);
	Translate(pDC, -size * 4, 0);
	Rotate(pDC, 180);
	Part4(pDC, size);

	pDC->SetWorldTransform(&ogForm);
}
void CSimplePuzzleView::OnDraw(CDC* pDC)
{
	CSimplePuzzleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int oldGM = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);
	drawGrid(pDC);
	DrawPuzzle(pDC, 50, rotAngle1, 500, 500);
	pDC->SetWorldTransform(&oldForm);
	pDC->SetGraphicsMode(oldGM);
}


// CSimplePuzzleView printing

BOOL CSimplePuzzleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSimplePuzzleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSimplePuzzleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CSimplePuzzleView diagnostics

#ifdef _DEBUG
void CSimplePuzzleView::AssertValid() const
{
	CView::AssertValid();
}

void CSimplePuzzleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSimplePuzzleDoc* CSimplePuzzleView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimplePuzzleDoc)));
	return (CSimplePuzzleDoc*)m_pDocument;
}
#endif //_DEBUG


// CSimplePuzzleView message handlers

void CSimplePuzzleView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
	case 'D': {
		rotAngle1 += 5;
		break;
	}
	case 'A': {
		rotAngle1 -= 5;
		break;
	}
	case 'E': {
		rotAngle2 += 5;
		break;
	}
	case 'Q': {
		rotAngle2 -= 5;
		break;
	}
	default: break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	Invalidate();
}
