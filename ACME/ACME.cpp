/*
---------------------------------------------
Huge thanks to Hydro for making the website.
Go check him out at : http://hydrocorp.space
---------------------------------------------

============================ DISCLAIMER ==============================
This project is not a "definitive" product and will never be one.
This is just a project I made to improve my skills in C++ so you can
imagine that the code is not so clean and efficient.
You can still use it or improve it if you want but in any case I'm not
responsible for any problems that may happen to you.
----------------------------------------------------------------------
THIS IS NOT INTENDED TO BE A "CHEAT" FOR GAMES. IT IS JUST A TOOL !
YOU CAN NOT HOLD ME RESPONSIBLE FOR ANYTHING THAT MAY HAPPEN TO YOU.
======================================================================

*/

/*
======================================================================

ACME.cpp - Entry point of the program.

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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CACMEApp

BEGIN_MESSAGE_MAP(CACMEApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CACMEApp construction

CACMEApp::CACMEApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_NO_AUTOSAVE;
}


// The one and only CACMEApp object
CACMEApp ACMEApp;


// CACMEApp initialization

BOOL CACMEApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application manifest specifies use of ComCtl32.dll version 6 or later to enable visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	// Set this to include all the common control classes you want to use in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Create a unique pointer to the Config object to be able to save and load the configuration from the ini file.
	std::unique_ptr<Config> conf = std::make_unique<Config>(); //Generate Config object
	
	// Create a unique pointer to the AutoClick object
	std::unique_ptr<ACME::AutoClick> autoClick = std::make_unique < ACME::AutoClick > (conf->getAllValues(), conf->getInputs());

	// Start the main windows (which is a dialog btw) with the Config pointer as argument [using move to send it]
	CACMEDlg dlg(std::move(conf),std::move(autoClick));

	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	autoClick.reset(nullptr);
	conf.reset(nullptr);


	// Since the dialog has been closed, return FALSE so that we exit the application, rather than start the application's message pump.
	return FALSE;
}

