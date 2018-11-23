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

#include <stdio.h>
#include "Utility.h"
#include "Error.h"
#include "Parameters.h"

void Utility::GetAlpacaDirectory(char* alpacaDirectory)
{
	const char* UNABLE_TO_GET_CURRENT_DIR = "There was an error retrieving your current directory. Exiting.";

	// Game.exe and Alpaca.exe need to be in the same directory (The Diablo II Root Directory)
	char currentDirectory[MAX_PATH];

	if (!GetCurrentDirectory(MAX_PATH-1, currentDirectory))
	{
		Assertion(UNABLE_TO_GET_CURRENT_DIR);
	}

	// Not sure if this is possible (Where the current directory function
	// doesn't fail but the returned value is empty. Leaving this code
	// here for safety since it was here in the original PlugY code.
	int currentDirectoryPathLength = strlen(currentDirectory);
	if (currentDirectoryPathLength == 0)
	{
		Assertion(UNABLE_TO_GET_CURRENT_DIR);
	}

	strcpy(alpacaDirectory, currentDirectory);
}

void Utility::Assertion(const char* pFormat, ...)
{
	char msg[200];
	va_list lArgs;
	va_start(lArgs, pFormat);

	vsprintf(&msg[0], pFormat, lArgs);

	va_end(lArgs);
	MessageBox(0, msg, PROGRAM_NAME, MB_OK | MB_ICONASTERISK);
	exit(1);
}