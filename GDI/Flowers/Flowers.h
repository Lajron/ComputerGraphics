
// Flowers.h : main header file for the Flowers application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CFlowersApp:
// See Flowers.cpp for the implementation of this class
//

class CFlowersApp : public CWinApp
{
public:
	CFlowersApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFlowersApp theApp;
