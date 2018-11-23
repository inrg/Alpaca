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
#include "Common.h"

char* DiabloVersionText = "v 1.13d";
BYTE DiabloVersionColor = 0;
BYTE AlpacaVersionColor = 4;

DWORD newTextBoxData[]={4, 0x237, 0x243, 0xC8, 0x14, 0, 0, 0, 0, 0, 0, 2};

void __stdcall PrintVersion(void** childrens, DWORD* sgnNumChildren)
{
	char buf[30];
	void* textbox = D2CreateTextBox(newTextBoxData);
	childrens[*sgnNumChildren] = textbox;
	d2_assert((*sgnNumChildren)++ >= 40, "sgnNumChildren < MAX_CHILDREN", __FILE__, __LINE__);
	sprintf(buf, "%s %s", PROGRAM_NAME, PROGRAM_VERSION);
	D2PrintLineOnTextBox(textbox, buf, AlpacaVersionColor);
}

FCT_ASM ( caller_PrintVersion )
	POP ESI
	PUSH DWORD PTR [ESI+2]
	PUSH DWORD PTR [ESI+9]
	CALL PrintVersion
	CALL D2CreateTextBox
	JMP ESI
}}

void Install_PrintVersion()
{
	static bool isInstalled = false;
	if (isInstalled) return;

	log_msg("[Patch] Print Alpaca Version - Main Menu\n");

	DWORD PrintVersionOffset = D2Launch::GetAddress(0x10A11);

	Memory::SetCursor(PrintVersionOffset);
	Memory::ChangeCallB((DWORD)D2Win::D2CreateTextBox, (DWORD)caller_PrintVersion);

	if (active_logFileMemory) log_msg("\n");
	isInstalled = true;
}

FCT_ASM(caller_VersionChange_10)
MOV CL, BYTE PTR DS : [DiabloVersionColor]
MOV BYTE PTR SS : [ESP + 4], CL
MOV EDX, DiabloVersionText
MOV ECX, EDI
RETN
}}

void Install_VersionChange()
{
	static bool isInstalled = false;
	if (isInstalled) return;

	log_msg("[Patch] Print Diablo Version - Main Screen\n");

	DWORD PrintLodModVersionOffset = D2Launch::GetAddress(0x10AE4);

	// Print LoD/Mod version.
	Memory::SetCursor(PrintLodModVersionOffset);
	Memory::ChangeByte(0x8D, 0xE8);
	Memory::ChangeCallA(0x8B102454, (DWORD)caller_VersionChange_10);
	Memory::ChangeByte(0xCF, 0x90);
	
	if (active_logFileMemory) log_msg("\n");
	isInstalled = true;
}