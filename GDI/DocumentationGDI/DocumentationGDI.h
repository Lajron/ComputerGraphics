
// DocumentationGDI.h : main header file for the DocumentationGDI application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CDocumentationGDIApp:
// See DocumentationGDI.cpp for the implementation of this class
//

class CDocumentationGDIApp : public CWinApp
{
public:
	CDocumentationGDIApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDocumentationGDIApp theApp;
