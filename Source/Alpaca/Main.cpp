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

#include "Commands.h"
#include "MainScreen.h"
#include "BigStash.h"
#include "InfinityStash.h"
#include "Common.h"
#include "ExtraPatches.h"

void freeLibrary(DWORD library)
{
	if (library && library != Game::Offset)
		FreeLibrary((HMODULE)library);
}

void freeD2Libraries()
{
	log_msg("\nFree Libraries\n");
	log_msg("====================================\n");

	freeLibrary(D2Client::Offset);
	freeLibrary(D2Common::Offset);
	freeLibrary(D2Game::Offset);
	freeLibrary(D2gfx::Offset);
	freeLibrary(D2Launch::Offset);
	freeLibrary(D2Net::Offset);
	freeLibrary(D2Win::Offset);
	freeLibrary(Fog::Offset);
	freeLibrary(Storm::Offset);
}

//////////////////////////////////// EXPORTS FUNCTIONS ////////////////////////////////////

void InstallAlpacaFunctions();

extern "C" __declspec(dllexport) bool __stdcall Release()
{
	active_logFile = true;

	log_msg("\nExiting Diablo II\n");
	log_msg("====================================\n");

	freeD2Libraries();
	return true;
}

extern "C" __declspec(dllexport) void* __stdcall Init(LPSTR IniName)
{
	// If you want to debug all of the initialization code
	// you can uncomment the below MessageBox and put a breakpoint after it.
	// Reason for this is that this code happens quickly and very early before D2
	// fully starts, thus by the time you can attach your debugger to Game.exe,
	// all of this code already finished. So we can use the MessageBox trick.
	// Thanks to Necrolis @ PhrozenKeep for bringing this trick up.
	//MessageBox(GetActiveWindow(), "The Alpacas have arrived!", "Alpaca", MB_APPLMODAL);

	LibraryLoader::Init();
	InitializeDiabloFunctions();
	LoadParameters();
	InstallAlpacaFunctions();

	log_msg("Entering Diablo II\n");
	log_msg("====================================\n");

	active_logFile = active_logFileIniOriginal;

	return NULL;
}

void InstallAlpacaFunctions()
{
	LibraryLoader::HookLibraries();

	log_msg("Alpaca Patches\n");
	log_msg("====================================\n");

	Install_Commands();
	Install_VersionChange();
	Install_PrintVersion();
	Install_BigStash();
	Install_MultiPageStash();
	ExtraPatches::DisplayItemLevel();

	log_msg("\n");

	LibraryLoader::UnhookLibraries();
}