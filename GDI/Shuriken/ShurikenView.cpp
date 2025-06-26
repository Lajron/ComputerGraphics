
// ShurikenView.cpp : implementation of the CShurikenView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Shuriken.h"
#endif

#include "ShurikenDoc.h"
#include "ShurikenView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CShurikenView

IMPLEMENT_DYNCREATE(CShurikenView, CView)

BEGIN_MESSAGE_MAP(CShurikenView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CShurikenView construction/destruction

CShurikenView::CShurikenView() noexcept
{
	// TODO: add construction code here

}

CShurikenView::~CShurikenView()
{
}

BOOL CShurikenView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}
#pragma region DefaultFunctions
double rotAngle1 = 0.0;
int moveTo = 0;

static double degreeToRad(double degreeAngle) {
	return degreeAngle * (M_PI / 180);
}
static double radToDegree(double radAngle) {
	return radAngle * (180 / M_PI);
}
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
static void Mirror(CDC* pDC, bool mx = false, bool my = false, bool rightMultiply = true) {
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
#pragma endregion
// CShurikenView drawing

void CShurikenView::DrawBlade(CDC* pDC, int size)
{
	XFORM prevForm; 
	pDC->GetWorldTransform(&prevForm);
	Translate(pDC, size, 0);
	//BLADE
	CBrush greyLight(RGB(180, 180, 180));
	CBrush greyDark(RGB(90, 90, 90));
	CPen bladeOutline(PS_SOLID, 1, RGB(200, 200, 200));

	CBrush* oldBrush = pDC->SelectObject(&greyLight);
	CPen* oldPen = pDC->SelectObject(&bladeOutline);

	CPoint lightTriangle[] = { {-size, 0}, {0, -size}, {3 * size, 0} };
	pDC->Polygon(lightTriangle, 3);

	CPoint darkTriangle[] = { {-size,0}, {0, size}, {3 * size, 0} };
	pDC->SelectObject(&greyDark);
	pDC->Polygon(darkTriangle, 3);

	//TEXT: Shuricane
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CString text = L"Shuricane";

	COLORREF oldTextColor = pDC->SetTextColor(RGB(255, 255, 255));
	CFont fontShadow; fontShadow.CreateFontW(size * 0.6, 0, 0, 0, FW_NORMAL, 1, 0, 0, 0, 0, 0, 0, 0, L"Monotype Corsiva");
	CFont* oldFont = pDC->SelectObject(&fontShadow);
	CSize cs = pDC->GetTextExtent(text);

	pDC->TextOutW(0, -cs.cy / 2, text);
	pDC->SetTextColor(RGB(50, 50, 50));
	pDC->TextOutW(0, (-cs.cy / 2)-size*0.020, text);


	pDC->SetBkMode(oldBkMode);
	pDC->SetTextColor(oldTextColor);
	pDC->SelectObject(oldFont);
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);

	pDC->SetWorldTransform(&prevForm);
}

void CShurikenView::DrawStar(CDC* pDC, int size)
{
	XFORM prevForm;
	pDC->GetWorldTransform(&prevForm);

	for (int i = 0; i < 4; i++) {
		DrawBlade(pDC, size);
		Rotate(pDC, 90);
	}

	pDC->SetWorldTransform(&prevForm);
}

void CShurikenView::DrawScene(CDC* pDC)
{
	CRect window;
	GetClientRect(&window);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, window.Width(), window.Height());
	CBitmap* oldMemBitmap = memDC.SelectObject(&memBitmap);

	int oldGM = memDC.SetGraphicsMode(GM_ADVANCED);
	XFORM prevForm; memDC.GetWorldTransform(&prevForm);

	memDC.FillSolidRect(&window, RGB(100, 100, 100));

	DImage skyBitmap;
	DImage barkBitmap;

	bool isSkyLoaded = skyBitmap.Load(L"img/simpleSky.jpg");
	bool isBarkLoaded = barkBitmap.Load(L"img/treeBark.jpg");

	if (!isSkyLoaded) { // Corrected: Check if NOT loaded
		//TRACE(L"ERROR: Failed to load simpleSky.jpg! Path: img/simpleSky.jpg\n");
	}
	else {
		//TRACE(L"SUCCESS: simpleSky.jpg loaded. Width: %d, Height: %d\n", skyBitmap.Width(), skyBitmap.Height());
	}

	if (!isBarkLoaded) { // Added check for barkBitmap
		//TRACE(L"ERROR: Failed to load treeBark.jpg! Path: img/treeBark.jpg\n");
	}
	else {
		//TRACE(L"SUCCESS: treeBark.jpg loaded. Width: %d, Height: %d\n", barkBitmap.Width(), barkBitmap.Height());
	}
	CRect skyRect(0, 0, skyBitmap.Width(), skyBitmap.Height());
	CRect barkRect(0, 0, barkBitmap.Width(), barkBitmap.Height());
	skyBitmap.Draw(&memDC, skyRect, window);


	Translate(&memDC, moveTo+250, 250);
	Rotate(&memDC, rotAngle1);
	DrawStar(&memDC, 50);

	memDC.SetWorldTransform(&prevForm);
	memDC.SetGraphicsMode(oldGM);

	barkBitmap.Draw(&memDC, barkRect, CRect(window.right - (window.Width() / 10), 0, window.right, window.Height()));

	pDC->BitBlt(
		0, 0, window.Width(), window.Height(),
		&memDC, 0, 0,
		SRCCOPY
	);
	memDC.SelectObject(oldMemBitmap);

}

void CShurikenView::OnDraw(CDC* pDC)
{
	CShurikenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DrawScene(pDC);


}


// CShurikenView printing

BOOL CShurikenView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CShurikenView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CShurikenView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CShurikenView diagnostics

#ifdef _DEBUG
void CShurikenView::AssertValid() const
{
	CView::AssertValid();
}

void CShurikenView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CShurikenDoc* CShurikenView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CShurikenDoc)));
	return (CShurikenDoc*)m_pDocument;
}
#endif //_DEBUG


// CShurikenView message handlers

BOOL CShurikenView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}

void CShurikenView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 'A': {
		rotAngle1 -= 30;
		moveTo -= 50;
		break;
	}
	case 'D': {
		rotAngle1 += 30;
		moveTo += 50;
		break;
	}
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	Invalidate();
}
