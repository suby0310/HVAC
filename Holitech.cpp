// Holitech.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Holitech.h"
#include "HolitechDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHolitechApp

BEGIN_MESSAGE_MAP(CHolitechApp, CWinApp)
	//{{AFX_MSG_MAP(CHolitechApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHolitechApp construction

CHolitechApp::CHolitechApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHolitechApp object

CHolitechApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CHolitechApp initialization

BOOL CHolitechApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	m_hwndDialog = NULL;

	CHolitechDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CHolitechApp::ProcessMessageFilter(int code, LPMSG lpMsg)
{		
	// Check to make sure CXMDemoDlg is up
	if (m_hwndDialog != NULL)
	{
		if ((lpMsg->hwnd == m_hwndDialog) || ::IsChild(m_hwndDialog, lpMsg->hwnd))
		// Use ::IsChild to get messages that may be going
		// to the dialog's controls, catch keydown and up messages
		{
#if 1
			((CHolitechDlg*)m_pMainWnd)->SetTimer(1, 1000, NULL);
#else

			//Catch Messages to determine when keys and mouse clicks go down or up
			if ((lpMsg->message == WM_KEYDOWN) || (lpMsg->message == WM_SYSKEYDOWN))
			{
				((CHolitechDlg*)m_pMainWnd)->m_KeyDown = lpMsg->wParam;
				((CHolitechDlg*)m_pMainWnd)->SetTimer(TIMER_BUTTON_DOWN, TIMER_BUTTON_DOWN_TIME, NULL);
				return true;
			}
			else if ((lpMsg->message == WM_KEYUP) || (lpMsg->message == WM_SYSKEYUP))
			{
				((CHolitechDlg*)m_pMainWnd)->KillTimer(TIMER_BUTTON_DOWN);
				((CHolitechDlg*)m_pMainWnd)->KeyUp(lpMsg->wParam);
				return true;
			}
			else if (lpMsg->message == WM_LBUTTONUP)
			{
				((CHolitechDlg*)m_pMainWnd)->KillTimer(TIMER_BUTTON_DOWN);
			}
			else if (lpMsg->message == WM_LBUTTONDOWN)
			{
				((CHolitechDlg*)m_pMainWnd)->m_DownPoint = lpMsg->pt;
				((CHolitechDlg*)m_pMainWnd)->SetTimer(TIMER_BUTTON_DOWN, TIMER_BUTTON_DOWN_TIME, NULL);
			}
#endif
		}
	}

	// Default processing of the message.
	return CWinApp::ProcessMessageFilter(code, lpMsg);
}
