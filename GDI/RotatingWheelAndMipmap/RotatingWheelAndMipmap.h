
// RotatingWheelAndMipmap.h : main header file for the RotatingWheelAndMipmap application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRotatingWheelAndMipmapApp:
// See RotatingWheelAndMipmap.cpp for the implementation of this class
//

class CRotatingWheelAndMipmapApp : public CWinApp
{
public:
	CRotatingWheelAndMipmapApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRotatingWheelAndMipmapApp theApp;
