/*
======================================================================

AutoClick.h - Header file of AutoClick.cpp

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
#include <thread>
#include <future>
namespace ACME
{
class AutoClick

{
public:
	AutoClick();													// Default constructor
	AutoClick(std::vector<int> conf, std::vector<DWORD> inputs);	// Constructor taking the config from a vector
	~AutoClick();													// Ensure that the thread is terminated to prevent memory leak

	void StartClick();												// Start the thread
	void StopClick();												// Stop the thread
	void UpdateValues(std::vector<int> conf, std::vector<DWORD> inputs);// For updating the local variables 

	bool isRunning = false;											// Public variable to know if the main loop is running

private:
	void MainLoop();												// The main loop where it keeps listening for events
	void GenerateClick();											// The function that generates the click (Or key in the future)
	bool KeyPressed(char key);										// Check if a key is pressed
		
private:
	int m_modeTrigger = 0;											// 0 : Push Mode ; 1 : Switch Mode
	int m_modeInterval = 0;											// 0 : Constant Mode ; 1 : Random Mode
	int m_key = VK_XBUTTON1;										// VK Key Code for trigger button
	int m_randMin = 10;												// For random intervals, minimum
	int m_randMax = 100;											// For random intervals, maximum
	int m_constDelay = 55;											// For constant delay, delay
	bool m_suicideLoop = false;										// If true then kill the thread
	bool m_switchModeState = false;									// False : Swtich mode isn't activated ; True : switch mode is activated

	std::mutex m_mutex_conf;										// Lil' mutex u'know
	std::thread _clickThread;										// Main thread
	
	HWND m_currentWindow = GetForegroundWindow();					// Represent the windows the program is acting on
	POINT m_pt;														// The cursor position
	RECT m_wrect;													// Window size
	INPUT m_input = {};												// The input to send

	DWORD m_keyInDOWN = MOUSEEVENTF_LEFTDOWN;						// The key to press (DOWN)
	DWORD m_keyInUP = MOUSEEVENTF_LEFTUP;							// The key to release (UP)

};

}