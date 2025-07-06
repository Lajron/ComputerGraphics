
// DocumentationGDIView.cpp : implementation of the CDocumentationGDIView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DocumentationGDI.h"
#endif

#include "DocumentationGDIDoc.h"
#include "DocumentationGDIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <vector>


// CDocumentationGDIView

IMPLEMENT_DYNCREATE(CDocumentationGDIView, CView)

BEGIN_MESSAGE_MAP(CDocumentationGDIView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CDocumentationGDIView construction/destruction

CDocumentationGDIView::CDocumentationGDIView() noexcept
{
	// TODO: add construction code here

}

CDocumentationGDIView::~CDocumentationGDIView()
{
}

BOOL CDocumentationGDIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDocumentationGDIView drawing
void CDocumentationGDIView::DrawGrid(CDC* pDC, int gridSize, int sqNum)
{
	// COLORREF is where we save RGB color
	// RGB(Red, Green, Blue) is used always for creating color 0-255 
	COLORREF blackColor = RGB(0, 0, 0);

	// Create a pen with CPen
	// PS_SOLID, PS_DASH, PS_DOT etc.. type of lines
	CPen gridPen(PS_SOLID, 1, blackColor);
	// Always use SelectObject() to select the created object, CPen, CBrush, CBitmap etc...
	// SelectObject() returns the old pointer which we should save
	// Once done with drawing the specific thing, we return the oldPen in this case
	CPen* oldPen = pDC->SelectObject(&gridPen);

	int a = gridSize / sqNum;
	for (int i = 0; i <= gridSize; i += a) {
		// CPoint points(x,y) is generally the class used for points
		// MoveTo will move to starting position from which to START with drawing
		pDC->MoveTo(i, 0);
		// LineTo will go from the MoveTo(CPoint) to LineTo(CPoint)
		pDC->LineTo(i, gridSize);

		pDC->MoveTo(0, i);
		pDC->LineTo(gridSize, i);
	}
	// Selecting the old object
	pDC->SelectObject(oldPen);
	// Usually we need to delete the CPen object we created:
	// gridPen.DeleteObject(); since this is in functions stack, it will automatically get removed;
	// delete gridPen; if the pen was created in heap memory with new CPen(PS_SOLID, 1, blackColor)
	// Recommendation in functions to use functions stack so we don't have to manage heap memory
}

void CDocumentationGDIView::DrawPuzzleShape(CDC* pDC, int size)
{
	// CBrush same idea as CPen but it's the FILL of the outline
	CBrush yellowBrush(RGB(255, 255, 0));
	CPen newPen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(&yellowBrush);
	CPen* oldPen = pDC->SelectObject(&newPen);

	// When creating shapes, center of the shape should be 0,0 so it's easier to rotate
	// Same idea will apply to OpenGL and other 3D models
	CPoint points[] = { {size, -size}, {size, size}, {-size, size}, {-size, -size} };
	// Option vector instead of CPoint[]
	 std::vector<CPoint> pointsVector = { {size, -size}, {size, size}, {-size, size}, {-size, -size} };

	// If the shape is not primite like CRect, ->Rectangle(), ->Ellipse() etc...
	// Always use BeginPath() and EndPath() with StrokeAndFillPath() at the end
	pDC->BeginPath();
	{
		// Always set the arc direction, depending on the starting and ending point
		// Never use default direction
		int oldArcDirection = pDC->SetArcDirection(AD_CLOCKWISE);
		// Set the starting point
		pDC->MoveTo(points[0]);
		// Pass the array
		// pDC->PolylineTo(pointsVector.data(), pointsVector.size());
		pDC->PolylineTo(points, 4);
		pDC->ArcTo(CRect(-size, -size * 2, size, 0), points[3], points[0]);
		// Return the old arc direction
		pDC->SetArcDirection(oldArcDirection);
	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

	//pDC->Rectangle(CRect()); 
	//pDC->Ellipse(CRect());
	//pDC->Polygon(points, points.size);
	//pDC->Polyline(points, points.size); Creates closed shape and doesn't care the starting points MoveTo(...)
}

void CDocumentationGDIView::DrawScorpionsNeedle(CDC* pDC, int size)
{
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
		pDC->ArcTo(CRect(-sizeHalf / 2, sizeHalf, sizeHalf / 2, size), points[1], points[2]);
		pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
		pDC->ArcTo(CRect(-sizeHalf, -size, sizeHalf, size), points[2], points[0]);

		pDC->SetArcDirection(ogAD);
	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
}

void CDocumentationGDIView::DrawCybinWithText(CDC* pDC, int w, int d, COLORREF clr, CString str)
{
	// Take CLR argument and make the color 20% lighter
	// To get the actualy values we have to take each channel with GetRValue(clr), GetGValue() and GetBValue()
	// Increase it by 1.2 (20%) and check if it's in the range of 0 - 255
	// Use min(x,y) to get the smaller value, if we get 260, it will return 255, if we get 240 it returns 240
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

	XFORM prevForm; 
	pDC->GetWorldTransform(&prevForm);
	Translate(pDC, 0, w + wHalf, false); // We are moving the whole thing down so the end of the rope is at (0,0)
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
	pDC->LineTo(0, -(w + wHalf)); // End of the rope

	//Text inside the figure
	int size = w * 0.8; //Find the best size for Font

	int oldBkMode = pDC->SetBkMode(TRANSPARENT); // Set the background mode to transparent
	COLORREF oldTextColor = pDC->SetTextColor(RGB(255, 255, 255)); // Set new color
	CFont font;
	font.CreateFontW(size, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial"); // Way of creating the font
	CFont* oldFont = pDC->SelectObject(&font);
	CSize cs = pDC->GetTextExtent(str); // Call this to get the actual size of the str/font

	// cs.cx is full width of the string with the selected font
	// cs.cy is full width of the string with the select font
	// Div 2 and adding minus will set the str in center
	pDC->TextOutW(-cs.cx / 2, -cs.cy / 2, str);

	// Pulling a character from a CString object
	//CString character;
	//character = str.Mid(i, 1); // i - starting index, 1 - substring, in this case one
	//character = CString(str.GetAt(0)); // getAt returns char and then create CString out of it
	//cs = pDC->GetTextExtent(character);

	// Return everything to previous state
	// Don't have to manually delete or deleteObject() anything because we're using function stack
	pDC->SetWorldTransform(&prevForm);
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldTextColor);
	pDC->SetBkMode(oldBkMode);
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CDocumentationGDIView::DrawWheelWithEllipses(CDC* pDC, int r, int d, int n, COLORREF clr)
{
	CPen wheelPen(PS_SOLID, 3 * d, clr);
	CPen* oldPen = pDC->SelectObject(&wheelPen);
	CBrush sBrush(clr);
	CBrush* oldBrush = pDC->GetCurrentBrush(); 
	pDC->SelectStockObject(HOLLOW_BRUSH); // SelectStockObject() if we want hollow_brush

	//Additional brushes like hatch brushes
	// Set background and text colors that affect hatch
	//pDC->SetBkColor(RGB(255, 255, 255));    // Background color
	//pDC->SetTextColor(RGB(0, 0, 255));      // Hatch color (lines)
	//CBrush hatchBrush;
	//hatchBrush.CreateHatchBrush(HS_BDIAGONAL, RGB(0, 0, 255)); // Blue diagonal lines
	//pDC->SelectObject(&hatchBrush);

	int or = r;
	int ir = 0.8 * r;
	int sr = 0.1 * r;

	pDC->Ellipse(CRect(-or , -or , or , or ));
	pDC->Ellipse(CRect(-ir, -ir, ir, ir));

	pDC->SelectObject(&sBrush);
	pDC->Ellipse(CRect(-sr, -sr, sr, sr));

	CPen linePen(PS_SOLID, d, clr);
	pDC->SelectObject(&linePen);
	// Draw lines
	// Get the angle between 2 points
	double rad = degreeToRad(360 / n);
	for (int i = 0; i < n; i++) {
		float x = r * cos(rad * i); // always use cos for X
		float y = r * sin(rad * i); // always use sin for Y
		// tan() when you want to find the other side of a side

		pDC->MoveTo(0, 0);
		pDC->LineTo(x, y);
	}

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
}

void CDocumentationGDIView::DrawWheelWithClipRegion(CDC* pDC, double r1, double r2, double w)
{
	CPen outerLine(PS_SOLID, 4, RGB(50, 50, 50));
	CBrush fillBrush(RGB(150, 150, 150));

	CPen* oldPen = pDC->SelectObject(&outerLine);
	CBrush* oldBrush = pDC->SelectObject(&fillBrush);

	//Rectangles inside
	XFORM prevForm; pDC->GetWorldTransform(&prevForm);
	for (int i = 0; i < 3; i++) {
		Rotate(pDC, -60 * i, false);
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
	//pDC->FrameRgn(&ring, &outlinePen, penWidth, penHeight); draw the outline (border) of the region
	//pDC->PaintRgn(&ring); fill region with the current brush in the DC

	// Clipping the region
	// Doesn't work with world transformations
	//pDC->SelectClipRgn(&ring); anything draw after this will be only be shown if it overlaps with the region
	//pDC->SelectClipRgn(NULL); must have to return the original region... RESET

	pDC->SelectStockObject(HOLLOW_BRUSH);

	pDC->Ellipse(outerRect);
	pDC->Ellipse(innerRect);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CDocumentationGDIView::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	// Load the image bitmap
	//DImage img;
	//img.Load("/some/path.png");
	//img.Save("/some/path.png")
	//img.Draw(pDC, rectFromImageToDraw, rectWeWantToDrawTo);

	// GetDIBBits returns the first pixel
	unsigned char* firstPixel = pImage->GetDIBBits();
	COLORREF removeColor = RGB(
		// Reverse order BGR, so we transform it to RGB
		firstPixel[2], 
		firstPixel[1],
		firstPixel[0]
	);

	CRect imgRect(0, 0, pImage->Width(), pImage->Height());
	// Already existing function that takes the color that we want to remove
	pImage->DrawTransparent(pDC, imgRect, imgRect, removeColor);
}

void CDocumentationGDIView::SetupForNoFlickr(CDC* pDC)
{
	// We need to get the window/canvas size
	CRect window;
	GetClientRect(&window);

	// This is the temporary DC or memory DC, on which we draw and the return to pDC to avoid flickr
	// EraseBkg MFC function has to be set to TRUE or 1
	CDC memDC;
	memDC.CreateCompatibleDC(pDC); // Needs to compatible otherwise it's fucky

	// The memory bitmap in which we will draw everything and copy to pDC bitmap
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, window.Width(), window.Height()); // Needs to be compatible also.

	// As said before SelectObject() is the main thing and in this case we need to save old bitmap so we can return it
	CBitmap* oldBitmap = memDC.SelectObject(&memBitmap);
	// Optional so we don't have black space around the bitmap if it's different size compared to pDC bitmap.
	memDC.FillSolidRect(&window, RGB(200, 200, 200));

	// If pDC Bitmap and memDC Bitmap are the same size, we use this function to just copy from one to another
	pDC->BitBlt(
		0, 0, window.Width(), window.Height(),
		&memDC, 0, 0,
		SRCCOPY
	);
	// If pDC Bitmap and memDC Bitmap are of different size, this is the function we use
	// We can also resize it based on this
	// If we set memDC width and height to half, it will 2x it in pDC Bitmap
	pDC->StretchBlt(
		0, 0, window.Width(), window.Height(),
		&memDC, 0, 0, window.Width()/2, window.Height()/2,
		SRCCOPY
	);

	DImage img(memBitmap); // DImage also accepts a bitmap so we can later do whatever we want with it
	img.Save(L"img/image.png"); // In this case SAVE if need be

	memDC.SelectObject(oldBitmap); // Return the old bitmap (a must)

	// To use a console to check params etc.. we use TRACE function that works like printf
	TRACE(L"This will show windows width and height: %d %d", window.Width(), window.Height());
}

void CDocumentationGDIView::ScaleWithoutStretchBlt(CDC* pSrcDC, int srcWidth, int srcHeight, int zoom)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pSrcDC);

	// Create a CBitmap with zoomed dimensions
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pSrcDC, srcWidth * zoom, srcHeight * zoom);
	CBitmap* pOldBmp = memDC.SelectObject(&bmp);

	// Enable anisotropic mapping mode
	memDC.SetMapMode(MM_ANISOTROPIC);

	// Define logical space (unchanged size)
	memDC.SetWindowExt(srcWidth, srcHeight);

	// Define device space (scaled up)
	memDC.SetViewportExt(srcWidth * zoom, srcHeight * zoom);

	// Optional: fill background
	memDC.FillSolidRect(0, 0, srcWidth * zoom, srcHeight * zoom, RGB(255, 255, 255));

	// Draw content from source (it gets scaled by mapping mode)
	memDC.BitBlt(0, 0, srcWidth, srcHeight, pSrcDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
}

void CDocumentationGDIView::OnDraw(CDC* pDC)
{
	CDocumentationGDIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// (X,Y) starts at 0,0 TOP-LEFT corner
	// ---------------> X
	// |
	// |
	// |
	// |
	// y
	// Generally we should call functions and pass pDC pointer or some other CDC* pointer
	// To remove flicker we create a temporary CDC* in this case memoryDC
	// We also need to use StretchBlt or BitBlt to copy from memoryDC to pDC
	// Additional override EraseBkg to return TRUE
	// Additional override PRESS_ON_KEY_DOWN so we can in real-time move object/drawing in the scene

	CRect window; // CRect that will represent the window size
	GetClientRect(&window); // Pass the data to window, it will always get different data when resizing;

	// To use world transform methods, we need to change the graphics one in the OnDraw() function, *only once
	int oldGraphicsMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldXFORM;
	pDC->GetWorldTransform(&oldXFORM);

	Translate(pDC, window.Width() / 2, window.Height() / 2, false); // Set (0,0) at the center of the window
	DrawGrid(pDC);
	//DrawPuzzleShape(pDC, 100);
	//DrawScorpionsNeedle(pDC, 100);
	//DrawCybinWithText(pDC, 100, 3, RGB(150, 150, 150), _T("1"));// Passing CString with L"" or _T()
	//DrawWheelWithEllipses(pDC, 200, 3, 10, RGB(100, 100, 100));
	//DrawWheelWithClipRegion(pDC, 200, 150, 50);


	pDC->SetWorldTransform(&oldXFORM);
	pDC->SetGraphicsMode(oldGraphicsMode);


}


// CDocumentationGDIView printing

BOOL CDocumentationGDIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDocumentationGDIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDocumentationGDIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CDocumentationGDIView diagnostics

#ifdef _DEBUG
void CDocumentationGDIView::AssertValid() const
{
	CView::AssertValid();
}

void CDocumentationGDIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDocumentationGDIDoc* CDocumentationGDIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDocumentationGDIDoc)));
	return (CDocumentationGDIDoc*)m_pDocument;
}
#endif //_DEBUG


// CDocumentationGDIView message handlers

double CDocumentationGDIView::degreeToRad(double degreeAngle)
{
	// cos/sin/tan and other functions work in radians so this is important to have.
	// radians are what we work with for angles
	return degreeAngle * (M_PI / 180);
}

double CDocumentationGDIView::radToDegree(double radAngle)
{
	// never had a reason to use this one so far, maybe in OpenGL 1.0 (?)
	return radAngle * (180 / M_PI);
}

void CDocumentationGDIView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xForm = {
		1.0, 0.0,
		0.0, 1.0,
		dX, dY
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CDocumentationGDIView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	double rad = degreeToRad(angle);
	XFORM xForm = {
		cos(rad), sin(rad),
		-sin(rad), cos(rad),
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CDocumentationGDIView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xForm = {
		sX, 0.0,
		0.0, sY,
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CDocumentationGDIView::Mirror(CDC* pDC, bool mX, bool mY, bool rightMultiply)
{
	XFORM xForm = {
		(mX ? -1.0 : 1.0), 0.0,
		0.0, (mY ? -1.0 : 1.0),
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CDocumentationGDIView::Skew(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xForm = {
		1.0, sY,
		sX, 1.0,
		0.0, 0.0
	};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CDocumentationGDIView::ResetWorldTransformation(CDC* pDC)
{
	pDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
}



BOOL CDocumentationGDIView::OnEraseBkgnd(CDC* pDC)
{
	// return CView::OnEraseBkgnd(pDC);
	// Set to return TRUE or return 1
	return TRUE;
}

void CDocumentationGDIView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	
	switch (nChar)
	{
	case 'A': {
		// rotAngle += 10;
		break;
	}
	case 'D': {
		// rotAngle -= 10;
		break;
	}
	default:
		break;
	}
	// When calling Rotate()
	// PLUS Rotates it CLOCKWISE
	// MINUS ROtates it COUNTER_CLOCKWISE
	// -------------X
	// |
	// |
	// |
	// |
	// Y
	// Rotate(90*) in GDI is like -90* or 270* in math class
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	// Call Invalidate() so it refreshes the whole OnDraw(CDC* pDC) function
	Invalidate();
}
