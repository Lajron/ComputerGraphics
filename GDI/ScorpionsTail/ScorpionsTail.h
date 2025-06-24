
// ScorpionsTail.h : main header file for the ScorpionsTail application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CScorpionsTailApp:
// See ScorpionsTail.cpp for the implementation of this class
//

class CScorpionsTailApp : public CWinApp
{
public:
	CScorpionsTailApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CScorpionsTailApp theApp;
