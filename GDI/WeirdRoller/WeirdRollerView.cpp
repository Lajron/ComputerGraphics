
// WeirdRollerView.cpp : implementation of the CWeirdRollerView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WeirdRoller.h"
#endif

#include "WeirdRollerDoc.h"
#include "WeirdRollerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWeirdRollerView

IMPLEMENT_DYNCREATE(CWeirdRollerView, CView)

BEGIN_MESSAGE_MAP(CWeirdRollerView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CWeirdRollerView construction/destruction

CWeirdRollerView::CWeirdRollerView() noexcept
{
	bkg = new DImage(); bkg->Load(L"img/Background.jpg");
	basket = new DImage(); basket->Load(L"img\\Basket.png");
	arm = new DImage(); arm->Load(L"img\\Arm.png");
}

CWeirdRollerView::~CWeirdRollerView()
{
	delete bkg;
	delete basket;
	delete arm;
}

BOOL CWeirdRollerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CWeirdRollerView drawing
static double degreeToRad(double degreeAngle) {
	return degreeAngle * (M_PI / 180);
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

void CWeirdRollerView::DrawBackground(CDC* pDC)
{
	CRect window;
	GetClientRect(&window);

	double imgWidth = bkg->Width();
	double imgHeight = bkg->Height();

	bkg->Draw(
		pDC,
		CRect(0, 0, imgWidth, imgHeight),
		CRect(
			(window.Width()-imgWidth)/2,
			window.Height()-imgHeight,
			(window.Width() + imgWidth) / 2,
			window.Height()
		)
	);
}

void CWeirdRollerView::DrawImageTransparent(CDC* pDC, DImage* pImage)
{
	//Getting color out from DImage
	unsigned char* firstPixel = pImage->GetDIBBits();
	COLORREF removeColor = RGB(
		firstPixel[2],
		firstPixel[1],
		firstPixel[0]
	);

	pImage->DrawTransparent(
		pDC,
		CRect(0, 0, pImage->Width(), pImage->Height()),
		CRect(0, 0, pImage->Width(), pImage->Height()),
		removeColor
	);
}

void CWeirdRollerView::DrawArm(CDC* pDC)
{
	XFORM prevForm; pDC->GetWorldTransform(&prevForm);

	Translate(pDC, -10, -10, false);
	DrawImageTransparent(pDC, arm);

	pDC->SetWorldTransform(&prevForm);
}

void CWeirdRollerView::DrawBasket(CDC* pDC, int r)
{
	XFORM prevForm; pDC->GetWorldTransform(&prevForm);

	int basketWidth = basket->Width();
	int basketHeight = basket->Height();

	Scale(pDC, 0.675, 0.675, false);
	//drawGrid(pDC);
	Translate(pDC, -basketWidth / 2, -basketHeight / 2, false);
	DrawImageTransparent(pDC, basket);

	pDC->SetWorldTransform(&prevForm);

	CString txt("R_G");
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF oldTextColor = pDC->SetTextColor(RGB(0, 0, 120));
	CFont font; font.CreateFontW(0.9 * r, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"Verdana");
	CFont* oldFont = pDC->SelectObject(&font);
	CSize cs = pDC->GetTextExtent(txt);

	Rotate(pDC, -30, false);
	pDC->TextOutW(-cs.cx / 2, -cs.cy / 2, txt);

	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldTextColor);
	pDC->SetBkMode(oldBkMode);
	pDC->SetWorldTransform(&prevForm);
}

void CWeirdRollerView::DrawBasketCouple(CDC* pDC, int l, int r, float angle)
{
	XFORM prevForm; pDC->GetWorldTransform(&prevForm);

	Rotate(pDC, angle, false);
	DrawBasket(pDC, r);
	Rotate(pDC, -angle, false);

	Translate(pDC, l, 0, false);
	Rotate(pDC, angle, false);
	DrawBasket(pDC, r);
	Rotate(pDC, -angle, false);
	Translate(pDC, -l, 0, false);

	Rotate(pDC, angle, false);
	Translate(pDC, 0.8 * r, 0, false);
	Rotate(pDC, -90 - angle, false);
	DrawArm(pDC);

	pDC->SetWorldTransform(&prevForm);
}

void CWeirdRollerView::DrawPlatform(CDC* pDC, int l, int r, double angle)
{
	XFORM prevForm; pDC->GetWorldTransform(&prevForm);

	Rotate(pDC, angle, false);
	DrawBasket(pDC, r);

	double rad = degreeToRad(360 / 6);
	double distance = basket->Width() * 0.62;
	XFORM forForm;
	for (int i = 0; i < 6; i++) {
		pDC->GetWorldTransform(&forForm);
		double rotation = 60 * i;
		float x = distance * cos(rad*i);
		float y = distance * sin(rad*i);
		Translate(pDC, x, y, false);
		Rotate(pDC, rotation, false);
		//drawGrid(pDC);
		DrawBasketCouple(pDC, l, r, -angle);

		pDC->SetWorldTransform(&forForm);
	}


	pDC->SetWorldTransform(&prevForm);
}

void CWeirdRollerView::DrawCarousel(CDC* pDC, int h, int r, double offset, double alpha, double beta, double angle)
{
	XFORM ogForm; pDC->GetWorldTransform(&ogForm);

	//Base basket
	Translate(pDC, offset, 0, false);
	Rotate(pDC, offset, false);
	DrawBasket(pDC, r);
	Rotate(pDC, -offset, false);

	//Base arm PART_1
	Rotate(pDC, -180 + alpha, false);
	DrawArm(pDC);
	//Base arm PART_2
	Translate(pDC, 0, 180, false);
	Rotate(pDC, beta, false);
	DrawArm(pDC);

	Translate(pDC, 0, 180, false);
	DrawPlatform(pDC, h, r, angle);

	pDC->SetWorldTransform(&ogForm);
}

void CWeirdRollerView::OnDraw(CDC* pDC)
{
	CWeirdRollerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect window;
	GetClientRect(&window);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, window.Width(), window.Height());
	CBitmap* oldMemBitmap = memDC.SelectObject(&memBitmap);

	int oldGM = memDC.SetGraphicsMode(GM_ADVANCED);
	XFORM ogForm; memDC.GetWorldTransform(&ogForm);

	DrawBackground(&memDC);
	Translate(&memDC, window.Width()/2, window.Height()-100, false);
	DrawCarousel(&memDC, 182, 50, moveX, rot1, rot2, rot3);

	memDC.SetWorldTransform(&ogForm);
	memDC.SetGraphicsMode(oldGM);

	pDC->BitBlt(
		0, 0, window.Width(), window.Height(),
		&memDC, 0, 0,
		SRCCOPY
	);

	memDC.SelectObject(oldMemBitmap);
}


// CWeirdRollerView printing

BOOL CWeirdRollerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWeirdRollerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWeirdRollerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CWeirdRollerView diagnostics

#ifdef _DEBUG
void CWeirdRollerView::AssertValid() const
{
	CView::AssertValid();
}

void CWeirdRollerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWeirdRollerDoc* CWeirdRollerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWeirdRollerDoc)));
	return (CWeirdRollerDoc*)m_pDocument;
}
#endif //_DEBUG


// CWeirdRollerView message handlers
void CWeirdRollerView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xForm = {
		1.0, 0.0,
		0.0, 1.0,
		dX, dY
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CWeirdRollerView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	double rad = degreeToRad(angle);
	XFORM xForm = {
		cos(rad), sin(rad),
		-sin(rad), cos(rad),
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CWeirdRollerView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xForm = {
		sX, 0.0,
		0.0, sY,
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}



void CWeirdRollerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 'A': {
		rot1 -= 10;
		break;
	}
	case 'D': {
		rot1 += 10;
		break;
	}
	case 'Q': {
		rot2 -= 10;
		break;
	}
	case 'E': {
		rot2 += 10;
		break;
	}
	case 'Z': {
		rot3 -= 10;
		break;
	}
	case 'C': {
		rot3 += 10;
		break;
	}
	case 'S': {
		moveX += 10;
		break;
	}
	case 'W': {
		moveX -= 10;
		break;
	}
	default:
		break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	Invalidate();
}

BOOL CWeirdRollerView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}
