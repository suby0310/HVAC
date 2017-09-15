// HolitechDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Holitech.h"
#include "HolitechDlg.h"
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include "Digitalv.h"


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
MCI_DGV_SETAUDIO_PARMS mciParams = {0};


int dwID_hvac;					//mci에서 사용
int g_playtime_hvac=0;			//재생 시간
int g_tend_hvac=0xFFFF;			//현재 재생되는 노래의 총 재생시간을 저장
int g_stop_hvac=FOFFSTOP;		//스탑버튼이 눌렸는지의 유무
int g_repect_hvac=FONEREPECT;	//반복모드
int g_playbutt_hvac=0;			//재생버튼이 눌렸는지 여부

MCI_OPEN_PARMS mciOpen_hvac;	// Load files
MCI_PLAY_PARMS mciPlay_hvac;	// Play files
MCI_GENERIC_PARMS mciGeneric_hvac;	// Status of files
MCI_DGV_SETAUDIO_PARMS mciParams_hvac = {0};

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

	m_AVNvolume = 50;
	m_AVNmode = CD;
	m_AVNmute = false;

	m_Tem = 23;
	m_TemCo = 26;
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
	ON_BN_CLICKED(IDC_BUTTON_COOLSEAT	, OnCoolSeat)
	ON_BN_CLICKED(IDC_BUTTON_BODYFOOT	, OnBodyFoot)
	ON_BN_CLICKED(IDC_BUTTON_HEAD		, OnHead)
	ON_BN_CLICKED(IDC_BUTTON_FOOT		, OnFoot)
	ON_BN_CLICKED(IDC_BUTTON_HEADFOOT	, OnHeadFoot)
	ON_BN_CLICKED(IDC_BUTTON_VEN_DN		, OnVenDn)
	ON_BN_CLICKED(IDC_BUTTON_VEN_UP		, OnVenUp)
	ON_BN_CLICKED(IDC_BUTTON_TEM_DN_CO	, OnTemDnCo)
	ON_BN_CLICKED(IDC_BUTTON_TEM_UP_CO	, OnTemUpCo)
	ON_BN_CLICKED(IDC_BUTTON_HEATSEAT_CO, OnHeatSeatCo)
	ON_BN_CLICKED(IDC_BUTTON_COOLSEAT_CO, OnCoolSeatCo)
	ON_BN_CLICKED(IDC_BUTTON_FRONTHEAT	, OnFrontHeat)
	ON_BN_CLICKED(IDC_BUTTON_REARHEAT	, OnRearHeat)
	ON_BN_CLICKED(IDC_BUTTON_AUTO		, OnAuto)
	ON_BN_CLICKED(IDC_BUTTON_AC			, OnAC)
	ON_BN_CLICKED(IDC_BUTTON_MAX_AC		, OnMaxAC)
	ON_BN_CLICKED(IDC_BUTTON_AIR		, OnAir)
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

	ON_WM_DESTROY()
	ON_WM_CLOSE()
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

	SetWindowPos(this, 0, 0, m_bmpBackgroundImage.GetBitmapDimension().cx, m_bmpBackgroundImage.GetBitmapDimension().cy, SW_SHOWMAXIMIZED|SWP_SHOWWINDOW);

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
//	m_TrayMenu.LoadMenu(IDR_TRAYMENU);

	//Add to the tray
	Shell_NotifyIcon(NIM_ADD, &m_TrayData);

	MediaClose();
	MediaClose_HVAC();

	SetVolume(0.2);		//SetVolume(m_AVNvolume);
	

	mciParams.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mciParams.dwValue = 500;

	mciParams_hvac.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mciParams_hvac.dwValue = 0;

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
	DrawVentilation(&memDC, &dc);

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
	int x = 500;
	int y = 150;
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
	int x = 100, y = 50;
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
	int x = 850, y = 50;
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
	int x = 1550, y = 50;
	int max_x = x+600;
	CString text;
	
	CBitmap* pBmp = NULL;	
	bool filledDisplay = false;

	text.Format("VOL.%3u", m_AVNvolume);

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
	int x = 255, y = 655;
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
	int x = 1590, y = 655;
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

void CHolitechDlg::DrawVentilation(CDC* memDC, CClientDC* dc)
{
	int x1 = 976;
	int y = 605;

	switch(m_Ven)
	{
		case 0:
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOff,	x1,	y);
			break;
		case 1:
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn1,	x1,	y);
			break;
		case 2:
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn2,	x1,	y);
			break;
		case 3:
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn3,	x1,	y);
			break;
		case 4:
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn4,	x1,	y);
			break;
		case 5:
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn5,	x1,	y);
			break;
		case 6:
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn6,	x1,	y);
			break;
		case 7:
			DrawBitmapOnDC(memDC, dc, &m_bmpVenOn7,	x1,	y);
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

#define	ROW1 305
#define ROW2 450
#define ROW3 606
#define ROW4 750
#define ROW5 902

	CreateBitmapButton(&m_Mode,			179,	ROW1,	IDB_BITMAP_MODE,		IDB_BITMAP_MODE_1,		IDC_BUTTON_MODE,		"");
	CreateBitmapButton(&m_Preset1,		413,	ROW1,	IDB_BITMAP_P1,			IDB_BITMAP_P1_1,		IDC_BUTTON_PRESET1,		"");
	CreateBitmapButton(&m_Preset2,		632,	ROW1,	IDB_BITMAP_P2,			IDB_BITMAP_P2_1,		IDC_BUTTON_PRESET2,		"");
	CreateBitmapButton(&m_Preset3,		851,	ROW1,	IDB_BITMAP_P3,			IDB_BITMAP_P3_1,		IDC_BUTTON_PRESET3,		"");
	CreateBitmapButton(&m_Preset4,		1071,	ROW1,	IDB_BITMAP_P4,			IDB_BITMAP_P4_1,		IDC_BUTTON_PRESET4,		"");
	CreateBitmapButton(&m_Preset5,		1290,	ROW1,	IDB_BITMAP_P5,			IDB_BITMAP_P5_1,		IDC_BUTTON_PRESET5,		"");
	CreateBitmapButton(&m_Preset6,		1510,	ROW1,	IDB_BITMAP_P6,			IDB_BITMAP_P6_1,		IDC_BUTTON_PRESET6,		"");
	CreateBitmapButton(&m_Pre,			413,	ROW2,	IDB_BITMAP_PRE,			IDB_BITMAP_PRE_1,		IDC_BUTTON_PRE,			"");
	CreateBitmapButton(&m_Play,			632,	ROW2,	IDB_BITMAP_PLAY,		IDB_BITMAP_PLAY_1,		IDC_BUTTON_PLAY,		"");
	CreateBitmapButton(&m_Next,			851,	ROW2,	IDB_BITMAP_NEXT,		IDB_BITMAP_NEXT_1,		IDC_BUTTON_NEXT,		"");
	CreateBitmapButton(&m_Mute,			1071,	ROW2,	IDB_BITMAP_MUTE,		IDB_BITMAP_MUTE_1,		IDC_BUTTON_MUTE,		"");
	CreateBitmapButton(&m_VolDn,		1290,	ROW2,	IDB_BITMAP_VOL_DN,		IDB_BITMAP_VOL_DN_1,	IDC_BUTTON_VOL_DN,		"");
	CreateBitmapButton(&m_VolUp,		1510,	ROW2,	IDB_BITMAP_VOL_UP,		IDB_BITMAP_VOL_UP_1,	IDC_BUTTON_VOL_UP,		"");

	CreateBitmapButton(&m_TemDn,		86,		ROW3,	IDB_BITMAP_TEM_DN,		IDB_BITMAP_TEM_DN_1,	IDC_BUTTON_TEM_DN,		"");
	CreateBitmapButton(&m_TemUp,		374,	ROW3,	IDB_BITMAP_TEM_UP,		IDB_BITMAP_TEM_UP_1,	IDC_BUTTON_TEM_UP,		"");

	m_HeatSeat.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_NOTIFY, CRect(86, ROW4, 86, ROW4), this, IDC_BUTTON_HEATSEAT);
	m_HeatSeat.LoadBitmaps(IDB_BITMAP_HEATSEAT, IDB_BITMAP_HEATSEAT1);
	m_HeatSeat.SizeToContent();

	m_CoolSeat.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_NOTIFY, CRect(291, ROW4, 291, ROW4), this, IDC_BUTTON_COOLSEAT);
	m_CoolSeat.LoadBitmaps(IDB_BITMAP_COOLSEAT, IDB_BITMAP_COOLSEAT1);
	m_CoolSeat.SizeToContent();

	m_BodyFoot.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_NOTIFY, CRect(532, ROW3, 532, ROW3), this, IDC_BUTTON_BODYFOOT);
	m_BodyFoot.LoadBitmaps(IDB_BITMAP_BODYFOOT, IDB_BITMAP_BODYFOOT1);
	m_BodyFoot.SizeToContent();

	m_Head.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_NOTIFY, CRect(736, ROW3, 736, ROW3), this, IDC_BUTTON_HEAD);
	m_Head.LoadBitmaps(IDB_BITMAP_HEAD, IDB_BITMAP_HEAD1);
	m_Head.SizeToContent();

	m_Foot.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_NOTIFY, CRect(532, ROW4, 532, ROW4), this, IDC_BUTTON_FOOT);
	m_Foot.LoadBitmaps(IDB_BITMAP_FOOT, IDB_BITMAP_FOOT1);
	m_Foot.SizeToContent();

	m_HeadFoot.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_NOTIFY, CRect(736, ROW4, 736, ROW4), this, IDC_BUTTON_HEADFOOT);
	m_HeadFoot.LoadBitmaps(IDB_BITMAP_HEADFOOT, IDB_BITMAP_HEADFOOT1);
	m_HeadFoot.SizeToContent();

	CreateBitmapButton(&m_VenDn,	976,	ROW4,	IDB_BITMAP_VEN_DN,		IDB_BITMAP_VEN_DN_1,	IDC_BUTTON_VEN_DN,		"");
	CreateBitmapButton(&m_VenUp,	1184,	ROW4,	IDB_BITMAP_VEN_UP,		IDB_BITMAP_VEN_UP_1,	IDC_BUTTON_VEN_UP,		"");

	CreateBitmapButton(&m_TemDn_Co,	1422,	ROW3,	IDB_BITMAP_TEM_DN_CO,	IDB_BITMAP_TEM_DN_CO_1,	IDC_BUTTON_TEM_DN_CO,	"");
	CreateBitmapButton(&m_TemUp_Co,	1712,	ROW3,	IDB_BITMAP_TEM_UP_CO,	IDB_BITMAP_TEM_UP_CO_1,	IDC_BUTTON_TEM_UP_CO,	"");

	m_HeatSeat_Co.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_NOTIFY, CRect(1422, ROW4, 1422, ROW4), this, IDC_BUTTON_HEATSEAT_CO);
	m_HeatSeat_Co.LoadBitmaps(IDB_BITMAP_HEATSEAT_CO, IDB_BITMAP_HEATSEAT1_CO);
	m_HeatSeat_Co.SizeToContent();

	m_CoolSeat_Co.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_NOTIFY, CRect(1629, ROW4, 1629, ROW4), this, IDC_BUTTON_COOLSEAT_CO);
	m_CoolSeat_Co.LoadBitmaps(IDB_BITMAP_COOLSEAT_CO, IDB_BITMAP_COOLSEAT1_CO);
	m_CoolSeat_Co.SizeToContent();

	m_FrontHeat.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_NOTIFY, CRect(279, ROW5, 279, ROW5), this, IDC_BUTTON_FRONTHEAT);
	m_FrontHeat.LoadBitmaps(IDB_BITMAP_FRONTHEAT, IDB_BITMAP_FRONTHEAT1);
	m_FrontHeat.SizeToContent();

	m_RearHeat.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_NOTIFY, CRect(511, ROW5, 511, ROW5), this, IDC_BUTTON_REARHEAT);
	m_RearHeat.LoadBitmaps(IDB_BITMAP_REARHEAT, IDB_BITMAP_REARHEAT1);
	m_RearHeat.SizeToContent();

	m_Auto.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_NOTIFY, CRect(736, ROW5, 736, ROW5), this, IDC_BUTTON_AUTO);
	m_Auto.LoadBitmaps(IDB_BITMAP_AUTO, IDB_BITMAP_AUTO1);
	m_Auto.SizeToContent();

	m_AC.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_NOTIFY, CRect(960, ROW5, 960, ROW5), this, IDC_BUTTON_AC);
	m_AC.LoadBitmaps(IDB_BITMAP_AC, IDB_BITMAP_AC1);
	m_AC.SizeToContent();

	m_MaxAC.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_NOTIFY, CRect(1184, ROW5, 1184, ROW5), this, IDC_BUTTON_MAX_AC);
	m_MaxAC.LoadBitmaps(IDB_BITMAP_MAX_AC, IDB_BITMAP_MAX_AC1);
	m_MaxAC.SizeToContent();

	m_Air.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_NOTIFY, CRect(1408, ROW5, 1408, ROW5), this, IDC_BUTTON_AIR);
	m_Air.LoadBitmaps(IDB_BITMAP_AIR, IDB_BITMAP_AIR1);
	m_Air.SizeToContent();
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
	LoadBitmapAndCoordinates(&m_bmpVenOff, IDB_BITMAP_VEN_OFF);
	LoadBitmapAndCoordinates(&m_bmpVenOn1, IDB_BITMAP_VEN_ON1);
	LoadBitmapAndCoordinates(&m_bmpVenOn2, IDB_BITMAP_VEN_ON2);
	LoadBitmapAndCoordinates(&m_bmpVenOn3, IDB_BITMAP_VEN_ON3);
	LoadBitmapAndCoordinates(&m_bmpVenOn4, IDB_BITMAP_VEN_ON4);
	LoadBitmapAndCoordinates(&m_bmpVenOn5, IDB_BITMAP_VEN_ON5);
	LoadBitmapAndCoordinates(&m_bmpVenOn6, IDB_BITMAP_VEN_ON6);
	LoadBitmapAndCoordinates(&m_bmpVenOn7, IDB_BITMAP_VEN_ON7);
}

void CHolitechDlg::LoadBitmapAndCoordinates(CBitmap* bmp, UINT resourceID)
{
	BITMAP bitmap;
	
	bmp->LoadBitmap(resourceID);
	bmp->GetBitmap(&bitmap);
	bmp->SetBitmapDimension(bitmap.bmWidth, bitmap.bmHeight);
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

	if(m_AVNmute) return;
	mciSendCommand(dwID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD) & mciParams);
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

		mciOpen_hvac.lpstrElementName = "Ventilating.mp3";

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

	mciSendCommand(dwID_hvac, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD) & mciParams_hvac);
}

void CHolitechDlg::MediaClose_HVAC()
{
	g_playbutt_hvac=FPLAY;																//새로 재생
	mciSendCommand(dwID_hvac,MCI_CLOSE,NULL,(DWORD)(LPVOID)&mciPlay_hvac);					//다른 노래 실행시 mci를 닫고 하기 위해서
	KillTimer(1);
}

void CHolitechDlg::OnMode()
{
	if(m_AVNmute) OnMute();

	MediaClose();

	m_AVNmode = m_AVNmode+1;

	if(m_AVNmode == MODE_END)
		m_AVNmode = RADIO;

	UpdateMode();
}

void CHolitechDlg::OnPreset1()
{
	if(m_AVNmute) OnMute();

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
	if(m_AVNmute) OnMute();

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
	if(m_AVNmute) OnMute();

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
	if(m_AVNmute) OnMute();

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
	if(m_AVNmute) OnMute();

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
	if(m_AVNmute) OnMute();

	MediaClose();

	m_TrackNo = 6;

	if(m_AVNmode == RADIO)
	{
		m_Frequency = 107.7;

		//Setting file path
		//mciOpen.lpstrElementName = "E:\\MyDrive\\0. Work\\AVN GUI\\Source\\R6_Sheeran-Shape Of You.mp3";
		//mciOpen.lpstrElementName = "R6_Sheeran-Shape Of You.mp3";
		mciOpen.lpstrElementName = "R6_IU-Palette.mp3";
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

	//SetMute();

	if(m_AVNmute) mciParams.dwValue = 0;
	else mciParams.dwValue = m_AVNvolume*10;

	mciSendCommand(dwID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD) & mciParams);

	UpdateMute();
}

void CHolitechDlg::OnVolDn()
{
	if(m_AVNmute) return;

	if(m_AVNvolume <= 0) m_AVNvolume = 0;
	else m_AVNvolume = m_AVNvolume-5;

	//	SetVolume(m_AVNvolume);
	mciParams.dwValue = m_AVNvolume*10;
	mciSendCommand(dwID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD) & mciParams);

	UpdateVolume();
}

void CHolitechDlg::OnVolUp()
{
	if(m_AVNmute) return;

	if(m_AVNvolume >= 100) m_AVNvolume = 100;
	else m_AVNvolume = m_AVNvolume+5;

//	SetVolume(m_AVNvolume);
	mciParams.dwValue = m_AVNvolume*10;
	mciSendCommand(dwID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD) & mciParams);

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
			m_HeatSeat.LoadBitmaps(IDB_BITMAP_HEATSEAT, IDB_BITMAP_HEATSEAT1);
			break;
		case 1:
			m_HeatSeat.LoadBitmaps(IDB_BITMAP_HEATSEAT1, IDB_BITMAP_HEATSEAT2);
			break;
		case 2:
			m_HeatSeat.LoadBitmaps(IDB_BITMAP_HEATSEAT2, IDB_BITMAP_HEATSEAT3);
			break;
		case 3:
			m_HeatSeat.LoadBitmaps(IDB_BITMAP_HEATSEAT3, IDB_BITMAP_HEATSEAT);
			break;
	}
	m_HeatSeat.SizeToContent();
	m_HeatSeat.Invalidate();

	m_CSDrv = 0;
	m_CoolSeat.LoadBitmaps(IDB_BITMAP_COOLSEAT, IDB_BITMAP_COOLSEAT1);
	m_CoolSeat.SizeToContent();
	m_CoolSeat.Invalidate();
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
			m_CoolSeat.LoadBitmaps(IDB_BITMAP_COOLSEAT, IDB_BITMAP_COOLSEAT1);
			break;
		case 1:
			m_CoolSeat.LoadBitmaps(IDB_BITMAP_COOLSEAT1, IDB_BITMAP_COOLSEAT2);
			break;
		case 2:
			m_CoolSeat.LoadBitmaps(IDB_BITMAP_COOLSEAT2, IDB_BITMAP_COOLSEAT3);
			break;
		case 3:
			m_CoolSeat.LoadBitmaps(IDB_BITMAP_COOLSEAT3, IDB_BITMAP_COOLSEAT);
			break;
	}
	m_CoolSeat.SizeToContent();
	m_CoolSeat.Invalidate();

	m_HSDrv = 0;
	m_HeatSeat.LoadBitmaps(IDB_BITMAP_HEATSEAT, IDB_BITMAP_HEATSEAT1);
	m_HeatSeat.SizeToContent();
	m_HeatSeat.Invalidate();
}

void CHolitechDlg::OnBodyFoot()
{
	m_BF = !m_BF;

	if(m_BF)
	{
		m_BodyFoot.LoadBitmaps(IDB_BITMAP_BODYFOOT1, IDB_BITMAP_BODYFOOT);
	}
	else
	{
		m_BodyFoot.LoadBitmaps(IDB_BITMAP_BODYFOOT, IDB_BITMAP_BODYFOOT1);
	}
	m_BodyFoot.SizeToContent();
	m_BodyFoot.Invalidate();

	m_H = FALSE;
	m_Head.LoadBitmaps(IDB_BITMAP_HEAD,	IDB_BITMAP_HEAD1);
	m_Head.SizeToContent();
	m_Head.Invalidate();

	m_F = FALSE;
	m_Foot.LoadBitmaps(IDB_BITMAP_FOOT,	IDB_BITMAP_FOOT1);
	m_Foot.SizeToContent();
	m_Foot.Invalidate();

	m_HF = FALSE;
	m_HeadFoot.LoadBitmaps(IDB_BITMAP_HEADFOOT,	IDB_BITMAP_HEADFOOT1);
	m_HeadFoot.SizeToContent();
	m_HeadFoot.Invalidate();
}

void CHolitechDlg::OnHead()
{
	m_H = !m_H;

	if(m_H)
	{
		m_Head.LoadBitmaps(IDB_BITMAP_HEAD1,	IDB_BITMAP_HEAD);
	}
	else
	{
		m_Head.LoadBitmaps(IDB_BITMAP_HEAD,	IDB_BITMAP_HEAD1);
	}
	m_Head.SizeToContent();
	m_Head.Invalidate();

	m_BF = FALSE;
	m_BodyFoot.LoadBitmaps(IDB_BITMAP_BODYFOOT, IDB_BITMAP_BODYFOOT1);
	m_BodyFoot.SizeToContent();
	m_BodyFoot.Invalidate();

	m_F = FALSE;
	m_Foot.LoadBitmaps(IDB_BITMAP_FOOT,	IDB_BITMAP_FOOT1);
	m_Foot.SizeToContent();
	m_Foot.Invalidate();

	m_HF = FALSE;
	m_HeadFoot.LoadBitmaps(IDB_BITMAP_HEADFOOT,	IDB_BITMAP_HEADFOOT1);
	m_HeadFoot.SizeToContent();
	m_HeadFoot.Invalidate();
}

void CHolitechDlg::OnFoot()
{
	m_F = !m_F;

	if(m_F)
	{
		m_Foot.LoadBitmaps(IDB_BITMAP_FOOT1,	IDB_BITMAP_FOOT);
	}
	else
	{
		m_Foot.LoadBitmaps(IDB_BITMAP_FOOT,	IDB_BITMAP_FOOT1);
	}
	m_Foot.SizeToContent();
	m_Foot.Invalidate();

	m_BF = FALSE;
	m_BodyFoot.LoadBitmaps(IDB_BITMAP_BODYFOOT, IDB_BITMAP_BODYFOOT1);
	m_BodyFoot.SizeToContent();
	m_BodyFoot.Invalidate();

	m_H = FALSE;
	m_Head.LoadBitmaps(IDB_BITMAP_HEAD,	IDB_BITMAP_HEAD1);
	m_Head.SizeToContent();
	m_Head.Invalidate();

	m_HF = FALSE;
	m_HeadFoot.LoadBitmaps(IDB_BITMAP_HEADFOOT,	IDB_BITMAP_HEADFOOT1);
	m_HeadFoot.SizeToContent();
	m_HeadFoot.Invalidate();
}

void CHolitechDlg::OnHeadFoot()
{
	m_HF = !m_HF;

	if(m_HF)
	{
		m_HeadFoot.LoadBitmaps(IDB_BITMAP_HEADFOOT1,	IDB_BITMAP_HEADFOOT);
	}
	else
	{
		m_HeadFoot.LoadBitmaps(IDB_BITMAP_HEADFOOT,	IDB_BITMAP_HEADFOOT1);
	}
	m_HeadFoot.SizeToContent();
	m_HeadFoot.Invalidate();

	m_BF = FALSE;
	m_BodyFoot.LoadBitmaps(IDB_BITMAP_BODYFOOT, IDB_BITMAP_BODYFOOT1);
	m_BodyFoot.SizeToContent();
	m_BodyFoot.Invalidate();

	m_H = FALSE;
	m_Head.LoadBitmaps(IDB_BITMAP_HEAD,	IDB_BITMAP_HEAD1);
	m_Head.SizeToContent();
	m_Head.Invalidate();

	m_F = FALSE;
	m_Foot.LoadBitmaps(IDB_BITMAP_FOOT,	IDB_BITMAP_FOOT1);
	m_Foot.SizeToContent();
	m_Foot.Invalidate();
}

void CHolitechDlg::OnVenDn()
{
	if(m_Ven > 0) m_Ven--;

	DrawBackground();

	mciParams_hvac.dwValue = m_Ven*100;
	mciSendCommand(dwID_hvac, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD) & mciParams_hvac);

	if(m_HVACmaxac) OnMaxAC();
}

void CHolitechDlg::OnVenUp()
{
	if(m_Ven < 7) m_Ven++;

	DrawBackground();

	mciParams_hvac.dwValue = m_Ven*100;
	mciSendCommand(dwID_hvac, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD) & mciParams_hvac);
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
			m_HeatSeat_Co.LoadBitmaps(IDB_BITMAP_HEATSEAT_CO, IDB_BITMAP_HEATSEAT1_CO);
			break;
		case 1:
			m_HeatSeat_Co.LoadBitmaps(IDB_BITMAP_HEATSEAT1_CO, IDB_BITMAP_HEATSEAT2_CO);
			break;
		case 2:
			m_HeatSeat_Co.LoadBitmaps(IDB_BITMAP_HEATSEAT2_CO, IDB_BITMAP_HEATSEAT3_CO);
			break;
		case 3:
			m_HeatSeat_Co.LoadBitmaps(IDB_BITMAP_HEATSEAT3_CO, IDB_BITMAP_HEATSEAT_CO);
			break;
	}
	m_HeatSeat_Co.SizeToContent();
	m_HeatSeat_Co.Invalidate();

	m_CSCo = 0;
	m_CoolSeat_Co.LoadBitmaps(IDB_BITMAP_COOLSEAT_CO, IDB_BITMAP_COOLSEAT1_CO);
	m_CoolSeat_Co.SizeToContent();
	m_CoolSeat_Co.Invalidate();
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
			m_CoolSeat_Co.LoadBitmaps(IDB_BITMAP_COOLSEAT_CO, IDB_BITMAP_COOLSEAT1_CO);
			break;
		case 1:
			m_CoolSeat_Co.LoadBitmaps(IDB_BITMAP_COOLSEAT1_CO, IDB_BITMAP_COOLSEAT2_CO);
			break;
		case 2:
			m_CoolSeat_Co.LoadBitmaps(IDB_BITMAP_COOLSEAT2_CO, IDB_BITMAP_COOLSEAT3_CO);
			break;
		case 3:
			m_CoolSeat_Co.LoadBitmaps(IDB_BITMAP_COOLSEAT3_CO, IDB_BITMAP_COOLSEAT_CO);
			break;
	}
	m_CoolSeat_Co.SizeToContent();
	m_CoolSeat_Co.Invalidate();

	m_HSCo = 0;
	m_HeatSeat_Co.LoadBitmaps(IDB_BITMAP_HEATSEAT_CO, IDB_BITMAP_HEATSEAT1_CO);
	m_HeatSeat_Co.SizeToContent();
	m_HeatSeat_Co.Invalidate();
}

void CHolitechDlg::OnFrontHeat()
{
	m_FH = !m_FH;

	if(m_FH)
	{
		m_FrontHeat.LoadBitmaps(IDB_BITMAP_FRONTHEAT1, IDB_BITMAP_FRONTHEAT);
	}
	else
	{
		m_FrontHeat.LoadBitmaps(IDB_BITMAP_FRONTHEAT, IDB_BITMAP_FRONTHEAT1);
	}
	m_FrontHeat.SizeToContent();
	m_FrontHeat.Invalidate();
}

void CHolitechDlg::OnRearHeat()
{
	m_RH = !m_RH;

	if(m_RH)
	{
		m_RearHeat.LoadBitmaps(IDB_BITMAP_REARHEAT1, IDB_BITMAP_REARHEAT);
	}
	else
	{
		m_RearHeat.LoadBitmaps(IDB_BITMAP_REARHEAT, IDB_BITMAP_REARHEAT1);
	}
	m_RearHeat.SizeToContent();
	m_RearHeat.Invalidate();
}

void CHolitechDlg::OnAuto()
{
	m_HVACauto = !m_HVACauto;

	if(m_HVACauto)
	{
		m_Auto.LoadBitmaps(IDB_BITMAP_AUTO1, IDB_BITMAP_AUTO);

		if(!m_H) OnHead();

		if(m_HVACmaxac) OnMaxAC();

		mciParams_hvac.dwValue = 500;
		mciSendCommand(dwID_hvac, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD) & mciParams_hvac);
	}
	else
	{
		m_Auto.LoadBitmaps(IDB_BITMAP_AUTO, IDB_BITMAP_AUTO1);

		mciParams_hvac.dwValue = m_Ven*100;
		mciSendCommand(dwID_hvac, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD) & mciParams_hvac);
	}
	m_Auto.SizeToContent();
	m_Auto.Invalidate();
}

void CHolitechDlg::OnAC()
{
	m_HVACac = !m_HVACac;

	if(m_HVACac)
	{
		m_AC.LoadBitmaps(IDB_BITMAP_AC1, IDB_BITMAP_AC);
	}
	else
	{
		m_AC.LoadBitmaps(IDB_BITMAP_AC, IDB_BITMAP_AC1);
	}
	m_AC.SizeToContent();
	m_AC.Invalidate();
}

void CHolitechDlg::OnMaxAC()
{
	m_HVACmaxac = !m_HVACmaxac;

	if(m_HVACmaxac)
	{
		m_MaxAC.LoadBitmaps(IDB_BITMAP_MAX_AC1, IDB_BITMAP_MAX_AC);

		// Auto Off
		m_HVACauto = FALSE;
		m_Auto.LoadBitmaps(IDB_BITMAP_AUTO, IDB_BITMAP_AUTO1);
		m_Auto.SizeToContent();
		m_Auto.Invalidate();

		// A/C On
		m_HVACac = TRUE;
		m_AC.LoadBitmaps(IDB_BITMAP_AC1, IDB_BITMAP_AC);
		m_AC.SizeToContent();
		m_AC.Invalidate();

		// Ventilation Max
		m_Ven = 7;	
		DrawBackground();

		mciParams_hvac.dwValue = 1000;
		mciSendCommand(dwID_hvac, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD) & mciParams_hvac);
	}
	else
	{
		m_MaxAC.LoadBitmaps(IDB_BITMAP_MAX_AC, IDB_BITMAP_MAX_AC1);

		mciParams_hvac.dwValue = m_Ven*100;
		mciSendCommand(dwID_hvac, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD) & mciParams_hvac);
	}
	m_MaxAC.SizeToContent();
	m_MaxAC.Invalidate();
}

void CHolitechDlg::OnAir()
{
	m_HVACair = !m_HVACair;

	if(m_HVACair)
	{
		m_Air.LoadBitmaps(IDB_BITMAP_AIR1, IDB_BITMAP_AIR);
	}
	else
	{
		m_Air.LoadBitmaps(IDB_BITMAP_AIR, IDB_BITMAP_AIR1);
	}
	m_Air.SizeToContent();
	m_Air.Invalidate();
}

void CHolitechDlg::OnTimer(UINT nIDEvent) 
{
#if 1

	static bool VenFlag = FALSE;

	g_playtime++;
	if(g_playtime>=g_tend)
	{
		//OnNext();
		//g_playbutt=FPLAY;														//현재 노래 재생이 끝났으므로
	}


	// Ventilating

	//if((VenFlag==TRUE)&&((m_Ven==0)&&(m_HVACauto==FALSE)&&(m_HVACmaxac==FALSE)))
	if((m_Ven==0)&&(m_HVACauto==FALSE)&&(m_HVACmaxac==FALSE))
	{
		if(VenFlag==TRUE)
		{
			VenFlag = FALSE;

			g_playbutt_hvac = FPAUSE;
			MediaPlay_HVAC();
		}
	}

	//if((VenFlag==FALSE)&&((m_Ven>0)||(m_HVACauto==TRUE)||(m_HVACmaxac==TRUE)))
	if((m_Ven>0)||(m_HVACauto==TRUE)||(m_HVACmaxac==TRUE))
	{
		if(VenFlag==FALSE)
		{
			VenFlag = TRUE;

			g_playbutt_hvac = FPLAY;
			MediaPlay_HVAC();
		}
	}

	if(m_HVACauto)
	{
		if(m_Ven == 7) m_Ven = 0;
		else m_Ven++;

		DrawBackground();
	}

	g_playtime_hvac++;
	if((g_playtime_hvac>=(g_tend_hvac-15))&&(VenFlag==TRUE))
	{
		g_playbutt_hvac = FPLAY;														//현재 노래 재생이 끝났으므로
		MediaPlay_HVAC();
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
