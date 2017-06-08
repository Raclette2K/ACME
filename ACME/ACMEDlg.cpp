/*
======================================================================

ACMEDlg.cpp - Main dialog (main window) of the program. 

Copyright (c) 2017 Pierre Boisselier. All rights reserved.

**********************************************************************

This file is part of ACME.

ACME is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ACME is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ACME.  If not, see <http://www.gnu.org/licenses/>.

======================================================================
*/

#include "stdafx.h"
#include "ACME.h"
#include "ACMEDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	
};
CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX){}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)	
END_MESSAGE_MAP()

//////////////////////
// CACMEDlg dialog //
/////////////////////
CACMEDlg::CACMEDlg(std::shared_ptr<Config> conf,std::unique_ptr<ACME::AutoClick> autoClick,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ACME_DIALOG, pParent)
{
	// Load the app icon
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// Move the config pointer to the private variable for further user
	m_conf = std::move(conf);

	m_auto_click = std::move(autoClick);
}

// Data "linker"
void CACMEDlg::DoDataExchange(CDataExchange* pDX)
{
	// Define events
	CDialogEx::DoDataExchange(pDX);

	// Link UI IDs to variables
	DDX_Control(pDX, IDC_TRIGGERMODE, m_CTriggerMode);
	DDX_Control(pDX, IDC_INTERVALMODE, m_CIntervalMode);
	DDX_Control(pDX, IDC_RANDMIN, m_CRandMin);
	DDX_Control(pDX, IDC_RANDMAX, m_CRandMax);
	DDX_Control(pDX, IDC_CONSTDELAY, m_CConstDelay);
	DDX_Control(pDX, IDC_ACMESTATE_STR, m_txt_acmestate);
	DDX_Control(pDX, IDC_CLICKTODO, m_CClickKey);
	DDX_Control(pDX, IDC_KEYSELECTER, m_CBSelectkey);
}

// Event handler
BEGIN_MESSAGE_MAP(CACMEDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	// When the apply button is clicked -> Call "OnBnClickedApplyBtn"
	ON_BN_CLICKED(IDC_APPLYBTN, &CACMEDlg::OnBnClickedApplyBtn)
	
	ON_BN_CLICKED(IDC_KEYSELECTER, &CACMEDlg::OnBnClickedKeyselecter)
END_MESSAGE_MAP()


// Window initialization
BOOL CACMEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Display values from config into the UI 
	m_CTriggerMode.SetCurSel(m_conf->getTriggerMode()); 
	m_CIntervalMode.SetCurSel(m_conf->getIntervalMode()); 
	m_CRandMin.SetWindowTextW(CA2T(std::to_string(m_conf->getRandMin()).c_str()));
	m_CRandMax.SetWindowTextW(CA2T(std::to_string(m_conf->getRandMax()).c_str()));
	m_CConstDelay.SetWindowTextW(CA2T(std::to_string(m_conf->getConstDelay()).c_str()));
	m_CClickKey.SetCurSel(m_conf->getClick());
	// Start the AutoClick thread.
	m_auto_click->StartClick();
	
	m_txt_acmestate.SetWindowTextW(L"ACME loaded !");

	return TRUE;  // return TRUE unless you set the focus to a control
}

// System handler
void CACMEDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == SC_CLOSE)
	{
		m_auto_click->~AutoClick();
		CDialogEx::OnSysCommand(nID, lParam);
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CACMEDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CACMEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// On apply button clicked
void CACMEDlg::OnBnClickedApplyBtn()
{

	// Get the values from the delay input
	CString _temp_RandMin;
	CString _temp_RandMax;
	CString _temp_ConstDelay;
	m_CRandMin.GetWindowTextW(_temp_RandMin);
	m_CRandMax.GetWindowTextW(_temp_RandMax);
	m_CConstDelay.GetWindowTextW(_temp_ConstDelay);

	// Modify config with the value entered
	m_conf->setDelays(_ttoi(_temp_RandMin), _ttoi(_temp_RandMax), _ttoi(_temp_ConstDelay));
	m_conf->setModes(m_CTriggerMode.GetCurSel(), m_CIntervalMode.GetCurSel(), m_CClickKey.GetCurSel());
	m_conf->setKey(_temp_Key);
	
	// Force the config to be updated and thus saving it into the ini file
	m_conf->updateConf(); 

	// Update the autoclick values 
	m_auto_click->StopClick();
	m_auto_click->UpdateValues(m_conf->getAllValues(),m_conf->getInputs());
	m_auto_click->StartClick();

	// Change the text to indicate that the changes have been applied
	m_txt_acmestate.SetWindowTextW(L"Changes applied !");
}


// On exit button
void CACMEDlg::OnBnClickedExit()
{
	CDialogEx::OnOK();
}

// On select key button
void CACMEDlg::OnBnClickedKeyselecter()
{
	if (!_selectKeyInUse) {
		
		_selectKeyInUse = true; // An instance is already working
		m_auto_click->StopClick(); // Prevent the user from autoclicking while selecting the key (especially in SwitchMode)

		// Change the text to indicate that the user must send an input
		m_txt_acmestate.SetWindowTextW(L"Waiting for input...");

		// Create a MSG object to get the windows message
		MSG msg = {};
		// Get the currend window handle
		HWND hwnd = ::GetForegroundWindow();

		// Wait for an input (which isn't the MOUSEMOVE event)
		do {
			// Get the message to the msg buffer
			PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE | PM_QS_INPUT);
			// Translate the message 
			TranslateMessage(&msg);
			// Dispatch the message
			DispatchMessage(&msg);
		} while ((msg.message >= 0x200 && msg.message <= 0x206) || msg.message == 0xA0); // Prevent the use of the right click and right click. Also cancel WM_MOUSEMOVE and the event of the mouse leaving the windows considered as Input (?!). [0x200 = WM_MOUSEMOVe ; 0x206 = WM_RBUTTONDBLCLK]

		// Set the trigger key with the input value 
		if (msg.wParam == 65568)			// For XBUTTON1
			_temp_Key = VK_XBUTTON1;
		else if (msg.wParam == 131136)		// For XBUTTON2
			_temp_Key = VK_XBUTTON2;
		else
			_temp_Key = (int)msg.wParam;	// For everything else

		// Change the text to indicate that the trigger key was changed
		m_txt_acmestate.SetWindowTextW(L"Trigger key changed ! Use Apply.");
		
		_selectKeyInUse = false; // The instance has finished
		
		// The StartClick() will be called once the user applies change, until nothing happens
	}

}
                            