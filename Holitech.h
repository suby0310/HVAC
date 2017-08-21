// Holitech.h : main header file for the USBRADIO application
//

#if !defined(AFX_HOLITECH_H__C364CC05_2D38_4F78_9DAE_44956FE936BC__INCLUDED_)
#define AFX_HOLITECH_H__C364CC05_2D38_4F78_9DAE_44956FE936BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHolitechApp:
// See Holitech.cpp for the implementation of this class
//

class CHolitechApp : public CWinApp
{
public:
	HWND m_hwndDialog;
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);

	CHolitechApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHolitechApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHolitechApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOLITECH_H__C364CC05_2D38_4F78_9DAE_44956FE936BC__INCLUDED_)
