#if !defined(AFX_TOOLBARDLG_H__F39C723F_CB16_4DD9_9896_9A3B8E69A88D__INCLUDED_)
#define AFX_TOOLBARDLG_H__F39C723F_CB16_4DD9_9896_9A3B8E69A88D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolbarDlg.h : header file
//

#include "resource.h"
#include "HolitechDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CToolbarDlg dialog

class CToolbarDlg : public CDialog
{
// Construction
public:
	CToolbarDlg(CWnd* pParent = NULL);   // standard constructor

	CHolitechDlg*	m_pHolitechDlg;

// Dialog Data
	//{{AFX_DATA(CToolbarDlg)
	enum { IDD = IDD_DIALOG_TOOLBAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolbarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolbarDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARDLG_H__F39C723F_CB16_4DD9_9896_9A3B8E69A88D__INCLUDED_)
