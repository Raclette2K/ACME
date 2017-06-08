/*
======================================================================

Config.cpp - Handles the configuration file

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
#include "Config.h"

	// Default constructor
	Config::Config()
	{
		// Set the parser to UTF-8 (all hail unicode)
		m_iniParser.SetUnicode("UTF-8");
		// Call _checkConf() to know if the configuration file exists
		_checkConf();
	}
	
	/////////////////////////////////////////////////////////
	// Return values from private variables [see Config.h] //
	/////////////////////////////////////////////////////////
	int Config::getTriggerMode() { return m_modeTrigger; }
	int Config::getIntervalMode() { return m_modeInterval; }
	int Config::getRandMin() { return m_randMin; }
	int Config::getRandMax() { return m_randMax; }
	int Config::getConstDelay() { return m_constDelay; }
	int Config::getKey() { return m_key; }
	int Config::getClick() { return m_click; }
	std::vector<int> Config::getAllValues()
	{
		return { m_modeTrigger,m_modeInterval,m_key,m_randMin,m_randMax,m_constDelay };
	}
	std::vector<DWORD> Config::getInputs()
	{
		return { m_keyInDOWN, m_keyInUP };
	}
	
	/////////////////////////////////////////////////////////
	// Modify values of private variables [see Config.h]   //
	/////////////////////////////////////////////////////////
	void Config::setModes(int modeTrigger, int modeInterval, int click) { m_modeTrigger = modeTrigger; m_modeInterval = modeInterval; setClick(click); }
	void Config::setDelays(int randMin, int randMax, int constDelay) { m_randMin = randMin; m_randMax = randMax; m_constDelay = constDelay;  }
	void Config::setKey(int key) { m_key = key; }
	void Config::setClick(int click) 
	{
		m_click = click; 
		switch (click)
		{
		case 0:
			m_keyInDOWN = MOUSEEVENTF_LEFTDOWN;
			m_keyInUP = MOUSEEVENTF_LEFTUP;
			break;
		case 1:
			m_keyInDOWN = MOUSEEVENTF_RIGHTDOWN;
			m_keyInUP = MOUSEEVENTF_RIGHTUP;
			break;
		case 2:
			m_keyInDOWN = MOUSEEVENTF_WHEEL;
			m_keyInUP = MOUSEEVENTF_WHEEL;
			break;
		default:
			m_click = 0;
			m_keyInDOWN = MOUSEEVENTF_LEFTDOWN;
			m_keyInUP = MOUSEEVENTF_LEFTUP;
			break;

		}
	}
	
	// Update the configuration (just saving it to the file) 
	void Config::updateConf()
	{
		// Call the _writeConf() function to write in the ini file
		_writeConf(); 
	}
	// Uses the ini parser to write values in the ini file (convert int to string too) 
	void Config::_writeConf()
	{
		m_iniParser.SetValue("Modes", "Trigger", std::to_string(m_modeTrigger).c_str());
		m_iniParser.SetValue("Modes", "Interval", std::to_string(m_modeInterval).c_str());
		m_iniParser.SetValue("Modes", "VK", std::to_string(m_key).c_str());
		m_iniParser.SetValue("Modes", "Click", std::to_string(m_click).c_str());
		m_iniParser.SetValue("Delay", "RandMin", std::to_string(m_randMin).c_str());
		m_iniParser.SetValue("Delay", "RandMax", std::to_string(m_randMax).c_str());
		m_iniParser.SetValue("Delay", "Const", std::to_string(m_constDelay).c_str());
		m_iniParser.SaveFile(m_iniFile);
	}
	// Uses the ini parser to load values from the ini file (convert string to int using _intParser)
	void Config::_loadConf()
	{
		m_modeTrigger = _intParser(m_iniParser.GetValue("Modes", "Trigger"));
		m_modeInterval = _intParser(m_iniParser.GetValue("Modes", "Interval"));
		m_key = _intParser(m_iniParser.GetValue("Modes", "VK"));
		m_click = _intParser(m_iniParser.GetValue("Modes", "Click")); setClick(m_click);
		m_randMin = _intParser(m_iniParser.GetValue("Delay", "RandMin"));
		m_randMax = _intParser(m_iniParser.GetValue("Delay", "RandMax"));
		m_constDelay = _intParser(m_iniParser.GetValue("Delay", "Const"));
	}
	// Checks if the file exists and if not create it by writing default values
	void Config::_checkConf()
	{
		if (m_iniParser.LoadFile(m_iniFile) < 0)// Check if file doesn't exist
			_writeConf();						// Write defaults values and save it
		else
			_loadConf();						// Load the values from the ini file because it exists
	}
	// Parses string to integers via streams
	int Config::_intParser(std::string str)
	{
		std::stringstream stream;
		stream << str;
		int value;
		stream >> value;
		return value;
	}



