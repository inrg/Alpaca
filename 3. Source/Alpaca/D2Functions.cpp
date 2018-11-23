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

// This file should be used when attempting to access any of the functions provided
// in the libraries. The reason for this is the following:
// 1. Most of the existing code currently attempts to find the global function reference to the function.
// 2. Some of the mem_seek operations (and possibly more) pass a function pointer to its call.
//    However, if the function has been redirected internally, then passing a function pointer to this
//    redirected library function will cause the application to crash or lead to unintended behavior.
// If you really want to use the library function directly (ideally we would), you should make sure
// that nothing breaks.
#include "Common.h"

s_shifting shifting;

// D2Common
D2Common::TD2InvAddItem D2InvAddItem;
D2Common::TD2InvRemoveItem D2InvRemoveItem;
D2Common::TD2InventoryGetFirstItem D2InventoryGetFirstItem;
D2Common::TD2UnitGetNextItem D2UnitGetNextItem;
D2Common::TD2GetRealItem D2GetRealItem;
D2Common::TD2GetMaxGoldBank D2GetMaxGoldBank;
D2Common::TD2InitPlayerData D2InitPlayerData;
D2Common::TD2GetMaxGold D2GetMaxGold;
D2Common::TD2AddPlayerStat D2AddPlayerStat;
D2Common::TD2GetPlayerStat D2GetPlayerStat;
D2Common::TD2CompileTxtFile D2CompileTxtFileDirect;
D2Common::TD2CompileTxtFile D2CompileTxtFile;
D2Common::TD2ItemGetPage D2ItemGetPage;
D2Common::TD2SaveItem D2SaveItem;
D2Common::TD2ReadFile D2ReadFile;

// D2Client
D2Client::TD2LoadImage D2LoadImage;
D2Client::TD2FreeImage D2FreeImage;
D2Client::TD2PlaySound D2PlaySound;
D2Client::TD2LoadBuySelBtn D2LoadBuySelBtn;
D2Client::TD2GetMouseX D2GetMouseX;
D2Client::TD2GetMouseY D2GetMouseY;
D2Client::TD2GetClientPlayer D2GetClientPlayer;
D2Client::TD2SendToServer3 D2SendToServer3;
D2Client::TD2ClickOnStashButton D2ClickOnStashButton;

// D2Client: Variables
DWORD* ptNegWindowStartY;
DWORD* ptWindowStartX;

// D2Game
D2Game::TD2SendPacket D2SendPacket;
D2Game::TD2LoadInventory D2LoadInventory;
D2Game::TD2GameGetObject D2GameGetObject;
D2Game::TD2GetClient D2GetClient;
D2Game::TD2SaveGame D2SaveGame;

// D2Game: Variables
NetClient** ptClientTable;

// D2Net
D2Net::TD2SendToServer D2SendToServer;
D2Net::TD2SendToClient D2SendToClient;

// Fog
Fog::TD2FogMemAlloc D2FogMemAlloc;
Fog::TD2FogMemDeAlloc D2FogMemDeAlloc;
Fog::TD2AllocMem D2AllocMem;
Fog::TD2FreeMem D2FreeMem;
Fog::TD2MPQOpenFile D2MPQOpenFile;
Fog::TD2MPQCloseFile D2MPQCloseFile;
Fog::TD2MPQReadFile D2MPQReadFile;
Fog::TD2MPQGetSizeFile D2MPQGetSizeFile;
Fog::TD2FogGetSavePath D2FogGetSavePath;

// Storm
Storm::TD2StormMPQOpenFile D2StormMPQOpenFile;
Storm::TD2FreeWinMessage D2FreeWinMessage;

// D2gfx
D2gfx::TD2GetResolution D2GetResolution;
D2gfx::TD2FillArea D2FillArea;
D2gfx::TD2PrintImage D2PrintImage;

// D2Win
D2Win::TD2PrintLineOnTextBox D2PrintLineOnTextBox;
D2Win::TD2PrintString D2PrintString;
D2Win::TD2GetPixelLen D2GetPixelLen;
D2Win::TD2SetFont D2SetFont;
D2Win::TD2PrintPopup D2PrintPopup;
D2Win::TD2CreateTextBox D2CreateTextBox;

void setImage(sDrawImageInfo* data, void* image){((void**)data)[shifting.ptImage/4]=image;} //0x4 0x8 0x3C
void setFrame(sDrawImageInfo* data, DWORD frame){((DWORD*)data)[shifting.ptFrame/4]=frame;} //0x8 0x44 0x40

void InitializeDiabloFunctions()
{
	// D2Common
	D2InvAddItem = D2Common::D2InvAddItem;
	D2InvRemoveItem = D2Common::D2InvRemoveItem;
	D2InventoryGetFirstItem = D2Common::D2InventoryGetFirstItem;
	D2UnitGetNextItem = D2Common::D2UnitGetNextItem;
	D2GetRealItem = D2Common::D2GetRealItem;
	D2GetMaxGoldBank = D2Common::D2GetMaxGoldBank;
	D2InitPlayerData = D2Common::D2InitPlayerData;
	D2GetMaxGold = D2Common::D2GetMaxGold;
	D2AddPlayerStat = D2Common::D2AddPlayerStat;
	D2GetPlayerStat = D2Common::D2GetPlayerStat;
	D2CompileTxtFileDirect = D2Common::D2CompileTxtFileDirect;
	D2CompileTxtFile = D2Common::D2CompileTxtFile;
	D2ItemGetPage = D2Common::D2ItemGetPage;
	D2SaveItem = D2Common::D2SaveItem;
	D2ReadFile = D2Common::D2ReadFile;

	// D2Client
	D2LoadImage = D2Client::D2LoadImage;
	D2FreeImage = D2Client::D2FreeImage;
	D2PlaySound = D2Client::D2PlaySound;
	D2LoadBuySelBtn = D2Client::D2LoadBuySelBtn;
	D2GetMouseX = D2Client::D2GetMouseX;
	D2GetMouseY = D2Client::D2GetMouseY;
	D2GetClientPlayer = D2Client::D2GetClientPlayer;
	D2SendToServer3 = D2Client::D2SendToServer3;
	D2ClickOnStashButton = D2Client::D2ClickOnStashButton;

	// D2Client: Variables
	ptNegWindowStartY = D2Client::ptNegWindowStartY;
	ptWindowStartX = D2Client::ptWindowStartX;

	// D2Game
	D2SendPacket = D2Game::D2SendPacket;
	D2LoadInventory = D2Game::D2LoadInventory;
	D2GameGetObject = D2Game::D2GameGetObject;
	D2GetClient = D2Game::D2GetClient;
	D2SaveGame = D2Game::D2SaveGame;

	// D2Game: Variables
	ptClientTable = D2Game::ptClientTable;

	// D2Net
	D2SendToServer = D2Net::D2SendToServer;
	D2SendToClient = D2Net::D2SendToClient;

	// Fog
	D2FogMemAlloc = Fog::D2FogMemAlloc;
	D2FogMemDeAlloc = Fog::D2FogMemDeAlloc;
	D2AllocMem = Fog::D2AllocMem;
	D2FreeMem = Fog::D2FreeMem;
	D2MPQOpenFile = Fog::D2MPQOpenFile;
	D2MPQCloseFile = Fog::D2MPQCloseFile;
	D2MPQReadFile = Fog::D2MPQReadFile;
	D2MPQGetSizeFile = Fog::D2MPQGetSizeFile;
	D2FogGetSavePath = Fog::D2FogGetSavePath;

	// Storm
	D2StormMPQOpenFile = Storm::D2StormMPQOpenFile;
	D2FreeWinMessage = Storm::D2FreeWinMessage;

	// D2gfx
	D2GetResolution = D2gfx::D2GetResolution;
	D2FillArea = D2gfx::D2FillArea;
	D2PrintImage = D2gfx::D2PrintImage;

	// D2Win
	D2PrintLineOnTextBox = D2Win::D2PrintLineOnTextBox;
	D2PrintString = D2Win::D2PrintString;
	D2GetPixelLen = D2Win::D2GetPixelLen;
	D2SetFont = D2Win::D2SetFont;
	D2PrintPopup = D2Win::D2PrintPopup;
	D2CreateTextBox = D2Win::D2CreateTextBox;
	
	// Structure Management
	shifting.ptPYPlayerData = *(DWORD*)((DWORD)D2InitPlayerData + D2Common::ptPYPlayerDataOffset);
	shifting.ptSpecificData = D2Common::ptSpecificDataOffset;
	shifting.ptGame = D2Common::ptGameOffset;
	shifting.ptClientGame = D2Common::ptClientGameOffset;
	shifting.ptInventory = D2Common::ptInventoryOffset;
	shifting.ptSkills = D2Common::ptSkillsOffset;
	shifting.ptImage = D2Common::ptImageOffset;
	shifting.ptFrame = D2Common::ptFrameOffset;
}