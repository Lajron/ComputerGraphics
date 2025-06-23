
// ObservationWheel.h : main header file for the ObservationWheel application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CObservationWheelApp:
// See ObservationWheel.cpp for the implementation of this class
//

class CObservationWheelApp : public CWinApp
{
public:
	CObservationWheelApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CObservationWheelApp theApp;
