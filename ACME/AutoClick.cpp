/*
======================================================================

AutoClick.cpp - Where the magic happens...

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
#include "AutoClick.h"

namespace ACME
{
	// Default Constructor
	AutoClick::AutoClick() {}
	// Constructor taking the config as a vector
	AutoClick::AutoClick(std::vector<int> conf, std::vector<DWORD> inputs)
	{ 
		m_modeTrigger = conf.at(0);
		m_modeInterval = conf.at(1);
		m_key = conf.at(2);
		m_randMin = conf.at(3);
		m_randMax = conf.at(4);
		m_constDelay = conf.at(5);

		m_keyInDOWN = inputs.at(0);
		m_keyInUP = inputs.at(1);
	}
	// Ensure that the thread is terminated to avoid problems and memory leak
	AutoClick::~AutoClick() 
	{ 
		// Stop the thread 
		StopClick();
		// Ensure that the thread is destroyed
		_clickThread.~thread(); 
	}
	//	Update the config 
	void AutoClick::UpdateValues(std::vector<int> conf, std::vector<DWORD> inputs)
	{
		m_mutex_conf.lock(); // Prevent other threads to modify variables

		m_modeTrigger = conf.at(0);
		m_modeInterval = conf.at(1);
		m_key = conf.at(2);
		m_randMin = conf.at(3);
		m_randMax = conf.at(4);
		m_constDelay = conf.at(5);

		m_keyInDOWN = inputs.at(0);
		m_keyInUP = inputs.at(1);

		m_mutex_conf.unlock(); // Unlock :)

	}
	// Start the thread
	void AutoClick::StartClick()
	{	
		// Check if the thread isn't runnin
		if (!_clickThread.joinable())
		{
			// Ensure that the loop is working
			m_suicideLoop = false;
			// Ensure that the switch will not start on his own
			m_switchModeState = false;
			// Start the thread
			_clickThread = std::thread(&AutoClick::MainLoop, this);
		}
		
	} 
	// Stop the thread
	void AutoClick::StopClick()
	{
		// Check if the thread is running
		if (_clickThread.joinable())
		{
			// Stop the loop
			m_suicideLoop = true;
			// Join the thread to have it stop
			_clickThread.join();
		}
	}
	// Main loop where the magic happens
	void AutoClick::MainLoop()
	{
		TRACE("Main loop thread launched with id %d\n",std::this_thread::get_id());
		// Change the state of isRunning to indicate that the thread is running
		isRunning = true;
		// Allow to set on foreground the selected window : Required for a lot of softwares/games
		AllowSetForegroundWindow(true);

		// While we don't want to kill the loop
		while (!m_suicideLoop)
		{
			// Prevent the loop from going crazy and ensure a 1ms delay
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			// When the left mouse button is clicked and the windows it has clicked isn't the same then select the window it was clicked on
			if (KeyPressed(VK_LBUTTON) && m_currentWindow != GetForegroundWindow()) 
			{
				// Get the window the user clicked on
				m_currentWindow = GetForegroundWindow();	
			}

			// When the user click on the trigger button 
			if (GetAsyncKeyState(m_key))
			{
				// If it is in Push Mode
				if (m_modeTrigger == 0) {
					GenerateClick(); 
				}
				// If it is in Switch Mode
				if (m_modeTrigger == 1)
				{
					// Change to on/off, depends on the current state
					m_switchModeState = !m_switchModeState;
					// Wait 200ms for preventing "re-enabling" 
					std::this_thread::sleep_for(std::chrono::milliseconds(200));
				}

			}

			// If the switch is on 
			if (m_switchModeState) {
				// Call the GenerateClick function to click
				GenerateClick();
			}

		}
		// Change the state of isRunning to inform that the thread is not running anymore
		isRunning = false;
		// Ensure that the switch will not start on his own
		m_switchModeState = false;
	}
	// Generate the input 
	void AutoClick::GenerateClick()
	{
		// Get the current position of the cursor
		GetCursorPos(&m_pt);
		// Get the size of the window
		GetClientRect(m_currentWindow, &m_wrect);

		// Define the first input to send (DOWN)
		m_input.type = INPUT_MOUSE;
		m_input.mi.dx = m_pt.x - m_wrect.left;
		m_input.mi.dy = m_pt.y - m_wrect.top;
		m_input.mi.dwFlags = m_keyInDOWN;
		m_input.mi.time = 0;
		if (m_keyInDOWN == MOUSEEVENTF_WHEEL)
			m_input.mi.mouseData = 20;
		// Send the first input (DOWN)
		SendInput(1, &m_input, sizeof(INPUT));

		// Reset the Input variable to prepare for the second input (UP)
		ZeroMemory(&m_input, sizeof(INPUT));

		// Define the second input to send (UP)
		m_input.type = INPUT_MOUSE;
		m_input.mi.dx = m_pt.x - m_wrect.left;
		m_input.mi.dy = m_pt.y - m_wrect.top;
		m_input.mi.dwFlags = m_keyInUP;
		m_input.mi.time = 0;
		if (m_keyInUP == MOUSEEVENTF_WHEEL)
			m_input.mi.mouseData = 20;
		// Send the second input (UP)
		SendInput(1, &m_input, sizeof(INPUT));

		// If the interval mode is set to random
		if (m_modeInterval == 0)
		{
			// Generate a random number in the defined interval
			int delay(rand() % (m_randMax - m_randMin + 1) + m_randMin);
			// Sleep the thread for [random number] ms (-1 for the MainLoop 1ms sleep)
			std::this_thread::sleep_for(std::chrono::milliseconds(delay-1));
			TRACE("Delay %d\n", delay);
		}
		else
		{
			// Sleep the thread for the constant delay defined (-1 for the MainLoop 1ms sleep)
			std::this_thread::sleep_for(std::chrono::milliseconds(m_constDelay-1));
			TRACE("Delay %d\n", m_constDelay);
		}	
	}
	// Return the state of a key 
	bool AutoClick::KeyPressed(char key)
	{
		USHORT status = GetAsyncKeyState(key);
		return (((status & 0x8000) >> 15) == 1) || ((status & 1) == 1);
	}

	
}

