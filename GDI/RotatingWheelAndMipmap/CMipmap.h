#pragma once
#include <vector>
#include "DImage.h"

class CMipmap
{
public:
	std::vector<CBitmap*> bitmaps;

	CMipmap(CDC* pDC, CString name);

	void DrawMipmap(CDC* pDC, int x, int y, int size);

	~CMipmap();
};

