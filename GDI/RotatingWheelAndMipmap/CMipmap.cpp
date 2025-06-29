#include "pch.h"
#include "CMipmap.h"

CMipmap::CMipmap(CDC* pDC, CString name)
{	
	ASSERT(pDC != nullptr);
	ASSERT(pDC->GetSafeHdc() != NULL);
	TRACE(_T("pDC is valid, HDC: %p\n"), pDC->GetSafeHdc());

	TCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);
	TRACE(_T("Current dir: %s\n"), currentDir);

	DImage img;
	img.Load(name);
	if (!img.isValid()) {
		TRACE(_T("Slika nije validna!\n"));
		return;
	}
	int offsetX = 0;
	int size = img.Width(); // could have used .Height(), same value
	int totalWidth = 2 * size; // close enough to actual total width of all dimensions
	int totalHeight = size;

	CDC bigDC;
	bigDC.CreateCompatibleDC(pDC);
	CBitmap* bigBitmap = new CBitmap();
	bigBitmap->CreateCompatibleBitmap(pDC, totalWidth, totalHeight);
	CBitmap* oldBigBitmap = bigDC.SelectObject(bigBitmap);
	bigDC.FillSolidRect(0, 0, totalWidth, totalHeight, RGB(255, 255, 255));

	while (size >= 1) {
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);

		CBitmap* memBitmap = new CBitmap;
		BOOL created = memBitmap->CreateCompatibleBitmap(pDC, size, size);
		TRACE(_T("Trying to create bitmap of size %d x %d, result: %d\n"), size, size, created);
		CBitmap* oldMemBitmap = memDC.SelectObject(memBitmap);
		memDC.FillSolidRect(0, 0, size, size, RGB(255, 255, 255)); 

		CRect ogImage(0, 0, img.Width(), img.Height());
		CRect modifiedImage(0, 0, size, size);

		img.Draw(&memDC, ogImage, modifiedImage);

		memDC.SelectObject(oldMemBitmap);

		bitmaps.push_back(memBitmap);

		CRect bigDestRect(offsetX, 0, offsetX + size, size);
		img.Draw(&bigDC, ogImage, bigDestRect);

		offsetX += size;
		size /= 2;
	}
	bigDC.SelectObject(oldBigBitmap);
	bitmaps.push_back(bigBitmap);

}

void CMipmap::DrawMipmap(CDC* pDC, int x, int y, int size)
{
	if (bitmaps.empty())
		return;


	BITMAP bmpInfo0;
	bitmaps[0]->GetObject(sizeof(BITMAP), &bmpInfo0);
	//closestSize, assume [0] element is closest
	int closestSize = abs(bmpInfo0.bmWidth - size);
	int closestIndex = 0;

	//Check if there's a closer element, start with [1]
	for (int i = 1; i < bitmaps.size(); i++) {
		BITMAP currentBmpInfo;
		bitmaps[i]->GetObject(sizeof(BITMAP), &currentBmpInfo); // Get actual dimensions
		int width = currentBmpInfo.bmWidth;
		int currentDiff = abs(width - size);

		if (currentDiff < closestSize) {
			closestSize = currentDiff;
			closestIndex = i;
		}
	}

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* oldMemBMP = memDC.SelectObject(bitmaps[closestIndex]);
	pDC->FillSolidRect(0, 0, 600, 600, RGB(255, 255, 255));

	BITMAP selectedBmpInfo;
	bitmaps[closestIndex]->GetObject(sizeof(BITMAP), &selectedBmpInfo);
	int bmpWidth = selectedBmpInfo.bmWidth;
	int bmpHeight = selectedBmpInfo.bmHeight;

	pDC->BitBlt(x, y, bmpWidth, bmpHeight, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(oldMemBMP);

}

CMipmap::~CMipmap()
{
	for (int i = 0; i < bitmaps.size(); i++) {
		delete bitmaps[i];
	}
	bitmaps.clear();
}
