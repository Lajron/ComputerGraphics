
// RotatingWheelAndMipmapView.cpp : implementation of the CRotatingWheelAndMipmapView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RotatingWheelAndMipmap.h"
#endif

#include "RotatingWheelAndMipmapDoc.h"
#include "RotatingWheelAndMipmapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CMipmap.h"


// CRotatingWheelAndMipmapView

IMPLEMENT_DYNCREATE(CRotatingWheelAndMipmapView, CView)

BEGIN_MESSAGE_MAP(CRotatingWheelAndMipmapView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CRotatingWheelAndMipmapView construction/destruction

CRotatingWheelAndMipmapView::CRotatingWheelAndMipmapView() noexcept
{
	// TODO: add construction code here

}

CRotatingWheelAndMipmapView::~CRotatingWheelAndMipmapView()
{
}

BOOL CRotatingWheelAndMipmapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRotatingWheelAndMipmapView drawing
static void drawGrid(CDC* pDC, int gridSize = 500, int sqNumber = 20) {
	CPen* oldPen = pDC->SelectObject(new CPen(PS_SOLID, 1, RGB(0, 0, 0)));

	float sqSize = gridSize / sqNumber;
	for (int i = 0; i <= sqNumber; i++) {
		pDC->MoveTo(i * sqSize, 0);
		pDC->LineTo(i * sqSize, gridSize);

		pDC->MoveTo(0, i * sqSize);
		pDC->LineTo(gridSize, i * sqSize);
	}

	delete(pDC->SelectObject(oldPen));
}

void CRotatingWheelAndMipmapView::DrawWheel(CDC* pDC, double r1, double r2, double w)
{
	CPen outerLine(PS_SOLID, 4, RGB(50, 50, 50));
	CBrush fillBrush(RGB(150, 150, 150));

	CPen* oldPen = pDC->SelectObject(&outerLine);
	CBrush* oldBrush = pDC->SelectObject(&fillBrush);

	//Rectangles inside
	XFORM prevForm; pDC->GetWorldTransform(&prevForm);
	//Rotate(pDC, 45);
	for (int i = 0; i < 3; i++) {
		Rotate(pDC, -60 * i);
		pDC->Rectangle(-w / 2, -r2 / 2, w / 2, r2 / 2);
		pDC->SetWorldTransform(&prevForm);
	}

	//Ring
	CRect outerRect(-r1 / 2, -r1 / 2, r1 / 2, r1 / 2);
	CRect innerRect(-r2 / 2, -r2 / 2, r2 / 2, r2 / 2);

	CRgn outer, inner, ring;
	outer.CreateEllipticRgnIndirect(&outerRect);
	inner.CreateEllipticRgn(-r2 / 2, -r2 / 2, r2 / 2, r2 / 2); //Actual difference between these 2
	ring.CreateRectRgn(0, 0, 0, 0); // Only this RectRgn can be turned into complex region shapes
	ring.CombineRgn(&outer, &inner, RGN_DIFF); // Getting the ring region

	pDC->FillRgn(&ring, &fillBrush);

	pDC->SelectStockObject(HOLLOW_BRUSH);

	pDC->Ellipse(outerRect);
	pDC->Ellipse(innerRect);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CRotatingWheelAndMipmapView::DrawWP(CDC* pDC, double r1, double r2, double w, double L, double alpha, double d)
{
	double rad = degreeToRad(alpha);
	int S = L * tan(rad);

	CPen outline(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush fillBrush(RGB(190, 190, 190));

	CPen* oldPen = pDC->SelectObject(&outline);
	CBrush* oldBrush = pDC->SelectObject(&fillBrush);

	XFORM prevForm; pDC->GetWorldTransform(&prevForm);
	Rotate(pDC, alpha);
	Translate(pDC, moveBy, 0);
	
	Rotate(pDC, -alpha + moveBy);
	DrawWheel(pDC, r1, r2, w);

	pDC->SetWorldTransform(&prevForm);
	Translate(pDC, 0, r1 / 2);
	CPoint triPoints[] = { {0,0}, {0, S}, {(int)L, S} };
	pDC->Polygon(triPoints, 3);
	pDC->SetWorldTransform(&prevForm);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CRotatingWheelAndMipmapView::OnDraw(CDC* pDC)
{
	CRotatingWheelAndMipmapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	/*CRect window;
	GetClientRect(&window);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);

	CBitmap* memBitmap = new CBitmap();
	memBitmap->CreateCompatibleBitmap(pDC, window.Width(), window.Height());

	CBitmap* oldMemBitmap = memDC->SelectObject(memBitmap);
	memDC->FillSolidRect(window, RGB(240, 240, 240));

	int oldGM = memDC->SetGraphicsMode(GM_ADVANCED);
	XFORM ogForm; memDC->GetWorldTransform(&ogForm);
	drawGrid(memDC);
	Translate(memDC, 250, 250);
	DrawWP(memDC, 200, 150, 20, 500, 20, moveBy);

	memDC->SetWorldTransform(&ogForm);
	memDC->SetGraphicsMode(oldGM);

	pDC->BitBlt(
		window.left, window.top, window.Width(), window.Height(),
		memDC, 0, 0,
		SRCCOPY
	);

	memDC->SelectObject(oldMemBitmap);
	delete memBitmap;
	delete memDC;*/

	TCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);
	TRACE(_T("Current dir: %s\n"), currentDir);

	CMipmap mipmap(pDC, L"img/chick1.jpg");
	mipmap.DrawMipmap(pDC, 0, 0, 512);


	/*DImage img;
	img.Load(L"img/chick1.jpg");
	img.Draw(pDC, CRect(0, 0, 256, 256), CRect(0, 0, 256, 256));*/
}


// CRotatingWheelAndMipmapView printing

BOOL CRotatingWheelAndMipmapView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRotatingWheelAndMipmapView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRotatingWheelAndMipmapView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CRotatingWheelAndMipmapView diagnostics

#ifdef _DEBUG
void CRotatingWheelAndMipmapView::AssertValid() const
{
	CView::AssertValid();
}

void CRotatingWheelAndMipmapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRotatingWheelAndMipmapDoc* CRotatingWheelAndMipmapView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRotatingWheelAndMipmapDoc)));
	return (CRotatingWheelAndMipmapDoc*)m_pDocument;
}
#endif //_DEBUG


// CRotatingWheelAndMipmapView message handlers

void CRotatingWheelAndMipmapView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar)
	{
	case 'A': {
		moveBy -= 10;
		break;
	}
	case 'D': {
		moveBy += 10;
		break;
	}
	default:
		break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	Invalidate();
}

BOOL CRotatingWheelAndMipmapView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}

double CRotatingWheelAndMipmapView::degreeToRad(double degreeAngle)
{
	return degreeAngle * (M_PI / 180);
}

void CRotatingWheelAndMipmapView::Translate(CDC* pDC, float dx, float dy, bool rightMultiply)
{
	XFORM xForm = {
		1.0, 0.0,
		0.0, 1.0,
		dx, dy
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CRotatingWheelAndMipmapView::Rotate(CDC* pDC, double angle, bool rightMultiply)
{
	double rad = degreeToRad(angle);
	XFORM xForm = {
		cos(rad), sin(rad),
		-sin(rad), cos(rad),
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CRotatingWheelAndMipmapView::Scale(CDC* pDC, double sx, double sy, bool rightMultiply)
{
	XFORM xForm = {
		sx, 0.0,
		0.0, sy,
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CRotatingWheelAndMipmapView::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	XFORM xForm = {
		(mx ? -1.0 : 1.0), 0.0,
		0.0, (my ? -1.0 : 1.0),
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CRotatingWheelAndMipmapView::Skew(CDC* pDC, float sx, float sy, bool rightMultiply)
{
	XFORM xForm = {
		1.0, sy,
		sx, 1.0,
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CRotatingWheelAndMipmapView::ResetWorldTransformation(CDC* pDC)
{
	pDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
}


