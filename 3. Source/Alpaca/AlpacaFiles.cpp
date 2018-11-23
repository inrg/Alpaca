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

#include "AlpacaFiles.h"
#include "Common.h"
#include "Utility.h"

void* stashBtnsImages = NULL;
void* sharedGoldBtnsImages = NULL;

DWORD __stdcall isModFile (char* filename)
{
	if (strstr(filename, modDataDirectory))
	{
		log_msg("Loading Custom File: %s", filename);

		char currentDirectory[MAX_PATH];
		Utility::GetAlpacaDirectory(currentDirectory);

		char pathToFile[MAX_PATH];
		strcpy(pathToFile, currentDirectory);
		strcat(pathToFile, "\\");
		strcat(pathToFile, filename);

		log_msg(" -> %s\n", pathToFile);
		if ((GetFileAttributesA(pathToFile) & 0x10) == 0) return true;
	}

	return false;
}

FCT_ASM(caller_isModFile_111)
TEST EDI, EDI
JNZ ISNOTMODDATA
MOV ESI, DWORD PTR SS : [ESP + 0x130]
PUSH ESI
CALL isModFile
TEST EAX, EAX
JE ISNOTMODDATA
MOV BL, BYTE PTR SS : [ESP + 0x134]
POP EAX
POP EAX
MOV EAX, D2StormMPQOpenFile
ADD EAX, 0xBD
MOV EBP, GetFileAttributesA
JMP EAX
ISNOTMODDATA :
JMP DWORD PTR CS : [LeaveCriticalSection]
}}

void Install_AlpacaFiles()
{
	static bool isInstalled = false;
	if (isInstalled) return;

	log_msg("[Patch] Find Custom Files\n");

	DWORD FindCustomFileOffset = Storm::GetAddress(0x2DA79);

	// Try in Diablo II\Alpaca\ if file not found
	Memory::SetCursor(FindCustomFileOffset);
	Memory::ChangeByte(0xFF, 0x90);
	Memory::ChangeByte(0x15, 0xE8);
	Memory::ChangeCallD((DWORD)LeaveCriticalSection, (DWORD)caller_isModFile_111);

	if (active_logFileMemory) log_msg("\n");
	isInstalled = true;
}

void loadImagesFile(void** images, const char* name)
{
	if(!images || !name) return;
	if(!*images)
	{
		char buffer[MAX_PATH];

		// The path to the .dc6 is relative here, however, D2LoadImage will eventually call our custom
		// "isModFile" function that returns checks the absolute path from the Alpaca.exe into the Alpaca
		// directory that is in the same level as the Alpaca.exe.
		strcpy(buffer, modDataDirectory);
		strcat(buffer, "\\");
		strcat(buffer, name);

		*images = D2LoadImage(buffer, 0);
		if (!*images)
		{
			sprintf(buffer, "Unable to find image file: %s.dc6", name);
			d2_assert(!*images, buffer, __FILE__, __LINE__);
		}
	}
}

void __stdcall loadCustomImages()
{
	const char* stashBtnsFile = "StashBtns";
	const char* sharedGoldBtnsFile = "SharedGoldBtns";

	loadImagesFile(&stashBtnsImages, stashBtnsFile);
	loadImagesFile(&sharedGoldBtnsImages, sharedGoldBtnsFile);
}

#define freeImagesFile(I) if(I) {D2FreeImage(I);I=NULL;}

void __stdcall freeCustomImages()
{
	freeImagesFile(stashBtnsImages);
	freeImagesFile(sharedGoldBtnsImages);
}

FCT_ASM ( caller_loadCustomImages )
	CALL loadCustomImages
	MOV ECX,0x0C
	RETN
}}

FCT_ASM ( caller_freeCustomImages )
	CALL freeCustomImages
	MOV ECX,0x0C
	RETN
}}

void Install_AlpacaImagesFiles()
{
	static bool isInstalled = false;
	if (isInstalled) return;

	Install_AlpacaFiles();

	log_msg("[Patch] Load & Free Custom Images\n");

	DWORD LoadCustomImageOffset = D2Client::GetAddress(0x6E0BE);
	DWORD FreeCustomImageOffset = D2Client::GetAddress(0x6D07D);

	// Load custom images
	Memory::SetCursor(LoadCustomImageOffset);
	Memory::ChangeByte(0xB9, 0xE8);
	Memory::ChangeCallA(0x0C, (DWORD)caller_loadCustomImages);

	// Free custom images
	Memory::SetCursor(FreeCustomImageOffset);
	Memory::ChangeByte(0xB9, 0xE8);
	Memory::ChangeCallA(0x0C, (DWORD)caller_freeCustomImages);

	if (active_logFileMemory) log_msg("\n");
	isInstalled = true;
}