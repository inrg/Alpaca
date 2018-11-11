// Copyright (C) 2004-2017 Yohann Nicolas
// Copyright (C) 2017-2018 Jonathan Vasquez <jon@xyinn.org>
//
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "Error.h"
#include "IniFile.h"
#include "Parameters.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "MainScreen.h"
#include "BigStash.h"
#include "InfinityStash.h"
#include "Commands.h"

const int BUFFER_SIZE = 1024;

char* modDataDirectory = "Alpaca";
char* parametersFileName = "Alpaca.ini";
bool active_plugin = true;

// Active Check Memory is on by default. It should only be disabled by a developer.
// Users should report bugs and/or the users environment/configuration should be inspected.
bool active_CheckMemory = true;

const char* S_GENERAL = "GENERAL";
const char* S_active_logFile = "ActiveLogFile";
const char* S_active_logFilePatches = "ActiveLogFilePatches";

const char* S_STASH = "STASH";
const char* S_nbPagesPerIndex = "NumberOfPagesPerIndex";
const char* S_nbPagesPerIndex2 = "NumberOfPagesPerIndexWhenShiftPressed";
const char* S_openSharedStashOnLoading = "OpenSharedStashOnLoading";

// Convert 4 char code in a DWORD code
#define BIN(A,B,C,D) ((DWORD)A) + (((DWORD)B) << 8) + (((DWORD)C) << 16) + (((DWORD)D) << 24)

bool IsEnabled(char* symbol)
{
	return atoi(symbol) == 1;
}

void GetValueFromIni(INIFile* iniFile, const char* areaName, const char* optionName, const char* defaultValue, char* buffer, DWORD maxSize)
{
	if (!iniFile->GetPrivateProfileString(areaName, optionName, defaultValue, buffer, maxSize))
	{
		log_msg("[Error] Failed to load \"%s\" from configuration file. Defaulting to %s ...", optionName, defaultValue);
	}
}

void LogParameterBooleanValue(const char* parameterName, bool parameterValue)
{
	log_msg("%s = %u\n", parameterName, parameterValue);
}

void LogParameterIntegerValue(const char* parameterName, DWORD parameterValue)
{
	log_msg("%s = %d\n", parameterName, parameterValue);
}

void init_General(INIFile* iniFile, char* buffer, DWORD maxSize)
{
	// Active Log File
	GetValueFromIni(iniFile, S_GENERAL, S_active_logFile, "0", buffer, maxSize);

	// Temporarily save the log value into this value since we are still logging data
	// during initialization. We will turn off logging at the end before we go into D2 if needed.
	active_logFileIniOriginal = IsEnabled(buffer);

	LogParameterBooleanValue(S_active_logFile, active_logFileIniOriginal);

	// Active Log File [Patches]
	GetValueFromIni(iniFile, S_GENERAL, S_active_logFilePatches, "0", buffer, maxSize);
	active_logFileMemory = IsEnabled(buffer);
	LogParameterBooleanValue(S_active_logFilePatches, active_logFileMemory);
}

void init_Stash(INIFile* iniFile, char* buffer, DWORD maxSize)
{
	GetValueFromIni(iniFile, S_STASH, S_nbPagesPerIndex, "10", buffer, maxSize);
	nbPagesPerIndex = atoi(buffer);
	LogParameterIntegerValue(S_nbPagesPerIndex, nbPagesPerIndex);

	GetValueFromIni(iniFile, S_STASH, S_nbPagesPerIndex2, "100", buffer, maxSize);
	nbPagesPerIndex2 = atoi(buffer);
	LogParameterIntegerValue(S_nbPagesPerIndex2, nbPagesPerIndex2);

	GetValueFromIni(iniFile, S_STASH, S_openSharedStashOnLoading, "0", buffer, maxSize);
	openSharedStashOnLoading = IsEnabled(buffer);
	LogParameterBooleanValue(S_openSharedStashOnLoading, openSharedStashOnLoading);

	log_msg("\n");
}

void LoadParameters()
{
	char buffer[BUFFER_SIZE];
	INIFile* iniFile = new INIFile;

	srand((UINT)time(NULL));

	log_msg("Parameters\n");
	log_msg("====================================\n");

	bool wasFileSuccessfullyOpened = iniFile->InitReadWrite(parametersFileName, INIFILE_READ, 0);

	if (wasFileSuccessfullyOpened)
	{
		init_General(iniFile, buffer, BUFFER_SIZE);
		init_Stash(iniFile, buffer, BUFFER_SIZE);
	}
	else
	{
		log_msg("There was an error opening the config file. The game will run with the Alpaca Defaults!\n\n");
	}

	if (iniFile)
	{
		iniFile->close();
	}

	delete iniFile;
}