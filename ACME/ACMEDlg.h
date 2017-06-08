/*
======================================================================

ACME.h - Header file of ACMEDlg.h

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

#pragma once

#include "Config.h"
#include <memory>
#include "afxwin.h"
#include "AutoClick.h"
// CACMEDlg dialog
class CACMEDlg : public CDialogEx
{
// Construction
public:
	CACMEDlg(std::shared_ptr<Config> conf,std::unique_ptr<ACME::AutoClick> autoClick, CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ACME_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	

// Implementation
protected:
	// Icon
	HICON m_hIcon;
	
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	
private:
	CComboBox m_CTriggerMode;						// Combo box for the Trigger Mode selection
	CComboBox m_CIntervalMode;						// Combo box for the Interval Mode selection
	CEdit m_CRandMin;								// Edit zone for the minimum random delay
	CEdit m_CRandMax;								// Edit zone for the maximum random delay
	CEdit m_CConstDelay;							// Edit zone for the constant delay
	std::shared_ptr<Config> m_conf;					// Pointer to the config object declared in ACME.cpp

	int _temp_Key = 0;								// Represent the VK code for the trigger button

	std::unique_ptr<ACME::AutoClick> m_auto_click;	// Pointer to the AutoClick object 
	
	CButton m_IDC_ExitBtn;							// Button used for exiting the app
	CStatic m_txt_acmestate;						// Text at the lower left that indicates what happens to the user
	
	CComboBox m_CClickKey;							// Combo box for selecting which type of click 
	CButton m_CBSelectkey;							// The Select Key button (for future use like changing the text to the current assigned key)

	bool _selectKeyInUse = false;					// Prevent the user from launching multiple selectkey event
public:
	afx_msg void OnBnClickedExit();					// When the user push the Exit Button
	afx_msg void OnBnClickedKeyselecter();			// When the user push the Select Key button
	afx_msg void OnBnClickedApplyBtn();				// When the user push the Apply Button



};
