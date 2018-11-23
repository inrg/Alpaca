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

#pragma once

#include "LibraryLoader.h"

struct s_shifting {
	DWORD ptInventory;
	DWORD ptSpecificData;
	DWORD ptPYPlayerData;
	DWORD ptGame;
	DWORD ptClientGame;
	DWORD ptSkills;
	DWORD ptImage;
	DWORD ptFrame;
};
extern s_shifting shifting;

#define FCT_ASM(N) __declspec(naked) void N() {__asm{
#define RANDOM(V) ((int)(rand()/(RAND_MAX+1.0)*(V)))

#define PCPlayerData (*(PlayerData**)((DWORD)(ptChar)+shifting.ptSpecificData))
#define PCGame (*(GameStruct**)((DWORD)(ptChar)+shifting.ptGame))
#define PClientGame (*(GameStruct**)((DWORD)(ptClient)+shifting.ptClientGame))
#define PCInventory (*(Inventory**)((DWORD)(ptChar)+shifting.ptInventory))
#define PCPY ((PYPlayerData*)((DWORD)PCPlayerData+shifting.ptPYPlayerData))
#define PCSkills (*(Skills**)((DWORD)(ptChar)+shifting.ptSkills))

#define RX(v) (*ptWindowStartX + (v))
#define RY(v) (D2Client::ResolutionY() + *ptNegWindowStartY - (v))

// D2Common
extern D2Common::TD2InvAddItem D2InvAddItem;
extern D2Common::TD2InvRemoveItem D2InvRemoveItem;
extern D2Common::TD2InventoryGetFirstItem D2InventoryGetFirstItem;
extern D2Common::TD2UnitGetNextItem D2UnitGetNextItem;
extern D2Common::TD2GetRealItem D2GetRealItem;
extern D2Common::TD2GetMaxGoldBank D2GetMaxGoldBank;
extern D2Common::TD2InitPlayerData D2InitPlayerData;
extern D2Common::TD2GetMaxGold D2GetMaxGold;
extern D2Common::TD2AddPlayerStat D2AddPlayerStat;
extern D2Common::TD2GetPlayerStat D2GetPlayerStat;
extern D2Common::TD2CompileTxtFile D2CompileTxtFileDirect;
extern D2Common::TD2ItemGetPage D2ItemGetPage;
extern D2Common::TD2SaveItem D2SaveItem;
extern D2Common::TD2ReadFile D2ReadFile;

// D2Client
extern D2Client::TD2LoadImage D2LoadImage;
extern D2Client::TD2FreeImage D2FreeImage;
extern D2Client::TD2PlaySound D2PlaySound;
extern D2Client::TD2LoadBuySelBtn D2LoadBuySelBtn;
extern D2Client::TD2GetMouseX D2GetMouseX;
extern D2Client::TD2GetMouseY D2GetMouseY;
extern D2Client::TD2GetClientPlayer D2GetClientPlayer;
extern D2Client::TD2SendToServer3 D2SendToServer3;
extern D2Client::TD2ClickOnStashButton D2ClickOnStashButton;

// D2Client: Variables
extern DWORD* ptNegWindowStartY;
extern DWORD* ptWindowStartX;

// D2Game
extern D2Game::TD2SendPacket D2SendPacket;
extern D2Game::TD2LoadInventory D2LoadInventory;
extern D2Game::TD2GameGetObject D2GameGetObject;
extern D2Game::TD2GetClient D2GetClient;
extern D2Game::TD2SaveGame D2SaveGame;

// D2Game: Variables
extern NetClient** ptClientTable;

// D2Net
extern D2Net::TD2SendToServer D2SendToServer;
extern D2Net::TD2SendToClient D2SendToClient;

// Fog
extern Fog::TD2FogMemAlloc D2FogMemAlloc;
extern Fog::TD2FogMemDeAlloc D2FogMemDeAlloc;
extern Fog::TD2AllocMem D2AllocMem;
extern Fog::TD2FreeMem D2FreeMem;
extern Fog::TD2MPQOpenFile D2MPQOpenFile;
extern Fog::TD2MPQCloseFile D2MPQCloseFile;
extern Fog::TD2MPQReadFile D2MPQReadFile;
extern Fog::TD2MPQGetSizeFile D2MPQGetSizeFile;
extern Fog::TD2FogGetSavePath D2FogGetSavePath;

// Storm
extern Storm::TD2StormMPQOpenFile D2StormMPQOpenFile;
extern Storm::TD2FreeWinMessage D2FreeWinMessage;

// D2gfx
extern D2gfx::TD2GetResolution D2GetResolution;
extern D2gfx::TD2FillArea D2FillArea;
extern D2gfx::TD2PrintImage D2PrintImage;

// D2Win
extern D2Win::TD2PrintLineOnTextBox D2PrintLineOnTextBox;
extern D2Win::TD2PrintString D2PrintString;
extern D2Win::TD2GetPixelLen D2GetPixelLen;
extern D2Win::TD2SetFont D2SetFont;
extern D2Win::TD2PrintPopup D2PrintPopup;
extern D2Win::TD2CreateTextBox D2CreateTextBox;

extern void* (__stdcall *D2CompileTxtFile)(DWORD unused, const char* filename, BINField* ptFields, DWORD* ptRecordCount, DWORD recordLength);
void setImage(sDrawImageInfo* data, void* image);
void setFrame(sDrawImageInfo* data, DWORD frame);
void __inline fillRect(DWORD x, DWORD y, DWORD Width, DWORD Height, DWORD color, DWORD transTbl){D2FillArea(x,y,x+Width,y+Height,color,transTbl);};

void InitializeDiabloFunctions();