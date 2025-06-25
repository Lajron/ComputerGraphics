
// FlowersView.cpp : implementation of the CFlowersView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Flowers.h"
#endif

#include "FlowersDoc.h"
#include "FlowersView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <vector>


// CFlowersView

IMPLEMENT_DYNCREATE(CFlowersView, CView)

BEGIN_MESSAGE_MAP(CFlowersView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CFlowersView construction/destruction

CFlowersView::CFlowersView() noexcept
{
	// TODO: add construction code here

}

CFlowersView::~CFlowersView()
{
}

BOOL CFlowersView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CFlowersView drawing
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

void CFlowersView::DrawPetal(CDC* pDC, CRect rect, int nArcs, COLORREF clrFill, COLORREF clrLine)
{
	CPen petalPen(PS_SOLID, 2, clrLine);
	CBrush petalBrush(clrFill);

	CPen* oldPen = pDC->SelectObject(&petalPen);
	CBrush* oldBrush = pDC->SelectObject(&petalBrush);

	int arc2R = rect.Width() / nArcs;
	int arcR = arc2R / 2;

	std::vector<CPoint> figureLower = { {rect.Width() / 2, 0}, {0, rect.Height()}, {-rect.Width() / 2, 0} };

	pDC->BeginPath();
	{
		pDC->MoveTo(figureLower[0]);
		pDC->PolylineTo(figureLower.data(), 3);

		int oldAD = pDC->SetArcDirection(AD_CLOCKWISE);

		for (int i = 0; i < nArcs; i++) {
			int xStart = figureLower[2].x + (arc2R * i);
			int xEnd = figureLower[2].x + (arc2R * (i+1));
			pDC->ArcTo(CRect( xStart, -arcR, xEnd, arcR), { xStart, 0 }, { xEnd, 0 });
		}

		pDC->SetArcDirection(oldAD);

	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CFlowersView::DrawFlower(CDC* pDC, CRect rect, int nArcs, COLORREF clrFill, COLORREF clrLine, float dRot, CString str)
{
	XFORM ogForm; pDC->GetWorldTransform(&ogForm);
	//Petals
	Rotate(pDC, dRot);
	for (int i = 0; i < 4; i++) {
		Rotate(pDC, i * 90);
		Translate(pDC, 0, -rect.Height());

		DrawPetal(pDC, rect, nArcs, clrFill, clrLine);

		Translate(pDC, 0, rect.Height());
		Rotate(pDC, -i * 90);
	}
	pDC->SetWorldTransform(&ogForm);
	
	//Center
	CPen pen(PS_SOLID, 2, clrLine);
	CBrush brush(RGB(255, 255, 0));
	CPen* oldPen = pDC->SelectObject(&pen);
	CBrush* oldBrush = pDC->SelectObject(&brush);

	pDC->Ellipse(-rect.Width() / 2, -rect.Width() / 2, rect.Width() / 2, rect.Width() / 2);

	//Text
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF oldTextColor = pDC->SetTextColor(RGB(0, 0, 0));
	CFont font; font.CreateFontW(rect.Height() / 3, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");
	CFont* oldFont = pDC->SelectObject(&font);
	CSize cs;

	float rot = (135 - 45) / (str.GetLength()-1);
	XFORM prevForm;
	for (int i = 0; i < str.GetLength(); i++) {
		pDC->GetWorldTransform(&prevForm);

		CString character = str.Mid(i, 1);
		cs = pDC->GetTextExtent(character);

		Rotate(pDC, 45 - (i * rot));
		Translate(pDC, 0, 1.5 * rect.Height());
		pDC->TextOutW(-cs.cx / 2, -cs.cy / 2, character);

		pDC->SetWorldTransform(&prevForm);
	}

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldTextColor);
	pDC->SetBkMode(oldBkMode);
	pDC->SetWorldTransform(&ogForm);
}

void CFlowersView::DrawFlowers(CDC* pDC, CRect* aRect, int* anArcs, COLORREF* aClrFill, COLORREF* aClrLine, float* aRot, CString* aStr, CPoint* aPts, int nFlower)
{
	XFORM state;
	pDC->GetWorldTransform(&state);

	int x = 0, y = 0;

	for (int i = 0; i < nFlower; i++)
	{
		Translate(pDC, aPts[i].x - x, aPts[i].y - y, false);
		DrawFlower(pDC, aRect[i], anArcs[i], aClrFill[i], aClrLine[i], aRot[i], aStr[i]);
		x = aPts[i].x;
		y = aPts[i].y;
	}
	
	pDC->SetWorldTransform(&state);
}

void CFlowersView::SaveDC(CDC* pDC, CRect rcDC, CRect rcBmp)
{
}

void CFlowersView::OnDraw(CDC* pDC)
{
	CFlowersDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int oldGM = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM ogForm; pDC->GetWorldTransform(&ogForm);

	//drawGrid(pDC);
	Translate(pDC, 250, 250);

	CRect aRect[3] = { CRect(0, 0, 60, 60), CRect(0, 0, 80, 80), CRect(0, 0, 70, 70) };
	int anArcs[3] = { 1, 2, 3 };
	COLORREF aClrFill[3] = { RGB(200, 0, 0), RGB(200, 100, 0), RGB(200, 0, 200) },
		aClrLine[3] = { RGB(100, 0, 0), RGB(100, 50, 0), RGB(100, 0, 100) };
	float aRot[3] = { 3.14 / 4, 3.14 / 6, 3.14 / 8 };
	CString aStr[3] = { L"Ruzicica", L"Lalicica", L"Ljubicica" };
	CPoint aPts[3] = { { 300, 100 }, { 500, 400 }, { 1000, 300 } };

	DrawFlowers(pDC, aRect, anArcs, aClrFill, aClrLine, aRot, aStr, aPts, 3);

	pDC->SetWorldTransform(&ogForm);
	pDC->SetGraphicsMode(oldGM);
}


// CFlowersView printing

BOOL CFlowersView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFlowersView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFlowersView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CFlowersView diagnostics

#ifdef _DEBUG
void CFlowersView::AssertValid() const
{
	CView::AssertValid();
}

void CFlowersView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFlowersDoc* CFlowersView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFlowersDoc)));
	return (CFlowersDoc*)m_pDocument;
}
#endif //_DEBUG


// CFlowersView message handlers

double CFlowersView::degreeToRad(double degreeAngle)
{
	return degreeAngle * (M_PI / 180);
}

void CFlowersView::Translate(CDC* pDC, int dx, int dy, bool rightMultiply)
{
	XFORM xForm = {
		1.0, 0.0,
		0.0, 1.0,
		dx, dy
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CFlowersView::Rotate(CDC* pDC, double angle, bool rightMultiply)
{
	double rad = degreeToRad(angle);
	XFORM xForm = {
		cos(rad), sin(rad),
		-sin(rad), cos(rad),
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CFlowersView::Scale(CDC* pDC, double sx, double sy, bool rightMultiply)
{
	XFORM xForm = {
		sx, 0.0,
		0.0, sy,
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CFlowersView::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	XFORM xForm = {
		(mx ? -1.0 : 1.0), 0.0,
		0.0, (my ? -1.0 : 1.0),
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}


