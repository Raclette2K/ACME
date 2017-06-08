/*
======================================================================

Config.h - Header file of Config.cpp

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
#include <vector>
#include "SimpleIni.h"
#include <sstream>

#define AMCE_VERSION 1.0


class Config
{
public:
	Config();							// Default constructor

	//GETs
	int getTriggerMode();				// Return the trigger mode value
	int getIntervalMode();				// Return the Interval Mode value
	int getRandMin();					// Return the minimum random delay value
	int getRandMax();					// Return the maximum random delay value
	int getConstDelay();				// Return the constand delay value
	int getKey();						// Return the code for the trigger key
	int getClick();						// Return the type of click
	std::vector<int> getAllValues();	// Return a vector containing all the values (except key event)
	std::vector<DWORD> getInputs();		// Return a vector containing the key events to execute
	
	//SETs
	void setModes(int modeTrigger, int modeInterval, int modeClick);		// Change the modes 
	void setDelays(int randMin,int randMax,int constDelay);					// Change the delays	
	void setKey(int);														// Change the trigger key
	void setClick(int click);												// Change the type of click

																		
	void updateConf();														// Update the configuration (save it and load it) 
	
private: 
	
	int m_modeTrigger = 0;				// 0 : Push Mode ; 1 : Switch Mode
	int m_modeInterval = 0;				// 0 : Constant Mode ; 1 : Random Mode
	int m_key = VK_XBUTTON1;			// VK Code for trigger button
	int m_randMin = 10;					// For random intervals, minimum
	int m_randMax = 100;				// For random intervals, maximum
	int m_constDelay = 55;				// For constant delay, delay
	int m_click = 0;					// Type of click; 0 : Left ; 1 : Right; 2 : Mouse wheel (not click)

	CSimpleIniA m_iniParser;			// Ini parser for saving/loading the config
	LPCWSTR m_iniFile = L"(ノಠ益ಠ)ノ彡┻━┻.ini";// Path of the file
	
	DWORD m_keyInDOWN = MOUSEEVENTF_LEFTDOWN;	// Key event to send first
	DWORD m_keyInUP = MOUSEEVENTF_LEFTUP;		// Key event to send last
	
	void _writeConf();							// Write the configuration in the ini file
	void _checkConf();							// Check if the configuration file exists
	int _intParser(std::string str);			// Parse the string from the configuration file to integers
	void _loadConf();							// Load the configuration from the file
	


};


