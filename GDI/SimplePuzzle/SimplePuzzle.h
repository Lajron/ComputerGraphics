
// SimplePuzzle.h : main header file for the SimplePuzzle application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSimplePuzzleApp:
// See SimplePuzzle.cpp for the implementation of this class
//

class CSimplePuzzleApp : public CWinApp
{
public:
	CSimplePuzzleApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSimplePuzzleApp theApp;
