
// ObservationWheelView.cpp : implementation of the CObservationWheelView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ObservationWheel.h"
#endif

#include "ObservationWheelDoc.h"
#include "ObservationWheelView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CObservationWheelView

IMPLEMENT_DYNCREATE(CObservationWheelView, CView)

BEGIN_MESSAGE_MAP(CObservationWheelView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CObservationWheelView construction/destruction

CObservationWheelView::CObservationWheelView() noexcept
{
	// TODO: add construction code here

}

CObservationWheelView::~CObservationWheelView()
{
}

BOOL CObservationWheelView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CObservationWheelView drawing
float rotAngle1 = 0.0;
float rotAngle2 = 0.0;

static double degreeToRad(double degreeAngle) {
	return degreeAngle * (M_PI / 180);
}
static double radToDegree(double radAngle) {
	return radAngle * (180 / M_PI);
}

static void Translate(CDC* pDC, float dx, float dy, bool rightMultiplty = false) {
	XFORM xForm = {
		1.0, 0.0,
		0.0, 1.0,
		dx, dy
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiplty ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
static void Rotate(CDC* pDC, float angle, bool rightMultiplty = false) {
	double rad = degreeToRad(angle);
	XFORM xForm = {
		cos(rad), sin(rad),
		-sin(rad), cos(rad),
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiplty ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
static void Scale(CDC* pDC, float sx, float sy, bool rightMultiplty = false) {
	XFORM xForm = {
		sx, 0.0,
		0.0, sy,
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiplty ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
static void Mirror(CDC* pDC, bool mx = false, float my = false, bool rightMultiplty = false) {
	XFORM xForm = {
		(mx ? -1.0 : 1.0), 0.0,
		0.0, (my ? -1.0 : 1.0),
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiplty ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
static void ResetWorldTransformation(CDC* pDC) {
	pDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
}

static void drawGrid(CDC* pDC, int gridSize = 500, int sqNum = 20) {
	CPen nPen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* oldPen = pDC->SelectObject(&nPen);

	int a = gridSize / sqNum;
	for (int i = 0; i <= gridSize; i += a) {
		pDC->MoveTo(i, 0);
		pDC->LineTo(i, gridSize);

		pDC->MoveTo(0, i);
		pDC->LineTo(gridSize, i);
	}
	pDC->SelectObject(oldPen);
}
static void DrawCybin(CDC* pDC, int w, int d, COLORREF clr, CString str) {
	COLORREF clrBkg = RGB(
		min((int)GetRValue(clr) * 1.2, 255),
		min((int)GetGValue(clr) * 1.2, 255),
		min((int)GetBValue(clr) * 1.2, 255)
	);
	CBrush cybinBrush(clrBkg);
	CPen cybinPen(PS_SOLID, d, clr);
	CBrush* oldBrush = pDC->SelectObject(&cybinBrush);
	CPen* oldPen = pDC->SelectObject(&cybinPen);

	int wHalf = w / 2;
	CPoint points[] = { {wHalf, -wHalf}, {wHalf, wHalf}, {-wHalf, wHalf}, {-wHalf, -wHalf } };

	XFORM prevForm; pDC->GetWorldTransform(&prevForm);
	Translate(pDC, 0, w + wHalf);
	//Base figure
	pDC->BeginPath();
	{
		pDC->MoveTo(points[0]);
		pDC->PolylineTo(points, 4);
		int oldAD = pDC->SetArcDirection(AD_CLOCKWISE);
		pDC->ArcTo(CRect(-wHalf, -w, wHalf, 0), points[3], points[0]);
		pDC->SetArcDirection(oldAD);
	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	//Rope of the base figure
	pDC->MoveTo(0, -w);
	pDC->LineTo(0, -(w + wHalf));

	//Text inside the figure
	int size = w * 0.8; //Find the best size
	CFont font; font.CreateFontW(size, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");

	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF oldTextColor = pDC->SetTextColor(RGB(255, 255, 255));
	CFont* oldFont = pDC->SelectObject(&font);
	CSize cs = pDC->GetTextExtent(str);

	pDC->TextOutW(-cs.cx / 2, -cs.cy / 2, str);

	pDC->SetWorldTransform(&prevForm);
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldTextColor);
	pDC->SetBkMode(oldBkMode);
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}
static void DrawWheel(CDC* pDC, int r, int d, int n, COLORREF clr) {
	CPen wheelPen(PS_SOLID, 3 * d, clr);
	CPen* oldPen = pDC->SelectObject(&wheelPen);
	CBrush sBrush(clr);
	CBrush* oldBrush = pDC->GetCurrentBrush();
	pDC->SelectStockObject(HOLLOW_BRUSH);

	int or = r;
	int ir = 0.8 * r;
	int sr = 0.1 * r;

	pDC->Ellipse(CRect(-or , -or , or , or ));
	pDC->Ellipse(CRect(-ir , -ir , ir , ir ));

	pDC->SelectObject(&sBrush);
	pDC->Ellipse(CRect(-sr, -sr, sr, sr));

	CPen linePen(PS_SOLID, d, clr);
	pDC->SelectObject(&linePen);
	double rad = degreeToRad(360 / n);
	for (int i = 0; i < n; i++) {
		float x = r * cos(rad*i);
		float y = r * sin(rad*i);

		pDC->MoveTo(0, 0);
		pDC->LineTo(x, y);
	}

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
}
static void DrawObservationWheel(CDC* pDC, int r, int d, int n, COLORREF clr, CString arStr[], int k, CPoint ptCenter, float alpha) {
	CPen owPen(PS_SOLID, d, clr);
	CPen* oldPen = pDC->SelectObject(&owPen);
	CBrush* oldBrush = pDC->GetCurrentBrush();
	pDC->SelectStockObject(HOLLOW_BRUSH);

	XFORM prevForm; pDC->GetWorldTransform(&prevForm);

	Translate(pDC, ptCenter.x, ptCenter.y);

	CPoint trougao[] = { {0, 0}, {(int)-(0.8 * r / 2), (int)(1.6 * r)}, {(int)(0.8 * r / 2), (int)(1.6 * r)} };
	pDC->Polygon(trougao, 3);

	Rotate(pDC, alpha);
	DrawWheel(pDC, r, d, n, clr);
	
	double rad = degreeToRad(360 / k);
	for (int i = 0; i < k; i++) {
		XFORM forForm; pDC->GetWorldTransform(&forForm);
		float x = r * cos(rad * i);
		float y = r * sin(rad * i);
		
		Translate(pDC, x, y);
		Rotate(pDC, -alpha);
		DrawCybin(pDC, 0.2 * r, d, clr, arStr[i]);

		pDC->SetWorldTransform(&forForm);
	}

	pDC->SetWorldTransform(&prevForm);
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
}

void CObservationWheelView::OnDraw(CDC* pDC)
{
	CObservationWheelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	CRect window;
	GetClientRect(&window);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, window.Width(), window.Height());

	CBitmap* oldBitmap = memDC.SelectObject(&memBitmap);

	memDC.FillSolidRect(&window, RGB(255, 255, 255));

	CString letters[] = { L"1", L"2", L"3", L"4", L"5", L"6" };
	XFORM oldForm;
	int oldGM = memDC.SetGraphicsMode(GM_ADVANCED);
	memDC.GetWorldTransform(&oldForm);
	drawGrid(&memDC);
	DrawObservationWheel(&memDC, 200, 2, 18, RGB(100, 100, 100), letters, 6, { 250, 250 }, rotAngle1);
	memDC.SetWorldTransform(&oldForm);
	memDC.SetGraphicsMode(oldGM);

	pDC->BitBlt(
		window.left, window.top, window.Width(), window.Height(),
		&memDC, window.left, window.top,
		SRCCOPY
	);

	memDC.SelectObject(oldBitmap);
}


// CObservationWheelView printing

BOOL CObservationWheelView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CObservationWheelView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CObservationWheelView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CObservationWheelView diagnostics

#ifdef _DEBUG
void CObservationWheelView::AssertValid() const
{
	CView::AssertValid();
}

void CObservationWheelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CObservationWheelDoc* CObservationWheelView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CObservationWheelDoc)));
	return (CObservationWheelDoc*)m_pDocument;
}
#endif //_DEBUG


// CObservationWheelView message handlers

void CObservationWheelView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

void CObservationWheelView::SaveDC(CDC* pDC, CRect rcDC, CRect rcBmp, CString strBmp)
{
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);

	CBitmap* newBitmap = new CBitmap();
	newBitmap->CreateCompatibleBitmap(pDC, rcBmp.Width(), rcBmp.Height());

	CBitmap* oldBitmap = memDC->SelectObject(newBitmap);

	memDC->StretchBlt(
		rcBmp.left, rcBmp.top, rcBmp.Width(), rcBmp.Height(),
		pDC, rcDC.left, rcDC.top, rcDC.Width(), rcDC.Height(),
		SRCCOPY
	);

	//DImage image(memDC->GetCurrentBitmap())
	//image.Save(strBmp)

	memDC->SelectObject(oldBitmap);
	delete newBitmap;
	delete(memDC);
}

BOOL CObservationWheelView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}
