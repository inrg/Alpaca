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

#include "Library.h"

// C++ ISO Standards prohibit using a pointer-to-member address as a way to access the function in the instance.
// There are workarounds available (static functions being one (They are essentially the same as non-member functions)
// and the other workaround is to have a regular non-member (top level/non-class) function and then inside that function
// we call our real instanced based member function. I'm going with this approach.
// More info here: https://isocpp.org/wiki/faq/pointers-to-members#fnptr-vs-memfnptr-types

class D2Client : public Library<D2Client>
{
public:
	static void Init();

    typedef void* (__fastcall *TD2LoadImage) (const char* filename, DWORD filetype);
	typedef void(__fastcall *TD2FreeImage) (void* image);
	typedef DWORD(__fastcall *TD2PlaySound) (DWORD id, DWORD, DWORD, DWORD, DWORD);
	typedef void(__fastcall *TD2SendToServerXX) (DWORD size, BYTE * data);
	typedef void* (__stdcall *TD2LoadBuySelBtn) ();
	typedef DWORD(__fastcall *TD2IsExpansion) ();
	typedef void(__fastcall *TD2ClickOnStashButton) (DWORD x, DWORD y);
	typedef DWORD(__stdcall *TD2GetMouseX) ();
	typedef DWORD(__stdcall *TD2GetMouseY) ();
	typedef Unit* (__stdcall *TD2GetClientPlayer) ();
	typedef void(__fastcall *TD2SendToServer3) (BYTE type, WORD p);
	typedef LPWSTR(__fastcall *TD2SetColorPopup) (LPWSTR popupText, DWORD color);

	static TD2LoadImage D2LoadImage;
	static TD2FreeImage D2FreeImage;
	static TD2PlaySound D2PlaySound;
	static TD2SendToServerXX D2SendToServerXX;
	static TD2LoadBuySelBtn D2LoadBuySelBtn;
	static TD2IsExpansion IsExpansion;
	static TD2GetMouseX D2GetMouseX;
	static TD2GetMouseY D2GetMouseY;
	static TD2GetClientPlayer D2GetClientPlayer;
	static TD2SendToServer3 D2SendToServer3;
	static TD2ClickOnStashButton D2ClickOnStashButton;
	static TD2SetColorPopup D2SetColorPopup;

	static DWORD ResolutionX();
	static DWORD ResolutionY();

	// Variables
	static DWORD* ptResolutionY;
	static DWORD* ptResolutionX;
	static DWORD* ptMouseY;
	static DWORD* ptMouseX;
	static DWORD* ptNegWindowStartY;
	static DWORD* ptWindowStartX;
	static Unit** ptptClientChar;
private:
	static void SetFunctions();
	static Unit* __stdcall	D2GetClientPlayer_111();
	static DWORD __fastcall D2isLODGame_111();
	static const char* D2Client::D2FreeImage_FILE;
	static void D2FreeImage_111();
	static void D2SendToServer3_111();
	static DWORD __stdcall	D2GetMouseX_111();
	static DWORD __stdcall	D2GetMouseY_111();
	static TD2LoadImage D2LoadImageDirect;
	static void D2LoadImage_111();
	static TD2PlaySound D2PlaySoundDirect;
	static void D2PlaySound_111();
	static TD2SetColorPopup D2SetColorPopupDirect;
	static void D2SetColorPopup_111();
};