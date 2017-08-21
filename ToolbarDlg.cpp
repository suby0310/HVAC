// ToolbarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Holitech.h"
#include "ToolbarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolbarDlg dialog


CToolbarDlg::CToolbarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolbarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolbarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CToolbarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolbarDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolbarDlg, CDialog)
	//{{AFX_MSG_MAP(CToolbarDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolbarDlg message handlers
