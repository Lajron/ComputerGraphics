
// Bicycle.h : main header file for the Bicycle application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CBicycleApp:
// See Bicycle.cpp for the implementation of this class
//

class CBicycleApp : public CWinApp
{
public:
	CBicycleApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBicycleApp theApp;
