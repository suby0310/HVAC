// HolitechDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Holitech.h"
#include "HolitechDlg.h"
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MCI Player
enum PlayFlag{
	FPLAY, FPAUSE, FRESUME,						//플레이 상태 (재생, 일시정지, 재개)
	FONSTOP, FOFFSTOP,							//정지 상태(정지버튼이 눌렸는지 여부)
	FNOREPECT, FONEREPECT, FALLREPECT,			//반복 재생 종류(반복없음, 한곡반복, 전체반복)
	FSEQ, FRAN									//랜덤재생 상태(랜덤재생 여부
};

int dwID;					//mci에서 사용

int g_playtime=0;			//재생 시간
int g_tend=0;				//현재 재생되는 노래의 총 재생시간을 저장
int g_stop=FOFFSTOP;		//스탑버튼이 눌렸는지의 유무
int g_repect=FNOREPECT;		//반복모드
int g_random=FSEQ;			//랜덤모드
int g_playbutt=FPLAY;		//재생버튼이 눌렸는지 여부

MCI_OPEN_PARMS mciOpen;		// Load files
MCI_PLAY_PARMS mciPlay;		// Play files
MCI_GENERIC_PARMS mciGeneric;	// Status of files


int dwID_hvac;					//mci에서 사용

int g_playtime_hvac=0;			//재생 시간
int g_tend_hvac=0;				//현재 재생되는 노래의 총 재생시간을 저장
int g_stop_hvac=FOFFSTOP;		//스탑버튼이 눌렸는지의 유무
int g_repect_hvac=FNOREPECT;	//반복모드
int g_playbutt_hvac=FPLAY;		//재생버튼이 눌렸는지 여부

MCI_OPEN_PARMS mciOpen_hvac;	// Load files
MCI_PLAY_PARMS mciPlay_hvac;	// Play files
MCI_GENERIC_PARMS mciGeneric_hvac;	// Status of files


HRESULT hr;

IMMDeviceEnumerator *deviceEnumerator ;
IMMDevice *defaultDevice;
IAudioEndpointVolume *endpointVolume;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CHolitechDlg dialog

CHolitechDlg::CHolitechDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHolitechDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHolitechDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pToolTip = NULL;

	//Inistialize state variables
	m_AlwaysOnTop = false;
	m_VolumeToggle = false;
	m_DownPoint = CPoint(-1, -1);
	m_KeyDown = 0x00;
	m_CurrentPreset.presetNum = 0;
	m_CurrentPreset.onPreset = false;
	m_Text = false;

	m_Status = 0;

	m_Frequency = 87.5;
	m_TrackNo = 1;

	m_AVNvolume = 0.1;
	m_AVNmode = CD;
	m_AVNmute = false;

	m_Tem = m_TemCo = 26;
	m_Ven = 0;
	m_HSDrv = 0;
	m_CSDrv = 0;
	m_HSCo = 0;
	m_CSCo = 0;
	m_BF = false;
	m_H = false;
	m_F = false;
	m_HF = false;
	m_FH = false;
	m_RH = false;
	m_HVACauto = false;
	m_HVACac = false;
	m_HVACmaxac = false;
	m_HVACair = false;
}

void CHolitechDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHolitechDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHolitechDlg, CDialog)
	//{{AFX_MSG_MAP(CHolitechDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_MESSAGE(WM_TRAY_ICON_NOTIFY_MESSAGE, OnTrayNotify)
	ON_BN_CLICKED(IDC_BUTTON_MINIMIZE, OnMinimize)
	ON_MESSAGE(WM_COMMANDHELP, OnCommandHelp)
	ON_BN_CLICKED(IDC_BUTTON_MODE, OnMode)
	ON_BN_CLICKED(IDC_BUTTON_PRESET1, OnPreset1)
	ON_BN_CLICKED(IDC_BUTTON_PRESET2, OnPreset2)
	ON_BN_CLICKED(IDC_BUTTON_PRESET3, OnPreset3)
	ON_BN_CLICKED(IDC_BUTTON_PRESET4, OnPreset4)
	ON_BN_CLICKED(IDC_BUTTON_PRESET5, OnPreset5)
	ON_BN_CLICKED(IDC_BUTTON_PRESET6, OnPreset6)
	ON_BN_CLICKED(IDC_BUTTON_PRE	, OnPre)
	ON_BN_CLICKED(IDC_BUTTON_PLAY	, OnPlay)
	ON_BN_CLICKED(IDC_BUTTON_NEXT	, OnNext)
	ON_BN_CLICKED(IDC_BUTTON_MUTE	, OnMute)
	ON_BN_CLICKED(IDC_BUTTON_VOL_DN	, OnVolDn)
	ON_BN_CLICKED(IDC_BUTTON_VOL_UP	, OnVolUp)

	ON_BN_CLICKED(IDC_BUTTON_TEM_DN		, OnTemDn)
	ON_BN_CLICKED(IDC_BUTTON_TEM_UP		, OnTemUp)
	ON_BN_CLICKED(IDC_BUTTON_HEATSEAT	, OnHeatSeat)
	ON_BN_CLICKED(IDC_BUTTON_HEATSEAT1	, OnHeatSeat)
	ON_BN_CLICKED(IDC_BUTTON_HEATSEAT2	, OnHeatSeat)
	ON_BN_CLICKED(IDC_BUTTON_HEATSEAT3	, OnHeatSeat)
	ON_BN_CLICKED(IDC_BUTTON_COOLSEAT	, OnCoolSeat)
	ON_BN_CLICKED(IDC_BUTTON_COOLSEAT1	, OnCoolSeat)
	ON_BN_CLICKED(IDC_BUTTON_COOLSEAT2	, OnCoolSeat)
	ON_BN_CLICKED(IDC_BUTTON_COOLSEAT3	, OnCoolSeat)
	ON_BN_CLICKED(IDC_BUTTON_BODYFOOT	, OnBodyFoot)
	ON_BN_CLICKED(IDC_BUTTON_BODYFOOT1	, OnBodyFoot)
	ON_BN_CLICKED(IDC_BUTTON_HEAD		, OnHead)
	ON_BN_CLICKED(IDC_BUTTON_HEAD1		, OnHead)
	ON_BN_CLICKED(IDC_BUTTON_FOOT		, OnFoot)
	ON_BN_CLICKED(IDC_BUTTON_FOOT1		, OnFoot)
	ON_BN_CLICKED(IDC_BUTTON_HEADFOOT	, OnHeadFoot)
	ON_BN_CLICKED(IDC_BUTTON_HEADFOOT1	, OnHeadFoot)
	ON_BN_CLICKED(IDC_BUTTON_VEN_DN		, OnVenDn)
	ON_BN_CLICKED(IDC_BUTTON_VEN_UP		, OnVenUp)
	ON_BN_CLICKED(IDC_BUTTON_TEM_DN_CO	, OnTemDnCo)
	ON_BN_CLICKED(IDC_BUTTON_TEM_UP_CO	, OnTemUpCo)
	ON_BN_CLICKED(IDC_BUTTON_HEATSEAT_CO, OnHeatSeatCo)
	ON_BN_CLICKED(IDC_BUTTON_HEATSEAT_CO1, OnHeatSeatCo)
	ON_BN_CLICKED(IDC_BUTTON_HEATSEAT_CO2, OnHeatSeatCo)
	ON_BN_CLICKED(IDC_BUTTON_HEATSEAT_CO3, OnHeatSeatCo)
	ON_BN_CLICKED(IDC_BUTTON_COOLSEAT_CO, OnCoolSeatCo)
	ON_BN_CLICKED(IDC_BUTTON_COOLSEAT_CO1, OnCoolSeatCo)
	ON_BN_CLICKED(IDC_BUTTON_COOLSEAT_CO2, OnCoolSeatCo)
	ON_BN_CLICKED(IDC_BUTTON_COOLSEAT_CO3, OnCoolSeatCo)
	ON_BN_CLICKED(IDC_BUTTON_FRONTHEAT	, OnFrontHeat)
	ON_BN_CLICKED(IDC_BUTTON_FRONTHEAT1	, OnFrontHeat)
	ON_BN_CLICKED(IDC_BUTTON_REARHEAT	, OnRearHeat)
	ON_BN_CLICKED(IDC_BUTTON_REARHEAT1	, OnRearHeat)
	ON_BN_CLICKED(IDC_BUTTON_AUTO		, OnAuto)
	ON_BN_CLICKED(IDC_BUTTON_AUTO1		, OnAuto)
	ON_BN_CLICKED(IDC_BUTTON_AC			, OnAC)
	ON_BN_CLICKED(IDC_BUTTON_AC1		, OnAC)
	ON_BN_CLICKED(IDC_BUTTON_MAX_AC		, OnMaxAC)
	ON_BN_CLICKED(IDC_BUTTON_MAX_AC1	, OnMaxAC)
	ON_BN_CLICKED(IDC_BUTTON_AIR		, OnAir)
	ON_BN_CLICKED(IDC_BUTTON_AIR1		, OnAir)

	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()

	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnCancel)
	ON_COMMAND(ID_PRESET1, OnPreset1)
	ON_COMMAND(ID_PRESET2, OnPreset2)
	ON_COMMAND(ID_PRESET3, OnPreset3)
	ON_COMMAND(ID_PRESET4, OnPreset4)
	ON_COMMAND(ID_PRESET5, OnPreset5)
	ON_COMMAND(ID_PRESET6, OnPreset6)
	ON_COMMAND(ID_EXIT, OnCancel)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_ERASEBKGND()

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHolitechDlg message handlers

BOOL CHolitechDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//Store the handle of this window in the App class, used to override keystrokes
	((CHolitechApp*)AfxGetApp())->m_hwndDialog = m_hWnd;

	//Set the current window text
	SetWindowText(TITLE);

	//Set up the tooltips
	m_pToolTip = new CToolTipCtrl;
	m_pToolTip->Create(this);
	m_pToolTip->SetDelayTime(TTDT_INITIAL, 350);
	m_pToolTip->SetDelayTime(TTDT_RESHOW, 0);
	m_pToolTip->EnableToolTips(TRUE);
	
	//Load the bitmaps of character sets and display icons
	LoadBitmaps();
#if 1
	SetWindowPos(this, 0, 0, m_bmpBackgroundImage.GetBitmapDimension().cx, m_bmpBackgroundImage.GetBitmapDimension().cy, SW_SHOWMAXIMIZED|SWP_SHOWWINDOW);
#else	// Support Sub-Monitor
	// Find monitor quantity
	if(::GetSystemMetrics(SM_CMONITORS) < 2)
	{
		//Center the window
		SetWindowPos(this, 0, 0, m_bmpBackgroundImage.GetBitmapDimension().cx, m_bmpBackgroundImage.GetBitmapDimension().cy, SW_SHOWMAXIMIZED/*SWP_SHOWWINDOW*/);
	}
	else
	{
		// Display dialog on sub monitor position
		HMONITOR	hMonitorThis, hMonitorTarget;
		MONITORINFO oMonitorThis, oMonitorTarget;
		POINT oPoint;

		// Find monitor position of current window
		hMonitorThis = ::MonitorFromWindow(this->GetSafeHwnd(), MONITOR_DEFAULTTONEAREST);

		oMonitorThis.cbSize = sizeof(MONITORINFO);
		::GetMonitorInfo(hMonitorThis, &oMonitorThis);

		// Find monitor position of target window
		if(oMonitorThis.rcMonitor.left != 0)
		{
			// There is current window on second monitor. Therefore, target window is main monitor(0, 0)
			oPoint.x = 0;
			oPoint.y = 0;
			hMonitorTarget = ::MonitorFromPoint(oPoint, MONITOR_DEFAULTTONEAREST);
			oMonitorTarget.cbSize = sizeof(MONITORINFO);
			::GetMonitorInfo(hMonitorTarget, &oMonitorTarget);
		}
		else
		{
			// There is current window on Main monitor.
			// It needs to find where second monitor is.
			oPoint.x = -1;
			oPoint.y = 0;
			hMonitorTarget = ::MonitorFromPoint(oPoint, MONITOR_DEFAULTTONULL);
			if(hMonitorTarget <= 0)
			{
				oPoint.x = oMonitorThis.rcMonitor.right+1;
				oPoint.y = 0;
				hMonitorTarget = ::MonitorFromPoint(oPoint, MONITOR_DEFAULTTONULL);
			}
			oMonitorTarget.cbSize = sizeof(MONITORINFO);
			::GetMonitorInfo(hMonitorTarget, &oMonitorTarget);
		}

		MoveWindow(oPoint.x+700, oPoint.y+100, m_bmpBackgroundImage.GetBitmapDimension().cx, m_bmpBackgroundImage.GetBitmapDimension().cy);
	}
#endif
	//Display all the bitmap butttons
	DisplayButtons();

	//Initialize the tray data structure
	m_TrayData.cbSize = sizeof(NOTIFYICONDATA);
	m_TrayData.hWnd = this->m_hWnd;
	m_TrayData.uID = 1;
	m_TrayData.uCallbackMessage = WM_TRAY_ICON_NOTIFY_MESSAGE;
	m_TrayData.hIcon = this->m_hIcon;
	m_TrayData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	strcpy(m_TrayData.szTip, TITLE);

	//Initialize the tray menu
	m_TrayMenu.LoadMenu(IDR_TRAYMENU);

	//Add to the tray
	Shell_NotifyIcon(NIM_ADD, &m_TrayData);

	MediaClose();
	MediaClose_HVAC();

	//Start the poll to find a device
//	SetTimer(TIMER_FIND_DEVICE, 1, NULL);
	SetTimer(1,1000,NULL);
			
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHolitechDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();

		DrawBackground();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHolitechDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CHolitechDlg::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	//When the tray is clicked do something
	if (wParam == 1)
	{
		CPoint pt;
		GetCursorPos(&pt);

		//On Double click, show the radio, and on single click, show the menu
		switch (lParam) 
		{ 
		case WM_LBUTTONDBLCLK	:	break;
		case WM_RBUTTONDOWN		:
		case WM_CONTEXTMENU		:	break;
		default					:	break;
		}
	}
}
  
void CHolitechDlg::ShowTrayMenu(CPoint pt)
{  
	//Popup the tray menu
	SetForegroundWindow();
	m_TrayMenu.GetSubMenu(0)->TrackPopupMenu(TPM_BOTTOMALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pt.x, pt.y, this);    
}

void CHolitechDlg::DrawBackground()
{
	//Get the current DC and make a memory DC compatible containing the background image
	CDC *pDC = GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CClientDC dc(this);

	//Draw the background image
	DrawBitmapOnDC(&memDC, &dc, &m_bmpBackgroundImage, 0, 0);

	DrawPannel(&memDC, &dc);
	DrawMode(&memDC, &dc);
	DrawMute(&memDC, &dc);
	DrawVolume(&memDC, &dc);
	DrawTemperature(&memDC, &dc);
	DrawTemperatureCo(&memDC, &dc);
	DrawLampVentilation(&memDC, &dc);

/*
	if(!m_Text)
	{
		DrawText(&memDC, &dc, "HOLITECH\0");

		//m_Text = true;
	}

	//If we are not muting, show the volume slide
	if (!m_Muting)
		//DrawVolumeSlide(&memDC, &dc);
*/
	ReleaseDC(pDC);
}

void CHolitechDlg::DrawPannel(CDC* memDC, CClientDC* dc)
{
	int i = 0;
	int x = 330;
	int y = 130;
	int max_x = x+1500;
	CString text;

	CBitmap* pBmp = NULL;	
	bool filledDisplay = false;

	switch(m_AVNmode)
	{
		case RADIO:
			x = 600;
			text.Format("%3.1f MHz", m_Frequency);
			break;
		case CD:
			text.Format("TRACK  NO.%1d", m_TrackNo);
			break;
		default:
			break;
	};

#if 1
	for (/*i assinged above*/; (i < text.GetLength()) ; i++)
	{
		pBmp = GetAlphanumericLargeBitmap(text[i]);

		if (pBmp) 
		{
			if ((x + pBmp->GetBitmapDimension().cx) < max_x)
			{
				DrawBitmapOnDC(memDC, dc, pBmp, x, y);
				x = x + pBmp->GetBitmapDimension().cx;
			}
			else
			{
				filledDisplay = true;
				i = text.GetLength();
			}
		}
	}
#else
	//Draw the station on the GUI from the x starting point
	for (int i = text.GetLength() - 1; i >= 0; i--)
	{
		CBitmap* pBmp = GetAlphanumericLargeBitmap(text[i]);

		if (pBmp)
		{
			x = x - pBmp->GetBitmapDimension().cx;
			DrawBitmapOnDC(memDC, dc, pBmp, x, y);
		}
	}
#endif
}

void CHolitechDlg::DrawMode(CDC* memDC, CClientDC* dc)
{
	int i = 0;
	int x = 130, y = 35;
	int max_x = x+400;
	CString text;
	
	CBitmap* pBmp = NULL;	
	bool filledDisplay = false;

	switch(m_AVNmode)
	{
		case RADIO:
			text = "RADIO";
			break;
		case CD:
			text = "CD";
			break;
		default :
			text = "";
			break;
	}

	for (/*i assinged above*/; (i < text.GetLength()) ; i++)
	{
		pBmp = GetAlphanumericSmallBitmap(text[i]);

		if (pBmp) 
		{
			if ((x + pBmp->GetBitmapDimension().cx) < max_x)
			{
				DrawBitmapOnDC(memDC, dc, pBmp, x, y);
				x = x + pBmp->GetBitmapDimension().cx;
			}
			else
			{
				filledDisplay = true;
				i = text.GetLength();
			}
		}
	}
}

void CHolitechDlg::DrawMute(CDC* memDC, CClientDC* dc)
{
	int i = 0;
	int x = 850, y = 35;
	int max_x = x+400;
	CString text;
	
	CBitmap* pBmp = NULL;	
	bool filledDisplay = false;

	if(m_AVNmute) text = "MUTE";
	else text = "";

	for (/*i assinged above*/; (i < text.GetLength()) ; i++)
	{
		pBmp = GetAlphanumericSmallBitmap(text[i]);

		if (pBmp) 
		{
			if ((x + pBmp->GetBitmapDimension().cx) < max_x)
			{
				DrawBitmapOnDC(memDC, dc, pBmp, x, y);
				x = x + pBmp->GetBitmapDimension().cx;
			}
			else
			{
				filledDisplay = true;
				i = text.GetLength();
			}
		}
	}
}

void CHolitechDlg::DrawVolume(CDC* memDC, CClientDC* dc)
{
	int i = 0;
	int x = 1450, y = 35;
	int max_x = x+600;
	CString text;
	
	CBitmap* pBmp = NULL;	
	bool filledDisplay = false;

	text.Format("VOL.%3u", (BYTE)(m_AVNvolume*100));

	for (/*i assinged above*/; (i < text.GetLength()) ; i++)
	{
		pBmp = GetAlphanumericSmallBitmap(text[i]);

		if (pBmp) 
		{
			if ((x + pBmp->GetBitmapDimension().cx) < max_x)
			{
				DrawBitmapOnDC(memDC, dc, pBmp, x, y);
				x = x + pBmp->GetBitmapDimension().cx;
			}
			else
			{
				filledDisplay = true;
				i = text.GetLength();
			}
		}
	}
}

void CHolitechDlg::DrawTemperature(CDC* memDC, CClientDC* dc)
{
	int i = 0;
	int x = 235, y = 660;
	int max_x = x+200;
	CString text;
	
	CBitmap* pBmp = NULL;	
	bool filledDisplay = false;

	text.Format("%2u", m_Tem);

	for (/*i assinged above*/; (i < text.GetLength()) ; i++)
	{
		pBmp = GetAlphanumericSmallBitmap(text[i]);

		if (pBmp) 
		{
			if ((x + pBmp->GetBitmapDimension().cx) < max_x)
			{
				DrawBitmapOnDC(memDC, dc, pBmp, x, y);
				x = x + pBmp->GetBitmapDimension().cx;
			}
			else
			{
				filledDisplay = true;
				i = text.GetLength();
			}
		}
	}
}

void CHolitechDlg::DrawTemperatureCo(CDC* memDC, CClientDC* dc)
{
	int i = 0;
	int x = 1558, y = 660;
	int max_x = x+200;
	CString text;
	
	CBitmap* pBmp = NULL;	
	bool filledDisplay = false;

	text.Format("%2u", m_TemCo);

	for (/*i assinged above*/; (i < text.GetLength()) ; i++)
	{
		pBmp = GetAlphanumericSmallBitmap(text[i]);

		if (pBmp) 
		{
			if ((x + pBmp->GetBitmapDimension().cx) < max_x)
			{
				DrawBitmapOnDC(memDC, dc, pBmp, x, y);
				x = x + pBmp->GetBitmapDimension().cx;
			}
			else
			{
				filledDisplay = true;
				i = text.GetLength();
			}
		}
	}
}

void CHolitechDlg::DrawLampVentilation(CDC* memDC, CClientDC* dc)
{
	int x1 = 978;
	int x2 = 1037;
	int x3 = 1096;
	int x4 = 1133;
	int x5 = 1214;
	int x6 = 1273;
	int x7 = 1332;
	int y = 647;
	int y1 = 645;

	switch(m_Ven)
	{
		case 0:
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x1,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x2,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x3,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenFanOff,	x4,	y1);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x5,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x6,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x7,	y);
			break;
		case 1:
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x1,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x2,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x3,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenFanOff,	x4,	y1);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x5,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x6,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x7,	y);
			break;
		case 2:
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x1,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x2,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x3,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenFanOff,	x4,	y1);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x5,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x6,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x7,	y);
			break;
		case 3:
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x1,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x2,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x3,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenFanOff,	x4,	y1);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x5,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x6,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x7,	y);

			break;
		case 4:
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x1,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x2,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x3,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenFanOn,	x4,	y1);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x5,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x6,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x7,	y);

			break;
		case 5:
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x1,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x2,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x3,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenFanOn,	x4,	y1);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x5,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x6,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x7,	y);
			break;
		case 6:
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x1,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x2,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x3,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenFanOn,	x4,	y1);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x5,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x6,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,		x7,	y);
			break;
		case 7:
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x1,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x2,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x3,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenFanOn,	x4,	y1);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x5,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x6,	y);
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn,		x7,	y);
			break;
		default:
			break;
	}
}

void CHolitechDlg::DrawBitmapOnDC(CDC* memDC, CClientDC* cDC, CBitmap* bmp, int x, int y)
{
	//Select the bitmap, and draw it at point x and y with it's current size
	memDC->SelectObject(*bmp);
	cDC->BitBlt(x, y, bmp->GetBitmapDimension().cx, bmp->GetBitmapDimension().cy, memDC, 0, 0, SRCCOPY);
/*
	SelectObject(MemDC, bit);
	DeleteObject(bit);
	DeleteDC(MemDC);
*/
}

void CHolitechDlg::DrawBitmapOnDC(CDC* memDC, CClientDC* cDC, CBitmap* bmp, int x, int y, UINT transparentColor)
{
	//Select the bitmap, and draw it at point x and y with it's current size
	memDC->SelectObject(*bmp);
	TransparentBlt(cDC->m_hDC, x, y, bmp->GetBitmapDimension().cx, bmp->GetBitmapDimension().cy, memDC->m_hDC, 0, 0, bmp->GetBitmapDimension().cx, bmp->GetBitmapDimension().cy, transparentColor);
}

void CHolitechDlg::UpdatePannel()
{
	CDC *pDC = GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CClientDC dc(this);

	InvalidateRect(CRect(CPoint(600, 130), CPoint(1600, 270)), FALSE);
	UpdateWindow();

	ReleaseDC(pDC);
}

void CHolitechDlg::UpdateMode()
{
	CDC *pDC = GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CClientDC dc(this);

	InvalidateRect(CRect(CPoint(130, 40), CPoint(400, 140)), FALSE);
	UpdateWindow();

	ReleaseDC(pDC);
}

void CHolitechDlg::UpdateMute()
{
	CDC *pDC = GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CClientDC dc(this);

	InvalidateRect(CRect(CPoint(850, 40), CPoint(1200, 140)), FALSE);
	UpdateWindow();

	ReleaseDC(pDC);
}

void CHolitechDlg::UpdateVolume()
{
	CDC *pDC = GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CClientDC dc(this);

	InvalidateRect(CRect(CPoint(1450, 40), CPoint(1900, 140)), FALSE);
	UpdateWindow();

	ReleaseDC(pDC);
}
/*
void CHolitechDlg::UpdateTemperature()
{
	CDC *pDC = GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CClientDC dc(this);

	InvalidateRect(CRect(CPoint(1450, 40), CPoint(1900, 140)), FALSE);
	UpdateWindow();

	ReleaseDC(pDC);
}

void CHolitechDlg::UpdateTemperatureCo()
{
	CDC *pDC = GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CClientDC dc(this);

	InvalidateRect(CRect(CPoint(1450, 40), CPoint(1900, 140)), FALSE);
	UpdateWindow();

	ReleaseDC(pDC);
}

void CHolitechDlg::UpdateLampVentilation()
{
	CDC *pDC = GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CClientDC dc(this);

	InvalidateRect(CRect(CPoint(1450, 40), CPoint(1900, 140)), FALSE);
	UpdateWindow();

	ReleaseDC(pDC);
}
*/
CBitmap* CHolitechDlg::GetAlphanumericLargeBitmap(char c)
{
	CBitmap* pBmp = NULL;

	//switch (toupper(c)) 소문자를 대문자로 변환
	switch (c)
	{
	case '0'	:	pBmp = &m_bmp0Large;	break;
	case '1'	:	pBmp = &m_bmp1Large;	break;
	case '2'	:	pBmp = &m_bmp2Large;	break;
	case '3'	:	pBmp = &m_bmp3Large;	break;
	case '4'	:	pBmp = &m_bmp4Large;	break;
	case '5'	:	pBmp = &m_bmp5Large;	break;
	case '6'	:	pBmp = &m_bmp6Large;	break;
	case '7'	:	pBmp = &m_bmp7Large;	break;
	case '8'	:	pBmp = &m_bmp8Large;	break;
	case '9'	:	pBmp = &m_bmp9Large;	break;
	case 'A'	:	pBmp = &m_bmpALarge;	break;
	case 'B'	:	pBmp = &m_bmpBLarge;	break;
	case 'C'	:	pBmp = &m_bmpCLarge;	break;
	case 'D'	:	pBmp = &m_bmpDLarge;	break;
	case 'E'	:	pBmp = &m_bmpELarge;	break;
	case 'F'	:	pBmp = &m_bmpFLarge;	break;
	case 'G'	:	pBmp = &m_bmpGLarge;	break;
	case 'H'	:	pBmp = &m_bmpHLarge;	break;
	case 'I'	:	pBmp = &m_bmpILarge;	break;
	case 'J'	:	pBmp = &m_bmpJLarge;	break;
	case 'K'	:	pBmp = &m_bmpKLarge;	break;
	case 'L'	:	pBmp = &m_bmpLLarge;	break;
	case 'M'	:	pBmp = &m_bmpMLarge;	break;
	case 'N'	:	pBmp = &m_bmpNLarge;	break;
	case 'O'	:	pBmp = &m_bmpOLarge;	break;
	case 'P'	:	pBmp = &m_bmpPLarge;	break;
	case 'Q'	:	pBmp = &m_bmpQLarge;	break;
	case 'R'	:	pBmp = &m_bmpRLarge;	break;
	case 'S'	:	pBmp = &m_bmpSLarge;	break;
	case 'T'	:	pBmp = &m_bmpTLarge;	break;
	case 'U'	:	pBmp = &m_bmpULarge;	break;
	case 'V'	:	pBmp = &m_bmpVLarge;	break;
	case 'W'	:	pBmp = &m_bmpWLarge;	break;
	case 'X'	:	pBmp = &m_bmpXLarge;	break;
	case 'Y'	:	pBmp = &m_bmpYLarge;	break;
	case 'Z'	:	pBmp = &m_bmpZLarge;	break;
	case '.'	:	pBmp = &m_bmpDotLarge;	break;

	case 'z'	:	pBmp = &m_bmpZSpecial;	break;
	default		:	pBmp = &m_bmpSpaceLarge;	break;
	}

	return pBmp;
}

CBitmap* CHolitechDlg::GetAlphanumericSmallBitmap(char c)
{
	CBitmap* pBmp = NULL;

	switch (toupper(c))
	{
	case '0'	:	pBmp = &m_bmp0Small;	break;
	case '1'	:	pBmp = &m_bmp1Small;	break;
	case '2'	:	pBmp = &m_bmp2Small;	break;
	case '3'	:	pBmp = &m_bmp3Small;	break;
	case '4'	:	pBmp = &m_bmp4Small;	break;
	case '5'	:	pBmp = &m_bmp5Small;	break;
	case '6'	:	pBmp = &m_bmp6Small;	break;
	case '7'	:	pBmp = &m_bmp7Small;	break;
	case '8'	:	pBmp = &m_bmp8Small;	break;
	case '9'	:	pBmp = &m_bmp9Small;	break;
	case 'A'	:	pBmp = &m_bmpASmall;	break;
	case 'B'	:	pBmp = &m_bmpBSmall;	break;
	case 'C'	:	pBmp = &m_bmpCSmall;	break;
	case 'D'	:	pBmp = &m_bmpDSmall;	break;
	case 'E'	:	pBmp = &m_bmpESmall;	break;
	case 'F'	:	pBmp = &m_bmpFSmall;	break;
	case 'G'	:	pBmp = &m_bmpGSmall;	break;
	case 'H'	:	pBmp = &m_bmpHSmall;	break;
	case 'I'	:	pBmp = &m_bmpISmall;	break;
	case 'J'	:	pBmp = &m_bmpJSmall;	break;
	case 'K'	:	pBmp = &m_bmpKSmall;	break;
	case 'L'	:	pBmp = &m_bmpLSmall;	break;
	case 'M'	:	pBmp = &m_bmpMSmall;	break;
	case 'N'	:	pBmp = &m_bmpNSmall;	break;
	case 'O'	:	pBmp = &m_bmpOSmall;	break;
	case 'P'	:	pBmp = &m_bmpPSmall;	break;
	case 'Q'	:	pBmp = &m_bmpQSmall;	break;
	case 'R'	:	pBmp = &m_bmpRSmall;	break;
	case 'S'	:	pBmp = &m_bmpSSmall;	break;
	case 'T'	:	pBmp = &m_bmpTSmall;	break;
	case 'U'	:	pBmp = &m_bmpUSmall;	break;
	case 'V'	:	pBmp = &m_bmpVSmall;	break;
	case 'W'	:	pBmp = &m_bmpWSmall;	break;
	case 'X'	:	pBmp = &m_bmpXSmall;	break;
	case 'Y'	:	pBmp = &m_bmpYSmall;	break;
	case 'Z'	:	pBmp = &m_bmpZSmall;	break;
	case '.'	:	pBmp = &m_bmpDotSmall;	break;
	case '-'	:	pBmp = &m_bmpDashSmall;	break;
	case ','	:	pBmp = &m_bmpCommaSmall;		break;
	case '\''	:	pBmp = &m_bmpApostropheSmall;	break;
	case '!'	:	pBmp = &m_bmpExclamationSmall;	break;
	case '&'	:	pBmp = &m_bmpAmpersandSmall;	break;
	case ' '	:	pBmp = &m_bmpSpaceSmall;		break;
	default		:	pBmp = &m_bmpSpaceSmall;		break;
	}

	return pBmp;
}

void CHolitechDlg::DisplayButtons()
{

#define	ROW1 300
#define ROW2 441
#define ROW3 615
#define ROW4 766
#define ROW5 930

	CreateBitmapButton(&m_Mode,			181,	ROW1,	IDB_BITMAP_MODE,		IDB_BITMAP_MODE,		IDC_BUTTON_MODE,		"");
	CreateBitmapButton(&m_Preset1,		398,	ROW1,	IDB_BITMAP_P1,			IDB_BITMAP_P1,			IDC_BUTTON_PRESET1,		"");
	CreateBitmapButton(&m_Preset2,		610,	ROW1,	IDB_BITMAP_P2,			IDB_BITMAP_P2,			IDC_BUTTON_PRESET2,		"");
	CreateBitmapButton(&m_Preset3,		825,	ROW1,	IDB_BITMAP_P3,			IDB_BITMAP_P3,			IDC_BUTTON_PRESET3,		"");
	CreateBitmapButton(&m_Preset4,		1040,	ROW1,	IDB_BITMAP_P4,			IDB_BITMAP_P4,			IDC_BUTTON_PRESET4,		"");
	CreateBitmapButton(&m_Preset5,		1260,	ROW1,	IDB_BITMAP_P5,			IDB_BITMAP_P5,			IDC_BUTTON_PRESET5,		"");
	CreateBitmapButton(&m_Preset6,		1480,	ROW1,	IDB_BITMAP_P6,			IDB_BITMAP_P6,			IDC_BUTTON_PRESET6,		"");
	CreateBitmapButton(&m_Pre,			399,	ROW2,	IDB_BITMAP_PRE,			IDB_BITMAP_PRE,			IDC_BUTTON_PRE,			"");
	CreateBitmapButton(&m_Play,			610,	ROW2,	IDB_BITMAP_PLAY,		IDB_BITMAP_PLAY,		IDC_BUTTON_PLAY,		"");
	CreateBitmapButton(&m_Next,			825,	ROW2,	IDB_BITMAP_NEXT,		IDB_BITMAP_NEXT,		IDC_BUTTON_NEXT,		"");
	CreateBitmapButton(&m_Mute,			1040,	ROW2,	IDB_BITMAP_MUTE,		IDB_BITMAP_MUTE,		IDC_BUTTON_MUTE,		"");
	CreateBitmapButton(&m_VolDn,		1260,	ROW2,	IDB_BITMAP_VOL_DN,		IDB_BITMAP_VOL_DN,		IDC_BUTTON_VOL_DN,		"");
	CreateBitmapButton(&m_VolUp,		1480,	ROW2,	IDB_BITMAP_VOL_UP,		IDB_BITMAP_VOL_UP,		IDC_BUTTON_VOL_UP,		"");
	CreateBitmapButton(&m_TemDn,		74,		ROW3,	IDB_BITMAP_TEM_DN,		IDB_BITMAP_TEM_DN,		IDC_BUTTON_TEM_DN,		"");
	CreateBitmapButton(&m_TemUp,		373,	ROW3,	IDB_BITMAP_TEM_UP,		IDB_BITMAP_TEM_UP,		IDC_BUTTON_TEM_UP,		"");
	CreateBitmapButton(&m_HeatSeat,		75,		ROW4,	IDB_BITMAP_HEATSEAT,	IDB_BITMAP_HEATSEAT,	IDC_BUTTON_HEATSEAT,	"");
	CreateBitmapButton(&m_HeatSeat1,	75,		ROW4,	IDB_BITMAP_HEATSEAT1,	IDB_BITMAP_HEATSEAT1,	IDC_BUTTON_HEATSEAT1,	"");
	CreateBitmapButton(&m_HeatSeat2,	75,		ROW4,	IDB_BITMAP_HEATSEAT2,	IDB_BITMAP_HEATSEAT2,	IDC_BUTTON_HEATSEAT2,	"");
	CreateBitmapButton(&m_HeatSeat3,	75,		ROW4,	IDB_BITMAP_HEATSEAT3,	IDB_BITMAP_HEATSEAT3,	IDC_BUTTON_HEATSEAT3,	"");
	CreateBitmapButton(&m_CoolSeat,		279,	ROW4,	IDB_BITMAP_COOLSEAT,	IDB_BITMAP_COOLSEAT,	IDC_BUTTON_COOLSEAT,	"");
	CreateBitmapButton(&m_CoolSeat1,	279,	ROW4,	IDB_BITMAP_COOLSEAT1,	IDB_BITMAP_COOLSEAT1,	IDC_BUTTON_COOLSEAT1,	"");
	CreateBitmapButton(&m_CoolSeat2,	279,	ROW4,	IDB_BITMAP_COOLSEAT2,	IDB_BITMAP_COOLSEAT2,	IDC_BUTTON_COOLSEAT2,	"");
	CreateBitmapButton(&m_CoolSeat3,	279,	ROW4,	IDB_BITMAP_COOLSEAT3,	IDB_BITMAP_COOLSEAT3,	IDC_BUTTON_COOLSEAT3,	"");
	CreateBitmapButton(&m_BodyFoot,		517,	ROW3,	IDB_BITMAP_BODYFOOT,	IDB_BITMAP_BODYFOOT,	IDC_BUTTON_BODYFOOT,	"");
	CreateBitmapButton(&m_BodyFoot1,	517,	ROW3,	IDB_BITMAP_BODYFOOT1,	IDB_BITMAP_BODYFOOT1,	IDC_BUTTON_BODYFOOT1,	"");
	CreateBitmapButton(&m_Head,			720,	ROW3,	IDB_BITMAP_HEAD,		IDB_BITMAP_HEAD,		IDC_BUTTON_HEAD,		"");
	CreateBitmapButton(&m_Head1,		720,	ROW3,	IDB_BITMAP_HEAD1,		IDB_BITMAP_HEAD1,		IDC_BUTTON_HEAD1,		"");
	CreateBitmapButton(&m_Foot,			517,	ROW4,	IDB_BITMAP_FOOT,		IDB_BITMAP_FOOT,		IDC_BUTTON_FOOT,		"");
	CreateBitmapButton(&m_Foot1,		517,	ROW4,	IDB_BITMAP_FOOT1,		IDB_BITMAP_FOOT1,		IDC_BUTTON_FOOT1,		"");
	CreateBitmapButton(&m_HeadFoot,		720,	ROW4,	IDB_BITMAP_HEADFOOT,	IDB_BITMAP_HEADFOOT,	IDC_BUTTON_HEADFOOT,	"");
	CreateBitmapButton(&m_HeadFoot1,	720,	ROW4,	IDB_BITMAP_HEADFOOT1,	IDB_BITMAP_HEADFOOT1,	IDC_BUTTON_HEADFOOT1,	"");
	CreateBitmapButton(&m_VenDn,		965,	ROW4,	IDB_BITMAP_VEN_DN,		IDB_BITMAP_VEN_DN,		IDC_BUTTON_VEN_DN,		"");
	CreateBitmapButton(&m_VenUo,		1170,	ROW4,	IDB_BITMAP_VEN_UP,		IDB_BITMAP_VEN_UP,		IDC_BUTTON_VEN_UP,		"");
	CreateBitmapButton(&m_TemDn_Co,		1400,	ROW3,	IDB_BITMAP_TEM_DN,		IDB_BITMAP_TEM_DN,		IDC_BUTTON_TEM_DN_CO,	"");
	CreateBitmapButton(&m_TemUp_Co,		1700,	ROW3,	IDB_BITMAP_TEM_UP,		IDB_BITMAP_TEM_UP,		IDC_BUTTON_TEM_UP_CO,	"");
	CreateBitmapButton(&m_HeatSeat_Co,	1399,	ROW4,	IDB_BITMAP_HEATSEAT,	IDB_BITMAP_HEATSEAT,	IDC_BUTTON_HEATSEAT_CO,	"");
	CreateBitmapButton(&m_HeatSeat_Co1,	1399,	ROW4,	IDB_BITMAP_HEATSEAT1,	IDB_BITMAP_HEATSEAT1,	IDC_BUTTON_HEATSEAT_CO1,"");
	CreateBitmapButton(&m_HeatSeat_Co2,	1399,	ROW4,	IDB_BITMAP_HEATSEAT2,	IDB_BITMAP_HEATSEAT2,	IDC_BUTTON_HEATSEAT_CO2,"");
	CreateBitmapButton(&m_HeatSeat_Co3,	1399,	ROW4,	IDB_BITMAP_HEATSEAT3,	IDB_BITMAP_HEATSEAT3,	IDC_BUTTON_HEATSEAT_CO3,"");
	CreateBitmapButton(&m_CoolSeat_Co,	1605,	ROW4,	IDB_BITMAP_COOLSEAT,	IDB_BITMAP_COOLSEAT,	IDC_BUTTON_COOLSEAT_CO,	"");
	CreateBitmapButton(&m_CoolSeat_Co1,	1605,	ROW4,	IDB_BITMAP_COOLSEAT1,	IDB_BITMAP_COOLSEAT1,	IDC_BUTTON_COOLSEAT_CO1,"");
	CreateBitmapButton(&m_CoolSeat_Co2,	1605,	ROW4,	IDB_BITMAP_COOLSEAT2,	IDB_BITMAP_COOLSEAT2,	IDC_BUTTON_COOLSEAT_CO2,"");
	CreateBitmapButton(&m_CoolSeat_Co3,	1605,	ROW4,	IDB_BITMAP_COOLSEAT3,	IDB_BITMAP_COOLSEAT3,	IDC_BUTTON_COOLSEAT_CO3,"");
	CreateBitmapButton(&m_FrontHeat,	290,	ROW5,	IDB_BITMAP_FRONTHEAT,	IDB_BITMAP_FRONTHEAT,	IDC_BUTTON_FRONTHEAT,	"");
	CreateBitmapButton(&m_FrontHeat1,	290,	ROW5,	IDB_BITMAP_FRONTHEAT1,	IDB_BITMAP_FRONTHEAT1,	IDC_BUTTON_FRONTHEAT1,	"");
	CreateBitmapButton(&m_RearHeat,		505,	ROW5,	IDB_BITMAP_REARHEAT,	IDB_BITMAP_REARHEAT,	IDC_BUTTON_REARHEAT,	"");
	CreateBitmapButton(&m_RearHeat1,	505,	ROW5,	IDB_BITMAP_REARHEAT1,	IDB_BITMAP_REARHEAT1,	IDC_BUTTON_REARHEAT1,	"");
	CreateBitmapButton(&m_Auto,			725,	ROW5,	IDB_BITMAP_AUTO,		IDB_BITMAP_AUTO,		IDC_BUTTON_AUTO,		"");
	CreateBitmapButton(&m_Auto1,		725,	ROW5,	IDB_BITMAP_AUTO1,		IDB_BITMAP_AUTO1,		IDC_BUTTON_AUTO1,		"");
	CreateBitmapButton(&m_AC,			940,	ROW5,	IDB_BITMAP_AC,			IDB_BITMAP_AC,			IDC_BUTTON_AC,			"");
	CreateBitmapButton(&m_AC1,			940,	ROW5,	IDB_BITMAP_AC1,			IDB_BITMAP_AC1,			IDC_BUTTON_AC1,			"");
	CreateBitmapButton(&m_MaxAC,		1160,	ROW5,	IDB_BITMAP_MAX_AC,		IDB_BITMAP_MAX_AC,		IDC_BUTTON_MAX_AC,		"");
	CreateBitmapButton(&m_MaxAC1,		1160,	ROW5,	IDB_BITMAP_MAX_AC1,		IDB_BITMAP_MAX_AC1,		IDC_BUTTON_MAX_AC1,		"");
	CreateBitmapButton(&m_Air,			1375,	ROW5,	IDB_BITMAP_AIR,			IDB_BITMAP_AIR,			IDC_BUTTON_AIR,			"");
	CreateBitmapButton(&m_Air1,			1375,	ROW5,	IDB_BITMAP_AIR1,		IDB_BITMAP_AIR1,		IDC_BUTTON_AIR1,		"");

	GetDlgItem(IDC_BUTTON_HEATSEAT)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_HEATSEAT1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_HEATSEAT2)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_HEATSEAT3)->ShowWindow(FALSE);

	GetDlgItem(IDC_BUTTON_COOLSEAT)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_COOLSEAT1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_COOLSEAT2)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_COOLSEAT3)->ShowWindow(FALSE);

	GetDlgItem(IDC_BUTTON_HEATSEAT_CO)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_HEATSEAT_CO1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_HEATSEAT_CO2)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_HEATSEAT_CO3)->ShowWindow(FALSE);

	GetDlgItem(IDC_BUTTON_COOLSEAT_CO)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_COOLSEAT_CO1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_COOLSEAT_CO2)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_COOLSEAT_CO3)->ShowWindow(FALSE);

	GetDlgItem(IDC_BUTTON_BODYFOOT)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_HEAD)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_FOOT)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_HEADFOOT)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_BODYFOOT1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_HEAD1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FOOT1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_HEADFOOT1)->ShowWindow(FALSE);

	GetDlgItem(IDC_BUTTON_FRONTHEAT)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_FRONTHEAT1)->ShowWindow(FALSE);

	GetDlgItem(IDC_BUTTON_REARHEAT)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_REARHEAT1)->ShowWindow(FALSE);

	GetDlgItem(IDC_BUTTON_AUTO)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_AUTO1)->ShowWindow(FALSE);

	GetDlgItem(IDC_BUTTON_AC)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_AC1)->ShowWindow(FALSE);

	GetDlgItem(IDC_BUTTON_MAX_AC)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_MAX_AC1)->ShowWindow(FALSE);

	GetDlgItem(IDC_BUTTON_AIR)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_AIR1)->ShowWindow(FALSE);

/*
#define PRE_X	117
#define PRE_Y	172
#define PRE_ITV	150

#define CTL_Y	777

	int x;

	//Create each button for the GUI
	CreateBitmapButton(&m_Minimize, 2045, 24, IDB_BITMAP_MINIMIZE_UP, IDB_BITMAP_MINIMIZE_DOWN, IDC_BUTTON_MINIMIZE, "Minimize");
	CreateBitmapButton(&m_Restore, 2105, 24, IDB_BITMAP_RESTORE_UP, IDB_BITMAP_RESTORE_DOWN, IDC_BUTTON_RESTORE, "Window Shade Toggle");
	CreateBitmapButton(&m_Close, 2165, 24, IDB_BITMAP_CLOSE_UP, IDB_BITMAP_CLOSE_DOWN, IDC_BUTTON_CLOSE, "Close");

	CreateBitmapButton(&m_Preset1,				x=x+PRE_ITV,	PRE_Y, IDB_BITMAP_PRESET1_UP, IDB_BITMAP_PRESET1_DOWN, IDC_BUTTON_PRESET1, "1");
	CreateBitmapButton(&m_Preset2,				x=x+PRE_ITV-4,	PRE_Y, IDB_BITMAP_PRESET2_UP, IDB_BITMAP_PRESET2_DOWN, IDC_BUTTON_PRESET2, "2");
	CreateBitmapButton(&m_Preset3,				x=x+PRE_ITV,	PRE_Y, IDB_BITMAP_PRESET3_UP, IDB_BITMAP_PRESET3_DOWN, IDC_BUTTON_PRESET3, "3");
	CreateBitmapButton(&m_Preset4,				x=x+PRE_ITV-4,	PRE_Y, IDB_BITMAP_PRESET4_UP, IDB_BITMAP_PRESET4_DOWN, IDC_BUTTON_PRESET4, "4");
	CreateBitmapButton(&m_Preset5,				x=x+PRE_ITV,	PRE_Y, IDB_BITMAP_PRESET5_UP, IDB_BITMAP_PRESET5_DOWN, IDC_BUTTON_PRESET5, "5");
	CreateBitmapButton(&m_Preset6,				x=x+PRE_ITV-4,	PRE_Y, IDB_BITMAP_PRESET6_UP, IDB_BITMAP_PRESET6_DOWN, IDC_BUTTON_PRESET6, "6");

	CreateBitmapButton(&m_Mute,				1310,		CTL_Y+24,		IDB_BITMAP_MUTE_UP, IDB_BITMAP_MUTE_DOWN, IDC_BUTTON_MUTE, "Mute");


	GetDlgItem(IDC_BUTTON_PRESETSCANUP_DOWN)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PRESETSCANDOWN_DOWN)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SCANUP_DOWN)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SCANDOWN_DOWN)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MUTE_DOWN)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STEREOMONO_DOWN)->ShowWindow(FALSE);
*/
}

void CHolitechDlg::CreateBitmapButton(CBitmapButton* bitmapButton, int left, int top, UINT upResource, UINT downresource, UINT buttonResource, CString toolTipText)
{
	//Creates a new BitmapButton
	bitmapButton = new CBitmapButton;

	//Creates a button at the point desired (left, top)
	bitmapButton->Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_NOTIFY, CRect(left, top, left, top), this, buttonResource);

	//Loads the up/down bitmaps for the button (disabled and focus not defined)
	bitmapButton->LoadBitmaps(upResource, downresource/*, 0, 0*/);

	//Since we specified a point, size the buttons to the bitmap image
	bitmapButton->SizeToContent();

	//Add specified tooltip text to the button
	m_pToolTip->AddTool((CWnd*)GetDlgItem(buttonResource), toolTipText);

	//Enable the buttons tooltips
	bitmapButton->EnableToolTips(TRUE);

//	bitmapButton->Invalidate();
}

void CHolitechDlg::LoadBitmaps()
{
	LoadBitmapAndCoordinates(&m_bmpBackgroundImage, IDB_BITMAP_BACKGROUND);
	LoadBitmapAndCoordinates(&m_bmpMHz, IDB_BITMAP_MHZ);
	LoadBitmapAndCoordinates(&m_bmpDotLarge, IDB_BITMAP_DOT_LARGE);
	LoadBitmapAndCoordinates(&m_bmpSpaceLarge, IDB_BITMAP_SPACE_LARGE);
	LoadBitmapAndCoordinates(&m_bmp0Large, IDB_BITMAP_0_LARGE);
	LoadBitmapAndCoordinates(&m_bmp1Large, IDB_BITMAP_1_LARGE);
	LoadBitmapAndCoordinates(&m_bmp2Large, IDB_BITMAP_2_LARGE);
	LoadBitmapAndCoordinates(&m_bmp3Large, IDB_BITMAP_3_LARGE);
	LoadBitmapAndCoordinates(&m_bmp4Large, IDB_BITMAP_4_LARGE);
	LoadBitmapAndCoordinates(&m_bmp5Large, IDB_BITMAP_5_LARGE);
	LoadBitmapAndCoordinates(&m_bmp6Large, IDB_BITMAP_6_LARGE);
	LoadBitmapAndCoordinates(&m_bmp7Large, IDB_BITMAP_7_LARGE);
	LoadBitmapAndCoordinates(&m_bmp8Large, IDB_BITMAP_8_LARGE);
	LoadBitmapAndCoordinates(&m_bmp9Large, IDB_BITMAP_9_LARGE);
	LoadBitmapAndCoordinates(&m_bmpALarge, IDB_BITMAP_A_LARGE);
	LoadBitmapAndCoordinates(&m_bmpBLarge, IDB_BITMAP_B_LARGE);
	LoadBitmapAndCoordinates(&m_bmpCLarge, IDB_BITMAP_C_LARGE);
	LoadBitmapAndCoordinates(&m_bmpDLarge, IDB_BITMAP_D_LARGE);
	LoadBitmapAndCoordinates(&m_bmpELarge, IDB_BITMAP_E_LARGE);
	LoadBitmapAndCoordinates(&m_bmpFLarge, IDB_BITMAP_F_LARGE);
	LoadBitmapAndCoordinates(&m_bmpGLarge, IDB_BITMAP_G_LARGE);
	LoadBitmapAndCoordinates(&m_bmpHLarge, IDB_BITMAP_H_LARGE);
	LoadBitmapAndCoordinates(&m_bmpILarge, IDB_BITMAP_I_LARGE);
	LoadBitmapAndCoordinates(&m_bmpJLarge, IDB_BITMAP_J_LARGE);
	LoadBitmapAndCoordinates(&m_bmpKLarge, IDB_BITMAP_K_LARGE);
	LoadBitmapAndCoordinates(&m_bmpLLarge, IDB_BITMAP_L_LARGE);
	LoadBitmapAndCoordinates(&m_bmpMLarge, IDB_BITMAP_M_LARGE);
	LoadBitmapAndCoordinates(&m_bmpNLarge, IDB_BITMAP_N_LARGE);
	LoadBitmapAndCoordinates(&m_bmpOLarge, IDB_BITMAP_O_LARGE);
	LoadBitmapAndCoordinates(&m_bmpPLarge, IDB_BITMAP_P_LARGE);
	LoadBitmapAndCoordinates(&m_bmpQLarge, IDB_BITMAP_Q_LARGE);
	LoadBitmapAndCoordinates(&m_bmpRLarge, IDB_BITMAP_R_LARGE);
	LoadBitmapAndCoordinates(&m_bmpSLarge, IDB_BITMAP_S_LARGE);
	LoadBitmapAndCoordinates(&m_bmpTLarge, IDB_BITMAP_T_LARGE);
	LoadBitmapAndCoordinates(&m_bmpULarge, IDB_BITMAP_U_LARGE);
	LoadBitmapAndCoordinates(&m_bmpVLarge, IDB_BITMAP_V_LARGE);
	LoadBitmapAndCoordinates(&m_bmpWLarge, IDB_BITMAP_W_LARGE);
	LoadBitmapAndCoordinates(&m_bmpXLarge, IDB_BITMAP_X_LARGE);
	LoadBitmapAndCoordinates(&m_bmpYLarge, IDB_BITMAP_Y_LARGE);
	LoadBitmapAndCoordinates(&m_bmpZLarge, IDB_BITMAP_Z_LARGE);
	LoadBitmapAndCoordinates(&m_bmpDotSmall, IDB_BITMAP_DOT_SMALL);
	LoadBitmapAndCoordinates(&m_bmpDashSmall, IDB_BITMAP_DASH_SMALL);
	LoadBitmapAndCoordinates(&m_bmpApostropheSmall, IDB_BITMAP_APOSTROPHE_SMALL);
	LoadBitmapAndCoordinates(&m_bmpExclamationSmall, IDB_BITMAP_EXCLAMATION_SMALL);
	LoadBitmapAndCoordinates(&m_bmpAmpersandSmall, IDB_BITMAP_AMPERSAND_SMALL);
	LoadBitmapAndCoordinates(&m_bmpCommaSmall, IDB_BITMAP_COMMA_SMALL);
	LoadBitmapAndCoordinates(&m_bmpSpaceSmall, IDB_BITMAP_SPACE_SMALL);
	LoadBitmapAndCoordinates(&m_bmp0Small, IDB_BITMAP_0_SMALL);
	LoadBitmapAndCoordinates(&m_bmp1Small, IDB_BITMAP_1_SMALL);
	LoadBitmapAndCoordinates(&m_bmp2Small, IDB_BITMAP_2_SMALL);
	LoadBitmapAndCoordinates(&m_bmp3Small, IDB_BITMAP_3_SMALL);
	LoadBitmapAndCoordinates(&m_bmp4Small, IDB_BITMAP_4_SMALL);
	LoadBitmapAndCoordinates(&m_bmp5Small, IDB_BITMAP_5_SMALL);
	LoadBitmapAndCoordinates(&m_bmp6Small, IDB_BITMAP_6_SMALL);
	LoadBitmapAndCoordinates(&m_bmp7Small, IDB_BITMAP_7_SMALL);
	LoadBitmapAndCoordinates(&m_bmp8Small, IDB_BITMAP_8_SMALL);
	LoadBitmapAndCoordinates(&m_bmp9Small, IDB_BITMAP_9_SMALL);
	LoadBitmapAndCoordinates(&m_bmpASmall, IDB_BITMAP_A_SMALL);
	LoadBitmapAndCoordinates(&m_bmpBSmall, IDB_BITMAP_B_SMALL);
	LoadBitmapAndCoordinates(&m_bmpCSmall, IDB_BITMAP_C_SMALL);
	LoadBitmapAndCoordinates(&m_bmpDSmall, IDB_BITMAP_D_SMALL);
	LoadBitmapAndCoordinates(&m_bmpESmall, IDB_BITMAP_E_SMALL);
	LoadBitmapAndCoordinates(&m_bmpFSmall, IDB_BITMAP_F_SMALL);
	LoadBitmapAndCoordinates(&m_bmpGSmall, IDB_BITMAP_G_SMALL);
	LoadBitmapAndCoordinates(&m_bmpHSmall, IDB_BITMAP_H_SMALL);
	LoadBitmapAndCoordinates(&m_bmpISmall, IDB_BITMAP_I_SMALL);
	LoadBitmapAndCoordinates(&m_bmpJSmall, IDB_BITMAP_J_SMALL);
	LoadBitmapAndCoordinates(&m_bmpKSmall, IDB_BITMAP_K_SMALL);
	LoadBitmapAndCoordinates(&m_bmpLSmall, IDB_BITMAP_L_SMALL);
	LoadBitmapAndCoordinates(&m_bmpMSmall, IDB_BITMAP_M_SMALL);
	LoadBitmapAndCoordinates(&m_bmpNSmall, IDB_BITMAP_N_SMALL);
	LoadBitmapAndCoordinates(&m_bmpOSmall, IDB_BITMAP_O_SMALL);
	LoadBitmapAndCoordinates(&m_bmpPSmall, IDB_BITMAP_P_SMALL);
	LoadBitmapAndCoordinates(&m_bmpQSmall, IDB_BITMAP_Q_SMALL);
	LoadBitmapAndCoordinates(&m_bmpRSmall, IDB_BITMAP_R_SMALL);
	LoadBitmapAndCoordinates(&m_bmpSSmall, IDB_BITMAP_S_SMALL);
	LoadBitmapAndCoordinates(&m_bmpTSmall, IDB_BITMAP_T_SMALL);
	LoadBitmapAndCoordinates(&m_bmpUSmall, IDB_BITMAP_U_SMALL);
	LoadBitmapAndCoordinates(&m_bmpVSmall, IDB_BITMAP_V_SMALL);
	LoadBitmapAndCoordinates(&m_bmpWSmall, IDB_BITMAP_W_SMALL);
	LoadBitmapAndCoordinates(&m_bmpXSmall, IDB_BITMAP_X_SMALL);
	LoadBitmapAndCoordinates(&m_bmpYSmall, IDB_BITMAP_Y_SMALL);
	LoadBitmapAndCoordinates(&m_bmpZSmall, IDB_BITMAP_Z_SMALL);
	LoadBitmapAndCoordinates(&m_bmpZSpecial, IDB_BITMAP_Z_SPECIAL);
	LoadBitmapAndCoordinates(&m_bmpLampColOff, IDB_BITMAP_LAMP_COL_OFF);
	LoadBitmapAndCoordinates(&m_bmpLampColOn, IDB_BITMAP_LAMP_COL_ON);
	LoadBitmapAndCoordinates(&m_bmpLampRowOff, IDB_BITMAP_LAMP_ROW_OFF);
	LoadBitmapAndCoordinates(&m_bmpLampRowOn, IDB_BITMAP_LAMP_ROW_ON);
	LoadBitmapAndCoordinates(&m_bmpLampSeatBlue, IDB_BITMAP_LAMP_SEAT_BLUE);
	LoadBitmapAndCoordinates(&m_bmpLampSeatOff, IDB_BITMAP_LAMP_SEAT_OFF);
	LoadBitmapAndCoordinates(&m_bmpLampSeatRed, IDB_BITMAP_LAMP_SEAT_RED);
	LoadBitmapAndCoordinates(&m_bmpVenFanOff, IDB_BITMAP_VEN_FAN_OFF);
	LoadBitmapAndCoordinates(&m_bmpVenFanOn, IDB_BITMAP_VEN_FAN_ON);
	LoadBitmapAndCoordinates(&m_bmpVenOff, IDB_BITMAP_VEN_OFF);
	LoadBitmapAndCoordinates(&m_bmpVenOn, IDB_BITMAP_VEN_ON);

//	LoadBitmapAndCoordinates(&m_bmpVolumeSlide, IDB_BITMAP_VOLUMESLIDE);
}

void CHolitechDlg::LoadBitmapAndCoordinates(CBitmap* bmp, UINT resourceID)
{
	BITMAP bitmap;
	
	bmp->LoadBitmap(resourceID);
	bmp->GetBitmap(&bitmap);
	bmp->SetBitmapDimension(bitmap.bmWidth, bitmap.bmHeight);
}

void CHolitechDlg::RemoveButtons()
{
	delete GetDlgItem(IDC_BUTTON_MINIMIZE);
	delete GetDlgItem(IDC_BUTTON_RESTORE);
	delete GetDlgItem(IDC_BUTTON_CLOSE);

	delete GetDlgItem(IDC_BUTTON_MODE);
	delete GetDlgItem(IDC_BUTTON_PRESET1);
	delete GetDlgItem(IDC_BUTTON_PRESET2);
	delete GetDlgItem(IDC_BUTTON_PRESET3);
	delete GetDlgItem(IDC_BUTTON_PRESET4);
	delete GetDlgItem(IDC_BUTTON_PRESET5);
	delete GetDlgItem(IDC_BUTTON_PRESET6);
	delete GetDlgItem(IDC_BUTTON_PRE);
	delete GetDlgItem(IDC_BUTTON_PLAY);
	delete GetDlgItem(IDC_BUTTON_NEXT);
	delete GetDlgItem(IDC_BUTTON_MUTE);
	delete GetDlgItem(IDC_BUTTON_VOL_DN);
	delete GetDlgItem(IDC_BUTTON_VOL_UP);
	delete GetDlgItem(IDC_BUTTON_TEM_DN);
	delete GetDlgItem(IDC_BUTTON_TEM_UP);
	delete GetDlgItem(IDC_BUTTON_HEATSEAT);
	delete GetDlgItem(IDC_BUTTON_COOLSEAT);
	delete GetDlgItem(IDC_BUTTON_BODYFOOT);
	delete GetDlgItem(IDC_BUTTON_HEAD);
	delete GetDlgItem(IDC_BUTTON_FOOT);
	delete GetDlgItem(IDC_BUTTON_HEADFOOT);
	delete GetDlgItem(IDC_BUTTON_VEN_DN);
	delete GetDlgItem(IDC_BUTTON_VEN_UP);
	delete GetDlgItem(IDC_BUTTON_TEM_DN_CO);
	delete GetDlgItem(IDC_BUTTON_TEM_UP_CO);
	delete GetDlgItem(IDC_BUTTON_HEATSEAT_CO);
	delete GetDlgItem(IDC_BUTTON_COOLSEAT_CO);
	delete GetDlgItem(IDC_BUTTON_FRONTHEAT);
	delete GetDlgItem(IDC_BUTTON_REARHEAT);
	delete GetDlgItem(IDC_BUTTON_AUTO);
	delete GetDlgItem(IDC_BUTTON_AC);
	delete GetDlgItem(IDC_BUTTON_MAX_AC);
	delete GetDlgItem(IDC_BUTTON_AIR);
}

void CHolitechDlg::KeyUp(BYTE key) 
{
	//Determine which key came up and handle it
	switch (key)
	{
	case KEY_F1 	:	OnPreset1();	break;
	case KEY_F2 	:	OnPreset2();	break;
	case KEY_F3 	:	OnPreset3();	break;
	case KEY_F4 	:	OnPreset4();	break;
	case KEY_F5 	:	OnPreset5();	break;
	case KEY_F6 	:	OnPreset6();	break;
	case KEY_UP		:					break;
	case KEY_DOWN	:					break;
	default	:							break;
	}
}

void CHolitechDlg::OnCancel()
{
	//Delete the tray icon
	Shell_NotifyIcon(NIM_DELETE, &m_TrayData);

	KillTimer(TIMER_FIND_DEVICE);
	KillTimer(TIMER_AUDIO_STREAM);
	KillTimer(TIMER_POLL_DEVICE);

	//Cleanup the tooltips
	delete(m_pToolTip);

	//Remove the buttons
	RemoveButtons();

	//Set the handle in the App to null
	((CHolitechApp*)AfxGetApp())->m_hwndDialog = NULL;

	CDialog::OnCancel();
}

void CHolitechDlg::OnMinimize()
{
	//Add the icon and minimize and hide the window
	ShowWindow(SW_MINIMIZE);
	
	//if (m_RadioData.showInTray)
	//	Shell_NotifyIcon(NIM_ADD, &m_TrayData);

	//ShowWindow(SW_HIDE);
}

LRESULT CHolitechDlg::OnCommandHelp(WPARAM wParam, LPARAM lParam)
{
	//Override the F1 Help to allow F1 to be a preset
	return true;
}

bool CHolitechDlg::TuneStation(bool tuneUp)
{
	//This function allows the GUI to continuously tune up
	//or down without calling a tune to the device (tunedevice)
	//by setting a timer that when timesout, actually does the
	//tune on the device

	//Kill the polling timers
	KillTimer(TIMER_TUNE_DELAY);
	KillTimer(TIMER_POLL_DEVICE);

	//Update the radio LED with the new station
//	UpdateRadioLED();

	return true;
}

void CHolitechDlg::SetVolume(float fPercent)
{
	// Volume control
	CoInitialize(NULL);
	deviceEnumerator = NULL;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
	defaultDevice = NULL; 
	
	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL; 
	
	endpointVolume = NULL;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL; 

	/*
		endpointVolume->GetMute(&bMute);		// 음소거 여부 얻기 (TRUE는 음소거)
		endpointVolume->SetMute(TRUE, NULL);	// 음소거 셋팅
		endpointVolume->SetMute(FALSE, NULL);	// 음소거 해제
	*/

//	float currentVolume = 0;
//	hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);	   // 마스터볼륨값 얻기 [0.0, 1.0]
//	m_AVNvolume = currentVolume;

	hr = endpointVolume->SetMasterVolumeLevelScalar(fPercent, NULL);	   // 마스터볼륨값 셋팅 [0.0, 1.0]
	
	endpointVolume->Release();
	CoUninitialize();
}

void CHolitechDlg::SetMute()
{
	// Volume control
	CoInitialize(NULL);
	deviceEnumerator = NULL;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
	defaultDevice = NULL; 
	
	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL; 
	
	endpointVolume = NULL;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL; 

//	endpointVolume->GetMute(&m_AVNmute);		// 음소거 여부 얻기 (TRUE는 음소거)
	endpointVolume->SetMute(m_AVNmute, NULL);	// TRUE: 음소거 셋팅,FALSE: 음소거 해제

	endpointVolume->Release();
	CoUninitialize();
}

void CHolitechDlg::MediaPlay()
{
/*
	g_playbutt=FPLAY;

	if(dwID)
	{
		//dwID 즉 mci가 한번이라도 오픈이 되었을 때부터 실행
		g_playbutt=FPLAY;																//새로 재생
		mciSendCommand(dwID,MCI_CLOSE,NULL,(DWORD)(LPVOID)&mciPlay);					//다른 노래 실행시 mci를 닫고 하기 위해서
		//KillTimer(1);
	}
*/
	switch(g_playbutt){ 															
	case FPLAY:
		if(dwID&&g_stop==FOFFSTOP)															//정지 후 시작을 다시 누를 경우와 실행 된적이 없을때는 실행을 안시킴
			MediaClose();

		KillTimer(1);

		// Changing Button image
		// ...
		g_playbutt=FPAUSE;

		mciOpen.lpstrElementName;

//		if(m_cname.Right(3)=="wav")
//			mciOpen.lpstrDeviceType = "waveaudio";
//		if(m_cname.Right(3)=="wma" || m_cname.Right(3)=="mp3")
			mciOpen.lpstrDeviceType = "mpegvideo";

		if(g_stop==FOFFSTOP){																//정지 버튼이 눌리지 안았을때는 mci를 새로 open
			mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE|MCI_OPEN_ELEMENT,(DWORD)&mciOpen);
			dwID = mciOpen.wDeviceID;
		}

		mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY,(DWORD)&mciPlay);
		MCI_STATUS_PARMS mciStatusParms;
		mciStatusParms.dwItem = MCI_FORMAT_HMS; 											//재생시간을 밀리 세컨드로 읽어옴
		mciSendCommand(dwID, MCI_STATUS, MCI_STATUS_ITEM ,(DWORD)(LPVOID)&mciStatusParms);
		g_tend = mciStatusParms.dwReturn/1000;
		g_playtime=0;
		SetTimer(1,1000,NULL);
		g_stop=FOFFSTOP;																	//정지버튼 누른후 시작 버튼을 눌렀을때를 위해
		break;

	case FPAUSE:																			//재생중이면 일시정지
		// Changing Button image
		// ...		
		g_playbutt=FRESUME;

		KillTimer(1);
		mciSendCommand(dwID,MCI_PAUSE,MCI_WAIT,(DWORD)&mciGeneric);
		break;														
	
	case FRESUME:																			//일시 정지가 된 부분부터시작
		// Changing Button image
		// ...
		g_playbutt=FPAUSE;

		MCI_PLAY_PARMS mciPlay;
		mciPlay.dwFrom = g_playtime*1000;
		SetTimer(1,1000,NULL);

		mciSendCommand(dwID, MCI_PLAY, MCI_FROM,(DWORD)(LPVOID)&mciPlay);
		break;
	}
}

void CHolitechDlg::MediaStop()
{
	// TODO: Add your control notification handler code here
	mciSendCommand(dwID,MCI_SEEK,MCI_SEEK_TO_START,(DWORD)(LPVOID)&mciPlay);		//정지버튼누른후 재생버튼 누를때를 위해서
	g_stop=FONSTOP;

	g_playbutt=FPLAY;																//처음부터 재생
	KillTimer(1);
}

void CHolitechDlg::MediaClose()
{
	g_playbutt=FPLAY;																//새로 재생
	mciSendCommand(dwID,MCI_CLOSE,NULL,(DWORD)(LPVOID)&mciPlay);					//다른 노래 실행시 mci를 닫고 하기 위해서
	KillTimer(1);
}

void CHolitechDlg::MediaPlay_HVAC()
{
	switch(g_playbutt_hvac){ 															
	case FPLAY:
		if(dwID_hvac&&g_stop_hvac==FOFFSTOP)															//정지 후 시작을 다시 누를 경우와 실행 된적이 없을때는 실행을 안시킴
			MediaClose_HVAC();

		KillTimer(1);

		// Changing Button image
		// ...
		g_playbutt_hvac=FPAUSE;

		mciOpen_hvac.lpstrElementName;

//		if(m_cname.Right(3)=="wav")
//			mciOpen.lpstrDeviceType = "waveaudio";
//		if(m_cname.Right(3)=="wma" || m_cname.Right(3)=="mp3")
			mciOpen_hvac.lpstrDeviceType = "mpegvideo";

		if(g_stop_hvac==FOFFSTOP){																//정지 버튼이 눌리지 안았을때는 mci를 새로 open
			mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE|MCI_OPEN_ELEMENT,(DWORD)&mciOpen_hvac);
			dwID_hvac = mciOpen_hvac.wDeviceID;
		}

		mciSendCommand(dwID_hvac, MCI_PLAY, MCI_NOTIFY,(DWORD)&mciPlay);
		MCI_STATUS_PARMS mciStatusParms;
		mciStatusParms.dwItem = MCI_FORMAT_HMS; 											//재생시간을 밀리 세컨드로 읽어옴
		mciSendCommand(dwID_hvac, MCI_STATUS, MCI_STATUS_ITEM ,(DWORD)(LPVOID)&mciStatusParms);
		g_tend_hvac = mciStatusParms.dwReturn/1000;
		g_playtime_hvac=0;
		SetTimer(1,1000,NULL);
		g_stop_hvac=FOFFSTOP;																	//정지버튼 누른후 시작 버튼을 눌렀을때를 위해
		break;

	case FPAUSE:																			//재생중이면 일시정지
		// Changing Button image
		// ...		
		g_playbutt_hvac=FRESUME;

		KillTimer(1);
		mciSendCommand(dwID_hvac,MCI_PAUSE,MCI_WAIT,(DWORD)&mciGeneric_hvac);
		break;														
	
	case FRESUME:																			//일시 정지가 된 부분부터시작
		// Changing Button image
		// ...
		g_playbutt_hvac=FPAUSE;

		MCI_PLAY_PARMS mciPlay;
		mciPlay.dwFrom = g_playtime_hvac*1000;
		SetTimer(1,1000,NULL);

		mciSendCommand(dwID_hvac, MCI_PLAY, MCI_FROM,(DWORD)(LPVOID)&mciPlay);
		break;
	}
}

void CHolitechDlg::MediaClose_HVAC()
{
	g_playbutt_hvac=FPLAY;																//새로 재생
	mciSendCommand(dwID_hvac,MCI_CLOSE,NULL,(DWORD)(LPVOID)&mciPlay_hvac);					//다른 노래 실행시 mci를 닫고 하기 위해서
	KillTimer(1);
}

void CHolitechDlg::OnMode()
{
	MediaClose();

	m_AVNmode = m_AVNmode+1;

	if(m_AVNmode == MODE_END)
		m_AVNmode = RADIO;

	UpdateMode();
}

void CHolitechDlg::OnPreset1()
{
	MediaClose();

	m_TrackNo = 1;

	if(m_AVNmode == RADIO)
	{
		m_Frequency = 88.9;

		//Setting file path
		//mciOpen.lpstrElementName = "E:\\MyDrive\\0. Work\\AVN GUI\\Source\\R1_Johann Sebastian Bach-Air on G string.mp3";
		mciOpen.lpstrElementName = "R1_Johann Sebastian Bach-Air on G string.mp3";
	}
	else
	{
		//Setting file path
		//mciOpen.lpstrElementName = "E:\\MyDrive\\0. Work\\AVN GUI\\Source\\C1_July-My Soul.mp3";
		mciOpen.lpstrElementName = "C1_July-My Soul.mp3";
	}

	MediaPlay();

	UpdatePannel();
}

void CHolitechDlg::OnPreset2()
{
	MediaClose();

	m_TrackNo = 2;

	if(m_AVNmode == RADIO)
	{
		m_Frequency = 90.3;

		//Setting file path
		//mciOpen.lpstrElementName = "E:\\MyDrive\\0. Work\\AVN GUI\\Source\\R2.mp3";
		mciOpen.lpstrElementName = "R2.mp3";
	}
	else
	{
		//Setting file path
		//mciOpen.lpstrElementName = "E:\\MyDrive\\0. Work\\AVN GUI\\Source\\C2_Santana-Smooth.mp3";
		mciOpen.lpstrElementName = "C2_Santana-Smooth.mp3";
	}

	MediaPlay();

	UpdatePannel();
}

void CHolitechDlg::OnPreset3()
{
	MediaClose();

	m_TrackNo = 3;

	if(m_AVNmode == RADIO)
	{
		m_Frequency = 93.5;

		//Setting file path
		//mciOpen.lpstrElementName = "E:\\MyDrive\\0. Work\\AVN GUI\\Source\\R3_Chopin - Nocturne Op9 No2.mp3";
		mciOpen.lpstrElementName = "R3_Chopin - Nocturne Op9 No2.mp3";
	}
	else
	{
		//Setting file path
		//mciOpen.lpstrElementName = "E:\\MyDrive\\0. Work\\AVN GUI\\Source\\C3_David Lanz-Return to the heart.mp3";
		mciOpen.lpstrElementName = "C3_David Lanz-Return to the heart.mp3";
	}


	MediaPlay();

	UpdatePannel();
}

void CHolitechDlg::OnPreset4()
{
	MediaClose();

	m_TrackNo = 4;

	if(m_AVNmode == RADIO)
	{
		m_Frequency = 98.3;

		//Setting file path
		//mciOpen.lpstrElementName = "E:\\MyDrive\\0. Work\\AVN GUI\\Source\\R4_Dusty Springfield-Spooky.mp3";
		mciOpen.lpstrElementName = "R4_Dusty Springfield-Spooky.mp3";
	}
	else
	{
		//Setting file path
		//mciOpen.lpstrElementName = "E:\\MyDrive\\0. Work\\AVN GUI\\Source\\C4_The Caesars-Jerk It Out.mp3";
		mciOpen.lpstrElementName = "C4_The Caesars-Jerk It Out.mp3";
	}

	MediaPlay();

	UpdatePannel();
}

void CHolitechDlg::OnPreset5()
{
	MediaClose();

	m_TrackNo = 5;
	
	if(m_AVNmode == RADIO)
	{
		m_Frequency = 104.1;

		//Setting file path
		//mciOpen.lpstrElementName = "E:\\MyDrive\\0. Work\\AVN GUI\\Source\\R5_ave maria.mp3";
		mciOpen.lpstrElementName = "R5_ave maria.mp3";
	}
	else
	{
		//Setting file path
		//mciOpen.lpstrElementName = "E:\\MyDrive\\0. Work\\AVN GUI\\Source\\C5_History.mp3";
		mciOpen.lpstrElementName = "C5_History.mp3";
	}

	MediaPlay();

	UpdatePannel();
}

void CHolitechDlg::OnPreset6()
{
	MediaClose();

	m_TrackNo = 6;

	if(m_AVNmode == RADIO)
	{
		m_Frequency = 107.7;

		//Setting file path
		//mciOpen.lpstrElementName = "E:\\MyDrive\\0. Work\\AVN GUI\\Source\\R6_Sheeran-Shape Of You.mp3";
		mciOpen.lpstrElementName = "R6_Sheeran-Shape Of You.mp3";
	}
	else
	{
		//Setting file path
		//mciOpen.lpstrElementName = "E:\\MyDrive\\0. Work\\AVN GUI\\Source\\C6_The Platters-Smoke Gets Into Your Eyes.mp3";
		mciOpen.lpstrElementName = "C6_The Platters-Smoke Gets Into Your Eyes.mp3";
	}

	MediaPlay();

	UpdatePannel();
}

void CHolitechDlg::OnPre()
{
	if(m_TrackNo <= 0) m_TrackNo = 6;
	else m_TrackNo--;

	switch(m_TrackNo)
	{
		case 1:
			OnPreset1();
			break;
		case 2:
			OnPreset2();
			break;
		case 3:
			OnPreset3();
			break;
		case 4:
			OnPreset4();
			break;
		case 5:
			OnPreset5();
			break;
		case 6:
			OnPreset6();
			break;
		default:
			break;
	}
}

void CHolitechDlg::OnPlay()
{
	if(m_AVNmode == CD) MediaPlay();
/*
	switch(g_playbutt)
	{
		case PLAY:
		case RESUME:
			m_PlayStatus = PAUSE;

			mciSendCommand(dwID,MCI_PAUSE,MCI_WAIT,(DWORD)&mciGeneric);
			break;
		case PAUSE:
			m_PlayStatus = RESUME;

			int pos;
			MCI_PLAY_PARMS mciPlay;
			pos = m_slidertime.GetPos();
			mciPlay.dwFrom = pos*1000;
			SetTimer(1,1000,NULL);
			
			mciSendCommand(dwID, MCI_PLAY, MCI_FROM,(DWORD)(LPVOID)&mciPlay);

			break;
		case STOP:
			m_PlayStatus = PLAY;
			OnPreset1();
			break;
		default:
			break;
	}

	if(m_PlayStatus)
	{
		switch(m_TrackNo)
		{
			case 1:
				OnPreset1();
				break;
			case 2:
				OnPreset2();
				break;
			case 3:
				OnPreset3();
				break;
			case 4:
				OnPreset4();
				break;
			case 5:
				OnPreset5();
				break;
			case 6:
				OnPreset6();
				break;
			default:
				break;
		}
	}
	else
	{
	}
*/
}

void CHolitechDlg::OnNext()
{
	if(m_TrackNo >= 6) m_TrackNo = 0;
	else m_TrackNo++;

	switch(m_TrackNo)
	{
		case 1:
			OnPreset1();
			break;
		case 2:
			OnPreset2();
			break;
		case 3:
			OnPreset3();
			break;
		case 4:
			OnPreset4();
			break;
		case 5:
			OnPreset5();
			break;
		case 6:
			OnPreset6();
			break;
		default:
			break;
	}
}

void CHolitechDlg::OnMute()
{
	m_AVNmute = !m_AVNmute;

	SetMute();

	UpdateMute();
}

void CHolitechDlg::OnVolDn()
{
	if(m_AVNvolume != 0) m_AVNvolume = m_AVNvolume-0.05;
	else m_AVNvolume = 0.2;

	SetVolume(m_AVNvolume);

	UpdateVolume();
}

void CHolitechDlg::OnVolUp()
{
	if(m_AVNvolume != 1.0) m_AVNvolume = m_AVNvolume+0.05;
	else m_AVNvolume = 0.2;

	SetVolume(m_AVNvolume);

	UpdateVolume();
}

void CHolitechDlg::OnTemDn()
{
	if(m_Tem > 10) m_Tem--;

	DrawBackground();
}

void CHolitechDlg::OnTemUp()
{
	if(m_Tem < 40) m_Tem++;

	DrawBackground();
}

void CHolitechDlg::OnHeatSeat()
{
	if(m_HSDrv >= 3) m_HSDrv = 0;
	else m_HSDrv++;

	//Draw the lamp image
	switch(m_HSDrv)
	{
		case 0:
		default:
			GetDlgItem(IDC_BUTTON_HEATSEAT3)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_HEATSEAT)->ShowWindow(TRUE);
			break;
		case 1:
			GetDlgItem(IDC_BUTTON_HEATSEAT)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_HEATSEAT1)->ShowWindow(TRUE);
			break;
		case 2:
			GetDlgItem(IDC_BUTTON_HEATSEAT1)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_HEATSEAT2)->ShowWindow(TRUE);
			break;
		case 3:
			GetDlgItem(IDC_BUTTON_HEATSEAT2)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_HEATSEAT3)->ShowWindow(TRUE);
			break;
	}

	switch(m_CSDrv)
	{
		case 0:
			break;
		case 1:
			GetDlgItem(IDC_BUTTON_COOLSEAT1)->ShowWindow(FALSE);
			break;
		case 2:
			GetDlgItem(IDC_BUTTON_COOLSEAT2)->ShowWindow(FALSE);
			break;
		case 3:
			GetDlgItem(IDC_BUTTON_COOLSEAT3)->ShowWindow(FALSE);
			break;
		default:
			GetDlgItem(IDC_BUTTON_COOLSEAT1)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_COOLSEAT2)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_COOLSEAT3)->ShowWindow(FALSE);
			break;
	}

	m_CSDrv = 0;
	GetDlgItem(IDC_BUTTON_COOLSEAT)->ShowWindow(TRUE);
}

void CHolitechDlg::OnCoolSeat()
{
	if(m_CSDrv >= 3) m_CSDrv = 0;
	else m_CSDrv++;

	//Draw the lamp image
	switch(m_CSDrv)
	{
		case 0:
		default:
			GetDlgItem(IDC_BUTTON_COOLSEAT3)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_COOLSEAT)->ShowWindow(TRUE);
			break;
		case 1:
			GetDlgItem(IDC_BUTTON_COOLSEAT)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_COOLSEAT1)->ShowWindow(TRUE);
			break;
		case 2:
			GetDlgItem(IDC_BUTTON_COOLSEAT1)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_COOLSEAT2)->ShowWindow(TRUE);
			break;
		case 3:
			GetDlgItem(IDC_BUTTON_COOLSEAT2)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_COOLSEAT3)->ShowWindow(TRUE);
			break;
	}

	switch(m_HSDrv)
	{
		case 0:
			break;
		case 1:
			GetDlgItem(IDC_BUTTON_HEATSEAT1)->ShowWindow(FALSE);
			break;
		case 2:
			GetDlgItem(IDC_BUTTON_HEATSEAT2)->ShowWindow(FALSE);
			break;
		case 3:
			GetDlgItem(IDC_BUTTON_HEATSEAT3)->ShowWindow(FALSE);
			break;
		default:
			GetDlgItem(IDC_BUTTON_HEATSEAT1)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_HEATSEAT2)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_HEATSEAT3)->ShowWindow(FALSE);
			break;
	}

	m_HSDrv = 0;
	GetDlgItem(IDC_BUTTON_HEATSEAT)->ShowWindow(TRUE);
}

void CHolitechDlg::OnBodyFoot()
{
	m_BF = !m_BF;

	if(m_BF)
	{
		GetDlgItem(IDC_BUTTON_BODYFOOT)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_BODYFOOT1)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_BODYFOOT)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_BODYFOOT1)->ShowWindow(FALSE);
	}

	m_H = FALSE;
	m_F = FALSE;
	m_HF = FALSE;

	GetDlgItem(IDC_BUTTON_HEAD)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_FOOT)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_HEADFOOT)->ShowWindow(TRUE);

	GetDlgItem(IDC_BUTTON_HEAD1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FOOT1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_HEADFOOT1)->ShowWindow(FALSE);
}

void CHolitechDlg::OnHead()
{
	m_H = !m_H;

	if(m_H)
	{
		GetDlgItem(IDC_BUTTON_HEAD)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_HEAD1)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_HEAD)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_HEAD1)->ShowWindow(FALSE);
	}

	m_BF = FALSE;
	m_F = FALSE;
	m_HF = FALSE;

	GetDlgItem(IDC_BUTTON_BODYFOOT)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_FOOT)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_HEADFOOT)->ShowWindow(TRUE);


	GetDlgItem(IDC_BUTTON_BODYFOOT1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FOOT1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_HEADFOOT1)->ShowWindow(FALSE);
}

void CHolitechDlg::OnFoot()
{
	m_F = !m_F;

	if(m_F)
	{
		GetDlgItem(IDC_BUTTON_FOOT)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FOOT1)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_FOOT)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_FOOT1)->ShowWindow(FALSE);
	}

	m_BF = FALSE;
	m_H = FALSE;
	m_HF = FALSE;

	GetDlgItem(IDC_BUTTON_BODYFOOT)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_HEAD)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_HEADFOOT)->ShowWindow(TRUE);

	GetDlgItem(IDC_BUTTON_BODYFOOT1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_HEAD1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_HEADFOOT1)->ShowWindow(FALSE);
}

void CHolitechDlg::OnHeadFoot()
{
	m_HF = !m_HF;

	if(m_HF)
	{
		GetDlgItem(IDC_BUTTON_HEADFOOT)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_HEADFOOT1)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_HEADFOOT)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_HEADFOOT1)->ShowWindow(FALSE);
	}

	m_BF = FALSE;
	m_H = FALSE;
	m_F = FALSE;

	GetDlgItem(IDC_BUTTON_BODYFOOT)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_HEAD)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_FOOT)->ShowWindow(TRUE);

	GetDlgItem(IDC_BUTTON_BODYFOOT1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_HEAD1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FOOT1)->ShowWindow(FALSE);
}

void CHolitechDlg::OnVenDn()
{
	if(m_Ven > 0) m_Ven--;

	DrawBackground();
}

void CHolitechDlg::OnVenUp()
{
	if(m_Ven < 7) m_Ven++;

	DrawBackground();
}

void CHolitechDlg::OnTemDnCo()
{
	if(m_TemCo > 10) m_TemCo--;

	DrawBackground();
}

void CHolitechDlg::OnTemUpCo()
{
	if(m_TemCo < 40) m_TemCo++;

	DrawBackground();
}

void CHolitechDlg::OnHeatSeatCo()
{
	if(m_HSCo >= 3) m_HSCo = 0;
	else m_HSCo++;

	//Draw the lamp image
	switch(m_HSCo)
	{
		case 0:
		default:
			GetDlgItem(IDC_BUTTON_HEATSEAT_CO3)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_HEATSEAT_CO)->ShowWindow(TRUE);
			break;
		case 1:
			GetDlgItem(IDC_BUTTON_HEATSEAT_CO)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_HEATSEAT_CO1)->ShowWindow(TRUE);
			break;
		case 2:
			GetDlgItem(IDC_BUTTON_HEATSEAT_CO1)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_HEATSEAT_CO2)->ShowWindow(TRUE);
			break;
		case 3:
			GetDlgItem(IDC_BUTTON_HEATSEAT_CO2)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_HEATSEAT_CO3)->ShowWindow(TRUE);
			break;
	}

	switch(m_CSCo)
	{
		case 0:
			break;
		case 1:
			GetDlgItem(IDC_BUTTON_COOLSEAT_CO1)->ShowWindow(FALSE);
			break;
		case 2:
			GetDlgItem(IDC_BUTTON_COOLSEAT_CO2)->ShowWindow(FALSE);
			break;
		case 3:
			GetDlgItem(IDC_BUTTON_COOLSEAT_CO3)->ShowWindow(FALSE);
			break;
		default:
			GetDlgItem(IDC_BUTTON_COOLSEAT_CO1)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_COOLSEAT_CO2)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_COOLSEAT_CO3)->ShowWindow(FALSE);
			break;
	}

	m_CSCo = 0;
	GetDlgItem(IDC_BUTTON_COOLSEAT_CO)->ShowWindow(TRUE);
}

void CHolitechDlg::OnCoolSeatCo()
{
	if(m_CSCo >= 3) m_CSCo = 0;
	else m_CSCo++;

	//Draw the lamp image
	switch(m_CSCo)
	{
		case 0:
		default:
			GetDlgItem(IDC_BUTTON_COOLSEAT_CO3)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_COOLSEAT_CO)->ShowWindow(TRUE);
			break;
		case 1:
			GetDlgItem(IDC_BUTTON_COOLSEAT_CO)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_COOLSEAT_CO1)->ShowWindow(TRUE);
			break;
		case 2:
			GetDlgItem(IDC_BUTTON_COOLSEAT_CO1)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_COOLSEAT_CO2)->ShowWindow(TRUE);
			break;
		case 3:
			GetDlgItem(IDC_BUTTON_COOLSEAT_CO2)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_COOLSEAT_CO3)->ShowWindow(TRUE);
		break;
	}

	switch(m_HSCo)
	{
		case 0:
			break;
		case 1:
			GetDlgItem(IDC_BUTTON_HEATSEAT_CO1)->ShowWindow(FALSE);
			break;
		case 2:
			GetDlgItem(IDC_BUTTON_HEATSEAT_CO2)->ShowWindow(FALSE);
			break;
		case 3:
			GetDlgItem(IDC_BUTTON_HEATSEAT_CO3)->ShowWindow(FALSE);
			break;
		default:
			GetDlgItem(IDC_BUTTON_HEATSEAT_CO1)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_HEATSEAT_CO2)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_HEATSEAT_CO3)->ShowWindow(FALSE);
			break;
	}

	m_HSCo = 0;
	GetDlgItem(IDC_BUTTON_HEATSEAT_CO)->ShowWindow(TRUE);
}

void CHolitechDlg::OnFrontHeat()
{
	m_FH = !m_FH;

	if(m_FH)
	{
		GetDlgItem(IDC_BUTTON_FRONTHEAT)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FRONTHEAT1)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_FRONTHEAT)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_FRONTHEAT1)->ShowWindow(FALSE);
	}
}

void CHolitechDlg::OnRearHeat()
{
	m_RH = !m_RH;

	if(m_RH)
	{
		GetDlgItem(IDC_BUTTON_REARHEAT)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REARHEAT1)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_REARHEAT)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_REARHEAT1)->ShowWindow(FALSE);
	}
}

void CHolitechDlg::OnAuto()
{
	m_HVACauto = !m_HVACauto;

	if(m_HVACauto)
	{
		GetDlgItem(IDC_BUTTON_AUTO)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AUTO1)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_AUTO)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_AUTO1)->ShowWindow(FALSE);
	}
}

void CHolitechDlg::OnAC()
{
	m_HVACac= !m_HVACac;

	if(m_HVACac)
	{
		GetDlgItem(IDC_BUTTON_AC)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AC1)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_AC)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_AC1)->ShowWindow(FALSE);
	}
}

void CHolitechDlg::OnMaxAC()
{
	m_HVACmaxac= !m_HVACmaxac;

	if(m_HVACmaxac)
	{
		GetDlgItem(IDC_BUTTON_MAX_AC)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MAX_AC1)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_MAX_AC)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_MAX_AC1)->ShowWindow(FALSE);
	}

	mciOpen_hvac.lpstrElementName = "C4_The Caesars-Jerk It Out.mp3";;

	MediaPlay_HVAC();
}

void CHolitechDlg::OnAir()
{
	m_HVACair= !m_HVACair;

	if(m_HVACair)
	{
		GetDlgItem(IDC_BUTTON_AIR)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AIR1)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_AIR)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_AIR1)->ShowWindow(FALSE);
	}
}

void CHolitechDlg::OnTimer(UINT nIDEvent) 
{
#if 1

	g_playtime++;
	if(g_playtime >= g_tend)
	{
		//OnNext();
		//g_playbutt=FPLAY;														//현재 노래 재생이 끝났으므로
	}

#else
	//Determine which event was triggered and act on it
	switch (nIDEvent)
	{
	case TIMER_FIND_DEVICE	:
								break;

	case TIMER_POLL_DEVICE	:	//Get the RDBS data from the devicem, if it fails kill the poll/audio timers update
								//the LED and start looking for a valid device again
								KillTimer(TIMER_POLL_DEVICE);
								
								//Store the temp values so we can determine if a change occurs
								break;

	case TIMER_AUDIO_STREAM	:
								break;

	case TIMER_BUTTON_DOWN	:	KillTimer(TIMER_BUTTON_DOWN);

								//Reset the down variables
								m_DownPoint = CPoint (-1, -1);
								m_KeyDown = 0x00;

								break;

								//On the scan timers, perform the seek up or down for device, or presets
	case TIMER_SCAN_UP		:
								break;

								break;

	case TIMER_SCANPRESET_UP	:
									break;

	case TIMER_SCANPRESET_DOWN	:
									break;

	case TIMER_VOLUME_MOVING	:	//If we are moving the volume, draw the cursor in a new position
									break;

	case TIMER_TUNE_DELAY		:	//If the tune delay timer goes off, then perform the actual tune
									break;

	default					:	break;
	}
#endif
	CDialog::OnTimer(nIDEvent);
}

void CHolitechDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
/*
	//Check to see if we are in the volume location of the dialog
	if ((point.x >= VOLUME_MIN_X) && (point.x <= VOLUME_MAX_X) &&
		(point.y >= VOLUME_MIN_Y) && (point.y <= VOLUME_MAX_Y))
	{
		m_Muting = false;
		if (GetDlgItem(IDC_BUTTON_MUTE_DOWN)->IsWindowVisible())
		{
			GetDlgItem(IDC_BUTTON_MUTE)->ShowWindow(TRUE);
			GetDlgItem(IDC_BUTTON_MUTE_DOWN)->ShowWindow(FALSE);
		}

		//float currentVolume = 0;
		//hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);				   // 마스터볼륨값 얻기 [0.0, 1.0]
		//m_AVNvolume = (BYTE)(currentVolume*100);

		InvalidateRect(CRect(CPoint(VOLUME_MIN_X, VOLUME_MIN_Y), CPoint(VOLUME_MAX_X, VOLUME_MAX_Y)), FALSE);
		UpdateWindow();

		//Set volume toggle to true to slide the volume up and down
		m_VolumeToggle = true;
	}
	else
	{
		//Store the move reference point and set moving to true
		m_MovePoint = point;
		SetTimer(TIMER_VOLUME_MOVING, 10, NULL);
	}
*/
	// Handling Dialog
//	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);

	CDialog::OnLButtonDown(nFlags, point);
}

void CHolitechDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	//Set the new volume if it is being toggled
	if ((nFlags & MK_LBUTTON) && m_VolumeToggle)
	{
		int x = point.x;

		if (x < VOLUME_MIN_X)
			x = VOLUME_MIN_X;
		else if (x > VOLUME_MAX_X)
			x = VOLUME_MAX_X;

		//Set the new volume
		SetVolume((float)((double)(x - VOLUME_MIN_X) / (double)(VOLUME_MAX_X - VOLUME_MIN_X)));
/*
		float currentVolume = 0;
		hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);				   // 마스터볼륨값 얻기 [0.0, 1.0]
		m_AVNvolume = (BYTE)(currentVolume*100);
*/
		//Update the window with the new volume settings
		InvalidateRect(CRect(CPoint(VOLUME_MIN_X, VOLUME_MIN_Y), CPoint(VOLUME_MAX_X, VOLUME_MAX_Y)), FALSE);
		UpdateWindow();
	}

	//If anything else comes up, then kill the volume timer/toggle
	if (!(nFlags & MK_LBUTTON))
	{
		KillTimer(TIMER_VOLUME_MOVING);
		m_VolumeToggle = false;
	}
		
	CDialog::OnMouseMove(nFlags, point);
}

void CHolitechDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//Kill the volume move timer
	KillTimer(TIMER_VOLUME_MOVING);
	m_VolumeToggle = false;

	CDialog::OnLButtonUp(nFlags, point);
}

void CHolitechDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDblClk(nFlags, point);
}

BOOL CHolitechDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (NULL != m_pToolTip)
		m_pToolTip->RelayEvent(pMsg);
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CHolitechDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return 0;

	return CDialog::OnEraseBkgnd(pDC);
}
