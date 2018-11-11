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

#include "AlpacaFiles.h"
#include "Common.h"

const char* tradeStash_RefFile= "%s\\TradeStash";
const BYTE gridX_BB = 10;
const BYTE gridY_BB = 10;
const DWORD gridLeft_BB = 16;
const DWORD gridRight_BB = 302;
const DWORD gridTop_BB = 82;
const DWORD gridbottom_BB = 371;

InventoryBIN* __stdcall modifStashGrid(InventoryBIN* ptInventoryBin)
{
	InventoryBIN* inventory=ptInventoryBin+12;
	inventory->gridX = gridX_BB;
	inventory->gridY = gridY_BB;
	inventory->gridLeft = gridLeft_BB;
	inventory->gridRight = gridRight_BB;
	inventory->gridTop = gridTop_BB;
	inventory->gridBottom = gridbottom_BB;

	inventory = ptInventoryBin+28;
	inventory->gridX = gridX_BB;
	inventory->gridY = gridY_BB;
	inventory->gridLeft = gridLeft_BB + 80;
	inventory->gridRight = gridRight_BB + 80;
	inventory->gridTop = gridTop_BB + 60;
	inventory->gridBottom = gridbottom_BB + 60;

	return ptInventoryBin;
}

FCT_ASM ( caller_modifStashGrid )
	POP ESI
	CALL D2CompileTxtFileDirect
	PUSH EAX
	CALL modifStashGrid
	JMP ESI
}}

FCT_ASM ( caller_changeTradeStash )
	MOV EAX, modDataDirectory
	MOV DWORD PTR SS:[ESP+0x8],EAX
	MOV EAX, tradeStash_RefFile
	MOV DWORD PTR SS:[ESP+0x4],EAX
	POP EAX
	PUSH 0x104
	JMP EAX
}}

void Install_BigStash()
{
	static bool isInstalled = false;
	if (isInstalled) return;

	Install_AlpacaFiles();

	log_msg("[Patch] Increased Stash (10x10)\n");

	DWORD StashGridOffset = D2Common::GetAddress(0x6CC25);
	DWORD StashBackgroundOffset = D2Client::GetAddress(0x943FC);

	// Modification of stash grid
	Memory::SetCursor(StashGridOffset);
	Memory::ChangeCallC((DWORD)D2CompileTxtFileDirect, (DWORD)caller_modifStashGrid);

	// Modification of stash background
	Memory::SetCursor(StashBackgroundOffset);
	Memory::ChangeByte(0x68, 0xE8);
	Memory::ChangeCallA(0x104, (DWORD)caller_changeTradeStash);

	if (active_logFileMemory) log_msg("\n");
	isInstalled = true;
}
