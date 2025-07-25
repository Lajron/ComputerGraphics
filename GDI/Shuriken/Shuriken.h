
// Shuriken.h : main header file for the Shuriken application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CShurikenApp:
// See Shuriken.cpp for the implementation of this class
//

class CShurikenApp : public CWinApp
{
public:
	CShurikenApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CShurikenApp theApp;
