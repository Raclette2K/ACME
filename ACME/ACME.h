/*
======================================================================

ACME.h - Main header file of ACME 

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

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "Config.h"

// CACMEApp:
// See ACME.cpp for the implementation of this class
//

class CACMEApp : public CWinApp
{
public:
	CACMEApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CACMEApp ACMEApp;
