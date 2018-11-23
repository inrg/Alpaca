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

#include "ExtraPatches.h"
#include "Common.h"
#include <stdio.h>

// Localization Strings (English Only)
WCHAR* STR_ITEM_LEVEL = L"Item Level";

void __stdcall displayItemlevel(LPWSTR popup, Unit* ptItem)
{
	WCHAR text[0x50];
	_snwprintf(text, sizeof(text), L"%s: %u\n", STR_ITEM_LEVEL, D2Common::D2GetItemLevel(ptItem));
	D2Client::D2SetColorPopup(text, ORANGE);
	wcscat(popup, text);
}

__declspec(naked) void caller_displayItemlevel_113()
{
	__asm {
		PUSH ECX
		PUSH EAX
		PUSH EAX
		LEA EAX, DWORD PTR SS : [ESP + 0x1E70]
		PUSH EAX
		CALL displayItemlevel
		POP EAX
		POP ECX
		POP EDX
		PUSH 0x100
		JMP EDX
	}
}

__declspec(naked) void caller_displayItemlevelSet_111()
{
	__asm {
		PUSH ECX
		PUSH EDX
		PUSH ECX
		LEA EAX, DWORD PTR SS : [ESP + 0x1958]
		PUSH EAX
		CALL displayItemlevel
		POP EDX
		POP ECX
		POP EAX
		PUSH 0x100
		JMP EAX
	}
}

void ExtraPatches::DisplayItemLevel()
{
	static bool isInstalled = false;
	if (isInstalled) return;

	log_patch("Display Item Level");

	// print the text in the final buffer
	DWORD FinalBufferOffset = D2Client::GetAddress(0x98590);
	Memory::SetCursor(FinalBufferOffset);
	Memory::ChangeByte(0x68, 0xE8);
	Memory::ChangeCallA(0x100, (DWORD)caller_displayItemlevel_113);

	// print the text in the final buffer (for set items)
	DWORD FinalBufferForSetsOffset = D2Client::GetAddress(0x973B3);
	Memory::SetCursor(FinalBufferForSetsOffset);
	Memory::ChangeByte(0x68, 0xE8);
	Memory::ChangeCallA(0x100, (DWORD)caller_displayItemlevelSet_111);

	if (active_logFileMemory) log_msg("\n");
	isInstalled = true;
}
