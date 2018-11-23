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

#include "D2Client.h"
#include "D2Common.h"
#include "D2CMP.h"
#include "Fog.h"

void D2Client::Init()
{
	Name = "D2Client.dll";
	Offset = LoadDiabloLibrary();
	SetFunctions();
}

void D2Client::SetFunctions()
{
	D2LoadBuySelBtn = (TD2LoadBuySelBtn)GetAddress(0x18AA0);
	D2GetClientPlayer = (TD2GetClientPlayer)D2GetClientPlayer_111;
	D2LoadImage = (TD2LoadImage)D2LoadImage_111;
	D2PlaySound = (TD2PlaySound)D2PlaySound_111;
	IsExpansion = (TD2IsExpansion)D2isLODGame_111;
	D2FreeImage = (TD2FreeImage)D2FreeImage_111;
	D2SendToServerXX = (TD2SendToServerXX)GetAddress(0xB61F0);
	D2SendToServer3 = (TD2SendToServer3)D2SendToServer3_111;
	D2ClickOnStashButton = (TD2ClickOnStashButton)GetAddress(0x90C10);
	D2GetMouseX = (TD2GetMouseX)D2GetMouseX_111;
	D2GetMouseY = (TD2GetMouseY)D2GetMouseY_111;
	D2SetColorPopup = (TD2SetColorPopup)D2SetColorPopup_111;
	D2SetColorPopupDirect = (TD2SetColorPopup)GetAddress(0x18820);

	D2LoadImageDirect = (TD2LoadImage)GetAddress(0xA9480);
	D2PlaySoundDirect = (TD2PlaySound)GetAddress(0x26270);

	// Variables [Offset]
	ptResolutionY = (DWORD*)GetAddress(0xF7038);
	ptResolutionX = (DWORD*)GetAddress(0xF7034);
	ptNegWindowStartY = (DWORD*)GetAddress(0x11D358);
	ptWindowStartX = (DWORD*)GetAddress(0x11D354);
	ptptClientChar = (Unit**)GetAddress(0x11D050);
	ptMouseY = (DWORD*)GetAddress(0x11C94C);
	ptMouseX = (DWORD*)GetAddress(0x11C950);
}

__declspec(naked) void D2Client::D2SetColorPopup_111()
{
	__asm {
		PUSH EDI
			PUSH EDX
			MOV EDI, ECX
			CALL D2SetColorPopupDirect
			POP EDI
			RETN
	}
}

Unit* __stdcall	D2Client::D2GetClientPlayer_111()
{
	return *ptptClientChar;
}

DWORD __fastcall D2Client::D2isLODGame_111()
{
	DWORD* func = (DWORD*)GetAddress(0x1087B4);
	return *func;
}

const char* D2Client::D2FreeImage_FILE = __FILE__;
__declspec(naked) void D2Client::D2FreeImage_111()
{
	__asm {
		PUSH ESI
		MOV ESI, ECX
		PUSH ESI
		CALL D2CMP::D2CMP10014
		TEST ESI, ESI
		JE END_D2Free
		PUSH 0
		PUSH __LINE__
		MOV EDX, D2FreeImage_FILE;  ASCII "C:\projects\D2\head\Diablo2\Source\D2Client\CORE\ARCHIVE.CPP"
		MOV ECX, ESI
		CALL Fog::D2FogMemDeAlloc
		END_D2Free :
		POP ESI
			RETN
	}
}

__declspec (naked) void D2Client::D2SendToServer3_111()
{
	__asm {
		PUSH EBX
		PUSH ECX
		MOV BYTE PTR SS : [ESP], CL
		MOV WORD PTR SS : [ESP + 1], DX
		MOV EBX, 3
		LEA EDX, DWORD PTR SS : [ESP]
		PUSH EDX
		CALL D2SendToServerXX
		POP ECX
		POP EBX
		RETN
	}
}

DWORD __stdcall	D2Client::D2GetMouseX_111()
{
	return *ptMouseX;
}

DWORD __stdcall	D2Client::D2GetMouseY_111()
{
	return *ptMouseY;
}

DWORD D2Client::ResolutionX()
{
	return *ptResolutionX;
}

DWORD D2Client::ResolutionY()
{
	return *ptResolutionY;
}

__declspec(naked) void D2Client::D2LoadImage_111()
{
	__asm {
		PUSH EDX
		MOV EAX, ECX
		CALL D2LoadImageDirect
		RETN
	}
}

__declspec(naked) void D2Client::D2PlaySound_111()
{
	__asm {
		PUSH EBX
		PUSH DWORD PTR SS : [ESP + 0x10]
		PUSH DWORD PTR SS : [ESP + 0x10]
		PUSH DWORD PTR SS : [ESP + 0x10]
		PUSH EDX
		MOV EBX, ECX
		CALL D2PlaySoundDirect
		POP EBX
		RETN 0xC
	}
}

D2Client::TD2IsExpansion D2Client::IsExpansion;
D2Client::TD2LoadImage D2Client::D2LoadImage;
D2Client::TD2FreeImage D2Client::D2FreeImage;
D2Client::TD2PlaySound D2Client::D2PlaySound;
D2Client::TD2SendToServerXX D2Client::D2SendToServerXX;
D2Client::TD2SendToServer3 D2Client::D2SendToServer3;
D2Client::TD2GetMouseX D2Client::D2GetMouseX;
D2Client::TD2GetMouseY D2Client::D2GetMouseY;
D2Client::TD2GetClientPlayer D2Client::D2GetClientPlayer;
D2Client::TD2LoadBuySelBtn D2Client::D2LoadBuySelBtn;
D2Client::TD2ClickOnStashButton D2Client::D2ClickOnStashButton;
D2Client::TD2LoadImage D2Client::D2LoadImageDirect;
D2Client::TD2PlaySound D2Client::D2PlaySoundDirect;
D2Client::TD2SetColorPopup D2Client::D2SetColorPopup;
D2Client::TD2SetColorPopup D2Client::D2SetColorPopupDirect;

DWORD* D2Client::ptResolutionY;
DWORD* D2Client::ptResolutionX;
DWORD* D2Client::ptMouseY;
DWORD* D2Client::ptMouseX;
Unit** D2Client::ptptClientChar;
DWORD* D2Client::ptNegWindowStartY;
DWORD* D2Client::ptWindowStartX;