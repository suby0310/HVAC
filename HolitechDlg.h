// HolitechDlg.h : header file
//

#if !defined(AFX_HOLITECH_H__9D7506DB_8939_4A97_A60A_6451826C7D93__INCLUDED_)
#define AFX_HOLITECH_H__9D7506DB_8939_4A97_A60A_6451826C7D93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "mmsystem.h"

//Application Title to be displayed in the title bar
#define TITLE "HOLITECH GUI"

//The Dialog height and width
#define DLGHEIGHT	255	//195
#define DLGWIDTH	560

//Volume Coordinates
#define VOLUME_MIN_X	1000	//357
#define VOLUME_MAX_X	1500	//446
#define VOLUME_MIN_Y	700		//157
#define VOLUME_MAX_Y	800		//175

//Volume Definitions
#define VOLUME_MIN	0		//0% - 100%
#define VOLUME_MAX	100

//Transparent Color definition
#define TRANSPARENT_COLOR	0x00FF00

//Define for the Tray notify message
#define WM_TRAY_ICON_NOTIFY_MESSAGE	WM_USER + 1

//Timer ID/Time definitions
#define TIMER_FIND_DEVICE		0
#define TIMER_FIND_DEVICE_TIME	500

#define TIMER_POLL_DEVICE		1
#define TIMER_POLL_DEVICE_TIME	40

#define TIMER_AUDIO_STREAM		2
#define TIMER_AUDIO_STREAM_TIME	25

#define TIMER_BUTTON_DOWN		3
#define TIMER_BUTTON_DOWN_TIME	1750

#define TIMER_SCAN_UP			4
#define TIMER_SCAN_DOWN			5
#define TIMER_SCANPRESET_UP		6
#define TIMER_SCANPRESET_DOWN	7

#define TIMER_VOLUME_MOVING		8

#define TIMER_TUNE_DELAY		9
#define TIMER_TUNE_DELAY_TIME	750

//Keycode definitions
#define KEY_RIGHT	0x27
#define KEY_LEFT	0x25
#define KEY_UP		0x26
#define KEY_DOWN	0x28
#define KEY_F1	0x70
#define KEY_F2	0x71
#define KEY_F3	0x72
#define KEY_F4	0x73
#define KEY_F5	0x74
#define KEY_F6	0x75
#define KEY_F7	0x76
#define KEY_F8	0x77
#define KEY_F9	0x78
#define KEY_F10 0x79
#define KEY_F11 0x7A
#define KEY_F12	0x7B

//Current preset struct
typedef struct CurrentPreset
{
	int presetNum;
	bool onPreset;
} CurrentPreset;

typedef enum AVN_MODE
{
	RADIO = 0,
	CD,
	MODE_END
};

/////////////////////////////////////////////////////////////////////////////
// CHolitechDlg dialog

class CHolitechDlg : public CDialog
{
// Construction
public:
	CHolitechDlg(CWnd* pParent = NULL);	// standard constructor

	void KeyUp(BYTE key);
	CPoint m_DownPoint;
	BYTE m_KeyDown;

// Dialog Data
	//{{AFX_DATA(CHolitechDlg)
	enum { IDD = IDD_MAIN_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHolitechDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//Tooltip control
	CToolTipCtrl* m_pToolTip;

	HICON m_hIcon;

	//Tray variables
	NOTIFYICONDATA m_TrayData;
	CMenu m_TrayMenu;

	//Always on top boolean
	bool m_AlwaysOnTop;

	//Point variables for mouse click/movement
	CPoint m_Point;
	CPoint m_MovePoint;

	//Volume/Display/Radio control variables
 	BYTE m_AVNmode;
	char m_AVNvolume;
	BOOL m_AVNmute;

	float m_Frequency;
	BYTE m_TrackNo;

	bool m_VolumeToggle;
	CurrentPreset m_CurrentPreset;
	bool m_Text;

	//HVAC control variables
	BYTE m_Tem, m_TemCo, m_Ven, m_HSDrv, m_CSDrv, m_HSCo, m_CSCo;
	bool m_BF, m_H, m_F, m_HF, m_FH, m_RH, m_HVACauto, m_HVACac, m_HVACmaxac, m_HVACair;

	//Temp Variables predefined for timer functions
	BYTE m_Status;

	//Bitmaps for the Buttons
	CBitmapButton m_Close;
	CBitmapButton m_Restore;
	CBitmapButton m_Minimize;
	CBitmapButton m_Mode;
	CBitmapButton m_Preset1;
	CBitmapButton m_Preset2;
	CBitmapButton m_Preset3;
	CBitmapButton m_Preset4;
	CBitmapButton m_Preset5;
	CBitmapButton m_Preset6;
	CBitmapButton m_Pre;
	CBitmapButton m_Play;
	CBitmapButton m_Next;
	CBitmapButton m_Mute;
	CBitmapButton m_VolDn;
	CBitmapButton m_VolUp;
	CBitmapButton m_TemDn;
	CBitmapButton m_TemUp;
	CBitmapButton m_HeatSeat;
	CBitmapButton m_CoolSeat;
	CBitmapButton m_VenDn;
	CBitmapButton m_VenUp;
	CBitmapButton m_TemDn_Co;
	CBitmapButton m_TemUp_Co;
	CBitmapButton m_HeatSeat_Co;
	CBitmapButton m_CoolSeat_Co;
	CBitmapButton m_FrontHeat;
	CBitmapButton m_RearHeat;
	CBitmapButton m_Auto;
	CBitmapButton m_AC;
	CBitmapButton m_MaxAC;
	CBitmapButton m_Air;
	CBitmapButton m_BodyFoot;
	CBitmapButton m_Head;
	CBitmapButton m_Foot;
	CBitmapButton m_HeadFoot;

	//Bitmaps for the Window and Letters
	CBitmap	m_bmpBackgroundImage;
	CBitmap m_bmpMHz;
	CBitmap m_bmpDotLarge;
	CBitmap m_bmpSpaceLarge;
	CBitmap	m_bmp0Large;
	CBitmap m_bmp1Large;
	CBitmap	m_bmp2Large;
	CBitmap m_bmp3Large;
	CBitmap	m_bmp4Large;
	CBitmap m_bmp5Large;
	CBitmap	m_bmp6Large;
	CBitmap m_bmp7Large;
	CBitmap	m_bmp8Large;
	CBitmap m_bmp9Large;
	CBitmap m_bmpALarge;
	CBitmap m_bmpBLarge;
	CBitmap m_bmpCLarge;
	CBitmap m_bmpDLarge;
	CBitmap m_bmpELarge;
	CBitmap m_bmpFLarge;
	CBitmap m_bmpGLarge;
	CBitmap m_bmpHLarge;
	CBitmap m_bmpILarge;
	CBitmap m_bmpJLarge;
	CBitmap m_bmpKLarge;
	CBitmap m_bmpLLarge;
	CBitmap m_bmpMLarge;
	CBitmap m_bmpNLarge;
	CBitmap m_bmpOLarge;
	CBitmap m_bmpPLarge;
	CBitmap m_bmpQLarge;
	CBitmap m_bmpRLarge;
	CBitmap m_bmpSLarge;
	CBitmap m_bmpTLarge;
	CBitmap m_bmpULarge;
	CBitmap m_bmpVLarge;
	CBitmap m_bmpWLarge;
	CBitmap m_bmpXLarge;
	CBitmap m_bmpYLarge;
	CBitmap m_bmpZLarge;
	CBitmap m_bmpDotSmall;
	CBitmap m_bmpDashSmall;
	CBitmap m_bmpApostropheSmall;
	CBitmap m_bmpExclamationSmall;
	CBitmap m_bmpAmpersandSmall;
	CBitmap m_bmpCommaSmall;
	CBitmap m_bmpSpaceSmall;
	CBitmap	m_bmp0Small;
	CBitmap m_bmp1Small;
	CBitmap	m_bmp2Small;
	CBitmap m_bmp3Small;
	CBitmap	m_bmp4Small;
	CBitmap m_bmp5Small;
	CBitmap	m_bmp6Small;
	CBitmap m_bmp7Small;
	CBitmap	m_bmp8Small;
	CBitmap m_bmp9Small;
	CBitmap m_bmpASmall;
	CBitmap m_bmpBSmall;
	CBitmap m_bmpCSmall;
	CBitmap m_bmpDSmall;
	CBitmap m_bmpESmall;
	CBitmap m_bmpFSmall;
	CBitmap m_bmpGSmall;
	CBitmap m_bmpHSmall;
	CBitmap m_bmpISmall;
	CBitmap m_bmpJSmall;
	CBitmap m_bmpKSmall;
	CBitmap m_bmpLSmall;
	CBitmap m_bmpMSmall;
	CBitmap m_bmpNSmall;
	CBitmap m_bmpOSmall;
	CBitmap m_bmpPSmall;
	CBitmap m_bmpQSmall;
	CBitmap m_bmpRSmall;
	CBitmap m_bmpSSmall;
	CBitmap m_bmpTSmall;
	CBitmap m_bmpUSmall;
	CBitmap m_bmpVSmall;
	CBitmap m_bmpWSmall;
	CBitmap m_bmpXSmall;
	CBitmap m_bmpYSmall;
	CBitmap m_bmpZSmall;
	CBitmap m_bmpZSpecial;
	CBitmap m_bmpVenOff;
	CBitmap m_bmpVenOn1;
	CBitmap m_bmpVenOn2;
	CBitmap m_bmpVenOn3;
	CBitmap m_bmpVenOn4;
	CBitmap m_bmpVenOn5;
	CBitmap m_bmpVenOn6;
	CBitmap m_bmpVenOn7;

	//Drawing functions
	void DrawBackground();
	void DrawPannel(CDC* memDC, CClientDC* dc);
	void DrawMode(CDC* memDC, CClientDC* dc);
	void DrawMute(CDC* memDC, CClientDC* dc);
	void DrawVolume(CDC* memDC, CClientDC* dc);
	void DrawTemperature(CDC* memDC, CClientDC* dc);
	void DrawTemperatureCo(CDC* memDC, CClientDC* dc);	
	void DrawVentilation(CDC* memDC, CClientDC* dc);
	void DrawBitmapOnDC(CDC* memDC, CClientDC* cDC, CBitmap* bmp, int x, int y);
	void DrawBitmapOnDC(CDC* memDC, CClientDC* cDC, CBitmap* bmp, int x, int y, UINT transparentColor);

	//Update functions
	void UpdatePannel();
	void UpdateMode();
	void UpdateMute();
	void UpdateVolume();

	void UpdateTemperature();
/*
	void UpdateTemperatureCo();
	void UpdateLampVentilation();
*/
	//Bitmap Button functions
	void DisplayButtons();
	void LoadBitmaps();
	void LoadBitmapAndCoordinates(CBitmap* bmp, UINT resourceID);
	void CreateBitmapButton(CBitmapButton* bitmapButton, int left, int top, UINT upResource, UINT downresource, UINT buttonResource, CString toolTipText);
	CBitmap* GetAlphanumericLargeBitmap(char c);
	CBitmap* GetAlphanumericSmallBitmap(char c);

	//Tray function
	void ShowTrayMenu(CPoint pt);

	//Radio Control functions
	bool TuneStation(bool tuneUp);

	//Endpoint Volume Control
	void SetVolume(float fPercent);
	void SetMute();

	//Hnadling MCI
	void MediaPlay();
	void MediaStop();
	void MediaClose();

	//Hnadling MCI HVAC
	void MediaPlay_HVAC();
	void MediaClose_HVAC();

	// Generated message map functions
	//{{AFX_MSG(CHolitechDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTrayNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCancel();
	afx_msg void OnMinimize();
	afx_msg LRESULT OnCommandHelp(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMode();
	afx_msg void OnPreset1();
	afx_msg void OnPreset2();
	afx_msg void OnPreset3();
	afx_msg void OnPreset4();
	afx_msg void OnPreset5();
	afx_msg void OnPreset6();
	afx_msg void OnPre();
	afx_msg void OnPlay();
	afx_msg void OnNext();
	afx_msg void OnMute();
	afx_msg void OnVolDn();
	afx_msg void OnVolUp();
	afx_msg void OnTemDn();
	afx_msg void OnTemUp();
	afx_msg void OnHeatSeat();
	afx_msg void OnCoolSeat();
	afx_msg void OnBodyFoot();
	afx_msg void OnHead();
	afx_msg void OnFoot();
	afx_msg void OnHeadFoot();
	afx_msg void OnVenDn();
	afx_msg void OnVenUp();
	afx_msg void OnTemDnCo();
	afx_msg void OnTemUpCo();
	afx_msg void OnHeatSeatCo();
	afx_msg void OnCoolSeatCo();
	afx_msg void OnFrontHeat();
	afx_msg void OnRearHeat();
	afx_msg void OnAuto();
	afx_msg void OnAC();
	afx_msg void OnMaxAC();
	afx_msg void OnAir();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOLITECH_H__9D7506DB_8939_4A97_A60A_6451826C7D93__INCLUDED_)
