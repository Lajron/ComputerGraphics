
// RobotView.cpp : implementation of the CRobotView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Robot.h"
#endif

#include "RobotDoc.h"
#include "RobotView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRobotView

IMPLEMENT_DYNCREATE(CRobotView, CView)

BEGIN_MESSAGE_MAP(CRobotView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CRobotView construction/destruction

CRobotView::CRobotView() noexcept
{
	glava = new DImage(); glava->Load(L"img/glava.png");
	nKolenica = new DImage(); nKolenica->Load(L"img/nadkolenica.png");
	nLaktica = new DImage(); nLaktica->Load(L"img/nadlaktica.png");
	pKolenica = new DImage(); pKolenica->Load(L"img/podkolenica.png");
	pLaktica = new DImage(); pLaktica->Load(L"img/podlaktica.png");
	saka = new DImage(); saka->Load(L"img/saka.png");
	stopalo = new DImage(); stopalo->Load(L"img/stopalo.png");
	telo = new DImage(); telo->Load(L"img/telo.png");
	pozadina = new DImage(); pozadina->Load(L"img/pozadina.jpg");
}

CRobotView::~CRobotView()
{
	delete glava;
	delete nKolenica;
	delete nLaktica;
	delete pKolenica;
	delete pLaktica;
	delete saka;
	delete stopalo;
	delete telo;
	delete pozadina;
}

BOOL CRobotView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
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

// CRobotView drawing
void CRobotView::DrawBackground(CDC* pDC)
{
	CRect img(0, 0, pozadina->Width(), pozadina->Height());
	pozadina->Draw(pDC, img, img);
}

void CRobotView::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	unsigned char* firstPixel = pImage->GetDIBBits();
	COLORREF removeColor = RGB(
		firstPixel[2],
		firstPixel[1],
		firstPixel[0]
	);

	CRect imgRect(0, 0, pImage->Width(), pImage->Height());
	pImage->DrawTransparent(pDC, imgRect, imgRect, removeColor);
}

void CRobotView::DrawPart(CDC* pDC, DImage* pImg, int dX, int dY) {
	Translate(pDC, dX, dY, false);
	DrawImgTransparent(pDC, pImg);
	drawGrid(pDC);
}

void CRobotView::DrawHalf(CDC* pDC)
{
	XFORM ogForm;
	pDC->GetWorldTransform(&ogForm);
	Translate(pDC, -telo->Width(), 0, false);
	DrawImgTransparent(pDC, telo);

	//UPPER PART
	Translate(pDC, 61, 262, false);
	Translate(pDC, -29, -20, false);
	DrawImgTransparent(pDC, nKolenica);

	Translate(pDC, 30, 184, false);
	Translate(pDC, -25, -37, false);
	DrawImgTransparent(pDC, pKolenica);

	Translate(pDC, 25, 248, false);
	Translate(pDC, -20, -16, false);
	DrawImgTransparent(pDC, stopalo);

	pDC->SetWorldTransform(&ogForm);
	Translate(pDC, -telo->Width(), 0, false);

	//LOWER PART
	Translate(pDC, 25, 65, false);
	Rotate(pDC, rot1, false);
	Translate(pDC, -35, -35, false);
	DrawImgTransparent(pDC, nLaktica);

	Translate(pDC, 22, 167, false);
	Rotate(pDC, rot2, false);
	Translate(pDC, -30, -33, false);
	DrawImgTransparent(pDC, pLaktica);

	Translate(pDC, 33, 140, false);
	Rotate(pDC, rot3, false);
	Translate(pDC, -25, -3, false);
	DrawImgTransparent(pDC, saka);

	pDC->SetWorldTransform(&ogForm);
}

void CRobotView::DrawHead(CDC* pDC)
{
	Translate(pDC, -glava->Width() / 2, -glava->Height()/2, false);
	DrawImgTransparent(pDC, glava);
	Translate(pDC, glava->Width() / 2, glava->Height()/2, false);

}

void CRobotView::DrawRobot(CDC* pDC)
{
	XFORM ogForm;
	pDC->GetWorldTransform(&ogForm);

	Translate(pDC, 0, -40, false);
	DrawHead(pDC);
	Translate(pDC, 0, 40, false);

	DrawHalf(pDC);
	Mirror(pDC, true, false, false);
	DrawHalf(pDC);

	pDC->SetWorldTransform(&ogForm);
}

void CRobotView::OnDraw(CDC* pDC)
{
	CRobotDoc* pDoc = GetDocument();
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

	Translate(&memDC, 515, 200, false);

	DrawRobot(&memDC);

	memDC.SetWorldTransform(&ogForm);
	memDC.SetGraphicsMode(oldGM);

	pDC->BitBlt(
		0, 0, window.Width(), window.Height(),
		&memDC, 0, 0,
		SRCCOPY
	);

	memDC.SelectObject(oldMemBitmap);
}



// CRobotView printing

BOOL CRobotView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRobotView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRobotView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CRobotView diagnostics

#ifdef _DEBUG
void CRobotView::AssertValid() const
{
	CView::AssertValid();
}

void CRobotView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRobotDoc* CRobotView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRobotDoc)));
	return (CRobotDoc*)m_pDocument;
}
#endif //_DEBUG


// CRobotView message handlers
static double degreeToRad(float degreeAngle) {
	return degreeAngle * (M_PI / 180);
}

void CRobotView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xForm = {
		1.0, 0.0,
		0.0, 1.0,
		dX, dY
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CRobotView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	double rad = degreeToRad(angle);
	XFORM xForm = {
		cos(rad), sin(rad),
		-sin(rad), cos(rad),
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CRobotView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xForm = {
		sX, 0.0,
		0.0, sY,
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CRobotView::Mirror(CDC* pDC, bool mX, bool mY, bool rightMultiply)
{
	XFORM xForm = {
		(mX ? -1.0 : 1.0), 0.0,
		0.0, (mY ? -1.0 : 1.0),
		0.0,0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CRobotView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 'A': {
		rot1 -= 10;
		break;
	}
	case 'S': {
		rot1 += 10;
		break;
	}
	case 'D': {
		rot2 -= 10;
		break;
	}
	case 'F': {
		rot2 += 10;
		break;
	}
	case 'G': {
		rot3 -= 10;
		break;
	}
	case 'H': {
		rot3 += 10;
		break;
	}
	case '1': {
		scale -= 1;
		break;
	}
	case '2': {
		scale += 1;
		break;
	}
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	Invalidate();
}

BOOL CRobotView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}
